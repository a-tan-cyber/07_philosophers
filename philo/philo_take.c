/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_take.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:05:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:37:56 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	unlock_both(t_table *table, pthread_mutex_t *a, pthread_mutex_t *b)
{
	int	rc;

	rc = 0;
	if (b && pthread_mutex_unlock(b))
		rc = 1;
	if (a && pthread_mutex_unlock(a))
		rc = 1;
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}

static void	pick_order(t_philo *philo,
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

static int	lock_first(t_philo *philo, pthread_mutex_t *first)
{
	t_table	*table;
	int		stop;

	table = philo->table;
	if (pthread_mutex_lock(first))
		return (fatal_return(table));
	if (get_stop(table, &stop))
	{
		pthread_mutex_unlock(first);
		return (fatal_return(table));
	}
	if (stop)
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	if (print_state(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(first);
		return (fatal_return(table));
	}
	return (0);
}

static int	lock_second(t_philo *philo,
		pthread_mutex_t *first, pthread_mutex_t *second)
{
	t_table	*table;
	int		stop;

	table = philo->table;
	if (pthread_mutex_lock(second))
	{
		pthread_mutex_unlock(first);
		return (fatal_return(table));
	}
	if (get_stop(table, &stop))
	{
		unlock_both(table, first, second);
		return (fatal_return(table));
	}
	if (stop)
	{
		unlock_both(table, first, second);
		return (1);
	}
	if (print_state(philo, "has taken a fork"))
	{
		unlock_both(table, first, second);
		return (fatal_return(table));
	}
	return (0);
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
	if (get_stop(table, &stop))
		return (fatal_return(table));
	if (stop)
		return (1);
	pick_order(philo, &first, &second);
	*out_first = first;
	*out_second = second;
	if (lock_first(philo, first))
		return (1);
	return (lock_second(philo, first, second));
}
