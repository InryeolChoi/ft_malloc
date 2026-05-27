typedef struct s_block
{
    size_t  size;
    int     free;
    struct s_block  *next;
    struct s_block  *prev;
}   t_block;

