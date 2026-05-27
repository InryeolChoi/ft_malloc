/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:00:32 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/17 12:02:25 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		size;
	int		i;

	i = 0;
	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup((char *)s2));
	else if (!s2)
		return (ft_strdup((char *)s1));
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = (char *)malloc(size * sizeof(char));
	if (!joined)
		return (0);
	while (i < size && *s1 != '\0')
		joined[i++] = *(s1++);
	while (i < size && *s2 != '\0')
		joined[i++] = *(s2++);
	joined[i] = '\0';
	return (joined);
}
