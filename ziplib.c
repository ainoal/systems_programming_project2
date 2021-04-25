/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "ziplib.h"


/* Reference: 
https://stackoverflow.com/questions/2513505/how-to-get-available-memory-c-g/26639774 */
unsigned long long getUsableMemory() {
    long pages, pageSize, memory; 

	pages = sysconf(_SC_PHYS_PAGES);
	pageSize = sysconf(_SC_PAGE_SIZE);
	
	memory = pages * pageSize * MAX_MEMORY_USAGE;

    return memory;
}

/* Allocate memory for a new list of rle structs. */
void allocate(RLE_LIST *rleList, int initialSize) {
    rleList->listSize = initialSize;
    rleList->listLength = 0;
    if ((rleList->data = (RLE*)calloc(initialSize, sizeof(RLE))) == NULL) {
        perror("Calloc failed");								// !!!!!!!!!!!!!!!!!!!!!
        exit(1);
    }
	printf("Calloc finished\n");
}

/* Source of inspiration:
https://gist.github.com/marcetcheverry/991042 */
void mapRead(char fileName[]) {    
    //const char *filepath = "/tmp/mmapped.bin";
    struct stat fileInfo = {0};
	int iFile;
	char *map;
    
    if ((iFile = open(fileName, O_RDONLY)) == 0) {
        perror("my-zip: error opening file");					// !!!!!!!!!!!!!!!!!!!!!!!!
        exit(1);
    }     
    
    if (fstat(iFile, &fileInfo) == -1) {
        perror("Error getting the file size");
        exit(1);
    }
    
    if (fileInfo.st_size == 0) {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(1);
    }
    
    printf("File size is %ji\n", (intmax_t)fileInfo.st_size);
    
    map = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, iFile, 0);

    if (map == MAP_FAILED) {
        close(iFile);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    for (off_t i = 0; i < fileInfo.st_size; i++)
    {
        printf("Found character %c at %ji\n", map[i], (intmax_t)i);
    }
    
    // Don't forget to free the mmapped memory
    if (munmap(map, fileInfo.st_size) == -1)
    {
        close(iFile);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }
    close(iFile);   
}

/*******************************************************************/
/* eof */
