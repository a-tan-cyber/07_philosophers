/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:40:03 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 17:50:43 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>

static long	compute_start_delay(t_philo *philo)
{
	t_table	*table;
	long	slack;
	long	delay;

	table = philo->table;
	slack = table->time_die - (table->time_eat + table->time_sleep);
	if (slack < 0)
		slack = 0;
	if (table->philo_count >= 32 && slack >= 2 && slack <= 20)
		return ((long)(philo->id & 1));
	delay = table->time_eat / 10;
	if (delay > 2)
		delay = 2;
	if (slack > 0 && slack <= 20 && delay > 1)
		delay = 1;
	if (slack > 0 && delay > slack)
		delay = slack;
	if (philo->id % 2 == 0)
		return (delay);
	return (0);
}

static int	loop_step(t_philo *philo)
{
	if (philo_take_forks(philo))
		return (1);
	if (philo_eat_step(philo))
	{
		philo_put_forks(philo);
		return (1);
	}
	if (philo_put_forks(philo))
		return (1);
	if (philo->table->must_eat_enabled
		&& philo->meals_eaten >= philo->table->must_eat_count)
		return (0);
	return (philo_sleep_think(philo));
}

int	philo_loop(t_philo *philo)
{
	long	delay;

	if (!philo || !philo->table)
		return (1);
	delay = compute_start_delay(philo);
	if (delay > 0)
		usleep((useconds_t)(delay * 1000));
	while (1)
	{
		if (loop_step(philo))
			return (1);
		if (philo->table->must_eat_enabled
			&& philo->meals_eaten >= philo->table->must_eat_count)
			return (0);
	}
}
