/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * my-zip.c
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include "ziplib.h"

int main (int argc, char **argv) {

	if (argc == 1) {
		printf("Usage: ./my-unzip file1 [file2][...] > output");
		exit(1);
	}

	return 0;
}


/*******************************************************************/
/* eof */
