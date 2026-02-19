/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:17:42 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:49:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monitor_find_dead_locked(t_table *table, t_philo **out_dead, long now_ms)
{
	int		i;
	long	elapsed_ms;
	long	best_ms;

	if (!table || !out_dead || !table->philos)
		return (1);
	*out_dead = NULL;
	best_ms = 0;
	i = 0;
	while (i < table->philo_count)
	{
		elapsed_ms = now_ms - table->philos[i].last_meal_ms;
		if (elapsed_ms < 0)
			elapsed_ms = 0;
		if (elapsed_ms >= table->time_die
			&& (*out_dead == NULL || elapsed_ms > best_ms))
		{
			*out_dead = &table->philos[i];
			best_ms = elapsed_ms;
		}
		i++;
	}
	return (0);
}

int	monitor_all_full_locked(t_table *table, int *out_full)
{
	int	i;

	if (!table || !out_full || !table->philos)
		return (1);
	*out_full = 1;
	i = 0;
	while (i < table->philo_count)
	{
		if (table->philos[i].meals_eaten < table->must_eat_count)
		{
			*out_full = 0;
			break ;
		}
		i++;
	}
	return (0);
}
