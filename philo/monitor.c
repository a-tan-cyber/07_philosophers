/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:42:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/09 16:59:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	scan_state(t_table *table, long now,
			t_philo **out_dead, int *out_full)
{
	int	rc;

	rc = 0;
	*out_dead = NULL;
	*out_full = 0;
	if (pthread_mutex_lock(&table->state_mtx))
		return (1);
	if (monitor_find_dead_locked(table, out_dead, now))
		rc = 1;
	if (!rc && table->must_eat_enabled)
		rc = monitor_all_full_locked(table, out_full);
	if (pthread_mutex_unlock(&table->state_mtx))
		rc = 1;
	return (rc);
}

static int	end_dead(int *ended, t_philo *dead)
{
	*ended = 1;
	return (print_death(dead));
}

static int	end_full(t_table *table, int *ended)
{
	*ended = 1;
	if (set_stop(table, 1))
		return (fatal_return(table));
	return (0);
}

static int	monitor_once(t_table *table, int *ended)
{
	long	now;
	t_philo	*dead;
	int		full;
	int		stop;

	*ended = 0;
	if (get_stop(table, &stop))
		return (fatal_return(table));
	if (stop)
		return (end_full(table, ended));
	if (now_ms(&now))
		return (fatal_return(table));
	if (scan_state(table, now, &dead, &full))
		return (fatal_return(table));
	if (dead)
		return (end_dead(ended, dead));
	if (full)
		return (end_full(table, ended));
	if (monitor_idle_sleep(table))
		return (fatal_return(table));
	return (0);
}

int	monitor_loop(t_table *table)
{
	int	ended;

	if (!table)
		return (1);
	ended = 0;
	while (!ended)
	{
		if (monitor_once(table, &ended))
			return (1);
	}
	return (0);
}
