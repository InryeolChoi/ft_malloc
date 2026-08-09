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
	t_tag	*tag;
	t_box	*box;

	if (ptr == NULL)
		return (malloc(size));
	box = find_box_pool(ptr);
	if (box == NULL)
		return (NULL);
	tag = find_tag_in_box(box, ptr);
	if (tag == NULL || tag->is_free == 1)
		return (NULL);
	if (size == 0)
		return (remake_minptr(ptr));
	if (size > 0 && size <= tag->capacity)
		return (remake_origin(ptr, size, tag));
	if (size > tag->capacity)
		return (remake_newptr(ptr, size, tag));
	return (NULL);
}

void	*remake_minptr(void *ptr)
{
	void	*new_ptr;

	new_ptr = malloc(1);
	if (new_ptr == NULL)
		return (NULL);
	free(ptr);
	return (new_ptr);
}

void	*remake_origin(void *ptr, size_t size, t_tag *tag)
{
	tag->origin_size = size;
	return (ptr);
}

void	*remake_newptr(void *ptr, size_t size, t_tag *tag)
{
	void	*new_ptr;

	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, tag->origin_size);
	free(ptr);
	return (new_ptr);
}
