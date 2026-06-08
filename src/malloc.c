#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	(void)size;
	return NULL;
}

size_t	get_box_size(t_zone_type type, size_t user_size, size_t page)
{
	size_t	boxsize;
	size_t	max_alloc;
	size_t	multi_num;

	max_alloc = (type == ZONE_LARGE ? user_size :
		(type == ZONE_SMALL ? SMALL_MAX : TINY_MAX));
	multi_num = (type == ZONE_LARGE ? 1 : 100);

	if ((boxsize = will_add_overflow(sizeof(t_tag), max_alloc)) == 0)
		return 0;
	if ((boxsize = will_multi_overflow(multi_num, boxsize)) == 0)
		return 0;
	if ((boxsize = will_add_overflow(sizeof(t_box), boxsize)) == 0)
		return 0;

	if (boxsize % page == 0)
		return boxsize;
	if ((boxsize = will_multi_overflow(page, boxsize / page + 1)) == 0)
		return 0;
	return boxsize;
}
