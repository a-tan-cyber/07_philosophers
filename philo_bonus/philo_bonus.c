/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:40:03 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 23:40:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>
#include <unistd.h>

static int	read_state(t_philo *philo, long *last, int *done)
{
	if (sem_wait_retry(philo->table->sem_state))
		return (1);
	*last = philo->last_meal_ms;
	*done = philo->done;
	if (sem_post(philo->table->sem_state))
		return (1);
	return (0);
}

static void	*monitor_thread(void *arg)
{
	t_philo	*philo;
	long	now;
	long	last;
	int		done;

	philo = (t_philo *)arg;
	while (1)
	{
		if (now_ms(&now))
			exit(1);
		if (read_state(philo, &last, &done))
			exit(1);
		if (done)
			return (NULL);
		if (now - last >= philo->table->time_die)
		{
			if (print_death(philo, now))
				exit(1);
			exit(1);
		}
		usleep(1000);
	}
}

static void	run_single(t_philo *philo)
{
	if (sem_wait_retry(philo->table->sem_forks))
		exit(1);
	if (print_state(philo, "has taken a fork"))
		exit(1);
	while (1)
		usleep(1000);
}


static void	philo_loop(t_philo *philo)
{
	int	done;

	if ((philo->id % 2) == 0)
		usleep(1000);
	while (1)
	{
		if (take_forks(philo) || eat_step(philo) || put_forks(philo))
			exit(1);
		if (sem_wait_retry(philo->table->sem_state))
			exit(1);
		done = philo->done;
		if (sem_post(philo->table->sem_state))
			exit(1);
		if (done)
			exit(0);
		if (sleep_think_step(philo))
			exit(1);
	}
}

void	philo_process(t_table *table, int id)
{
	t_philo	philo;

	philo.table = table;
	philo.id = id;
	philo.meals = 0;
	philo.done = 0;
	philo.last_meal_ms = table->start_ms;
	if (pthread_create(&philo.monitor, 0, monitor_thread, &philo))
		exit(1);
	pthread_detach(philo.monitor);
	if (table->philo_count == 1)
		run_single(&philo);
	else
		philo_loop(&philo);
	exit(0);
}
