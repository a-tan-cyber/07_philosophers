/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:40:03 by amtan             #+#    #+#             */
/*   Updated: 2026/02/13 19:17:08 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <unistd.h>

static int	stop_check(t_table *table, int *out_stop)
{
	if (get_stop(table, out_stop))
		return (fatal_return(table));
	if (*out_stop)
		return (1);
	return (0);
}

static int	unlock_forks(t_table *table,
	pthread_mutex_t *first, pthread_mutex_t *second)
{
	int	rc;

	rc = 0;
	if (second && pthread_mutex_unlock(second))
		rc = 1;
	if (first && pthread_mutex_unlock(first))
		rc = 1;
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}

static int	sleep_think(t_philo *philo)
{
	t_table	*table;
	long	think;

	table = philo->table;
	if (print_state(philo, "is sleeping"))
		return (fatal_return(table));
	if (ms_sleep(table, table->time_sleep))
		return (fatal_return(table));
	if (print_state(philo, "is thinking"))
		return (fatal_return(table));
	think = 0;
	if (table->philo_count % 2 == 1)
	{
		think = table->time_die - (table->time_eat + table->time_sleep);
		if (think > table->time_eat)
			think = table->time_eat;
		if (think > 0)
			if (ms_sleep(table, think))
				return (fatal_return(table));
	}
	return (0);
}

static int	loop_step(t_philo *philo)
{
	t_table			*table;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	int				stop;
	int				rc;

	table = philo->table;
	if (stop_check(table, &stop))
		return (1);
	if (philo_take_forks(philo, &first, &second))
		return (1);
	rc = philo_eat_step(philo);
	if (unlock_forks(table, first, second))
		return (1);
	if (rc)
		return (1);
	return (sleep_think(philo));
}

void	philo_loop(t_philo *philo)
{
	if (!philo || !philo->table)
		return ;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (loop_step(philo))
			return ;
	}
}
