/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:36:33 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/20 10:46:51 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tail;

	if (!lst)
		return ;
	while (*lst)
	{
		tail = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = tail;
	}
	*lst = 0;
}
