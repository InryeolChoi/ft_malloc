#include "ft_malloc.h"

void	connect_to_boxlist(t_box *box)
{
	t_box	**box_list;
	t_box	*cur;

	if (box->type == ZONE_TINY)
		box_list = &g_malloc.tiny_boxes;
	else if (box->type == ZONE_SMALL)
		box_list = &g_malloc.small_boxes;
	else
		box_list = &g_malloc.large_boxes;
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

int	can_split_tag(t_tag *tag, size_t new_user_area)
{
	size_t	remain;

	if (tag->user_area_size < new_user_area)
		return 0;
	remain = tag->user_area_size - new_user_area;
	if (remain >= sizeof(t_tag) + 1)
		return 1;
	return 0;
}

t_tag	*make_newtag(t_tag *tag, size_t new_user_area)
{
	t_tag	*newtag;

	newtag = (t_tag *)((char *)(tag_to_user(tag)) + new_user_area);
	newtag->user_area_size = tag->user_area_size - new_user_area - sizeof(t_tag);
	newtag->magic = TAG_MAGIC;
	newtag->is_free = 1;
	newtag->next_tag = NULL;
	newtag->prev_tag = NULL;
	return newtag;
}
