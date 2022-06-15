#include <stdio.h>

#include "Value.h"

#include "Debug.h"

void DisassembleChunk(Chunk* chunk, const char* name)
{
	printf("== %s ==\n", name);

	for (int offset = 0; offset < chunk->count;)
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
	case OP_CONSTANT:		{ return ConstantInstruction("OP_CONSTANT", chunk, offset); }			break;
	case OP_CONSTANT_LONG:	{ return ConstantLongInstruction("OP_CONSTANT_LONG", chunk, offset); }	break;
	case OP_RETURN:			{ return SimpleInstruction("OP_RETURN", offset); }						break;
	default:				{ return DefaultInstruction(instruction, offset); }						break;
	}
}

static int ConstantInstruction(const char* name, Chunk* chunk, int offset)
{
	int constantIdx = chunk->code[offset + 1];														// Getting the index of the constant in the chunk's constants ValueArray.

	printf("%-16s %4d '", name, constantIdx);
	PrintValue(chunk->constants.values[constantIdx]);												// Getting the actual value with the constantIdx.
	printf("'\n");

	return (offset + 2);																			// +2 since there are 2 bytes assigned to the OP_CONSTANT operation.
}

static int ConstantLongInstruction(const char* name, Chunk* chunk, int offset)
{
	int constantIdx = chunk->code[offset + 1];														// Getting the index of the constant in the chunk's constants ValueArray.

	printf("%-16s %4d '", name, constantIdx);
	PrintValue(chunk->constants.values[constantIdx]);												// Getting the actual value with the constantIdx.
	printf("'\n");

	return (offset + 4);																			// +4 since there are 4 bytes assigned to the OP_CONSTANT_LONG operation.
}

static int SimpleInstruction(const char* name, int offset)
{
	printf("%s\n", name);
	return (offset + 1);
}

static int DefaultInstruction(uint8_t instruction, int offset)
{
	printf("Unknown OP_CODE: %d\n", instruction);
	return (offset + 1);
}