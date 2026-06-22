/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myprintf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:02:20 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/24 20:07:41 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myprintf.h"

static void	ft_percent(int *cnt)
{
	char	c;
	ssize_t	a;

	c = 37;
	a = write(1, &c, 1);
	if (a == -1)
	{
		(*cnt) = -1;
		return ;
	}
	(*cnt)++;
}

void	apply_format(char param, va_list *ap, int *cnt)
{
	if (param == 'c')
		ft_putchar(va_arg(*ap, int), cnt);
	else if (param == 'd' || param == 'i')
		ft_putnbr(va_arg(*ap, int), cnt);
	else if (param == 's')
		ft_putstr(va_arg(*ap, char *), cnt);
	else if (param == 'p')
		ft_putvoid(va_arg(*ap, void *), cnt);
	else if (param == 'u')
		ft_putnbr_u(va_arg(*ap, unsigned int), cnt);
	else if (param == 'x')
		ft_hexadeci(va_arg(*ap, unsigned int), 'x', cnt);
	else if (param == 'X')
		ft_hexadeci(va_arg(*ap, unsigned int), 'X', cnt);
	else if (param == '%')
		ft_percent(cnt);
}

int	ft_printf(const char *text, ...)
{
	va_list	ap;
	int		cnt;

	cnt = 0;
	va_start(ap, text);
	while (*text != '\0')
	{
		if (*text == '%')
		{
			apply_format(*(text + 1), &ap, &cnt);
			if (cnt == -1)
				return (cnt);
			text += 2;
		}
		else
		{
			ft_putchar(*text++, &cnt);
			if (cnt == -1)
				return (cnt);
		}
	}
	va_end(ap);
	return (cnt);
}
