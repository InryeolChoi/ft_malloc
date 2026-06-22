/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:12:46 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/27 18:12:48 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myprintf.h"

static void	apply_null(int *cnt)
{
	ssize_t	a;

	a = write(1, "(null)", 6);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt) += 6;
}

void	ft_putchar(char c, int *cnt)
{
	ssize_t	a;

	a = write(1, &c, 1);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt)++;
}

void	ft_putstr(char *str, int *cnt)
{
	int		n;
	ssize_t	a;

	n = 0;
	if (str == 0)
		apply_null(cnt);
	else
	{
		while (str[n] != '\0')
		{
			a = write(1, &str[n++], 1);
			if (a == -1)
			{
				(*cnt) = -1;
				return ;
			}
			(*cnt)++;
		}
	}
}
