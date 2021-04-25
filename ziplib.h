/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.h
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

/* The highest quota of system memory allowed to be used */
#define MAX_MEMORY_USAGE 0.1

struct Rle {
	int charAmount;
	char character;
};

typedef struct Rle RLE;

unsigned long long getUsableMemory();

/*******************************************************************/
/* eof */
