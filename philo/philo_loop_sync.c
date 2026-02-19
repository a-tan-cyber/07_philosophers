/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_loop_sync.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:40:46 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_stop_check(t_table *table, int *out_stop)
{
	if (get_stop(table, out_stop))
		return (fatal_return(table));
	if (*out_stop)
		return (1);
	return (0);
}

int	philo_unlock_forks(t_table *table,
		pthread_mutex_t *first, pthread_mutex_t *second)
{
	int	rc;

	rc = 0;
	if (second && pthread_mutex_unlock(second))
		rc = 1;
	if (first && pthread_mutex_unlock(first))
		rc = 1;
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}
