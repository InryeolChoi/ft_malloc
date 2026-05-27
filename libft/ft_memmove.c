/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:37:37 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/15 10:18:29 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	unsigned char		*dest2;
	const unsigned char	*src2;
	size_t				i;

	dest2 = (unsigned char *)dest;
	src2 = (const unsigned char *)src;
	i = -1;
	if (src2 < dest2)
	{
		src2 += len;
		dest2 += len;
		while (len > 0)
		{
			*--dest2 = *--src2;
			len--;
		}
	}
	else if (src2 != dest2)
		while (++i < len)
			*dest2++ = *src2++;
	return (dest);
}
