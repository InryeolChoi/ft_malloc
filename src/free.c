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
	if (tag == NULL || tag->is_free == 1)
		return ;
	tag->origin_size = 0;
	tag->is_free = 1;
	tag = merge_with_prev(tag);
	tag = merge_with_next(tag);
	if (can_unmap_box(box, tag) == 1)
		unmap_box(box);
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
	t_box	**link;

	link = get_box_list(box->type);
	while ((*link) != NULL && (*link) != box)
		link = &((*link)->next_box);
	if ((*link) != box)
		return ;
	(*link) = box->next_box;
	munmap(box, box->size);
}
