/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:41:57 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/16 10:53:17 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(char *src);

static int	find_size(long long n)
{
	int	size;

	size = 0;
	if (n <= 0)
	{
		size++;
		n = -n;
	}
	while (n > 0)
	{
		size++;
		n /= 10;
	}
	return (size);
}

static char	*put(char *newarr, int n, int size)
{
	newarr[size--] = '\0';
	while (n > 0)
	{
		newarr[size--] = (n % 10) + '0';
		n /= 10;
	}
	return (newarr);
}

char	*ft_itoa(int n)
{
	int		size;
	char	*newarr;

	if (n == 0)
		return (ft_strdup("0"));
	else if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	size = find_size((long long) n);
	newarr = (char *)malloc((size + 1) * sizeof(char));
	if (!newarr)
		return (0);
	if (n < 0)
	{
		newarr[0] = '-';
		n *= -1;
	}
	return (put(newarr, n, size));
}
