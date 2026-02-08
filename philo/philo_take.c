/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_take.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:05:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/08 23:09:59 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	pick_fork_order(t_philo *philo,
				pthread_mutex_t **first, pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
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

int	philo_take_forks(t_philo *philo,
		pthread_mutex_t **out_first, pthread_mutex_t **out_second)
{
	t_table			*table;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	int				stop;

	if (!philo || !philo->table || !out_first || !out_second)
		return (1);
	table = philo->table;
	pick_fork_order(philo, &first, &second);
	*out_first = first;
	*out_second = second;
	if (pthread_mutex_lock(first))
		return (unlock_forks_fail(table, NULL, NULL, 1));
	if (print_state(philo, "has taken a fork"))
		return (unlock_forks_fail(table, first, NULL, 1));
	if (get_stop(table, &stop))
		return (unlock_forks_fail(table, first, NULL, 1));
	if (stop)
		return (unlock_forks_fail(table, first, NULL, 0));
	if (pthread_mutex_lock(second))
		return (unlock_forks_fail(table, first, NULL, 1));
	if (print_state(philo, "has taken a fork"))
		return (unlock_forks_fail(table, first, second, 1));
	return (0);
}
