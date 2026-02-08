/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 21:45:28 by amtan             #+#    #+#             */
/*   Updated: 2026/02/08 16:55:45 by amtan            ###   ########.fr       */
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
	i = 0;
	while (i < table->philo_count)
	{
		table->philos[i].last_meal_ms = start;
		i++;
	}
	table->stop = 0;
	table->fatal = 0;
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
