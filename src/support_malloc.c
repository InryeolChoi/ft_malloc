#include "ft_malloc.h"

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
