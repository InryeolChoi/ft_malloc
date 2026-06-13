#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	size_t		boxsize;
	t_zone_type	type;
	t_box		*box;
	t_tag		*tag;

	if (size == 0)
		size = 1;
	type = get_zone_type(size);
	if ((boxsize = get_box_size(type, size, get_basic_page_size())) == 0)
		return NULL;
	if ((tag = find_available_tag(get_box_list(type), size)) == NULL)
	{
		box = create_box(type, boxsize);
		if (box == NULL)
			return NULL;
		connect_to_boxlist(box);
		tag = box->first_tag;
	}
	tag = allocate_tag(tag, size);
	return tag_to_user(tag);
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

	if (box->size <= TINY_MAX)
		box_list = &g_malloc.tiny_boxes;
	else if (TINY_MAX < box->size && box->size <= SMALL_MAX)
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

t_tag	*allocate_tag(t_tag *tag, size_t user_area)
{
	t_tag	*newtag;

	tag->is_free = 0;
	if (tag->user_area_size < user_area)
	    return NULL;
	if (tag->user_area_size - user_area >= sizeof(t_tag) + 1)
	{
		newtag = (t_tag *)((char *)(tag_to_user(tag)) + user_area);
		newtag->user_area_size = tag->user_area_size - user_area - sizeof(t_tag);
		newtag->magic = TAG_MAGIC;
		newtag->is_free = 1;
		if (tag->next_tag == NULL)
			newtag->next_tag = NULL;
		else
		{
			tag->next_tag->prev_tag = newtag;
			newtag->next_tag = tag->next_tag;
		}
		newtag->prev_tag = tag;
		tag->next_tag = newtag;
	}
	else
		tag->user_area_size = user_area;
	return tag;
}
