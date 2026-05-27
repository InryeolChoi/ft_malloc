/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 09:08:58 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/17 16:54:35 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*tooshort(void)
{
	char	*arr;

	arr = (char *)malloc(sizeof(char));
	if (!arr)
		return (NULL);
	arr[0] = '\0';
	return (arr);
}

static int	makelen(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;

	s_len = ft_strlen(s);
	if (start + len > s_len)
		return (s_len - start);
	else
		return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*newstr;

	i = 0;
	j = 0;
	if (ft_strlen(s) < start)
		return (tooshort());
	newstr = (char *)malloc((makelen(s, start, len) + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	while (s[i])
	{
		if (i >= start && j < len)
			newstr[j++] = s[i];
		i++;
	}
	newstr[j] = '\0';
	return (newstr);
}
