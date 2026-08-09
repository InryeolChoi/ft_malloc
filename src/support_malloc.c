/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_malloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	connect_to_boxlist(t_box *box)
{
	t_box	**box_list;
	t_box	*cur;

	if (box == NULL)
		return ;
	box_list = get_box_list(box->type);
	if ((*box_list) == NULL)
	{
		(*box_list) = box;
		return ;
	}
	cur = (*box_list);
	while (cur->next_box != NULL)
		cur = cur->next_box;
	cur->next_box = box;
}

t_tag	*make_tag(t_box *box, size_t box_front, size_t tag_front)
{
	t_tag	*tag;

	tag = (t_tag *)((char *)box + box_front);
	tag->capacity = box->size - box_front - tag_front;
	tag->origin_size = 0;
	tag->is_free = 1;
	tag->next_tag = NULL;
	tag->prev_tag = NULL;
	tag->magic = TAG_MAGIC;
	return (tag);
}

int	can_split_tag(t_tag *tag, size_t needed_size)
{
	size_t	remain;
	size_t	tag_header_size;

	if (needed_size > SMALL_MAX)
		return (0);
	if (tag->capacity < needed_size)
		return (0);
	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return (0);
	remain = tag->capacity - needed_size;
	if (remain >= tag_header_size + ALIGNMENT)
		return (1);
	return (0);
}

t_tag	*make_newtag(t_tag *tag, size_t used_size)
{
	t_tag	*newtag;
	size_t	tag_header_size;

	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return (NULL);
	newtag = (t_tag *)((char *)(tag_to_user(tag)) + used_size);
	newtag->capacity = tag->capacity - used_size - tag_header_size;
	newtag->origin_size = 0;
	newtag->magic = TAG_MAGIC;
	newtag->is_free = 1;
	newtag->next_tag = NULL;
	newtag->prev_tag = NULL;
	return (newtag);
}

void	set_newtag(t_tag *tag, t_tag *newtag)
{
	if (tag->next_tag)
	{
		tag->next_tag->prev_tag = newtag;
		newtag->prev_tag = tag;
		newtag->next_tag = tag->next_tag;
		tag->next_tag = newtag;
	}
	else
	{
		tag->next_tag = newtag;
		newtag->prev_tag = tag;
	}
}
