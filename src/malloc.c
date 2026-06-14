#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	size_t		boxsize;
	t_zone_type	type;
	t_tag		*tag;

	if (size == 0)
		size = 1;
	type = get_zone_type(size);
	boxsize = get_box_size(type, size, get_basic_page_size());
	if (boxsize == 0)
		return NULL;
	tag = find_tag(type, size, boxsize);
	if (tag == NULL)
		return NULL;
	return tag_to_user(tag);
}

t_tag	*find_tag(t_zone_type type, size_t size, size_t boxsize)
{
	t_box	*box;
	t_tag	*tag;

	tag = find_available_tag(get_box_list(type), size);
	if (tag == NULL)
	{
		box = create_box(type, boxsize);
		if (box == NULL)
			return NULL;
		connect_to_boxlist(box);
		tag = box->first_tag;
	}
	tag = allocate_tag(tag, size);
	if (tag == NULL)
		return NULL;
	return tag;
}


t_tag	*find_available_tag(t_box **box_list, size_t size)
{
	t_box	*box;
	t_tag	*tag;

	box = *box_list;
	while (box != NULL)
	{
		tag = box->first_tag;
		while (tag != NULL)
		{
			if (tag->magic == TAG_MAGIC && tag->is_free == 1 && tag->user_area_size >= size)
				return tag;
			tag = tag->next_tag;
		}
		box = box->next_box;
	}
	return NULL;
}

t_box	*create_box(t_zone_type type, size_t boxsize)
{
	void	*mem;
	t_box	*box;
	t_tag	*tag;

	mem = mmap(NULL, boxsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return NULL;
	box = (t_box *)mem;
	box->size = boxsize;
	box->start = mem;
	box->first_tag = (t_tag *)(box + 1);
	box->type = type;
	box->next_box = NULL;

	tag = box->first_tag;
	tag->user_area_size = box->size - sizeof(t_box) - sizeof(t_tag);
	tag->is_free = 1;
	tag->next_tag = NULL;
	tag->prev_tag = NULL;
	tag->magic = TAG_MAGIC;
	return box;
}

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

t_tag	*allocate_tag(t_tag *tag, size_t new_user_area)
{
	t_tag	*newtag;

	if (tag->user_area_size < new_user_area)
	    return NULL;
	if (can_split_tag(tag, new_user_area) == 1)
	{
		newtag = make_newtag(tag, new_user_area);
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
		tag->user_area_size = new_user_area;
	}
	tag->is_free = 0;
	return tag;
}
