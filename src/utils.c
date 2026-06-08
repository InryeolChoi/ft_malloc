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
