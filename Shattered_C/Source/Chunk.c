#include <stdlib.h>

#include "Memory.h"
#include "Chunk.h";

void InitChunk(Chunk* chunk)
{
	chunk->count	= 0;
	chunk->capacity = 0;
	chunk->code		= NULL;
}

void FreeChunk(Chunk* chunk)
{
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	InitChunk(chunk);
}

void WriteChunk(Chunk* chunk, uint8_t byte)
{
	if (chunk->capacity < (chunk->count + 1))
	{
		int old_capacity = chunk->capacity;
		chunk->capacity  = EXPAND_CAPACITY(old_capacity);
		chunk->code		 = EXPAND_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;
	++chunk->count;
}