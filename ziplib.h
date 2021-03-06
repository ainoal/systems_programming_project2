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

/* Disable padding in struct Rle with pragma pack
https://stackoverflow.com/questions/3318410/pragma-pack-effect */
#pragma pack (push,1)
struct Rle {
	int charAmount;
	char character;
};

typedef struct Rle RLE;
#pragma pack(pop)

struct RleList {
	RLE *rleData;
	long listSize;		// Physical size of the list
	long listLength;	// Length in nodes
};

struct MappedFile {
    void* fileData;
    int fileSize;
};

struct String {
    char* stringData;
    int stringSize;		// Physical size of the string
    int stringLength;	// Virtual size of the string
};

typedef struct RleList RLE_LIST;
typedef struct MappedFile MAPPED_FILE;
typedef struct String STRING;

unsigned long long getUsableMemory();
MAPPED_FILE mapRead(char fileName[]);

void allocate(RLE_LIST *rleList, int initialSize);
void appendRleList(RLE_LIST* rleList, RLE* rle, unsigned long long maxMemory);
void zip(MAPPED_FILE *mappedFile, RLE_LIST *output, long pageSize, int lastFile, unsigned long long maxMemory);

void allocateString(STRING* string, int initialSize);
void unzip(MAPPED_FILE mappedFile, STRING buffer, unsigned long long bufferSize);
void expandString(STRING* string, int reqSize, unsigned long long maxMemory);

/*******************************************************************/
/* eof */
