/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 13.5.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "ziplib.h"

int main (int argc, char *argv[]) {
	unsigned long long maxMemory;
	unsigned long long bufferSize;
	MAPPED_FILE mappedFile;
	STRING buffer;
	int argNumber;

	if (argc == 1) {
		printf("usage: my-unzip: file1 [file2][...]\n");
		exit(1);
	}
	else {
		/* Get maximum usable memory */
		maxMemory = getUsableMemory();

		/* Count maximum buffer size */
		bufferSize = maxMemory / sizeof(RLE*) -1;

		/* Allocate memory for buffer */
		allocateString(&buffer, INITIAL_MEMORY);

        for (argNumber = 1; argNumber < argc; argNumber++) {
			/* Map each file to memory */
            mappedFile = mapRead(argv[argNumber]);

			/* unzip and unmap file */
			unzip(mappedFile, buffer, bufferSize);
			munmap(mappedFile.fileData, mappedFile.fileSize);
		}
		free(buffer.stringData);
	}

	return 0;
}


/*******************************************************************/
/* eof */
