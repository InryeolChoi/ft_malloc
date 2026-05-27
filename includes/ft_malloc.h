#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stddef.h>
# include <sys/mman.h>
# include <unistd.h>

# define TINY_MAX   128
# define SMALL_MAX  1024

typedef enum e_zone_type
{
    ZONE_TINY,
    ZONE_SMALL,
    ZONE_LARGE
}   t_zone_type;

typedef struct  s_block
{
  size_t  size;
  int     free;
  struct  s_block *next;
  struct  s_block *prev;
} t_block;

typedef struct  s_zone
{
  void      *start;// 주소
  size_t    size;                 // 전체 크기
  t_zone_type   type;                 // 
  struct s_block   *first_block;  //
  struct s_zone    *nextzone;     // 다음 존
} t_zone;

void  *malloc(size_t size);
void  free();
void  *realloc();
void  show_alloc_mem(void);

#endif
