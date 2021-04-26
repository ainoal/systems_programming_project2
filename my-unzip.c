/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 26.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ziplib.h"

int main (int argc, char **argv) {
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
		// TODO get "maxMemory" (a function already exists for that)
		maxMemory = getUsableMemory();

		// TODO count max buffer size
		bufferSize = maxMemory / sizeof(RLE*) -1;

		// TODO allocate memory for buffer
		allocateString(&buffer, INITIAL_MEMORY);

		// TODO map files (a function already exists for that)
		/* Map files to memory */
        for (argNumber = 1; argNumber < argc; argNumber++) {
            mappedFile = mapRead(argv[argNumber]);

			// TODO unzip
			unzip(mappedFile, buffer, bufferSize);
		}



		// TODO remember to free memory
	}

	return 0;
}


/*******************************************************************/
/* eof */
