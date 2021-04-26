/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 26.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include "ziplib.h"

int main (int argc, char **argv) {
	unsigned long long maxMemory;
	MAPPED_FILE mappedFile;

	if (argc == 1) {
		printf("usage: my-unzip: file1 [file2][...]\n");
		exit(1);
	}
	else {
		// TODO get "maxMemory" (a function already exists for that)

		// TODO count max buffer size

		// TODO allocate buffer

		// TODO map files (a function already exists for that)

		// TODO unzip

		// TODO remember to free memory
	}

	return 0;
}


/*******************************************************************/
/* eof */
