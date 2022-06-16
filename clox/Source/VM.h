#ifndef __CLOX_VM_H__
#define __CLOX_VM_H__

#include "Chunk.h"
#include "Value.h"

#define STACK_MAX 256

typedef struct
{
	Chunk*	 chunk;											// Chunk being currently executed by the Virtual Machine.
	uint8_t* ip;											// Cursor that keeps track of where in the bytecode the VM is while it is executing the instructions.
	Value	 stack[STACK_MAX];
	Value*	 stackTop;
} VM;

typedef struct
{
	int		capacity;
	int		count;
	Value*	stack;
	Value*	stackTop;
} Stack;

typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

void		InitVM		(VM* vm);
void		FreeVM		(VM* vm);

void		InitStack	(Stack* stack);
void		FreeStack	(Stack* stack);
static void	ResetStack	(VM* vm);
void		Push		(VM* vm, Value value);
Value		Pop			(VM* vm);
void		NegateTop	(VM* vm);

InterpretResult			Interpret	(VM* vm, Chunk* chunk);
static InterpretResult	Run			(VM* vm);

#endif // !__CLOX_VM_H__