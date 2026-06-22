/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:13:35 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/27 18:13:36 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myprintf.h"

static void	hexa_others(unsigned int n, char sign, int *cnt)
{
	ft_hexadeci(n / 16, sign, cnt);
	if (*cnt == -1)
		return ;
	ft_hexadeci(n % 16, sign, cnt);
	if (*cnt == -1)
		return ;
}

void	ft_hexadeci(unsigned int n, char sign, int *cnt)
{
	ssize_t	a;
	char	c;

	if (n < 16)
	{
		if (sign == 'x')
			c = "0123456789abcdef"[n];
		else if (sign == 'X')
			c = "0123456789ABCDEF"[n];
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
		hexa_others(n, sign, cnt);
}

static void	addr_hexadeci(unsigned long address, int *cnt)
{
	ssize_t	a;

	if (address >= 16)
	{
		addr_hexadeci(address / 16, cnt);
		if (*cnt == -1)
			return ;
	}
	a = write(1, &"0123456789abcdef"[address % 16], 1);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt)++;
}

void	ft_putvoid(void *ptr, int *cnt)
{
	unsigned long	address;
	ssize_t			a;

	a = write(1, "0x", 2);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt) += 2;
	address = (unsigned long)ptr;
	addr_hexadeci(address, cnt);
	if (*cnt == -1)
		return ;
}
