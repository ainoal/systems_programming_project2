/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.h
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

/* The highest quota of system memory allowed to be used */
#define MAX_MEMORY_USAGE 0.1

#define INITIAL_MEMORY 10000

struct Rle {
	int charAmount;
	char character;
};

struct MappedFile {
    void* fileData;
    int fileSize;
};

typedef struct Rle RLE;
typedef struct MappedFile MAPPED_FILE;

/* Disable padding in struct RleList with pragma pack
https://stackoverflow.com/questions/3318410/pragma-pack-effect */
#pragma pack (push,1)
struct RleList {
	RLE *data;
	long listSize;		// Physical size of the list
	long listLength;	// Length in nodes
}; 
typedef struct RleList RLE_LIST;
#pragma pack(pop)

unsigned long long getUsableMemory();
void allocate(RLE_LIST *rleList, int initialSize);

/*******************************************************************/
/* eof */
