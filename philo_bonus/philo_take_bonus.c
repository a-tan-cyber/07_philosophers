/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_take_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:05:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:36:55 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>

static void	fatal_post(sem_t *sem)
{
	if (!sem)
		exit(EXIT_ERR);
	if (sem_post_retry(sem))
		exit(EXIT_ERR);
}

static int	take_fork_print(t_philo *philo, t_table *table)
{
	if (sem_wait_retry(table->sem_forks))
		return (-1);
	if (print_state(philo, "has taken a fork"))
	{
		fatal_post(table->sem_forks);
		return (1);
	}
	return (0);
}

static int	take_second_fork(t_philo *philo, t_table *table)
{
	if (sem_wait_retry(table->sem_forks))
		return (-1);
	fatal_post(table->sem_room);
	if (print_state(philo, "has taken a fork"))
	{
		fatal_post(table->sem_forks);
		fatal_post(table->sem_forks);
		return (1);
	}
	return (0);
}

int	philo_take_forks(t_philo *philo)
{
	t_table	*table;
	int		rc;

	if (!philo || !philo->table)
		return (1);
	table = philo->table;
	if (sem_wait_retry(table->sem_room))
		exit(EXIT_ERR);
	rc = take_fork_print(philo, table);
	if (rc)
	{
		fatal_post(table->sem_room);
		if (rc < 0)
			exit(EXIT_ERR);
		return (1);
	}
	rc = take_second_fork(philo, table);
	if (rc < 0)
	{
		fatal_post(table->sem_forks);
		fatal_post(table->sem_room);
		exit(EXIT_ERR);
	}
	return (rc);
}

int	philo_put_forks(t_philo *philo)
{
	t_table	*table;

	if (!philo || !philo->table)
		return (1);
	table = philo->table;
	fatal_post(table->sem_forks);
	fatal_post(table->sem_forks);
	return (0);
}
