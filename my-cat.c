/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * my-cat.c
 * Aino Liukkonen
 * 20.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>

void readFile (char *fileName) {
	char fileContents[200];
	FILE *fp;

	if ((fp = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "my-cat: cannot open file\n");
		exit(1);
	}
 
	while (1) {
		if (fgets(fileContents, 200, fp) == NULL) {
			break;
		}
		printf("%s", fileContents);
	}

	fclose(fp);
}

int main (int argc, char *argv[]) {
	int i;

	for(i=1; i<argc; i++) {
		readFile(argv[i]);
	}

	return 0;
}

/*******************************************************************/
/* eof */
