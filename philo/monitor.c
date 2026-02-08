/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:42:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/08 22:15:27 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdio.h>
#include <unistd.h>

static int	end_unlock(t_table *table, int *ended, int rc)
{
	*ended = 1;
	return (unlock_both_return(table, rc));
}

static int	stop_unlock(t_table *table, int *ended, int rc)
{
	if (set_stop(table, 1))
	{
		fatal_stop_no_lock(table);
		rc = 1;
	}
	*ended = 1;
	return (unlock_both_return(table, rc));
}

static int	death_unlock(t_table *table, int *ended, t_philo *dead, long now)
{
	int	rc;

	rc = 0;
	if (set_stop(table, 1))
	{
		fatal_stop_no_lock(table);
		rc = 1;
	}
	*ended = 1;
	printf("%ld %d died\n", now - table->start_ms, dead->id);
	return (unlock_both_return(table, rc));
}

static int	monitor_once(t_table *table, int *ended)
{
	long	now;
	t_philo	*dead;
	int		full;
	int		stop;

	if (lock_print_state(table))
		return (1);
	if (get_stop(table, &stop))
		return (stop_unlock(table, ended, 1));
	if (stop)
		return (end_unlock(table, ended, 0));
	if (now_ms(&now))
		return (stop_unlock(table, ended, 1));
	if (monitor_find_dead_locked(table, &dead, now))
		return (stop_unlock(table, ended, 1));
	if (dead)
		return (death_unlock(table, ended, dead, now));
	if (table->must_eat_enabled && monitor_all_full_locked(table, &full))
		return (stop_unlock(table, ended, 1));
	if (table->must_eat_enabled && full)
		return (stop_unlock(table, ended, 0));
	return (unlock_both_return(table, 0));
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
		usleep(1000);
	}
	return (0);
}
