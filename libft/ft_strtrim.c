/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:26:11 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/15 10:36:08 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str);

int	in(char x, const char *set)
{
	while (*set != '\0')
	{
		if (*set == x)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	l;
	size_t	r;
	size_t	len;
	size_t	n;
	char	*s2;

	if (s1 == NULL || set == NULL)
		return ((char *)s1);
	l = 0;
	len = ft_strlen(s1);
	while (l < len && in(s1[l], set))
		l++;
	r = len;
	while (r > l && in(s1[r - 1], set))
		r--;
	s2 = (char *)malloc((r - l + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	n = 0;
	while (l < r)
		s2[n++] = s1[l++];
	s2[n] = '\0';
	return (s2);
}
