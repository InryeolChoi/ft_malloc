#include "ft_malloc.h"

void	*tag_to_user(t_tag *tag)
{
	return (char *)tag + sizeof(*tag);
}

t_tag	*user_to_tag(void *ptr)
{
	t_tag	*tag;

	tag = (t_tag *)((char *)ptr - sizeof(t_tag));
	return tag;
}
