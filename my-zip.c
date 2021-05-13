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
	int isLastFile = FALSE;

	if (argc == 1) {
		printf("Usage: ./my-unzip file1 [file2][...] > output\n");
		exit(1);
	}
	else {
		/* Get maximum usable memory */
		maxMemory = getUsableMemory();

		/*Allocate memory for rleList structs */
		allocate(&output, INITIAL_MEMORY);

		/* Calculate maximum page size */
		pageSize = (long) (maxMemory / sizeof(RLE*) - 1); 	

		/* Map files to memory */
        for (argNumber = 1; argNumber < argc; argNumber++) {
            mappedFile = mapRead(argv[argNumber]);

            if (argNumber == argc - 1) {
				/* This is the last file to be zipped
				--> later, write all chars to output (for other files, don't
				write the last one because you need to compare it to the 1st
				char of the next file */
				isLastFile = TRUE;
            }

			/* Compress */
            zip(&mappedFile, &output, pageSize, isLastFile, maxMemory);

			if (munmap(mappedFile.fileData, mappedFile.fileSize) == -1) {
				fprintf(stderr, "Error unmapping file\n");
				exit(1);
			}
        }
		free(output.rleData);
	}
	return 0;
}


/*******************************************************************/
/* eof */
