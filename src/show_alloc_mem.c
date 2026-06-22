#include "ft_malloc.h"

void	show_alloc_mem()
{
	t_box	*tiny;
	t_box	*small;
	t_box	*large;

	tiny = g_malloc.tiny_boxes;
	small = g_malloc.small_boxes;
	large = g_malloc.large_boxes;

	print_boxes("TINY", tiny);
	print_boxes("SMALL", small);
	print_boxes("LARGE", large);
	print_total();
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
				tag_end = tag_start + tag->original_size;
				ft_printf("%p - %p : %d bytes\n", tag_start,
					tag_end, (int)(tag->original_size));
			}
			tag = tag->next_tag;
		}
		box = box->next_box;
	}
}

void	print_total()
{
	size_t	total;
}
