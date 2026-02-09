/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_wait.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 23:16:56 by amtan             #+#    #+#             */
/*   Updated: 2026/02/09 16:59:02 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <unistd.h>

static int	next_deadline_locked(t_table *table, long *out_deadline)
{
	long	d;
	long	best;
	int		i;

	if (!table || !out_deadline || !table->philos)
		return (1);
	best = table->philos[0].last_meal_ms + table->time_die;
	i = 1;
	while (i < table->philo_count)
	{
		d = table->philos[i].last_meal_ms + table->time_die;
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

	if (!table)
		return (1);
	if (now_ms(&now))
		return (1);
	if (pthread_mutex_lock(&table->state_mtx))
		return (1);
	if (next_deadline_locked(table, &deadline))
	{
		pthread_mutex_unlock(&table->state_mtx);
		return (1);
	}
	if (pthread_mutex_unlock(&table->state_mtx))
		return (1);
	rem = deadline - now;
	if (rem > 6)
		usleep(5000);
	else if (rem > 2)
		usleep(1000);
	else
		usleep(200);
	return (0);
}
