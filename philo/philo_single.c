/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_single.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:39:33 by amtan             #+#    #+#             */
/*   Updated: 2026/02/08 18:11:56 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_single(t_philo *philo)
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
