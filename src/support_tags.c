#include "ft_malloc.h"

void	*tag_to_user(t_tag *tag)
{
	size_t	tag_header_size;

	if (tag == NULL)
		return (NULL);
	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return (NULL);
	return ((char *)tag + tag_header_size);
}

t_tag	*user_to_tag(void *ptr)
{
	t_tag	*tag;
	size_t	tag_header_size;

	tag_header_size = align_size(sizeof(t_tag));
	if (tag_header_size == 0)
		return (NULL);
	tag = (t_tag *)((char *)ptr - tag_header_size);
	return (tag);
}

t_tag	*find_tag_in_box(t_box *box, void *ptr)
{
	t_tag	*tag;

	tag = box->first_tag;
	while (tag)
	{
		if (ptr == tag_to_user(tag))
			return (tag);
		tag = tag->next_tag;
	}
	return (NULL);
}
