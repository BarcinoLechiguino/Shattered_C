#include "Common.h"
#include "Chunk.h"
#include "Debug.h"
#include "VM.h"

#include "Memory.h"

int main(int argc, const char* argv[])
{
	VM vm;
	Chunk chunk;
	
	InitVM(&vm);
	InitChunk(&chunk);

	//for (int i = 0; i < 300; ++i)
	//{
	//	WriteConstant(&chunk, i, 123);
	//}

	// a + (b * c) - (d / (-e))
	WriteConstant(&chunk, 1, 123);
	
	WriteConstant(&chunk, 2, 123);
	WriteConstant(&chunk, 3, 123);
	WriteChunk(&chunk, OP_MULTIPLY, 123);
	
	WriteConstant(&chunk, 4, 123);
	WriteConstant(&chunk, 5, 123);
	WriteChunk(&chunk, OP_NEGATE, 123);
	WriteChunk(&chunk, OP_DIVIDE, 123);
	
	WriteChunk(&chunk, OP_SUBTRACT, 123);
	WriteChunk(&chunk, OP_ADD, 123);
	
	WriteChunk(&chunk, OP_RETURN, 123);									// Writting a Return operation

	//DisassembleChunk(&chunk, "test_chunk");
	Interpret(&vm, &chunk);

	FreeVM(&vm);
	FreeChunk(&chunk);

	return 0;
}