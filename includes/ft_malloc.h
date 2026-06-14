#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stddef.h>
# include <sys/mman.h>
# include <unistd.h>

# define TINY_MAX 128
# define SMALL_MAX 1024
# define TAG_MAGIC ((size_t)0xC0FFEE)

typedef enum e_zone_type
{
	ZONE_TINY,
	ZONE_SMALL,
	ZONE_LARGE
}	t_zone_type;

typedef struct s_box
{
	void			*start;
	size_t			size;
	t_zone_type		type;
	struct s_tag	*first_tag;
	struct s_box	*next_box;
}	t_box;

typedef struct s_tag
{
	size_t			user_area_size;
	int				is_free;
	struct s_tag	*next_tag;
	struct s_tag	*prev_tag;
	size_t			magic;
}	t_tag;

typedef struct s_malloc_state
{
	t_box	*tiny_boxes;
	t_box	*small_boxes;
	t_box	*large_boxes;
}	t_malloc_state;

extern t_malloc_state	g_malloc;

size_t			get_basic_page_size(void);
t_zone_type		get_zone_type(size_t size);
t_box			**get_box_list(t_zone_type type);
t_box			*find_box_pool(void *ptr);
t_box			*find_box_list(t_box *box_list, void *ptr);
t_tag			*find_tag_in_box(t_box *box, void *ptr);
int				is_ptr_in_box(t_box *box, void *ptr);
size_t			get_box_size(t_zone_type type, size_t user_size, size_t page);

void			*malloc(size_t size);
t_tag			*get_tag(size_t size);
t_tag			*find_tag(t_box **box_list, size_t size);
t_tag			*create_tag(t_zone_type type, size_t boxsize);
void			connect_to_boxlist(t_box *box);
t_tag			*set_tag(t_tag *tag, size_t new_user_area);
void			*tag_to_user(t_tag *tag);
t_tag			*user_to_tag(void *ptr);
int				can_split_tag(t_tag *tag, size_t user_area);
t_tag			*make_newtag(t_tag *tag, size_t user_area);
void			set_newtag(t_tag *tag, t_tag *newtag);

size_t			will_add_overflow(size_t num1, size_t num2);
size_t			will_multi_overflow(size_t num1, size_t num2);

void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

#endif
