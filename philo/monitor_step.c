/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_step.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 10:15:00 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	state_fail_unlock(t_table *table, int *ended)
{
	table->fatal = 1;
	*ended = 1;
	if (pthread_mutex_unlock(&table->state_mtx))
		fatal_stop_no_lock(table);
	if (set_stop(table, 1))
		fatal_stop_no_lock(table);
	return (1);
}

static int	monitor_print_death(t_table *table, int dead_id)
{
	long	print_now;
	long	ts;
	int		rc;

	if (pthread_mutex_lock(&table->print_mtx))
		return (fatal_return(table));
	if (now_ms(&print_now))
	{
		pthread_mutex_unlock(&table->print_mtx);
		fatal_stop_best_effort(table);
		return (1);
	}
	ts = print_now - table->start_ms;
	if (ts < 0)
		ts = 0;
	rc = write_line(ts, dead_id, "died");
	if (pthread_mutex_unlock(&table->print_mtx))
		return (fatal_return_no_lock(table));
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}

static int	set_stop_quiet(t_table *table)
{
	if (pthread_mutex_lock(&table->print_mtx))
		return (fatal_return(table));
	if (pthread_mutex_lock(&table->stop_mtx))
	{
		pthread_mutex_unlock(&table->print_mtx);
		return (fatal_return(table));
	}
	table->stop = 1;
	if (pthread_mutex_unlock(&table->stop_mtx))
	{
		pthread_mutex_unlock(&table->print_mtx);
		return (fatal_return(table));
	}
	if (pthread_mutex_unlock(&table->print_mtx))
		return (fatal_return_no_lock(table));
	return (0);
}

static int	end_with_death(t_table *table, int dead_id)
{
	if (set_stop(table, 1))
		return (fatal_return(table));
	return (monitor_print_death(table, dead_id));
}

int	monitor_step(t_table *table, int *ended)
{
	long		now;
	t_mon_eval	eval;
	int			stop;

	if (get_stop(table, &stop))
		return (fatal_return(table));
	*ended = stop;
	if (stop)
		return (0);
	if (pthread_mutex_lock(&table->state_mtx))
		return (fatal_return(table));
	if (now_ms(&now))
		return (state_fail_unlock(table, ended));
	if (monitor_eval_locked(table, now, &eval))
		return (state_fail_unlock(table, ended));
	if (pthread_mutex_unlock(&table->state_mtx))
		return (fatal_return_no_lock(table));
	if (!eval.ended && monitor_idle_sleep(table))
		return (fatal_return(table));
	if (!eval.ended)
		return (0);
	*ended = 1;
	if (!eval.print_death)
		return (set_stop_quiet(table));
	return (end_with_death(table, eval.dead_id));
}
