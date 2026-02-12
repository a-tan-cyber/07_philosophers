/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_take.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:05:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/12 17:55:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	pick_fork_order(t_philo *philo,
		pthread_mutex_t **first, pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

static int	unlock_forks_fail(t_table *table, pthread_mutex_t *first,
				pthread_mutex_t *second, int fatal)
{
	int	unlock_failed;

	unlock_failed = 0;
	if (second && pthread_mutex_unlock(second))
		unlock_failed = 1;
	if (first && pthread_mutex_unlock(first))
		unlock_failed = 1;
	if (fatal || unlock_failed)
		fatal_stop_best_effort(table);
	return (1);
}

static int	unlock_forks_fail_waiter(t_table *table, pthread_mutex_t *first,
				pthread_mutex_t *second, int fatal)
{
	int	waiter_failed;

	waiter_failed = pthread_mutex_unlock(&table->waiter_mtx);
	if (waiter_failed)
		fatal = 1;
	return (unlock_forks_fail(table, first, second, fatal));
}

static int	take_fork_checked(t_philo *philo, pthread_mutex_t *fork,
				pthread_mutex_t **held)
{
	int	stop;

	if (pthread_mutex_lock(fork))
		return (unlock_forks_fail_waiter(philo->table, held[0], held[1], 1));
	if (!held[0])
		held[0] = fork;
	else
		held[1] = fork;
	if (print_state(philo, "has taken a fork"))
		return (unlock_forks_fail_waiter(philo->table, held[0], held[1], 1));
	if (get_stop(philo->table, &stop))
		return (unlock_forks_fail_waiter(philo->table, held[0], held[1], 1));
	if (stop)
		return (unlock_forks_fail_waiter(philo->table, held[0], held[1], 0));
	return (0);
}

int	philo_take_forks(t_philo *philo,
		pthread_mutex_t **out_first, pthread_mutex_t **out_second)
{
	pthread_mutex_t	*held[2];
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	held[0] = NULL;
	held[1] = NULL;
	pick_fork_order(philo, &first, &second);
	*out_first = first;
	*out_second = second;
	if (pthread_mutex_lock(&philo->table->waiter_mtx))
		return (unlock_forks_fail(philo->table, NULL, NULL, 1));
	if (take_fork_checked(philo, first, held))
		return (1);
	if (take_fork_checked(philo, second, held))
		return (1);
	if (pthread_mutex_unlock(&philo->table->waiter_mtx))
		return (unlock_forks_fail(philo->table, held[0], held[1], 1));
	return (0);
}
