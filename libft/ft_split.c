/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:40:56 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/15 11:36:20 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	count(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	*make_room(char const *s, char c)
{
	char	*room;
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	room = (char *)malloc((len + 1) * sizeof(char));
	if (!room)
		return (0);
	ft_strlcpy(room, s, len + 1);
	return (room);
}

char	**ft_free(char **box, int size)
{
	int	i;

	i = 0;
	while (i < size && box[i])
		free(box[i++]);
	free(box);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**box;
	size_t	i;
	size_t	len;

	len = count(s, c) + 1;
	box = (char **)malloc(len * sizeof(char *));
	if (!s || !box)
		return (NULL);
	i = 0;
	while (i < len && *s)
	{
		if (*s && *s != c)
		{
			box[i] = make_room(s, c);
			if (!box[i++])
				return (ft_free(box, len));
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	box[i] = 0;
	return (box);
}
