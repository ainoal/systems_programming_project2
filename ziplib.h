/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * ziplib.h
 * Aino Liukkonen
 * 25.4.2021
 */
/*******************************************************************/

#define TRUE 1
#define FALSE 0
#define INITIAL_MEMORY 10000

/* The highest quota of system memory allowed to be used */
#define MAX_MEMORY_USAGE 0.1

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
	RLE *rleData;
	long listSize;		// Physical size of the list
	long listLength;	// Length in nodes
}; 
typedef struct RleList RLE_LIST;
#pragma pack(pop)

unsigned long long getUsableMemory();
void allocate(RLE_LIST *rleList, int initialSize);
MAPPED_FILE mapRead(char fileName[]);
void appendRleList(RLE_LIST* rleList, RLE* rle);
void zip(MAPPED_FILE *mappedFile, RLE_LIST *output, long pageSize, int lastFile);

/*******************************************************************/
/* eof */
