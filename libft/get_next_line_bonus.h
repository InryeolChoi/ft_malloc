/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:49:15 by inchoi            #+#    #+#             */
/*   Updated: 2023/04/06 14:05:10 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# define OPEN_MAX 10240

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>

char	*get_next_line_bonus(int fd);
char	*ft_strdup_gnlX(char *src);
char	*ft_strchr_gnlX(const char *s, int c);
char	*ft_strjoin_gnlX(char const *s1, char const *s2);

#endif
