/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:59:23 by inchoi            #+#    #+#             */
/*   Updated: 2023/04/07 16:38:14 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*read_line_gnlx(int fd, char *buffer, char **backup)
{
	char	*temp;
	int		x;

	x = 1;
	while (x)
	{
		x = read(fd, buffer, BUFFER_SIZE);
		if (x == -1)
			return (NULL);
		if (x == 0)
			break ;
		buffer[x] = '\0';
		temp = backup[fd];
		backup[fd] = ft_strjoin_gnlx(backup[fd], buffer);
		free(temp);
		if (!backup)
			return (NULL);
		if (ft_strchr_gnlX(backup[fd], '\n'))
			break ;
	}
	return (backup[fd]);
}

static char	*sepearate_gnlx(char *total)
{
	int		i;
	char	*backup;

	i = 0;
	while (total[i] && total[i] != '\n')
		i++;
	if (total[i] == '\0' || total[i + 1] == '\0')
		return (NULL);
	backup = ft_strdup_gnlx(total + i + 1);
	if (backup == NULL)
		return (NULL);
	total[i + 1] = '\0';
	return (backup);
}

char	*get_next_line_bonus(int fd)
{
	static char	*backup[OPEN_MAX];
	char		buffer[BUFFER_SIZE + 1];
	char		*total;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	total = read_line_gnlx(fd, buffer, backup);
	if (total == NULL || *total == '\0')
	{
		free(backup[fd]);
		backup[fd] = (char *) NULL;
		return (NULL);
	}
	backup[fd] = sepearate_gnlx(total);
	return (total);
}
