#ifndef __Ç_CHUNK_H__
#define __Ç_CHUNK_H__

#include "Common.h"

typedef enum
{
	OP_RETURN,									// Return from the current function.
} OP_CODE;

typedef struct 									// Will store the instructions' data.
{
	int		 count;								// Amount of elements in use in the array.
	int		 capacity;							// Amount of elements allocated in the array.
	uint8_t* code;								// Dynamic array that stores the bytecode.
} Chunk;

void InitChunk	(Chunk* chunk);					// Initializes the chunk's variables.
void FreeChunk	(Chunk* chunk);					// Frees the memory allocated to the given Chunk.
void WriteChunk	(Chunk* chunk, uint8_t byte);	// Appends the given byte to the end of the Chunk's array.

#endif // !__Ç_CHUNK_H__