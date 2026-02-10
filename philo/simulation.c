/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 21:45:28 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 17:55:09 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	table->stop = 0;
	table->fatal = 0;
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

int	start_simulation(t_table *table)
{
	int	rc;
	int	created;

	if (init_start_state(table))
		return (1);
	rc = 0;
	created = 0;
	rc = start_philo_threads(table, &created);
	if (!rc && created == table->philo_count)
	{
		if (monitor_loop(table))
		{
			fatal_stop_best_effort(table);
			rc = 1;
		}
	}
	while (created > 0)
	{
		created--;
		if (pthread_join(table->philos[created].thread, NULL))
			rc = 1;
	}
	return (rc);
}
