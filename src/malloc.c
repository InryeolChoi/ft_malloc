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
	if ((boxsize = get_box_size(type, size, get_basic_page_size())))
		return NULL;
	if ((tag = find_available_tag(get_box_list(type), size)) == NULL)
	{
		box = create_box(type, boxsize);
		if (box == NULL)
			return NULL;
		// 새 box를 기존 box list 리스트 3가지 중 하나에 넣기

		// box에서 tag 발라내기
		tag = box->first_tag;
	}
	return tag_to_user(tag);
}

size_t	get_box_size(t_zone_type type, size_t user_size, size_t page)
{
	size_t	boxsize;
	size_t	max_alloc;
	size_t	multi_num;

	max_alloc = (type == ZONE_LARGE ? user_size : (type == ZONE_SMALL ? SMALL_MAX : TINY_MAX));
	multi_num = (type == ZONE_LARGE ? 1 : 100);

	if ((boxsize = will_add_overflow(sizeof(t_tag), max_alloc)) == 0)
		return 0;
	if ((boxsize = will_multi_overflow(multi_num, boxsize)) == 0)
		return 0;
	if ((boxsize = will_add_overflow(sizeof(t_box), boxsize)) == 0)
		return 0;

	if (boxsize % page == 0)
		return boxsize;
	if ((boxsize = will_multi_overflow(page, boxsize / page + 1)) == 0)
		return 0;
	return boxsize;
}

t_tag	*find_available_tag(t_box **box_list, size_t size)
{
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
