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

static int	is_env_enabled(const char *name)
{
	char	*val;

	val = getenv(name);
	if (val != NULL && val[0] == '1' && val[1] == '\0')
		return (1);
	return (0);
}

void	debug_free_error(t_tag *tag)
{
	if (is_env_enabled("FT_MALLOC_DEBUG") == 0)
		return ;
	if (tag == NULL)
		write(2, "invalid free\n", 13);
	else if (tag->is_free == 1)
		write(2, "double free\n", 12);
}

void	debug_scribble_alloc(t_tag *tag)
{
	if (tag == NULL || is_env_enabled("FT_MALLOC_SCRIBBLE") == 0)
		return ;
	ft_memset(tag_to_user(tag), 0xAA, tag->origin_size);
}

void	debug_scribble_free(t_tag *tag)
{
	if (tag == NULL || is_env_enabled("FT_MALLOC_SCRIBBLE") == 0)
		return ;
	ft_memset(tag_to_user(tag), 0x55, tag->origin_size);
}

void	debug_scribble_resize(t_tag *tag, size_t new_size)
{
	unsigned char	*user_area;

	if (tag == NULL || is_env_enabled("FT_MALLOC_SCRIBBLE") == 0)
		return ;
	user_area = (unsigned char *)tag_to_user(tag);
	if (new_size > tag->origin_size)
		ft_memset(user_area + tag->origin_size, 0xAA, new_size - tag->origin_size);
	else if (new_size < tag->origin_size)
		ft_memset(user_area + new_size, 0x55, tag->origin_size - new_size);
}
