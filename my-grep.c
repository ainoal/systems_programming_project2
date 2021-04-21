/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * my-cat.c
 * Aino Liukkonen
 * 21.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grepFile(char *searchterm, char *fileName) {
	FILE *fp;
	char *buffer;
	char *result;
	size_t bufferSize = 32;

	if((buffer = (char *)malloc(bufferSize * sizeof(char))) == NULL) {
		perror("Malloc error\n");
		exit(1);
	}

	if ((fp = fopen(fileName, "r")) == NULL) {
		perror("my-grep: cannot open file\n");
		exit(1);
	}
	
	while (getline(&buffer, &bufferSize, fp) != -1) {
		result = strstr(buffer, searchterm);
		if(result != NULL) {
			printf("%s", buffer);
		}	
	}

	fclose(fp);
}

int main (int argc, char *argv[]) {
	int i;

	if(argc == 1) {
		perror("my-grep: searchterm [file...]\n");
		exit(1);
	} 
	else if(argc == 2) {
		printf("Read from stdin\n");		// !!!!!!!!!!!!!!!
	}

	for(i=2; i<argc; i++) {
		grepFile(argv[1], argv[i]);
	}

	return 0;
}

/*******************************************************************/
