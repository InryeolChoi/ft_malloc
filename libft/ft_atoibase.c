/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoibase.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 10:50:38 by inchoi            #+#    #+#             */
/*   Updated: 2023/06/11 10:50:40 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	insider(char ch, char *base)
{
	int	i;

	i = 0;
	while (base[i] != '\0')
	{
		if (ch == base[i])
			return (i);
		i++;
	}
	return (-1);
}

static int	len(char *base)
{
	int	i;

	i = 0;
	while (base[i] != '\0')
		i++;
	return (i);
}

static int	checkminus(char *str)
{
	int	flag;

	flag = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			flag++;
		str++;
	}
	return (flag % 2);
}

static int	checkbase(char *base)
{
	int	i;
	int	temp;

	i = 0;
	while (base[i] != '\0')
	{
		temp = 1;
		if (base[i] == '+' || base[i] == '-')
			return (0);
		else if ((base[i] >= 9 && base[i] <= 13) || base[i] == 32)
			return (0);
		while (temp <= i)
		{
			if (base[i] == base[i - temp])
				return (0);
			temp++;
		}
		i++;
	}
	return (1);
}

int	ft_atoi_base(char *str, char *base)
{
	int	i;
	int	sign;
	int	number;

	i = 0;
	sign = 1;
	if (checkbase(base) == 0)
		return (0);
	if (checkminus(str) == 1)
		sign = -1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '-' || str[i] == '+')
		i++;
	number = 0;
	while (insider(str[i], base) != -1 && str[i] != '\0')
	{
		number *= len(base);
		number += insider(str[i], base);
		i++;
	}
	return (sign * number);
}
