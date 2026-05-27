/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:10:17 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/20 10:16:28 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tail;

	if (*lst == 0)
		*lst = new;
	else
	{
		tail = (*lst);
		while (tail->next != 0)
			tail = tail->next;
		tail->next = new;
	}
}
