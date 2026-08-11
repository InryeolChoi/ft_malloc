/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	*realloc(void *ptr, size_t size)
{
	t_tag		*tag;
	t_box		*box;
	t_zone_type	type;

	if (ptr == NULL)
		return (malloc(size));
	box = find_box_and_lock(ptr);
	if (box == NULL)
		return (NULL);
	type = box->type;
	tag = find_tag_in_box(box, ptr);
	if (tag == NULL || tag->is_free == 1)
	{
		(void)control_mutex(type, MUTEX_UNLOCK);
		return (NULL);
	}
	if (size == 0)
		return (remake_minptr(ptr, type));
	if (size <= tag->capacity)
		return (remake_origin(ptr, size, tag, type));
	return (remake_newptr(ptr, size, tag, type));
}

void	*remake_minptr(void *ptr, t_zone_type type)
{
	void	*new_ptr;

	(void)control_mutex(type, MUTEX_UNLOCK);
	new_ptr = malloc(1);
	if (new_ptr == NULL)
		return (NULL);
	free(ptr);
	return (new_ptr);
}

void	*remake_origin(void *ptr, size_t size, t_tag *tag, t_zone_type type)
{
	tag->origin_size = size;
	(void)control_mutex(type, MUTEX_UNLOCK);
	return (ptr);
}

void	*remake_newptr(void *ptr, size_t size, t_tag *tag, t_zone_type type)
{
	void	*new_ptr;
	size_t	copy_size;

	copy_size = tag->origin_size;
	(void)control_mutex(type, MUTEX_UNLOCK);
	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}
