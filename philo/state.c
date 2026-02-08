/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:44:07 by amtan             #+#    #+#             */
/*   Updated: 2026/02/08 23:53:46 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_stop(t_table *table, int *out)
{
	int	stop;

	if (!table || !out)
		return (1);
	if (pthread_mutex_lock(&table->stop_mtx))
		return (1);
	stop = table->stop;
	if (pthread_mutex_unlock(&table->stop_mtx))
		return (1);
	*out = stop;
	return (0);
}

int	set_stop(t_table *table, int value)
{
	if (!table)
		return (1);
	if (pthread_mutex_lock(&table->stop_mtx))
		return (1);
	table->stop = value;
	if (pthread_mutex_unlock(&table->stop_mtx))
		return (1);
	return (0);
}

int	get_last_meal(t_philo *philo, long *out)
{
	long	last_meal_ms;

	if (!philo || !out || !philo->table)
		return (1);
	if (pthread_mutex_lock(&philo->table->state_mtx))
		return (1);
	last_meal_ms = philo->last_meal_ms;
	if (pthread_mutex_unlock(&philo->table->state_mtx))
		return (1);
	*out = last_meal_ms;
	return (0);
}

int	set_last_meal(t_philo *philo, long value)
{
	if (!philo || !philo->table)
		return (1);
	if (pthread_mutex_lock(&philo->table->state_mtx))
		return (1);
	philo->last_meal_ms = value;
	if (pthread_mutex_unlock(&philo->table->state_mtx))
		return (1);
	return (0);
}

int	get_meals_eaten(t_philo *philo, int *out)
{
	int	meals_eaten;

	if (!philo || !out || !philo->table)
		return (1);
	if (pthread_mutex_lock(&philo->table->state_mtx))
		return (1);
	meals_eaten = philo->meals_eaten;
	if (pthread_mutex_unlock(&philo->table->state_mtx))
		return (1);
	*out = meals_eaten;
	return (0);
}
