/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:35:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <unistd.h>

static int	init_start_state(t_table *table)
{
	long	start;
	int		i;

	if (now_ms(&start))
		return (1);
	table->start_ms = start;
	if (pthread_mutex_lock(&table->state_mtx))
		return (1);
	i = 0;
	while (i < table->philo_count)
	{
		table->philos[i].last_meal_ms = start;
		table->philos[i].meals_eaten = 0;
		i++;
	}
	if (pthread_mutex_unlock(&table->state_mtx))
		return (1);
	return (0);
}

static int	start_philo_threads(t_table *table, int *created)
{
	while (*created < table->philo_count)
	{
		if (pthread_create(&table->philos[*created].thread, NULL, philo_routine,
				&table->philos[*created]))
		{
			fatal_stop_best_effort(table);
			return (1);
		}
		(*created)++;
	}
	return (0);
}

static int	wait_all_ready(t_table *table)
{
	int	ready;
	int	stop;

	ready = 0;
	while (ready < table->philo_count)
	{
		if (get_stop(table, &stop))
			return (1);
		if (stop)
			return (1);
		if (pthread_mutex_lock(&table->ready_mtx))
			return (1);
		ready = table->ready_count;
		if (pthread_mutex_unlock(&table->ready_mtx))
			return (1);
		if (ready < table->philo_count)
			usleep(50);
	}
	return (0);
}

static int	reset_run_state(t_table *table)
{
	if (set_stop(table, 0))
		return (1);
	if (pthread_mutex_lock(&table->state_mtx))
		return (1);
	table->fatal = 0;
	if (pthread_mutex_unlock(&table->state_mtx))
		return (1);
	table->ready_count = 0;
	return (0);
}

int	sim_setup(t_table *table, int *created, int *gate_locked)
{
	if (pthread_mutex_lock(&table->start_mtx))
		return (1);
	*gate_locked = 1;
	if (reset_run_state(table))
		return (1);
	if (start_philo_threads(table, created))
		return (1);
	if (wait_all_ready(table) || init_start_state(table))
	{
		fatal_stop_best_effort(table);
		return (1);
	}
	if (!pthread_mutex_unlock(&table->start_mtx))
		*gate_locked = 0;
	else
		return (1);
	return (0);
}
