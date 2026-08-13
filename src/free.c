/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	free(void *ptr)
{
	t_box		*box;
	t_tag		*tag;
	t_zone_type	type;

	if (ptr == NULL)
		return ;
	box = find_box_and_lock(ptr);
	if (box == NULL)
	{
		debug_free_error(NULL);
		return ;
	}
	type = box->type;
	tag = find_tag_in_box(box, ptr);
	debug_free_error(tag);
	if (tag != NULL && tag->is_free == 0)
	{
		tag->origin_size = 0;
		tag->is_free = 1;
		tag = merge_with_prev(tag);
		tag = merge_with_next(tag);
		if (can_unmap_box(box, tag) == 1)
			unmap_box(box);
	}
	(void)control_mutex(type, MUTEX_UNLOCK);
}

t_tag	*merge_with_prev(t_tag *tag)
{
	t_tag	*prev_tag;

	prev_tag = tag->prev_tag;
	if (prev_tag == NULL || prev_tag->is_free == 0)
		return (tag);
	prev_tag->capacity += align_size(sizeof(t_tag)) + tag->capacity;
	prev_tag->next_tag = tag->next_tag;
	if (tag->next_tag != NULL)
		tag->next_tag->prev_tag = prev_tag;
	return (prev_tag);
}

t_tag	*merge_with_next(t_tag *tag)
{
	t_tag	*next_tag;

	next_tag = tag->next_tag;
	if (next_tag == NULL || next_tag->is_free == 0)
		return (tag);
	tag->capacity += align_size(sizeof(t_tag)) + next_tag->capacity;
	tag->next_tag = next_tag->next_tag;
	if (next_tag->next_tag != NULL)
		next_tag->next_tag->prev_tag = tag;
	return (tag);
}

int	can_unmap_box(t_box *box, t_tag *tag)
{
	t_box	*boxlist;

	if (tag == box->first_tag && tag->is_free == 1
		&& tag->next_tag == NULL)
	{
		if (box->type == ZONE_LARGE)
			return (1);
		boxlist = *(get_box_list(box->type));
		while (boxlist != NULL)
		{
			if (boxlist != box)
				return (1);
			boxlist = boxlist->next_box;
		}
		return (0);
	}
	return (0);
}

void	unmap_box(t_box *box)
{
	t_box		*current;
	t_box		*previous;
	t_box		*next;
	t_zone_type	type;

	type = box->type;
	current = *(get_box_list(type));
	previous = NULL;
	while (current != NULL && current != box)
	{
		previous = current;
		current = current->next_box;
	}
	if (current == NULL)
		return ;
	next = current->next_box;
	if (munmap(current, current->size) != 0)
		return ;
	if (previous == NULL)
		*(get_box_list(type)) = next;
	else
		previous->next_box = next;
}
