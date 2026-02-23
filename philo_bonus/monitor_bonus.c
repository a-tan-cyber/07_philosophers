/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:00:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:36:30 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>
#include <unistd.h>

static int	read_last_meal(t_philo *philo, long *out)
{
	if (sem_wait_retry(philo->meal_sem))
		return (1);
	*out = philo->last_meal_ms;
	if (sem_post_retry(philo->meal_sem))
		return (1);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	now;
	long	last;

	philo = (t_philo *)arg;
	while (1)
	{
		if (now_ms(&now) || read_last_meal(philo, &last))
			exit(EXIT_ERR);
		if (now - last >= philo->table->time_die)
		{
			print_death(philo);
			exit(EXIT_DIED);
		}
		usleep(1000);
	}
	return (NULL);
}
