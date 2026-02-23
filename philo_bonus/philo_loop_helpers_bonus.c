/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_loop_helpers_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:36:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>

static long	compute_base_jitter(t_table *table)
{
	long	bj;

	bj = table->time_eat / 50;
	if (bj < 1)
		bj = 1;
	else if (bj > 5)
		bj = 5;
	return (bj);
}

static long	compute_think_delay_jitter(t_philo *philo)
{
	t_table	*table;
	long	slack;
	long	delay;

	table = philo->table;
	if (!table || table->philo_count <= 1)
		return (0);
	slack = table->time_die - (table->time_eat + table->time_sleep);
	if (slack <= 0)
		return (0);
	if (slack <= 20)
	{
		if (table->philo_count >= 32 && table->philo_count % 2 == 0)
			return ((long)(philo->id & 1));
		if (table->philo_count >= 32)
			return ((long)((philo->id + philo->think_phase) & 1));
		return (0);
	}
	delay = philo->id % 3;
	if (table->philo_count % 2 != 0 || table->philo_count >= 32)
		delay += ((philo->id * 3) % (compute_base_jitter(table) + 1));
	if (delay > table->time_eat)
		delay = table->time_eat;
	return (delay);
}

static long	compute_think_delay(t_philo *philo)
{
	t_table	*table;
	long	slack;
	long	delay;

	table = philo->table;
	if (!table || table->philo_count <= 1)
		return (0);
	slack = table->time_die - (table->time_eat + table->time_sleep);
	if (table->philo_count % 2 != 0 && table->time_sleep < table->time_eat
		&& slack > 0)
	{
		delay = slack / 2;
		if (delay > table->time_eat)
			delay = table->time_eat;
		return (delay);
	}
	return (compute_think_delay_jitter(philo));
}

int	philo_sleep_think(t_philo *philo)
{
	t_table	*table;
	long	delay;

	if (!philo || !philo->table)
		return (1);
	table = philo->table;
	if (print_state(philo, "is sleeping"))
		return (1);
	if (ms_sleep(table->time_sleep))
		return (1);
	if (print_state(philo, "is thinking"))
		return (1);
	philo->think_phase ^= 1;
	delay = compute_think_delay(philo);
	if (delay > 0)
	{
		if (delay <= 2)
			usleep((useconds_t)(delay * 1000));
		else if (ms_sleep(delay))
			return (1);
	}
	return (0);
}
