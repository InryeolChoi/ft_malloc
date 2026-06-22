/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_deci.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:13:09 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/27 18:13:10 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myprintf.h"

static void	get_intmin(int *cnt)
{
	ssize_t	a;

	a = write(1, "-2147483648", 11);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt) += 11;
}

static void	get_negative(int n, int *cnt)
{
	ssize_t	a;

	a = write(1, "-", 1);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt)++;
	ft_putnbr(-n, cnt);
}

static void	others(int n, int *cnt)
{
	ft_putnbr(n / 10, cnt);
	if (*cnt == -1)
		return ;
	ft_putnbr(n % 10, cnt);
	if (*cnt == -1)
		return ;
}

void	ft_putnbr(int n, int *cnt)
{
	char	c;
	ssize_t	a;

	if (n == -2147483648)
		get_intmin(cnt);
	else if (n < 0)
		get_negative(n, cnt);
	else if (n < 10)
	{
		c = '0' + n;
		a = write(1, &c, 1);
		if (a == -1)
		{
			(*cnt) = -1;
			return ;
		}
		(*cnt)++;
		return ;
	}
	else
		others(n, cnt);
}

void	ft_putnbr_u(unsigned int n, int *cnt)
{
	ssize_t	a;

	if (n >= 10)
	{
		ft_putnbr_u(n / 10, cnt);
		if (*cnt == -1)
			return ;
	}
	a = write(1, &"0123456789"[n % 10], 1);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}	
	(*cnt)++;
}
