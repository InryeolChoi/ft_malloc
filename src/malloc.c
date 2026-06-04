#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	(void)size;
	return (NULL);
}
