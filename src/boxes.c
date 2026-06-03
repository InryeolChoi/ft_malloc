#include "ft_malloc.h"

t_zone_type	get_zone_type(size_t size)
{
	if (size <= TINY_MAX)
		return (ZONE_TINY);
	if (size <= SMALL_MAX)
		return (ZONE_SMALL);
	return (ZONE_LARGE);
}

t_box	**get_box_list(t_zone_type type)
{
	if (type == ZONE_TINY)
		return (&g_malloc.tiny_boxes);
	if (type == ZONE_SMALL)
		return (&g_malloc.small_boxes);
	return (&g_malloc.large_boxes);
}

