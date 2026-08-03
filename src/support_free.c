#include "ft_malloc.h"

int	can_unmap_box(t_box *box, t_tag *tag)
{
	t_box	*first_box;

	if (box->first_tag != tag || tag->is_free == 0 || tag->next_tag != NULL)
		return (0);
	if (box->type == ZONE_LARGE)
		return (1);
	first_box = *get_box_list(box->type);
	if (first_box != NULL && first_box->next_box != NULL)
		return (1);
	return (0);
}
