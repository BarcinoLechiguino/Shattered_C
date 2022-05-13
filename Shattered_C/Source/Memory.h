#ifndef __�_MEMORY_H__
#define __�_MEMORY_H__

#include "Common.h"

#define EXPAND_CAPACITY(capacity)							( (capacity < 8) ? 8 : capacity * 2 )
#define EXPAND_ARRAY(type, pointer, old_count, new_count)	( (type*)Reallocate(pointer, sizeof(type) * old_count, sizeof(type) * new_count) )
#define FREE_ARRAY(type, pointer, old_count)				( Reallocate(pointer, sizeof(type) * old_count, 0) )

void* Reallocate(void* pointer, size_t old_size, size_t new_size);

#endif // !__�_MEMORY_H__