#include "Common.h"
#include "Chunk.h"
#include "Debug.h"

int main(int argc, char** argv)
{	
	Chunk chunk;

	InitChunk(&chunk);

	int constant = AddConstant(&chunk, 1.2);							// Idx of the constant in the chunk's constants ValueArray.
	WriteChunk(&chunk, OP_CONSTANT, 123);								// Appending the opcode for the added constant to the chunk's code array.
	WriteChunk(&chunk, constant, 123);									// Appending the idx of the constant to the chunk's code array.

	WriteChunk(&chunk, OP_RETURN, 123);

	DisassembleChunk(&chunk, "test chunk");

	FreeChunk(&chunk);
	
	return 0;
}