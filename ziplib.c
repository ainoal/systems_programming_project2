/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 13.5.2021
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

/* Function to map file to memory. File mapping associates the contents 
 * of a file with a portion of the virtual address space of the calling process.
 * Source of inspiration: https://gist.github.com/marcetcheverry/991042 */
MAPPED_FILE mapRead(char fileName[]) {    
    struct stat fileInfo = {0};
	MAPPED_FILE mappedFile;
	int iFile;
    
    if ((iFile = open(fileName, O_RDONLY)) == 0) {
		fprintf(stderr, "Error opening file\n");
        exit(1);
    }     
    
	/* Get file size */
    if (fstat(iFile, &fileInfo) == -1) {
		fprintf(stderr, "Error getting the file size\n");
        exit(1);
    }
    
    if (fileInfo.st_size == 0) {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(1);
    }

	mappedFile.fileSize = fileInfo.st_size;
    mappedFile.fileData = mmap(0, mappedFile.fileSize, PROT_READ, MAP_SHARED, iFile, 0);

    if (mappedFile.fileData == MAP_FAILED) {
        close(iFile);
		fprintf(stderr, "Error mapping the file to memory\n");
        exit(1);
    }

    close(iFile);  

	return  mappedFile;
}

/*******************************************************************/
/* Zip functions */

/* Allocate memory for a new list of RLE structs. */
void allocate(RLE_LIST *rleList, int initialSize) {
    rleList->listSize = initialSize;
    rleList->listLength = 0;
    if ((rleList->rleData = (RLE*)calloc(initialSize, sizeof(RLE))) == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

/* Add RLE struct to the list of structs. */
void appendRleList(RLE_LIST* rleList, RLE* rle, unsigned long long maxMemory) {
	int newSize;
	int len;

	/* Make sure that there is enough space for a new RLE element */
    if (rleList->listLength + 1 > rleList->listSize) {
    	
		/* If there is not enough space, then expand the RLE list */

		newSize = (rleList->listLength + 1) * sizeof(RLE*);

		if (newSize > maxMemory / sizeof(RLE*)) {
		    newSize = (int)(maxMemory / sizeof(RLE*));
		}
		if ((rleList->rleData = (RLE*)realloc(rleList->rleData, newSize)) == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			exit(1);
		}

		/* Update the expanded RLE list's new physical size in structs */
		rleList->listSize = newSize / sizeof(RLE*);
    }

    /* Add an RLE element to the list */
	len = rleList->listLength;
    rleList->rleData[len].character = rle->character;
    rleList->rleData[len].charAmount = rle->charAmount;
    rleList->listLength++;
}

/* The actual paging and zipping of the file(s) */
void zip(MAPPED_FILE *mappedFile, RLE_LIST *output, long pageSize, int isLastFile, unsigned long long maxMemory) {
	long start = 0;
	long end = 0;
	RLE element;
	char letter;
	int count;
	int position;
	char *letters;
	int len;

    while (TRUE) {
        /* Calculate start and end position based on the page. */
        if (end == 0) {
			start = 0;
            start = end + 1;
        } else {
            start = end + 1;
        }

        if (pageSize < mappedFile->fileSize - end) {
            end = end + pageSize;
        } else {
            end = mappedFile->fileSize;
        }

		/* Compress string with RLE */

		letters = mappedFile->fileData;

		/* If there is already data in rleList 'output', continue from the last item */
		if (output->listLength > 0) {
			len = output->listLength - 1;
			letter = output->rleData[len].character;
			count = output->rleData[len].charAmount;
			output->listLength = 0;
		}
		else {
			letter = letters[start];
			count = 0;
		}
		/* Compare current and previous characters. Count how many of the same characters 
		 * there are in a row */
		for (position = start; position <= end; position++) {
			if (letters[position] != letter) {
				element.character = letter;
				element.charAmount = count;
				appendRleList(output, &element, maxMemory);
				count = 0;
				letter = letters[position];
			}
			count++;
		}
		/* If there are multiple pages */
		if (letters[end] != '\0') {
		    element.character = letters[end];
		    element.charAmount = count;
		    appendRleList(output, &element, maxMemory);
		}


        if (end < mappedFile->fileSize) {
			
            /* This is not the end of file. When writing to output, leave the last element 
			 * out because the latest character needs to be compared to the next one */
            if (output->listLength > 1) {
                fwrite(output->rleData, sizeof(RLE), output->listLength - 1, stdout);
            }
        }
        else {
			/* When zipping multiple files at once, if this is not the last file to zip, 
 			 * don't write the last element to output because the last character of the file 
			 * needs to be compared  to the first character of the next file. */
            if (isLastFile == FALSE) {
            	fwrite(output->rleData, sizeof(RLE), output->listLength - 1, stdout);
            }
            else {
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
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void expandString(STRING* string, int bufferSize, unsigned long long maxMemory) {
	int newSize;

    if (bufferSize > maxMemory) {
        printf("Can't allocate more than defined max memory\n");
        exit(1);
    }
    newSize = bufferSize * sizeof(char*);
    if (newSize > maxMemory / sizeof(char*)) {
        newSize = (int)(maxMemory / sizeof(char*));
    }

    if ((string->stringData = (char*)realloc(string->stringData, newSize)) == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    /* Update new physical size */
    string->stringSize = newSize / sizeof(char*);
}

void printBuffer(STRING *string) {
	printf("%s", string->stringData);
    string->stringLength = 0;
}

void createChars(STRING* string, char character, int charAmount, unsigned long long maxMemory) {
	int i;

    /* Expand string if it is needed for generated characters */
    if (charAmount > string->stringSize - string->stringLength) {
        expandString(string, string->stringLength + charAmount, maxMemory);
    }
    /* Generate characters */
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

    /* Proceed two 5 bytes at a time  because the zipped files consist of
	 * 5-byte entries */
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
				for (i = string->stringLength; i < string->stringLength + add; i++) {
					string->stringData[i] = character;
				}
				string->stringLength += add;

                printBuffer(&buffer);
            }
        }
        /* If the amount of characters is smaller than buffer, but buffer
		 * is full, then print buffer first and create characters after that */
        else if (buffer.stringLength + length > bufferSize) {
			printBuffer(&buffer);
            createChars(&buffer, character, length, bufferSize);
        }
        /* If there is space in buffer, only create characters */
        else {
            createChars(&buffer, character, length, bufferSize);
        }
    }

    /* If there are still characters in the buffer, print them out */
    if (buffer.stringLength > 0) {
        printBuffer(&buffer);
    }
}

/*******************************************************************/
/* eof */
