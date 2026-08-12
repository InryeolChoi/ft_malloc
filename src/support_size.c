/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

size_t	get_box_size(t_zone_type type, size_t user_size, size_t page)
{
	size_t	box_size;
	size_t	box_header;
	size_t	box_content;
	size_t	page_count;

	if (page == 0)
		return (0);
	box_header = align_size(sizeof(t_box));
	box_content = get_box_content_size(type, user_size);
	if (box_header == 0 || box_content == 0)
		return (0);
	box_size = will_add_overflow(box_header, box_content);
	if (box_size == 0 || box_size % page == 0)
		return (box_size);
	page_count = will_add_overflow(box_size / page, 1);
	if (page_count == 0)
		return (0);
	return (will_multi_overflow(page, page_count));
}

size_t	align_size(size_t size)
{
	size_t	remain;

	remain = size % ALIGNMENT;
	if (remain == 0)
		return (size);
	return (will_add_overflow(size, ALIGNMENT - remain));
}

size_t	get_box_content_size(t_zone_type type, size_t user_size)
{
	size_t	max_alloc;
	size_t	unit_size;
	size_t	count;

	count = 100;
	if (type == ZONE_LARGE)
	{
		max_alloc = user_size;
		count = 1;
	}
	else if (type == ZONE_SMALL)
		max_alloc = SMALL_MAX;
	else
		max_alloc = TINY_MAX;
	unit_size = will_add_overflow(align_size(sizeof(t_tag)), max_alloc);
	if (unit_size == 0)
		return (0);
	return (will_multi_overflow(count, unit_size));
}

size_t	get_basic_page_size(void)
{
	long	page_size;

	page_size = getpagesize();
	if (page_size <= 0)
		return (4096);
	return ((size_t)page_size);
}
