#ifndef __CLOX_MEMORY_H__
#define __CLOX_MEMORY_H__

#include "Common.h"

#define GROW_CAPACITY(capacity)							( ((capacity) < 8) ? 8 : (capacity) * 2 )												// CHANGE GROW FOR EXPAND?
#define GROW_ARRAY(type, pointer, oldCount, newCount)	((type*)Reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount) )
#define FREE_ARRAY(type, pointer, oldCount)				( Reallocate(pointer, sizeof(type) * (oldCount), 0) )

void* Reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif // !__CLOX_MEMORY_H__