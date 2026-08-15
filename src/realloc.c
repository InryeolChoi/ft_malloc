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
	void		*new_ptr;
	uintptr_t	old_address;

	(void)control_mutex(type, MUTEX_UNLOCK);
	new_ptr = malloc(1);
	if (new_ptr == NULL)
		return (NULL);
	old_address = (uintptr_t)ptr;
	free(ptr);
	record_history(HISTORY_REALLOC, (void *)old_address, new_ptr, 0);
	return (new_ptr);
}

void	*remake_origin(void *ptr, size_t size, t_tag *tag, t_zone_type type)
{
	size_t	needed_size;
	t_tag	*newtag;

	debug_scribble_resize(tag, size);
	needed_size = align_size(size);
	if (type != ZONE_LARGE && size < tag->origin_size
		&& can_split_tag(tag, needed_size))
	{
		newtag = make_newtag(tag, needed_size);
		if (newtag != NULL)
		{
			set_newtag(tag, newtag);
			tag->capacity = needed_size;
			merge_with_next(newtag);
		}
	}
	tag->origin_size = size;
	(void)control_mutex(type, MUTEX_UNLOCK);
	record_history(HISTORY_REALLOC, ptr, ptr, size);
	return (ptr);
}

void	*remake_newptr(void *ptr, size_t size, t_tag *tag, t_zone_type type)
{
	void		*new_ptr;
	size_t		copy_size;
	uintptr_t	old_address;

	if (get_zone_type(size) == type
		&& tag->next_tag != NULL
		&& tag->next_tag->is_free == 1
		&& size <= tag->capacity + align_size(sizeof(t_tag))
		+ tag->next_tag->capacity)
		return (remake_addptr(ptr, size, tag, type));
	copy_size = tag->origin_size;
	(void)control_mutex(type, MUTEX_UNLOCK);
	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, copy_size);
	old_address = (uintptr_t)ptr;
	free(ptr);
	record_history(HISTORY_REALLOC, (void *)old_address, new_ptr, size);
	return (new_ptr);
}

void	*remake_addptr(void *ptr, size_t size, t_tag *tag, t_zone_type type)
{
	size_t	needed_size;
	t_tag	*newtag;

	needed_size = align_size(size);
	merge_with_next(tag);
	if (can_split_tag(tag, needed_size))
	{
		newtag = make_newtag(tag, needed_size);
		if (newtag != NULL)
		{
			set_newtag(tag, newtag);
			tag->capacity = needed_size;
		}
	}
	return (remake_origin(ptr, size, tag, type));
}
