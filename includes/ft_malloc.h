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
	struct s_block	*first_block;	// 처음 블록
	struct s_box	*next_box;		// 다음 박스
}	t_box;

typedef struct	s_block
{
	size_t			size;		// 블록의 크기
	int				is_free;	// 블록의 사용여부 결정
	struct s_block	*next;		// 다음 블록
	struct s_block	*prev;		// 그 전 블록
}	t_block;

void	*malloc(size_t size);
void	free();
void	*realloc();
void	show_alloc_mem(void);

#endif
