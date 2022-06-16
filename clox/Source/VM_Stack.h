#ifndef __CLOX_VM_STACK_H__
#define __CLOX_VM_STACK_H__

#include "Common.h"
#include "Value.h"

typedef struct
{
	int		capacity;
	int		count;
	Value*	elements;
	Value*	top;
} VM_Stack;

void		InitStack	(VM_Stack* stack);
void		FreeStack	(VM_Stack* stack);
//static void	ResetStack	(VM_Stack* stack);
void		Push		(VM_Stack* stack, Value value);
Value		Pop			(VM_Stack* stack);
void		NegateTop	(VM_Stack* stack);

#endif // !__CLOX_VM_STACK_H__