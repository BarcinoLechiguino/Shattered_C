#include <stdio.h>

#include "Common.h"
#include "Debug.h"

#include "VM.h"

void InitVM(VM* vm)
{
	ResetStack(vm);
}

void FreeVM(VM* vm)
{

}

static void ResetStack(VM* vm)
{
	vm->stackTop = vm->stack;
}

void Push(VM* vm, Value value)
{
	*vm->stackTop = value;
	++vm->stackTop;
}

Value Pop(VM* vm)
{
	--vm->stackTop;
	return (*vm->stackTop);
}

void NegateTop(VM* vm)
{
	Value* constant = (vm->stackTop - 1); 
	*constant = -(*constant);
}

InterpretResult Interpret(VM* vm, Chunk* chunk)
{
	vm->chunk	= chunk;
	vm->ip		= vm->chunk->code;

	return Run(vm);
}

static InterpretResult Run(VM* vm)
{
	#define READ_BYTE()		(*vm->ip++)																// Ex: Returns instruction 0 and increases cursor to instruction 1.
	#define READ_CONSTANT()	(vm->chunk->constants.values[READ_BYTE()])
	#define BINARY_OP(op)			\
		do							\
		{							\
			double b = Pop(vm);		\
			double a = Pop(vm);		\
			Push(vm, (a op b));		\
		} while (false)				

	for (;;)																						// Infinite loop
	{
		#ifdef DEBUG_TRACE_EXECUTION
			printf("          ");
			for (Value* slot = vm->stack; slot < vm->stackTop; ++slot)
			{
				printf("[ "); PrintValue(*slot); printf(" ]");
			}

			if (vm->stackTop == vm->stack)
			{
				printf("[ EMPTY STACK ]");
			}
			printf("\n");

			DisassembleInstruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
		#endif

		uint8_t instruction;
		switch (instruction = READ_BYTE())															// Should functions be made or is the overhead in this part of the critical path not worth it?
		{
		case OP_CONSTANT:		{ Push(vm, READ_CONSTANT());}								break;
		case OP_CONSTANT_LONG:	{ Push(vm, READ_CONSTANT());}								break;
		case OP_ADD:			{ BINARY_OP(+); }											break;
		case OP_SUBTRACT:		{ BINARY_OP(-); }											break;
		case OP_MULTIPLY:		{ BINARY_OP(*); }											break;
		case OP_DIVIDE:			{ BINARY_OP(/); }											break;
		case OP_NEGATE:			{ NegateTop(vm); }											break; // Push(vm, -Pop(vm)); would also work, but it implies unnecessary operations.
		case OP_RETURN:			{ PrintValue(Pop(vm)); printf("\n"); return INTERPRET_OK; }	break;
		}
	}

	#undef READ_BYTE
	#undef READ_CONSTANT
	#undef BINARY_OP
}