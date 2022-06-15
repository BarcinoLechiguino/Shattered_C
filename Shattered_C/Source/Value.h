#ifndef __�_VALUE_H__
#define __�_VALUE_H__

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

#endif // !__�_VALUE_H__