/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:50:39 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/15 10:27:34 by inchoi           ###   ########.fr       */
/*                                                							  */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str);

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	i = 0;
	while (i < len + 1)
	{
		if (*(s + i) == (char) c)
			return ((char *)(s + i));
		i++;
	}
	return (NULL);
}
