#include "ft_malloc.h"

void	free(void *ptr)
{
	t_box	*box;
	t_tag	*tag;

	if (ptr == NULL)
		return ;
	box = find_box(ptr);
	if (box == NULL)
		return ;
	tag = find_tag_in_box(box, ptr);
	if (tag == NULL)
		return ;
	if (tag->is_free == 1)
		return ;
	tag->is_free = 1;
}
