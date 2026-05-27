/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:02:28 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/17 19:41:02 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

unsigned int	ft_strlcat(char *dest, const char *src, unsigned int size)
{
	unsigned int	dest_len;
	unsigned int	src_len;
	unsigned int	x;
	unsigned int	n;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	x = dest_len;
	n = 0;
	while (src[n] && (x + 1) < size)
	{
		dest[x] = src[n];
		n++;
		x++;
	}
	dest[x] = '\0';
	if (size > dest_len)
		return (dest_len + src_len);
	else
		return (size + src_len);
}
