#include <stdio.h>

#include "Value.h"
#include "Debug.h"

void DisassembleChunk(Chunk* chunk, const char* name)
{
	printf("== %s ==\n", name);

	for (int offset = 0; offset < chunk->count;)					// offset is not incremented in the loop as instructions can have varying sizes.
	{
		offset = DisassembleInstruction(chunk, offset);
	}
}

int DisassembleInstruction(Chunk* chunk, int offset)
{
	printf("%04d ", offset);
	
	if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
	{
		printf("   | ");
	}
	else
	{
		printf("%4d ", chunk->lines[offset]);
	}

	uint8_t instruction = chunk->code[offset];
	switch (instruction)
	{
		case OP_CONSTANT:	{ return ConstantInstruction("OP_CONSTANT", chunk, offset); }	break;
		case OP_RETURN:		{ return SimpleInstruction("OP_RETURN", offset); }				break;
		default: 
			printf("Unknown opcode %d\n", instruction); 
			return offset + 1;	
		break;
	}
}

static int SimpleInstruction(const char* name, int offset)
{
	printf("%s\n", name);
	return offset + 1;															// offset + 1 so we go to the next byte, which contains the next instruction.
}

static int ConstantInstruction(const char* name, Chunk* chunk, int offset)
{
	uint8_t idx = chunk->code[offset + 1];										// offset + 1 so we go to the byte that contains the idx.
	printf("%-16s %4d '", name, idx);
	PrintValue(chunk->constants.values[idx]);
	printf("'\n");

	return offset + 2;															// offset + 2 so we go to the next byte, which contains the next instruction.
}