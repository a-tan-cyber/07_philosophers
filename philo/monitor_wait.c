/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_wait.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 23:16:56 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:44:01 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <unistd.h>

static long	deadline_of(t_table *table, int i, long now)
{
	long	elapsed;
	long	rem;

	elapsed = now - table->philos[i].last_meal_ms;
	if (elapsed < 0)
		elapsed = 0;
	rem = table->time_die - elapsed;
	if (rem < 0)
		rem = 0;
	return (now + rem);
}

static int	next_deadline_locked(t_table *table,
		long now_ms, long *out_deadline)
{
	long	best;
	long	d;
	int		i;

	if (!table || !out_deadline || !table->philos)
		return (1);
	best = deadline_of(table, 0, now_ms);
	i = 1;
	while (i < table->philo_count)
	{
		d = deadline_of(table, i, now_ms);
		if (d < best)
			best = d;
		i++;
	}
	*out_deadline = best;
	return (0);
}

int	monitor_idle_sleep(t_table *table)
{
	long	now;
	long	deadline;
	long	rem;
	int		rc;

	if (!table)
		return (1);
	if (now_ms(&now))
		return (1);
	if (pthread_mutex_lock(&table->state_mtx))
		return (1);
	rc = next_deadline_locked(table, now, &deadline);
	if (pthread_mutex_unlock(&table->state_mtx))
		rc = 1;
	if (rc)
		return (1);
	rem = deadline - now;
	if (rem > 6)
		usleep(500);
	else if (rem > 2)
		usleep(200);
	else
		usleep(50);
	return (0);
}
