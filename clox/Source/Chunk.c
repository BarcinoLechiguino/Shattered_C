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

void WriteConstant(Chunk* chunk, Value value, int line)
{
	int constantIdx		= AddConstant(chunk, value);												// Adding the constant to the chunk's constants array.
	uint8_t instruction = (constantIdx < 256) ? OP_CONSTANT : OP_CONSTANT_LONG;
	
	if (instruction == OP_CONSTANT)
	{
		WriteChunk(chunk, instruction, 123);														// Writting a constant operation.
		WriteChunk(chunk, constantIdx, 123);														// Writting the contant's index in the chunk's code stream.
	}
	else
	{
		WriteChunk(chunk, instruction, 123);														// REVISE IN THE FUTURE
		WriteChunk(chunk, 255, 123);
	}
}

int AddConstant(Chunk* chunk, Value value)
{
	WriteValueArray(&chunk->constants, value);
	return (chunk->constants.count - 1);
}