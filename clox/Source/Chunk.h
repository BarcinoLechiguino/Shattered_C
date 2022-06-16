#ifndef __CLOX_CHUNK_H__
#define __CLOX_CHUNK_H__

#include "Common.h"
#include "Value.h"

typedef enum										// Number that controls what kind of instruction is being dealt with.
{
	OP_CONSTANT,									// OP_CODE 0: Produce a particular constant. Operand is 1 byte long	(256 max)
	OP_CONSTANT_LONG,								// OP_CODE 1: Produce a particular constant. Operand is 3 bytes long (16777216 max)
	
	OP_ADD,											// OP_CODE 2: Apply the addition binary operation of two particular constants.
	OP_SUBTRACT,									// OP_CODE 3: Apply the subtraction binary operation of two particular constants.
	OP_MULTIPLY,									// OP_CODE 4: Apply the multiplication binary operation of two particular constants.
	OP_DIVIDE,										// OP_CODE 5: Apply the division binary operation of two particular constants.
	OP_NEGATE,										// OP_CODE 6: Apply the unary negation of a particular constant.
	
	OP_RETURN,										// OP_CODE 7: Return from current function.
} OP_CODE;

typedef struct
{
	int			capacity;							// Total elements allocated.
	int			count;								// Total entries in use.
	uint8_t*	code;								// Bytecode.
	int*		lines;								// Line information.
	ValueArray	constants;							// Constant values.
} Chunk;

void InitChunk	(Chunk* chunk);
void FreeChunk	(Chunk* chunk);
void WriteChunk	(Chunk* chunk, uint8_t byte, int line);

void WriteConstant(Chunk* chunk, Value value, int line);
int  AddConstant(Chunk* chunk, Value value);

#endif // !__CLOX_CHUNK_H__