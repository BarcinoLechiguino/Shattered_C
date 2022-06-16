#include "Memory.h"

#include "Value.h"

void InitValueArray(ValueArray* valArray)
{
	valArray->capacity	= 0;
	valArray->count		= 0;
	valArray->values	= NULL;
}

void FreeValueArray(ValueArray* valArray)
{
	FREE_ARRAY(Value, valArray->values, valArray->capacity);
	InitValueArray(valArray);
}

void WriteValueArray(ValueArray* valArray, Value value)
{
	if (valArray->capacity < valArray->count + 1)
	{
		int oldCapacity		= valArray->capacity;
		valArray->capacity	= GROW_CAPACITY(oldCapacity);
		valArray->values	= GROW_ARRAY(Value, valArray->values, oldCapacity, valArray->capacity));		// Why does it need the second ')' at the end?
	}

	valArray->values[valArray->count] = value;
	++valArray->count;
}

void PrintValue(Value value)
{
	printf("%g", value);
}