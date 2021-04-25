/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.c
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "ziplib.h"


/* Reference: https://stackoverflow.com/questions/2513505/how-to-get-available-memory-c-g/26639774 */
unsigned long long getUsableMemory() {
    long pages, pageSize, memory; 

	pages = sysconf(_SC_PHYS_PAGES);
	pageSize = sysconf(_SC_PAGE_SIZE);
	
	memory = pages * pageSize * MAX_MEMORY_USAGE;

    return memory;
}


/*******************************************************************/
/* eof */
