#include "Memory.h"
#include "Value.h"

void InitValueArray(ValueArray* v_array)
{
	v_array->capacity	= 0;
	v_array->count		= 0;
	v_array->values		= NULL;
}

void FreeValueArray(ValueArray* v_array)
{
	FREE_ARRAY(Value, v_array->values, v_array->capacity);
	InitValueArray(v_array);
}

void WriteValueArray(ValueArray* v_array, Value value)
{
	if (v_array->capacity < v_array->count + 1)
	{
		int old_capacity	= v_array->capacity;
		v_array->capacity	= EXPAND_CAPACITY(old_capacity);
		v_array->values		= EXPAND_ARRAY(Value, v_array->values, old_capacity, v_array->capacity);
	}

	v_array->values[v_array->count] = value;
	++v_array->count;
}

void PrintValue(Value value)
{
	printf("%g", value);
}