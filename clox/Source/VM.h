#ifndef __CLOX_VM_H__
#define __CLOX_VM_H__

#include "Chunk.h"
#include "VM_Stack.h"

typedef struct
{
	Chunk*	 chunk;													// Chunk being currently executed by the Virtual Machine.
	uint8_t* ip;													// Cursor that keeps track of where in the bytecode the VM is while it is executing the instructions.
	VM_Stack stack;
} VM;

typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

void InitVM(VM* vm);
void FreeVM(VM* vm);

InterpretResult			Interpret	(const char* source);
static InterpretResult	Run			(VM* vm);

void DebugTraceExecution(VM* vm);

#endif // !__CLOX_VM_H__