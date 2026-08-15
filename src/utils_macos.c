/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_macos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 00:00:00 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/16 00:00:00 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

size_t	get_basic_page_size(void)
{
	int	page_size;

	page_size = getpagesize();
	if (page_size <= 0)
		return (0);
	return ((size_t)page_size);
}
