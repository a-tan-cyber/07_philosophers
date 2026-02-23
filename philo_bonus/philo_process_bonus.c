/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:00:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:36:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>

static void	fatal_close(sem_t *sem)
{
	if (!sem)
		exit(EXIT_ERR);
	if (sem_close(sem))
		exit(EXIT_ERR);
}

static void	close_other_meals(t_philo *philo)
{
	t_table	*table;
	int		i;

	if (!philo || !philo->table)
		return ;
	table = philo->table;
	if (!table->philos)
		return ;
	i = 0;
	while (i < table->philo_count)
	{
		if (table->philos[i].id != philo->id
			&& table->philos[i].meal_sem
			&& table->philos[i].meal_sem != SEM_FAILED)
		{
			fatal_close(table->philos[i].meal_sem);
			table->philos[i].meal_sem = NULL;
		}
		i++;
	}
}

static int	init_last_meal(t_philo *philo)
{
	long	now;

	if (now_ms(&now))
		return (1);
	if (sem_wait_retry(philo->meal_sem))
		return (1);
	philo->last_meal_ms = now;
	if (sem_post_retry(philo->meal_sem))
		return (1);
	return (0);
}

static void	philo_single(t_philo *philo)
{
	if (sem_wait_retry(philo->table->sem_forks))
		exit(EXIT_ERR);
	print_state(philo, "has taken a fork");
	ms_sleep(philo->table->time_die + 1);
	exit(EXIT_DIED);
}

void	philo_process(t_philo *philo)
{
	int	rc;

	if (!philo || !philo->table)
		exit(EXIT_ERR);
	close_other_meals(philo);
	if (sem_wait_retry(philo->table->sem_start))
		exit(EXIT_ERR);
	fatal_close(philo->table->sem_start);
	philo->table->sem_start = NULL;
	if (init_last_meal(philo))
		exit(EXIT_ERR);
	if (pthread_create(&philo->monitor, 0, monitor_routine, philo))
		exit(EXIT_ERR);
	if (pthread_detach(philo->monitor))
		exit(EXIT_ERR);
	if (philo->table->philo_count == 1)
		philo_single(philo);
	rc = philo_loop(philo);
	if (rc == 0)
		exit(EXIT_FULL);
	exit(EXIT_ERR);
}
