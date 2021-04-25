/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * my-zip.c
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ziplib.h"

int main (int argc, char *argv[]) {
	unsigned long long maxMemory; // Amount of memory that can be given to output usage
	RLE_LIST output;
	int argNumber;
	long pageSize;

	if (argc == 1) {
		printf("Usage: ./my-unzip file1 [file2][...] > output\n");
		exit(1);
	}
	else {
		maxMemory = getUsableMemory();
		printf("%llu\n", maxMemory);

		// TODO  Allocate memory for rle list structs
		allocate(&output, INITIAL_MEMORY);

		// TODO calculate max page size: BETTER EXPLANATION HERE!
		pageSize = maxMemory / sizeof(RLE*) - 1;		

		// TODO Map files to memory
        for (int argumentNumber = 1; argNumber < argc; argNumber++) {
            mapped_file = map_file(argv[argument_number]);
            if (argNumber == argc -1) {
				/* This is the last file to be zipped
				--> write all chars to output (for other files, don't write
				 the last one because you need to compare it to the 1st char of 
				the next file */
            }

			// TODO Compress
			// TODO Remember to free memory
        }
		
	}

	printf("The end\n");

	return 0;
}


/*******************************************************************/
/* eof */
