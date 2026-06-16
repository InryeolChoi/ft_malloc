#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	size_t		boxsize;
	t_zone_type	type;
	t_tag		*tag;

	if (size == 0)
		size = 1;
	size = align_size(size);
	if (size == 0)
		return NULL;
	type = get_zone_type(size);
	boxsize = get_box_size(type, size, get_basic_page_size());
	if (boxsize == 0)
		return NULL;
	tag = get_tag(size, type, boxsize);
	if (tag == NULL)
		return NULL;
	return tag_to_user(tag);
}

t_tag	*get_tag(size_t size, t_zone_type type, size_t boxsize)
{
	t_box	*box;
	t_tag	*tag;

	tag = find_tag(get_box_list(type), size);
	if (tag == NULL)
	{
		box = create_box(type, boxsize);
		if (box == NULL)
			return NULL;
		tag = set_tag(box->first_tag, size);
		if (tag == NULL)
		{
			munmap(box, box->size);
			return NULL;
		}
		connect_to_boxlist(box);
		return tag;
	}
	tag = set_tag(tag, size);
	if (tag == NULL)
		return NULL;
	return tag;
}

t_tag	*find_tag(t_box **box_list, size_t size)
{
	t_box	*box;
	t_tag	*tag;

	box = *box_list;
	while (box != NULL)
	{
		tag = box->first_tag;
		while (tag != NULL)
		{
			if (tag->magic == TAG_MAGIC && tag->is_free == 1
				&& tag->payload_size >= size)
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
	size_t	box_header_size;
	size_t	tag_header_size;

	box_header_size = align_size(sizeof(t_box));
	tag_header_size = align_size(sizeof(t_tag));
	if (box_header_size == 0 || tag_header_size == 0)
		return NULL;
	mem = mmap(NULL, boxsize, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return NULL;
	box = (t_box *)mem;
	box->size = boxsize;
	box->start = mem;
	box->first_tag = (t_tag *)((char *)box + box_header_size);
	box->type = type;
	box->next_box = NULL;

	tag = box->first_tag;
	tag->payload_size = box->size - box_header_size - tag_header_size;
	tag->is_free = 1;
	tag->next_tag = NULL;
	tag->prev_tag = NULL;
	tag->magic = TAG_MAGIC;
	return box;
}

t_tag	*set_tag(t_tag *tag, size_t needed_size)
{
	t_tag	*newtag;

	if (tag == NULL)
		return NULL;
	if (tag->payload_size < needed_size)
		return NULL;
	if (can_split_tag(tag, needed_size) == 1)
	{
		newtag = make_newtag(tag, needed_size);
		if (newtag == NULL)
			return NULL;
		set_newtag(tag, newtag);
		tag->payload_size = needed_size;
	}
	tag->is_free = 0;
	return tag;
}
