#include "ft_malloc.h"

void	show_alloc_mem(void)
{
	t_box	*tiny;
	t_box	*small;
	t_box	*large;
	size_t	total;

	tiny = g_malloc.tiny_boxes;
	small = g_malloc.small_boxes;
	large = g_malloc.large_boxes;
	print_boxes("TINY", tiny);
	print_boxes("SMALL", small);
	print_boxes("LARGE", large);
	total = sum_tag_size(tiny) + sum_tag_size(small) + sum_tag_size(large);
	write(1, "TOTAL : ", 8);
	print_size(total);
	write(1, " bytes\n", 7);
}

void	print_boxes(char *str, t_box *box)
{
	t_tag	*tag;
	char	*tag_start;
	char	*tag_end;

	while (box)
	{
		ft_printf("%s : %p\n", str, box);
		tag = box->first_tag;
		while (tag)
		{
			if (tag->is_free == 0)
			{
				tag_start = (char *)(tag_to_user(tag));
				tag_end = tag_start + tag->origin_size;
				ft_printf("%p - %p : ", tag_start, tag_end);
				print_size(tag->origin_size);
				write(1, " bytes\n", 7);
			}
			tag = tag->next_tag;
		}
		box = box->next_box;
	}
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

void	print_size(size_t size)
{
	char	digit;

	if (size >= 10)
		print_size(size / 10);
	digit = '0' + size % 10;
	write(1, &digit, 1);
}
