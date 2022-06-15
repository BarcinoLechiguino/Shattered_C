#ifndef __CLOX_VALUE_H__
#define __CLOX_VALUE_H__

#include "Common.h"

typedef double Value;

typedef struct
{
	int		capacity;
	int		count;
	Value*	values;
} ValueArray;

void InitValueArray(ValueArray* valArray);
void FreeValueArray(ValueArray* valArray);
void WriteValueArray(ValueArray* valArray, Value value);

void PrintValue(Value value);

#endif // !__CLOX_VALUE_H__