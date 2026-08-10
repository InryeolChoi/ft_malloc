/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/11 00:18:36 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <pthread.h>
# include <stddef.h>
# include <stdint.h>

# include <sys/mman.h>
# include <unistd.h>

# include "libft/libft.h"

# define TINY_MAX 128
# define SMALL_MAX 1024
# define ALIGNMENT 16
# define TAG_MAGIC 0xC0FFEEUL

typedef enum e_zone_type
{
	ZONE_TINY,
	ZONE_SMALL,
	ZONE_LARGE
}	t_zone_type;

typedef enum e_mutex_action
{
	MUTEX_LOCK,
	MUTEX_UNLOCK
}	t_mutex_action;

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
	size_t			capacity;
	size_t			origin_size;
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

typedef struct s_thread_state
{
	pthread_mutex_t	mutex_tiny;
	pthread_mutex_t	mutex_small;
	pthread_mutex_t	mutex_large;
}	t_thread_state;

extern t_malloc_state	g_malloc;
extern t_thread_state	g_thread;

void			*malloc(size_t size);
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);
void			show_alloc_mem(void);

int				control_mutex(t_zone_type type, t_mutex_action action);

size_t			get_basic_page_size(void);
size_t			align_size(size_t size);
size_t			get_box_size(t_zone_type type, size_t user_size, size_t page);
size_t			get_zone_payload(t_zone_type type, size_t user_size);
size_t			will_add_overflow(size_t num1, size_t num2);
size_t			will_multi_overflow(size_t num1, size_t num2);

t_zone_type		get_zone_type(size_t size);
t_box			**get_box_list(t_zone_type type);
t_box			*find_box_pool(void *ptr);
t_box			*find_box_and_lock(void *ptr);
t_box			*find_box_list(t_box *box_list, void *ptr);
t_box			*create_box(t_zone_type type, size_t boxsize);
void			connect_to_boxlist(t_box *box);

t_tag			*get_tag(size_t origin_size, size_t needed_size,
					t_zone_type type, size_t boxsize);
t_tag			*find_tag(t_zone_type type, size_t size);
t_tag			*set_tag(t_tag *tag, size_t origin_size,
					size_t needed_size);
t_tag			*make_tag(t_box *box, size_t box_front, size_t tag_front);
void			*tag_to_user(t_tag *tag);
t_tag			*user_to_tag(void *ptr);
t_tag			*find_tag_in_box(t_box *box, void *ptr);
int				can_split_tag(t_tag *tag, size_t needed_size);
t_tag			*make_newtag(t_tag *tag, size_t used_size);
void			set_newtag(t_tag *tag, t_tag *newtag);

void			print_boxes(char *str, t_box *box);
t_box			*find_next_box(t_box *box, t_box *prev);
void			print_all_tags(t_box *box);
size_t			sum_tag_size(t_box *box);
void			print_tag_size(size_t size);

t_tag			*merge_with_prev(t_tag *tag);
t_tag			*merge_with_next(t_tag *tag);
int				can_unmap_box(t_box *box, t_tag *tag);
void			unmap_box(t_box *box);

void			*remake_minptr(void *ptr);
void			*remake_origin(void *ptr, size_t size, t_tag *tag);
void			*remake_newptr(void *ptr, size_t size, t_tag *tag);

#endif
