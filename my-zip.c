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

	if (argc == 1) {
		printf("Usage: ./my-unzip file1 [file2][...] > output\n");
		exit(1);
	}
	else {
		maxMemory = getUsableMemory();
		printf("%llu\n", maxMemory);
		
		// TODO  Allocate memory for rle structs
		// TODO count max page size
		// TODO Map files to memory (how?)
		// TODO Compress
		// TODO Remember to free memory
		
	}

	return 0;
}


/*******************************************************************/
/* eof */
