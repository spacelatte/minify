
#ifndef _MINIFY_H_
#define _MINIFY_H_

#include <stdint.h>

#ifndef SUFFIX
#define SUFFIX ".min"
#endif

#ifndef BLOCKSIZE
#define BLOCKSIZE 16
#endif

typedef enum { false, true, } bool;

typedef struct Chunk {
	char block[BLOCKSIZE];
	//size_t length; // TODO: impl later?
	int32_t count;
} chunk_t;

typedef struct Data {
	uint64_t size;
	void *data;
} data_t;

chunk_t* chunk_make(size_t, size_t);
chunk_t* compress(FILE*);
data_t* decompress(FILE*);
size_t action(FILE*, FILE*, bool);

#endif
