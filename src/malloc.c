#include "ft_malloc.h"

t_malloc_state	g_malloc = {0};

void	*malloc(size_t size)
{
	size_t		boxsize;
	t_zone_type	type;
	t_box		*box;
	t_box		**box_list;

	/*
	 * malloc 설계 흐름:
	 *
	 * 1. 먼저 size == 0 정책을 정한다.
	 *    malloc(0), realloc(ptr, 0)의 동작이 서로 어긋나지 않게 한다.
	 *
	 * 2. 요청 size로 zone type을 정한다.
	 *    type = get_zone_type(size)
	 *
	 * 3. type에 맞는 box list를 고른다.
	 *    box_list = get_box_list(type)
	 *
	 * 4. 새 box를 만들기 전에 기존 box들을 먼저 뒤진다.
	 *    아직 필요한 helper:
	 *    - find_free_tag_in_box(box, size)
	 *    - find_free_tag_in_boxes(*box_list, size)
	 *
	 *    malloc에 쓸 수 있는 tag 조건:
	 *    - tag->magic == TAG_MAGIC
	 *    - tag->is_free == 1
	 *    - tag->user_area_size >= size
	 *
	 * 5. 재사용 가능한 free tag가 있으면 그 tag를 할당 상태로 바꾼다.
	 *    나중에는 남은 공간이 충분할 때 tag를 split해야 한다.
	 *
	 * 6. 재사용 가능한 tag가 없을 때만 새 box를 만든다.
	 *    create_box(type, boxsize)는 mmap으로 메모리를 받고
	 *    t_box와 첫 번째 free t_tag를 초기화해야 한다.
	 *
	 * 7. 새 box를 *box_list에 연결한다.
	 *
	 * 8. 새 box의 첫 free tag에서 할당한다.
	 *
	 * 9. malloc의 반환값은 box 주소가 아니라 tag_to_user(tag)다.
	 *
	 * 현재 아래 코드는 create_box로 너무 빨리 간다.
	 * 다음에는 기존 box에서 재사용 가능한 free tag를 찾는 함수부터 만든다.
	 */
	type = get_zone_type(size);
	boxsize = get_box_size(type, size, get_basic_page_size());
	box = create_box(type, boxsize);
	box_list = get_box_list(type);

	return NULL;
}

size_t	get_box_size(t_zone_type type, size_t user_size, size_t page)
{
	size_t	boxsize;
	size_t	max_alloc;
	size_t	multi_num;

	max_alloc = (type == ZONE_LARGE ? user_size :
		(type == ZONE_SMALL ? SMALL_MAX : TINY_MAX));
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

void	*create_box(t_zone_type tyoe, size_t boxsize)
{
	void	*mem;
	t_box	*box;
	t_tag	*tag;

	mem = mmap(NULL, boxsize, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return NULL;
	box = (t_box *)mem;
	box->size = boxsize;
	box->start = mem;
	box->first_tag = (t_tag *)(box + 1);

	tag = box->first_tag;
	tag->user_area_size = box->size - sizeof(t_box) - sizeof(t_tag);
	tag->is_free = 1;
	tag->next_tag = NULL;
	tag->prev_tag = NULL;
	tag->magic = TAG_MAGIC;
}
