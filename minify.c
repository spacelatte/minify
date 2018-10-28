//
//  minify.c
//  minify-compress
//
//  Created by Mert Akengin on 26.10.2018.
//  Copyright © 2018 Mert Akengin. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include "minify.h"

chunk_t* chunk_make(size_t length, size_t count) {
	chunk_t *chunk = malloc(sizeof(chunk_t));
	//chunk->length = length ? length : BLOCKSIZE;
	chunk->count = count ? count : 1;
	return chunk;
}

chunk_t* compress(FILE *fp) {
	chunk_t* chunk = chunk_make(0, 0);
	size_t length = fread(chunk->block, 1, sizeof(chunk->block), fp);
	if(sizeof(chunk->block) != length) {
		chunk->count = -length;
		return length ? chunk : NULL;
	}
	char buffer[sizeof(chunk->block)];
	while(!feof(fp)) {
		length = fread(buffer, 1, sizeof(buffer), fp);
		if(length != sizeof(buffer) || memcmp(chunk->block, buffer, sizeof(buffer))) {
			fseek(fp, -length, SEEK_CUR);
			break;
		}
		chunk->count += 1;
		continue;
	}
	return chunk;
}

data_t* decompress(FILE *fp) {
	chunk_t chunk = { 0 };
	data_t *data = malloc(sizeof(data_t));
	size_t length = fread(&chunk, 1, sizeof(chunk_t), fp);
	if(length != sizeof(chunk) || !chunk.count) {
		return NULL;
	}
	if(chunk.count < 0) {
		data->size = abs(chunk.count);
		data->data = malloc(data->size);
		memcpy(data->data, chunk.block, data->size);
		return data;
	}
	data->size = sizeof(chunk.block) * chunk.count;
	data->data = malloc(data->size);
	for(int i=0; i<data->size; i+=sizeof(chunk.block)) {
		memcpy(data->data + i, chunk.block, sizeof(chunk.block));
		continue;
	}
	return data;
}

size_t action(FILE *out, FILE *in, bool mode) {
	size_t count = 0;
	while(!feof(in)) {
		printf("block: %8lu\r", count+1);
		if(mode) {
			chunk_t *chunk = compress(in);
			if(chunk != NULL) {
				fwrite(chunk, sizeof(chunk_t), 1, out);
				free(chunk);
			}
		} else {
			data_t *self = decompress(in);
			if(self != NULL) {
				fwrite(self->data, self->size, 1, out);
				free(self->data);
				free(self);
			}
		}
		count += 1;
		continue;
	}
	printf("\n");
	return count;
}
