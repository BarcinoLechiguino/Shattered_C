#include <stdlib.h>
#include <stdio.h>

#include "Memory.h"

#include "Chunk.h"

void InitChunk(Chunk* chunk)
{
	chunk->capacity = 0;
	chunk->count	= 0;
	chunk->code		= NULL;
	chunk->lines	= NULL;
	InitValueArray(&chunk->constants);
}

void FreeChunk(Chunk* chunk)
{
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	FreeValueArray(&chunk->constants);
	
	InitChunk(chunk);
}

void WriteChunk(Chunk* chunk, uint8_t byte, int line)
{
	if (chunk->capacity < chunk->count + 1)
	{
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code		= GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity));			// Why does it need the second ')' at the end?
		chunk->lines	= GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity));
	}

	chunk->code[chunk->count]	= byte;
	chunk->lines[chunk->count]	= line;
	++chunk->count;
}

void WriteChunkLong(Chunk* chunk, uint32_t bytes, int line)
{
	if (chunk->capacity < chunk->count + 4)
	{
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity * 2);
		chunk->code		= GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity));			// Why does it need the second ')' at the end?
		chunk->lines	= GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity));
	}

	((uint32_t*)(chunk->code))[chunk->count] = bytes;												// Casting the array to uint32_t so it can hold those kinds of values.
	chunk->lines[chunk->count]				 = line;
	chunk->lines[chunk->count + 1]			 = line;											// REVISE THIS. Write repeated lines?
	chunk->lines[chunk->count + 2]			 = line;
	chunk->lines[chunk->count + 3]			 = line;
	chunk->count += 4;
}

bool WriteConstant(Chunk* chunk, Value value, int line)
{
	int constantIdx	= AddConstant(chunk, value);													// Adding the constant to the chunk's constants array.
	if (constantIdx < UINT8_MAX)
	{
		WriteChunk(chunk, OP_CONSTANT, line);														// Writting a constant operation.
		WriteChunk(chunk, constantIdx, line);														// Writting the contant's index in the chunk's code stream.
		return true;
	}
	else
	{	
		//WriteChunk(chunk, OP_CONSTANT_LONG, line);													// REVISE IN THE FUTURE
		//WriteChunkLong(chunk, constantIdx, line);													// CHECK OP_CONSTANT_16 AND P.306
		fprintf(stderr, "Too many constants in one chunk. [CHUNK]");
		return false;
	}
}

int AddConstant(Chunk* chunk, Value value)
{
	WriteValueArray(&chunk->constants, value);
	return (chunk->constants.count - 1);
}