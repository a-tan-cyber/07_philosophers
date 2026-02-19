/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:28:29 by amtan             #+#    #+#             */
/*   Updated: 2026/02/18 18:51:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	wait_start_gate(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	if (pthread_mutex_lock(&table->ready_mtx))
		return (1);
	table->ready_count++;
	if (pthread_mutex_unlock(&table->ready_mtx))
		return (1);
	if (pthread_mutex_lock(&table->start_mtx))
		return (1);
	if (pthread_mutex_unlock(&table->start_mtx))
		return (1);
	return (0);
}

static int	philo_single(t_philo *philo)
{
	t_table	*table;
	int		locked;
	int		rc;

	if (!philo || !philo->table || !philo->left_fork)
		return (1);
	table = philo->table;
	locked = 0;
	rc = 0;
	if (pthread_mutex_lock(philo->left_fork))
		rc = 1;
	else
		locked = 1;
	if (!rc && print_state(philo, "has taken a fork"))
		rc = 1;
	if (!rc && ms_sleep(table, table->time_die + 1))
		rc = 1;
	if (locked && pthread_mutex_unlock(philo->left_fork))
		rc = 1;
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!philo || !philo->table)
		return (NULL);
	if (wait_start_gate(philo))
	{
		fatal_stop_best_effort(philo->table);
		return (NULL);
	}
	if (philo->table->philo_count == 1)
		philo_single(philo);
	else
		philo_loop(philo);
	return (NULL);
}
