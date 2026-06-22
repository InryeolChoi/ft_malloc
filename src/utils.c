#include "ft_malloc.h"

size_t	will_add_overflow(size_t num1, size_t num2)
{
	size_t	max;

	max = (size_t) - 1;
	if (num1 > max - num2)
		return (0);
	return (num1 + num2);
}

size_t	will_multi_overflow(size_t num1, size_t num2)
{
	size_t	max;

	max = (size_t) - 1;
	if (num2 != 0 && num1 > max / num2)
		return (0);
	return (num1 * num2);
}
