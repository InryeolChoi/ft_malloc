/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:59:32 by inchoi            #+#    #+#             */
/*   Updated: 2023/04/06 14:03:27 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static size_t	ft_strlen_gnlx(const char *str)
{
	size_t	count;

	if (str == 0)
		return (0);
	count = 0;
	while (*str != '\0')
	{
		count++;
		str++;
	}
	return (count);
}

char	*ft_strdup_gnlx(char *src)
{
	char	*arr;
	int		size;
	int		i;

	i = 0;
	size = ft_strlen_gnlx(src);
	arr = (char *)malloc(sizeof(char) * (size + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		arr[i] = src[i];
		i++;
	}
	arr[i] = '\0';
	return (arr);
}

char	*ft_strchr_gnlx(const char *s, int c)
{
	size_t	i;
	size_t	len;

	len = ft_strlen_gnlx(s);
	i = 0;
	while (i < len + 1)
	{
		if (*(s + i) == (char) c)
			return ((char *)(s + i));
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_gnlx(char const *s1, char const *s2)
{
	char	*joined;
	int		size;
	int		i;

	i = 0;
	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup_gnlx((char *)s2));
	else if (!s2)
		return (ft_strdup_gnlx((char *)s1));
	size = ft_strlen_gnlx(s1) + ft_strlen_gnlx(s2) + 1;
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
