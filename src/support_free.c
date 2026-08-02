#include "ft_malloc.h"

int	can_unmap_box(t_box *box, t_tag *tag)
{
	if (box->type != ZONE_LARGE)
		return (0);
	if (box->first_tag == tag
		&& tag->is_free == 1
		&& tag->next_tag == NULL)
		return (1);
	return (0);
}
