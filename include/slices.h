#ifndef _SLICE_ALLOCATOR_H
#define _SLICE_ALLOCATOR_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
	size_t size;
	void *bytes;
}Slice;


static void *g_block = NULL;
static size_t g_block_size = 0;

void
slice_init(size_t size){
	assert(size > 0);

	g_block_size = size;
	g_block = malloc(size);

	assert(g_block != NULL);
}

void
slice_deinit(){
	free(g_block);
	g_block = NULL;
	g_block_size = 0;
}

Slice 
slice_alloc(size_t memb_size, size_t nmemb){

	static size_t offset = 0;

	assert(memb_size > 0 && nmemb > 0);
	assert(g_block != NULL && "SLICE HAS TO BE INITIALIZED");

	void *new_slice_ptr = g_block + offset;
	size_t add_offset = memb_size * nmemb;

	offset += add_offset;

	assert(offset < g_block_size && "MEMORY FULL");

	Slice slice = {0};
	slice.bytes = new_slice_ptr;
	slice.size = add_offset;

	return slice;
}

typedef struct {
	Slice slice;
	size_t offset;
}Block;

Block 
slice_block_init(size_t size){
	Block new_block = {
		.slice = {
			.size = size,
			.bytes = malloc(size)
		},
		.offset = 0
	};

	assert(new_block.slice.bytes != NULL);

	return new_block;
}

void
slice_block_deinit(Block *block){
	free(block->slice.bytes);
	block->slice.size = 0;
	block->offset = 0;
}

Slice
slice_block(Block *block, size_t memb_size, size_t nmemb){
	assert(memb_size > 0 && nmemb > 0);
	assert(block != NULL && block->slice.bytes != NULL && "BLOCK HAS TO BE INITIALIZED");

	void *new_slice_ptr = block->slice.bytes + block->offset;
	size_t add_offset = memb_size * nmemb;

	block->offset += add_offset;

	assert(block->offset < block->slice.size && "MEMORY FULL");

	Slice slice = {0};
	slice.bytes = new_slice_ptr;
	slice.size = add_offset;

	return slice;	
}


#endif /* _SLICE_ALLOCATOR_H */
