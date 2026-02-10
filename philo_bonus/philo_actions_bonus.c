/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:28:07 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 23:50:24 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	rollback_take(t_philo *philo, int forks_taken, int room_taken)
{
	if (forks_taken >= 1 && sem_post(philo->table->sem_forks))
		return (1);
	if (forks_taken >= 2 && sem_post(philo->table->sem_forks))
		return (1);
	if (room_taken && sem_post(philo->table->sem_room))
		return (1);
	return (1);
}

int	take_forks(t_philo *philo)
{
	int	room_taken;
	int	forks_taken;

	room_taken = 0;
	forks_taken = 0;
	if (sem_wait_retry(philo->table->sem_room))
		return (1);
	room_taken = 1;
	if (sem_wait_retry(philo->table->sem_forks))
		return (rollback_take(philo, forks_taken, room_taken));
	forks_taken = 1;
	if (print_state(philo, "has taken a fork"))
		return (rollback_take(philo, forks_taken, room_taken));
	if (sem_wait_retry(philo->table->sem_forks))
		return (rollback_take(philo, forks_taken, room_taken));
	forks_taken = 2;
	if (print_state(philo, "has taken a fork"))
		return (rollback_take(philo, forks_taken, room_taken));
	return (0);
}

int	put_forks(t_philo *philo)
{
	if (sem_post(philo->table->sem_forks))
		return (1);
	if (sem_post(philo->table->sem_forks))
		return (1);
	if (sem_post(philo->table->sem_room))
		return (1);
	return (0);
}

int	eat_step(t_philo *philo)
{
	long	now;

	if (now_ms(&now))
		return (1);
	if (sem_wait_retry(philo->table->sem_state))
		return (1);
	philo->last_meal_ms = now;
	if (sem_post(philo->table->sem_state))
		return (1);
	if (print_state(philo, "is eating"))
		return (1);
	if (ms_sleep(philo->table->time_eat))
		return (1);
	if (sem_wait_retry(philo->table->sem_state))
		return (1);
	philo->meals++;
	if (philo->table->must_eat_enabled
		&& philo->meals >= philo->table->must_eat_count)
		philo->done = 1;
	if (sem_post(philo->table->sem_state))
		return (1);
	return (0);
}

int	sleep_think_step(t_philo *philo)
{
	if (print_state(philo, "is sleeping"))
		return (1);
	if (ms_sleep(philo->table->time_sleep))
		return (1);
	if (print_state(philo, "is thinking"))
		return (1);
	return (0);
}
