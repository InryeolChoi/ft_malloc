/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:34:13 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/21 18:53:50 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	is_blank(char c)
{
	if (c >= 9 && c <= 13)
		return (1);
	else if (c == 32)
		return (1);
	else
		return (0);
}

static int	figure_long(int sign)
{
	if (sign == 1)
		return ((int)LONG_MAX);
	else
		return ((int)LONG_MIN);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	num;

	sign = 1;
	while (*str && is_blank(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	num = 0;
	while (*str != '\0' && (*str >= '0' && *str <= '9'))
	{
		if (num >= LONG_MAX / 10 || (num == LONG_MAX / 10 - 1 && num % 10 > 7))
			return (figure_long(sign));
		num *= 10;
		num += *str - '0';
		str++;
	}
	return ((int)(sign * num));
}
