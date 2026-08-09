/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boxes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_box	*find_box_pool(void *ptr)
{
	t_box	*box;

	box = find_box_list(g_malloc.tiny_boxes, ptr);
	if (box != NULL)
		return (box);
	box = find_box_list(g_malloc.small_boxes, ptr);
	if (box != NULL)
		return (box);
	box = find_box_list(g_malloc.large_boxes, ptr);
	if (box != NULL)
		return (box);
	return (NULL);
}

t_box	*find_box_list(t_box *box_list, void *ptr)
{
	while (box_list != NULL)
	{
		if (is_ptr_in_box(box_list, ptr))
			return (box_list);
		box_list = box_list->next_box;
	}
	return (NULL);
}

int	is_ptr_in_box(t_box *box, void *ptr)
{
	char	*newptr;
	char	*start;
	char	*end;

	newptr = (char *)ptr;
	start = (char *)(box->start);
	end = start + box->size;
	if (start <= newptr && newptr < end)
		return (1);
	return (0);
}
