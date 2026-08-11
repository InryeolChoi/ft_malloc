/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boxes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/11 00:06:50 by inchoi           ###   ########.fr       */
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

t_box	*find_box_and_lock(void *ptr)
{
	t_box		*box;
	t_zone_type	type;

	type = ZONE_TINY;
	while (type <= ZONE_LARGE)
	{
		if (control_mutex(type, MUTEX_LOCK) != 0)
			return (NULL);
		box = find_box_list(*get_box_list(type), ptr);
		if (box != NULL)
			return (box);
		if (control_mutex(type, MUTEX_UNLOCK) != 0)
			return (NULL);
		type = (t_zone_type)(type + 1);
	}
	return (NULL);
}

t_box	*find_box_list(t_box *box_list, void *ptr)
{
	uintptr_t	address;
	uintptr_t	start;

	address = (uintptr_t)ptr;
	while (box_list != NULL)
	{
		start = (uintptr_t)box_list->start;
		if (address >= start && address - start < box_list->size)
			return (box_list);
		box_list = box_list->next_box;
	}
	return (NULL);
}
