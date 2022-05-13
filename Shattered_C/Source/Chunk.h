#ifndef __Ç_CHUNK_H__
#define __Ç_CHUNK_H__

#include "Common.h"
#include "Value.h"

typedef enum
{
	OP_CONSTANT,											// Produces a particular constant.
	OP_RETURN,												// Return from the current function.
} OP_CODE;

typedef struct 												// Will store the instructions' data.
{
	int			count;										// Amount of elements in use in the array.
	int			capacity;									// Amount of elements allocated in the array.
	uint8_t*	code;										// Dynamic array that stores the chunk's bytecode.
	int*		lines;										// Dynamic array that parallels the code array and that stores the line of the source code the byte in the bytecode was written in.
	ValueArray	constants;									// Dynamic array that stores the chunk's constants.
} Chunk;

void InitChunk	(Chunk* chunk);								// Initializes the chunk's variables.
void FreeChunk	(Chunk* chunk);								// Frees the memory allocated to the given Chunk.
void WriteChunk	(Chunk* chunk, uint8_t byte, int line);		// Appends the given byte to the end of the Chunk's array.
int  AddConstant(Chunk* chunk, Value value);				// Appends the given value to the end of the Chunk's constants array.

#endif // !__Ç_CHUNK_H__

