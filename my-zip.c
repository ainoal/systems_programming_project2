/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * my-zip.c
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ziplib.h"

int main (int argc, char *argv[]) {
	unsigned long long maxMemory; // Amount of memory that can be given to output usage
	RLE_LIST output;
	MAPPED_FILE mappedFile;
	long pageSize;
	int argNumber;
	int lastFile = FALSE;

	if (argc == 1) {
		printf("Usage: ./my-unzip file1 [file2][...] > output\n");
		exit(1);
	}
	else {
		maxMemory = getUsableMemory();
		printf("%llu\n", maxMemory);

		/*Allocate memory for rle list structs */
		allocate(&output, INITIAL_MEMORY);

		// TODO calculate max page size: BETTER EXPLANATION HERE!
		pageSize = maxMemory / sizeof(RLE*) - 1;
		printf("Page size: %ld\n", pageSize); 	

		/* Map files to memory */
        for (argNumber = 1; argNumber < argc; argNumber++) {
            mappedFile = mapRead(argv[argNumber]);

            if (argNumber == argc -1) {
				/* This is the last file to be zipped
				--> write all chars to output (for other files, don't write
				 the last one because you need to compare it to the 1st char of 
				the next file */
				lastFile = TRUE;
            }

			// TODO Compress
            zip(&mappedFile, &output, pageSize, lastFile);

			// TODO Remember to free memory

			if (munmap(mappedFile.fileData, mappedFile.fileSize) == -1) {
				perror("Error un-mmapping the file");
				exit(1);
			}
        }

		// TODO Free memory
	}

	printf("The end\n");

	return 0;
}


/*******************************************************************/
/* eof */
