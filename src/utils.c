#include "ft_malloc.h"

size_t	get_basic_page_size(void)
{
	long	page_size;

#if defined(__APPLE__)
	page_size = getpagesize();
#elif defined(__linux__)
	page_size = sysconf(_SC_PAGESIZE);
#else
	page_size = sysconf(_SC_PAGESIZE);
#endif
	if (page_size <= 0)
		return (4096);
	return ((size_t)page_size);
}

size_t	get_box_size(t_zone_type type, size_t user_size, size_t page)
{
	size_t	boxsize;
	size_t	max_alloc;
	size_t	multi_num;

	max_alloc = (type == ZONE_LARGE ? user_size : (type == ZONE_SMALL ? SMALL_MAX : TINY_MAX));
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

size_t	will_add_overflow(size_t num1, size_t num2)
{
	size_t max;

	max = (size_t) - 1;
	if (num1 > max - num2)
		return 0;
	return num1 + num2;
}

size_t	will_multi_overflow(size_t num1, size_t num2)
{
	size_t max;

	max = (size_t) - 1;
	if (num2 != 0 && num1 > max / num2)
		return 0;
	return num1 * num2;
}
