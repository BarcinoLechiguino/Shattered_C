#include "Common.h"
#include "Chunk.h"

int main(int argc, char** argv)
{
	Chunk chunk;

	InitChunk(&chunk);
	WriteChunk(&chunk, OP_RETURN);
	FreeChunk(&chunk);
	
	return 0;
}