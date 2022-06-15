#ifndef __�_DEBUG_H__
#define __�_DEBUG_H__

#include "Chunk.h"

void DisassembleChunk			(Chunk* chunk, const char* name);
int  DisassembleInstruction		(Chunk* chunk, int offset);

static int ConstantInstruction		(const char* name, Chunk* chunk, int offset);
static int ConstantLongInstruction	(const char* name, Chunk* chunk, int offset);
static int SimpleInstruction		(const char* name, int offset);
static int DefaultInstruction		(uint8_t instruction, int offset);

#endif // !__�_DEBUG_H__