#include "Common.h"
#include "Chunk.h"
#include "Debug.h"

int main(int argc, const char* argv[])
{
	Chunk chunk;
	
	InitChunk(&chunk);
	
	WriteConstant(&chunk, 1.2, 123);
	WriteChunk(&chunk, OP_RETURN, 123);									// Writting a Return operation

	DisassembleChunk(&chunk, "test_chunk");
	FreeChunk(&chunk);
	
	return 0;
}