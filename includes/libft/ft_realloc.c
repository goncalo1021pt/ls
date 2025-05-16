#include "libft.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;
	(void )old_size;

	if (ptr == NULL)
		return malloc(new_size);
	if (new_size == 0)
	{
		free(ptr);
		return NULL;
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return NULL;
	ft_memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
	free(ptr);
	return new_ptr;
}
