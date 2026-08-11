/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:54:44 by inchoi            #+#    #+#             */
/*   Updated: 2026/08/10 23:13:32 by inchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

t_thread_state	g_thread = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER
};

int	control_mutex(t_zone_type type, t_mutex_action action)
{
	pthread_mutex_t	*mutex;

	if (type == ZONE_TINY)
		mutex = &g_thread.mutex_tiny;
	else if (type == ZONE_SMALL)
		mutex = &g_thread.mutex_small;
	else if (type == ZONE_LARGE)
		mutex = &g_thread.mutex_large;
	else
		return (1);
	if (action == MUTEX_LOCK)
		return (pthread_mutex_lock(mutex));
	if (action == MUTEX_UNLOCK)
		return (pthread_mutex_unlock(mutex));
	return (1);
}

int	lock_all_mutex(void)
{
	if (pthread_mutex_lock(&g_thread.mutex_tiny) != 0)
		return (1);
	if (pthread_mutex_lock(&g_thread.mutex_small) != 0)
	{
		pthread_mutex_unlock(&g_thread.mutex_tiny);
		return (1);
	}
	if (pthread_mutex_lock(&g_thread.mutex_large) != 0)
	{
		pthread_mutex_unlock(&g_thread.mutex_small);
		pthread_mutex_unlock(&g_thread.mutex_tiny);
		return (1);
	}
	return (0);
}

int	unlock_all_mutex(void)
{
	int	failed;

	failed = 0;
	if (pthread_mutex_unlock(&g_thread.mutex_large) != 0)
		failed = 1;
	if (pthread_mutex_unlock(&g_thread.mutex_small) != 0)
		failed = 1;
	if (pthread_mutex_unlock(&g_thread.mutex_tiny) != 0)
		failed = 1;
	return (failed);
}
