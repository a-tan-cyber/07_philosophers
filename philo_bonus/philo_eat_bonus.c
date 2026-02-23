/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:28:07 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 17:52:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>

static int	unlock_meal(sem_t *meal_sem, int rc)
{
	if (sem_post_retry(meal_sem))
		exit(EXIT_ERR);
	return (rc);
}

static int	start_eating(t_philo *philo, long now)
{
	philo->last_meal_ms = now;
	if (print_state(philo, "is eating"))
		return (1);
	philo->meals_eaten++;
	return (0);
}

static int	begin_eat(t_philo *philo)
{
	long	last;
	long	now;

	if (!philo || !philo->table)
		return (1);
	if (sem_wait_retry(philo->meal_sem))
		return (1);
	last = philo->last_meal_ms;
	if (now_ms(&now))
		return (unlock_meal(philo->meal_sem, 1));
	if (now - last >= philo->table->time_die)
	{
		if (sem_post_retry(philo->meal_sem))
			exit(EXIT_ERR);
		if (print_death(philo))
			exit(EXIT_ERR);
		exit(EXIT_DIED);
	}
	if (start_eating(philo, now))
		return (unlock_meal(philo->meal_sem, 1));
	return (unlock_meal(philo->meal_sem, 0));
}

int	philo_eat_step(t_philo *philo)
{
	t_table	*table;

	if (!philo || !philo->table)
		return (1);
	table = philo->table;
	if (begin_eat(philo))
		return (1);
	if (ms_sleep(table->time_eat))
		return (1);
	return (0);
}
