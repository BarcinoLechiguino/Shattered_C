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

void InitValueArray		(ValueArray* v_array);
void FreeValueArray		(ValueArray* v_array);
void WriteValueArray	(ValueArray* v_array, Value value);

void PrintValue(value);

#endif __�_VALUE_H__


