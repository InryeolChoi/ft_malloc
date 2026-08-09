/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_tags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	*tag_to_user(t_tag *tag)
{
	size_t	tag_header_size;

	if (tag == NULL)
		return (NULL);
	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return (NULL);
	return ((char *)tag + tag_header_size);
}

t_tag	*user_to_tag(void *ptr)
{
	t_tag	*tag;
	size_t	tag_header_size;

	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return (NULL);
	tag = (t_tag *)((char *)ptr - tag_header_size);
	return (tag);
}

t_tag	*find_tag_in_box(t_box *box, void *ptr)
{
	t_tag	*tag;

	tag = box->first_tag;
	while (tag)
	{
		if (ptr == tag_to_user(tag))
			return (tag);
		tag = tag->next_tag;
	}
	return (NULL);
}

size_t	sum_tag_size(t_box *box)
{
	t_tag	*tag;
	size_t	total_size;

	total_size = 0;
	while (box)
	{
		tag = box->first_tag;
		while (tag)
		{
			if (tag->is_free == 0)
				total_size += tag->origin_size;
			tag = tag->next_tag;
		}
		box = box->next_box;
	}
	return (total_size);
}

void	print_tag_size(size_t size)
{
	char	digit;

	if (size >= 10)
		print_tag_size(size / 10);
	digit = '0' + size % 10;
	write(1, &digit, 1);
}
