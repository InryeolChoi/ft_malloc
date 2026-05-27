/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 15:42:49 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/15 10:09:34 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*n1;
	unsigned char	*n2;

	i = 0;
	n1 = (unsigned char *)s1;
	n2 = (unsigned char *)s2;
	while (i < n)
	{
		if (n1[i] != n2[i])
			return (n1[i] - n2[i]);
		i++;
	}
	return (0);
}
