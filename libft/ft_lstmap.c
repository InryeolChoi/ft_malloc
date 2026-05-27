/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:32:28 by inchoi            #+#    #+#             */
/*   Updated: 2023/03/20 12:09:47 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (del)(void *))
{
	t_list	*new;
	t_list	*first;
	void	*content;

	first = 0;
	if (!lst || !f || !del)
		return (0);
	while (lst)
	{
		content = (f)(lst->content);
		new = ft_lstnew(content);
		if (!new || !(new->content))
		{
			if (content)
				del(content);
			free(new);
			ft_lstclear(&first, del);
			return (0);
		}
		ft_lstadd_back(&first, new);
		lst = lst->next;
	}
	new = 0;
	return (first);
}
