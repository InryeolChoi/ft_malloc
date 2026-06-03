#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stddef.h>
# include <sys/mman.h>
# include <unistd.h>

# define TINY_MAX	128
# define SMALL_MAX	1024

typedef enum e_zone_type
{
	ZONE_TINY,
	ZONE_SMALL,
	ZONE_LARGE
}	t_zone_type;

typedef struct	s_box
{
	void      		*start;			// 주소
	size_t    		size;			// 사이즈
	t_zone_type		type;			// 사이즈 분류
	struct s_tag	*first_tag;		// 처음 태그
	struct s_box	*next_box;		// 다음 박스
}	t_box;

typedef struct	s_tag
{
	size_t			user_area_size;	// user area 크기
	int				is_free;		// 태그의 사용여부 결정
	struct s_tag	*next_tag;		// 다음 태그
	struct s_tag	*prev_tag;		// 그 전 태그
	size_t			magic;			// 태그 무결성 체크용
}	t_tag;

typedef struct s_malloc_state	// 사이즈 확인용
{
    t_box *tiny_boxes;
    t_box *small_boxes;
    t_box *large_boxes;
}   t_malloc_state;

extern t_malloc_state g_malloc;

t_zone_type	get_zone_type(size_t size);
t_box		**get_box_list(t_zone_type type);

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

#endif
