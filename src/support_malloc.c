#include "ft_malloc.h"

void	connect_to_boxlist(t_box *box)
{
	t_box	**box_list;
	t_box	*cur;

	if (box == NULL)
		return ;
	box_list = get_box_list(box->type);
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

int	can_split_tag(t_tag *tag, size_t needed_size)
{
	size_t	remain;
	size_t	tag_header_size;

	if (tag->capacity < needed_size)
		return 0;
	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return 0;
	remain = tag->capacity - needed_size;
	if (remain >= tag_header_size + ALIGNMENT)
		return 1;
	return 0;
}

t_tag	*make_newtag(t_tag *tag, size_t used_size)
{
	t_tag	*newtag;
	size_t	tag_header_size;

	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return NULL;
	newtag = (t_tag *)((char *)(tag_to_user(tag)) + used_size);
	newtag->capacity = tag->capacity - used_size - tag_header_size;
	newtag->original_size = 0;
	newtag->magic = TAG_MAGIC;
	newtag->is_free = 1;
	newtag->next_tag = NULL;
	newtag->prev_tag = NULL;
	return newtag;
}

void	set_newtag(t_tag *tag, t_tag *newtag)
{
	if (tag->next_tag)
	{
		tag->next_tag->prev_tag = newtag;
		newtag->prev_tag = tag;
		newtag->next_tag = tag->next_tag;
		tag->next_tag = newtag;
	}
	else
	{
		tag->next_tag = newtag;
		newtag->prev_tag = tag;
	}
}
