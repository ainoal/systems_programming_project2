/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "ziplib.h"

/*******************************************************************/
/* General functions */

/* Reference: 
https://stackoverflow.com/questions/2513505/how-to-get-available-memory-c-g/26639774 */
unsigned long long getUsableMemory() {
    long pages, pageSize, memory; 

	pages = sysconf(_SC_PHYS_PAGES);
	pageSize = sysconf(_SC_PAGE_SIZE);
	
	memory = pages * pageSize * MAX_MEMORY_USAGE;

    return memory;
}

/* Source of inspiration: https://gist.github.com/marcetcheverry/991042 */
MAPPED_FILE mapRead(char fileName[]) {    
    //const char *filepath = "/tmp/mmapped.bin";
    struct stat fileInfo = {0};
	MAPPED_FILE mappedFile;
	int iFile;
    
    if ((iFile = open(fileName, O_RDONLY)) == 0) {
        perror("my-zip: error opening file");					// !!!!!!!!!!!!!!!!!!!!!!!!
        exit(1);
    }     
    
    if (fstat(iFile, &fileInfo) == -1) {
        perror("Error getting the file size");
        exit(1);
    }
    
    if (fileInfo.st_size == 0) {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(1);
    }
    
    printf("File size is %ji\n", (intmax_t)fileInfo.st_size);

	mappedFile.fileSize = fileInfo.st_size;
    
    mappedFile.fileData = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, iFile, 0);

    if (mappedFile.fileData == MAP_FAILED) {
        close(iFile);
        perror("Error mmapping the file");
        exit(1);
    }
    
    // Don't forget to free the mmapped memory


    close(iFile);  

	return  mappedFile;
}

/*******************************************************************/
/* Zip functions */

/* Allocate memory for a new list of rle structs. */
void allocate(RLE_LIST *rleList, int initialSize) {
    rleList->listSize = initialSize;
    rleList->listLength = 0;
    if ((rleList->rleData = (RLE*)calloc(initialSize, sizeof(RLE))) == NULL) {
        perror("Calloc failed");								// !!!!!!!!!!!!!!!!!!!!!
        exit(1);
    }
	printf("Calloc finished\n");
}

void appendRleList(RLE_LIST* rleList, RLE* rle, unsigned long long maxMemory) {
	int newSize;

	/* Make sure that there is enough space for a new RLE element */
    if (rleList->listLength + 1 > rleList->listSize) {
    	/* If not, then expand the RLE list */

		/* !!! */
		newSize = (rleList->listLength + 1) * sizeof(RLE*);

		if (newSize > maxMemory / sizeof(RLE*)) {
		    newSize = (int)(maxMemory / sizeof(RLE*));
		}
		
		if ((rleList->rleData = (RLE*)realloc(rleList->rleData, newSize)) == NULL) {
			perror("Realloc error");
			exit(1);
		}

		/* Update new physical size in structs */
		rleList->listSize = newSize / sizeof(RLE*);
    }

    /* Add an RLE element to the list */
    rleList->rleData[rleList->listLength].character = rle->character;
    rleList->rleData[rleList->listLength].charAmount = rle->charAmount;
    rleList->listLength++;
}

/* The actual paging and zipping of the file(s) */
void zip(MAPPED_FILE *mappedFile, RLE_LIST *output, long pageSize, int lastFile, unsigned long long maxMemory) {
    long start = 0;
    long end = 0;
	RLE element;
	char letter;
	int count;
	int position;
	char *letters;
	// int i (=output->listLength -1);

	// letters[] = mappedFile->fileData;

	printf("zipping\n");

    while (TRUE) {
        /* Calculate start and end position based on the page. */
        if (end != 0) {
            start = end + 1;
        } else {
            start = 0;
        }

        if (pageSize < mappedFile->fileSize - end) {
            end = end + pageSize;
        } else {
            end = mappedFile->fileSize;
        }

		/* Compress given string with Run Length Encoding. */

		letters = mappedFile->fileData;

		/* Continue from the last item if there is already data in the output
		   and start buffer from the beginning. */
		if (output->listLength > 0) {
		    letter = output->rleData[output->listLength - 1].character;
		    count = output->rleData[output->listLength - 1].charAmount;
		    output->listLength = 0;
		}
		else {
		    letter = letters[start];
		    count = 0;
		}
		/*  */
		for (position = start; position <= end; position++) {
		    if (letters[position] != letter) {
		        element.character = letter;
		        element.charAmount = count;
		        /* Add struct to list of structs. */
		        appendRleList(output, &element, maxMemory);
		        count = 0;
		        letter = letters[position];
		    }
		    count++;
		}
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
		if (letters[end] != '\0') {
		    element.character = letters[end];
		    element.charAmount = count;
			/* Add struct to list of structs. */
		    appendRleList(output, &element, maxMemory);
		}


        if (end < mappedFile->fileSize) {
			
            /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
            if (output->listLength > 1) {
                fwrite(output->rleData, sizeof(RLE), output->listLength - 1, stdout);
            }
        }
        else {
			/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
            if (lastFile == FALSE) {
            	fwrite(output->rleData, sizeof(RLE), output->listLength - 1, stdout);
            }
            else if (lastFile == TRUE) {
                /* Write all bytes */
				fwrite(output->rleData, sizeof(RLE), output->listLength, stdout);
            }
            break;
        }
    }
}

