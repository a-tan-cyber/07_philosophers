/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:42:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/11 16:51:28 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdio.h>

static int	end_unlock(t_table *table, int *ended, int rc)
{
	*ended = 1;
	return (unlock_both_return(table, rc));
}

static int	fatal_unlock(t_table *table, int *ended)
{
	table->stop = 1;
	table->fatal = 1;
	return (end_unlock(table, ended, 1));
}

static int	monitor_eval_locked(t_table *table, long now, int *ended)
{
	t_philo	*dead;
	int		full;
	int		rc;

	dead = NULL;
	full = 0;
	if (monitor_find_dead_locked(table, &dead, now))
		return (1);
	if (table->must_eat_enabled && monitor_all_full_locked(table, &full))
		return (1);
	rc = 0;
	if (dead)
	{
		table->stop = 1;
		*ended = 1;
		rc = write_line(now - table->start_ms, dead->id, "died");
	}
	if (table->must_eat_enabled && full)
	{
		table->stop = 1;
		*ended = 1;
	}
	return (rc);
}

static int	monitor_once(t_table *table, int *ended)
{
	long	now;

	*ended = 0;
	if (lock_print_state(table))
		return (1);
	if (table->stop)
		return (end_unlock(table, ended, 0));
	if (now_ms(&now))
		return (fatal_unlock(table, ended));
	if (monitor_eval_locked(table, now, ended))
		return (fatal_unlock(table, ended));
	if (*ended)
		return (unlock_both_return(table, 0));
	if (unlock_both_return(table, 0))
		return (1);
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
