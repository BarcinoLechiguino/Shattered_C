#include <stdlib.h>

#include "Memory.h"

void* Reallocate(void* pointer, size_t old_size, size_t new_size)
{
	if (new_size == 0)
	{
		free(pointer);
		return NULL;
	}
	
	void* result = realloc(pointer, new_size);									// realloc() returns NULL when there is not enough memory available.
	if (result == NULL)
	{
		exit(1);
	}

	return result;																// Returning result instead of pointer as result might be a different address than pointer due to array expansions.
}