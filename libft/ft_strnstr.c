/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 15:15:13 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/17 12:02:53 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	tmp;

	i = 0;
	if (needle[0] == '\0')
		return (&((char *)haystack)[0]);
	while (haystack[i] && i < len)
	{
		tmp = 0;
		if (needle[tmp] == '\0')
			return (&((char *)haystack)[i]);
		if (haystack[i + tmp] == needle[tmp])
		{
			while (haystack[i + tmp] && needle[tmp++])
				if (needle[tmp] != haystack[i + tmp] || i + tmp >= len)
					break ;
			if (needle[tmp] == '\0')
				return (&((char *)haystack)[i]);
		}
		i++;
	}
	return (NULL);
}
