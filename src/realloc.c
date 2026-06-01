#include "ft_malloc.h"

void *realloc(void *ptr, size_t size)
{
	if (size == 0)
	{
		// ptr은 역참조를 하기 애매
		if (ptr != NULL)
		{
			
		}
	}
}
