#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	size_t		boxsize;
	size_t		origin_size;
	t_zone_type	type;
	t_tag		*tag;

	if (size == 0)
		size = 1;
	origin_size = size;
	size = align_size(size);
	if (size == 0)
		return (NULL);
	type = get_zone_type(origin_size);
	boxsize = get_box_size(type, size, get_basic_page_size());
	if (boxsize == 0)
		return (NULL);
	tag = get_tag(origin_size, size, type, boxsize);
	if (tag == NULL)
		return (NULL);
	return (tag_to_user(tag));
}

t_tag	*get_tag(size_t origin_size, size_t needed_size, t_zone_type type,
			size_t boxsize)
{
	t_box	*box;
	t_tag	*tag;

	tag = find_tag(type, needed_size);
	if (tag == NULL)
	{
		box = create_box(type, boxsize);
		if (box == NULL)
			return (NULL);
		tag = set_tag(box->first_tag, origin_size, needed_size);
		if (tag == NULL)
		{
			munmap(box, box->size);
			return (NULL);
		}
		connect_to_boxlist(box);
		return (tag);
	}
	tag = set_tag(tag, origin_size, needed_size);
	if (tag == NULL)
		return (NULL);
	return (tag);
}

t_tag	*find_tag(t_zone_type type, size_t size)
{
	t_box	*box;
	t_tag	*tag;

	if (type == ZONE_LARGE)
		return (NULL);
	box = *(get_box_list(type));
	while (box != NULL)
	{
		tag = box->first_tag;
		while (tag != NULL)
		{
			if (tag->magic == TAG_MAGIC && tag->is_free == 1
				&& tag->capacity >= size)
				return (tag);
			tag = tag->next_tag;
		}
		box = box->next_box;
	}
	return (NULL);
}

t_box	*create_box(t_zone_type type, size_t boxsize)
{
	void	*mem;
	t_box	*box;
	size_t	box_front;
	size_t	tag_front;

	box_front = align_size(sizeof(t_box));
	tag_front = align_size(sizeof(t_tag));
	if (box_front == 0 || tag_front == 0)
		return (NULL);
	mem = mmap(NULL, boxsize, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return (NULL);
	box = (t_box *)mem;
	box->size = boxsize;
	box->start = mem;
	box->type = type;
	box->next_box = NULL;
	box->first_tag = make_tag(box, box_front, tag_front);
	return (box);
}

t_tag	*set_tag(t_tag *tag, size_t origin_size, size_t needed_size)
{
	t_tag	*newtag;

	if (tag == NULL)
		return (NULL);
	if (tag->capacity < needed_size)
		return (NULL);
	if (can_split_tag(tag, needed_size) == 1)
	{
		newtag = make_newtag(tag, needed_size);
		if (newtag == NULL)
			return (NULL);
		set_newtag(tag, newtag);
		tag->capacity = needed_size;
	}
	tag->origin_size = origin_size;
	tag->is_free = 0;
	return (tag);
}
