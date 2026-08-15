/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 23:46:53 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/11 23:46:53 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	show_alloc_mem_ex(void)
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
	print_boxes_ex("TINY", tiny);
	print_boxes_ex("SMALL", small);
	print_boxes_ex("LARGE", large);
	total = sum_tag_size(tiny) + sum_tag_size(small) + sum_tag_size(large);
	write(1, "TOTAL : ", 8);
	print_tag_size(total);
	write(1, " bytes\n", 7);
	(void)unlock_all_mutex();
	print_history();
}

void	print_boxes_ex(char *str, t_box *box)
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
		print_all_tags_ex(cur);
		prev = cur;
	}
}

void	print_all_tags_ex(t_box *box)
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
			print_user_area((unsigned char *)tag_to_user(tag),
				tag->origin_size);
		}
		tag = tag->next_tag;
	}
}

void	print_user_area(unsigned char *user_area, size_t size)
{
	size_t	index;

	index = 0;
	while (index < size)
	{
		if (index % 16 == 0)
		{
			if (index != 0)
				write(1, "\n", 1);
			ft_printf("%p ", user_area + index);
		}
		print_hex_byte(user_area[index]);
		write(1, " ", 1);
		index++;
	}
	write(1, "\n", 1);
}

void	print_hex_byte(unsigned char byte)
{
	char	*hex;
	char	output[2];

	hex = "0123456789abcdef";
	output[0] = hex[byte / 16];
	output[1] = hex[byte % 16];
	write(1, output, 2);
}
