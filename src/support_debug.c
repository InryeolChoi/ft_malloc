/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 00:00:00 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/14 00:00:00 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static int	is_debug_enabled(void)
{
	char	*val;

	val = getenv("FT_MALLOC_DEBUG");
	if (val != NULL && val[0] == '1' && val[1] == '\0')
		return (1);
	return (0);
}

void	debug_free_error(t_tag *tag)
{
	if (is_debug_enabled() == 0)
		return ;
	if (tag == NULL)
		write(2, "invalid free\n", 13);
	else if (tag->is_free == 1)
		write(2, "double free\n", 12);
}
