/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	show_alloc_mem(void)
{
	t_box	*tiny;
	t_box	*small;
	t_box	*large;
	size_t	total;

	if (lock_all_mutex() != 0)
		return ;
	tiny = g_malloc.tiny_boxes;
	small = g_malloc.small_boxes;
	large = g_malloc.large_boxes;
	print_boxes("TINY", tiny);
	print_boxes("SMALL", small);
	print_boxes("LARGE", large);
	total = sum_tag_size(tiny) + sum_tag_size(small) + sum_tag_size(large);
	write(1, "TOTAL : ", 8);
	print_tag_size(total);
	write(1, " bytes\n", 7);
	(void)unlock_all_mutex();
}

void	print_boxes(char *str, t_box *box)
{
	t_box	*prev;
	t_box	*cur;

	prev = NULL;
	while (1)
	{
		cur = find_next_box(box, prev);
		if (!cur)
			break ;
		ft_printf("%s : %p\n", str, cur);
		print_all_tags(cur);
		prev = cur;
	}
}

t_box	*find_next_box(t_box *box, t_box *prev)
{
	t_box	*candidate;

	candidate = NULL;
	while (box)
	{
		if ((prev == NULL || (uintptr_t)box > (uintptr_t)prev)
			&& (candidate == NULL
				|| (uintptr_t)box < (uintptr_t)candidate))
			candidate = box;
		box = box->next_box;
	}
	return (candidate);
}

void	print_all_tags(t_box *box)
{
	t_tag	*tag;
	char	*tag_start;
	char	*tag_end;

	tag = box->first_tag;
	while (tag)
	{
		if (tag->is_free == 0)
		{
			tag_start = (char *)(tag_to_user(tag));
			tag_end = tag_start + tag->origin_size;
			ft_printf("%p - %p : ", tag_start, tag_end);
			print_tag_size(tag->origin_size);
			write(1, " bytes\n", 7);
		}
		tag = tag->next_tag;
	}
}