/*******************************************************************/
/* Unzip functions */

/* Allocate new list of STRING structs. */
void allocateString(STRING *string, int initialSize) {
    string->stringSize = initialSize;
    string->stringLength = 0;
    if ((string->stringData = (char*)calloc(initialSize, sizeof(char))) == NULL) {
        perror("Calloc error");
        exit(1);
    }
}

void expandString(STRING* string, int reqSize, unsigned long long maxMemory) {
	int newSize;

    if (reqSize > maxMemory) {
        printf("Can't allocate more than defined max memory\n");
        exit(1);
    }
    newSize = reqSize * sizeof(char*) * 2;
    if (newSize > maxMemory / sizeof(char*)) {
        newSize = (int)(maxMemory / sizeof(char*));
    }

    if ((string->stringData = (char*)realloc(string->stringData, newSize)) == NULL) {
        printf("Cannot allocate more memory\n");
        exit(1);
    }
    /* Update new physical size */
    string->stringSize = newSize / sizeof(char*);
}

void printBuffer(STRING *string) {
    fwrite(string->stringData, sizeof(char), string->stringLength, stdout);
	printf("%s", string->stringData);
    string->stringLength = 0;
}

void createChars(STRING* string, char character, int charAmount, unsigned long long maxMemory) {
	int i;

    /* Expand string if needed for generated letters */
    if (charAmount > string->stringSize - string->stringLength) {
        expandString(string, string->stringLength + charAmount, maxMemory);
    }
    /* Generate letters */
    for (i = string->stringLength; i < charAmount + string->stringLength; i++) {
        string->stringData[i] = character;
    }
    string->stringLength += charAmount;
}

/* The actual unzipping of the file(s) */
void unzip(MAPPED_FILE mappedFile, STRING buffer, unsigned long long bufferSize) {
    int *pLength;
    char character;
    char *pCharacter;
	STRING *string;
    int length;
	int byte;
	int page;
	int add;
	int i;

    /* Proceed two 5 bytes at a time */		// + Reason here? (=because of the coding + encoding)
    for (byte = 0; byte < mappedFile.fileSize; byte += 5) {

        /* !!! */
        pLength = (int*)(mappedFile.fileData + byte);
        pCharacter = (char*)(mappedFile.fileData + byte + 4);
        length = *pLength;
        character = *pCharacter;


        /* If  amount of characters to be printed is greater than buffer,
        then generate characters in chunks */
        if (length > bufferSize) {
            printBuffer(&buffer);
            for (page = 0; page < length; page += bufferSize) {
                if (length - page < bufferSize) {
					add = length - page;
                }
                else {
					add = bufferSize;
                }

				string = &buffer;

				/* Expand string if needed for generated letters */
				if (add > string->stringSize - string->stringLength) {
					expandString(string, string->stringLength + add, bufferSize);
				}

				/* Create letters */
				for (i = string->stringLength; i < add + string->stringLength; i++) {
					string->stringData[i] = character;
				}
				string->stringLength += add;

                printBuffer(&buffer);
            }
        }
        /* !!!!!!!!!!!!!!!!!! */
        else if (buffer.stringLength + length > bufferSize) {
            createChars(&buffer, character, length, bufferSize);
        }
        /* If there is still room in the buffer, just generate characters */
        else {
            createChars(&buffer, character, length, bufferSize);
        }
    }
    /* If there are characters left in the buffer, print them */
    if (buffer.stringLength > 0) {
        printBuffer(&buffer);
    }
}

/*******************************************************************/
/* eof */
