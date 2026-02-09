/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:28:07 by amtan             #+#    #+#             */
/*   Updated: 2026/02/09 16:59:47 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	commit_last_meal(t_philo *philo, long now, int *out_stop)
{
	t_table	*table;

	table = philo->table;
	if (pthread_mutex_lock(&table->state_mtx))
		return (fatal_return(table));
	*out_stop = table->stop;
	if (!*out_stop)
		philo->last_meal_ms = now;
	if (pthread_mutex_unlock(&table->state_mtx))
		return (fatal_return_no_lock(table));
	return (0);
}

static int	increment_meals_if_ok(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	if (pthread_mutex_lock(&table->state_mtx))
		return (fatal_return(table));
	if (!table->stop)
		philo->meals_eaten++;
	if (pthread_mutex_unlock(&table->state_mtx))
		return (fatal_return_no_lock(table));
	return (0);
}

int	philo_eat_step(t_philo *philo)
{
	t_table	*table;
	long	now;
	int		stop;

	table = philo->table;
	if (!philo || !table)
		return (1);
	if (now_ms(&now))
		return (fatal_return(table));
	if (commit_last_meal(philo, now, &stop) || stop)
		return (1);
	if (print_state(philo, "is eating"))
		return (fatal_return(table));
	if (ms_sleep(philo->table, philo->table->time_eat))
		return (fatal_return(table));
	return (increment_meals_if_ok(philo));
}
