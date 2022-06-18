#include "Memory.h"

#include "VM_Stack.h"

void InitStack(VM_Stack* stack)
{
	stack->capacity = 0;
	stack->count	= 0;
	stack->elements	= NULL;
	stack->top		= NULL;
}

void FreeStack(VM_Stack* stack)
{
	FREE_ARRAY(Value, stack->elements, stack->capacity);
	InitStack(stack);
}

static void ResetStack(VM_Stack* stack)
{
	stack->top = stack->elements;
}

void Push(VM_Stack* stack, Value value)
{
	if (stack->capacity < stack->count + 1)
	{
		int oldCapacity = stack->capacity;
		stack->capacity = GROW_CAPACITY(oldCapacity);
		stack->elements = GROW_ARRAY(Value, stack->elements, oldCapacity, stack->capacity));
		stack->top		= &stack->elements[stack->count];
	}
	
	*stack->top = value;
	++stack->count;
	++stack->top;
}

Value Pop(VM_Stack* stack)
{
	--stack->count;
	--stack->top;

	return (*stack->top);
}

void NegateTop(VM_Stack* stack)
{
	Value* constant = (stack->top - 1);
	*constant = -(*constant);
}