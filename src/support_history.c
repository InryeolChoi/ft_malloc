/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_history.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 00:00:00 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/16 00:00:00 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	record_history(t_history_type type, void *old_ptr,
		void *new_ptr, size_t size)
{
	t_history		*history;

	if (control_history_mutex(MUTEX_LOCK) != 0)
		return ;
	history = &g_malloc.history[g_malloc.history_next];
	history->sequence = g_malloc.history_sequence;
	g_malloc.history_sequence++;
	history->type = type;
	history->old_ptr = old_ptr;
	history->new_ptr = new_ptr;
	history->size = size;
	g_malloc.history_next = (g_malloc.history_next + 1) % HISTORY_CAPACITY;
	if (g_malloc.history_count < HISTORY_CAPACITY)
		g_malloc.history_count++;
	(void)control_history_mutex(MUTEX_UNLOCK);
}
