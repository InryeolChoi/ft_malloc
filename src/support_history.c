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

	if (is_env_enabled("FT_MALLOC_HISTORY") == 0)
		return ;
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

void	print_history_hex(uintptr_t value)
{
	char	*hex;
	char	digit;

	hex = "0123456789abcdef";
	if (value >= 16)
		print_history_hex(value / 16);
	digit = hex[value % 16];
	write(1, &digit, 1);
}

void	print_history_pointer(void *ptr)
{
	uintptr_t	address;

	address = (uintptr_t)ptr;
	write(1, "0x", 2);
	if (address == 0)
		write(1, "0", 1);
	else
		print_history_hex(address);
}

void	print_history_entry(t_history *history)
{
	write(1, "#", 1);
	print_tag_size(history->sequence);
	write(1, " ", 1);
	if (history->type == HISTORY_MALLOC)
		write(1, "MALLOC ", 7);
	else if (history->type == HISTORY_FREE)
		write(1, "FREE ", 5);
	else if (history->type == HISTORY_REALLOC)
		write(1, "REALLOC ", 8);
	else
		write(1, "UNKNOWN ", 8);
	print_history_pointer(history->old_ptr);
	write(1, " -> ", 4);
	print_history_pointer(history->new_ptr);
	write(1, " : ", 3);
	print_tag_size(history->size);
	write(1, " bytes\n", 7);
}

void	print_history(void)
{
	size_t	index;
	size_t	count;

	if (is_env_enabled("FT_MALLOC_HISTORY") == 0)
		return ;
	if (control_history_mutex(MUTEX_LOCK) != 0)
		return ;
	write(1, "HISTORY:\n", 9);
	index = 0;
	if (g_malloc.history_count == HISTORY_CAPACITY)
		index = g_malloc.history_next;
	count = 0;
	while (count < g_malloc.history_count)
	{
		print_history_entry(&g_malloc.history[index]);
		index = (index + 1) % HISTORY_CAPACITY;
		count++;
	}
	(void)control_history_mutex(MUTEX_UNLOCK);
}
