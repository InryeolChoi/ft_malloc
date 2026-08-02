#include "ft_malloc.h"

void	free(void *ptr)
{
	t_box	*box;
	t_tag	*tag;

	if (ptr == NULL)
		return ;
	box = find_box_pool(ptr);
	if (box == NULL)
		return ;
	tag = find_tag_in_box(box, ptr);
	if (tag == NULL)
		return ;
	if (tag->is_free == 1)
		return ;
	tag->origin_size = 0;
	tag->is_free = 1;
	tag = merge_with_prev(tag);
	tag = merge_with_next(tag);
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
