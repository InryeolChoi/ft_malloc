/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myprintf.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 10:28:50 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/26 14:17:59 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MYPRINTF_H
# define MYPRINTF_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

int		ft_printf(const char *text, ...);
void	apply_format(char param, va_list *ap, int *cnt);
void	ft_putchar(char c, int *cnt);
void	ft_putnbr(int n, int *cnt);
void	ft_putstr(char *str, int *cnt);
void	ft_putnbr_u(unsigned int n, int *cnt);
void	ft_hexadeci(unsigned int n, char sign, int *cnt);
void	ft_putvoid(void *ptr, int *cnt);

#endif
