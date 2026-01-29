/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:44:23 by amtan             #+#    #+#             */
/*   Updated: 2026/01/29 19:37:05 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <sys/time.h>

int	now_ms(long *out)
{
	struct timeval	tv;

	if (!out)
		return (1);
	if (gettimeofday(&tv, NULL))
		return (1);
	*out = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
	return (0);
}

int	since_start_ms(t_table *table, long *out)
{
	long	now;

	if (!table || !out || !table->start_ms)
		return (1);
	if (now_ms(&now))
		return (1);
	*out = now - table->start_ms;
	return (0);
}
