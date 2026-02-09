/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_lock.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 18:21:05 by amtan             #+#    #+#             */
/*   Updated: 2026/02/09 16:58:59 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock_print_state(t_table *table)
{
	if (!table)
		return (1);
	if (pthread_mutex_lock(&table->print_mtx))
		return (1);
	if (pthread_mutex_lock(&table->state_mtx))
	{
		pthread_mutex_unlock(&table->print_mtx);
		return (1);
	}
	return (0);
}

int	unlock_both_return(t_table *table, int rc)
{
	if (pthread_mutex_unlock(&table->state_mtx))
		rc = 1;
	if (pthread_mutex_unlock(&table->print_mtx))
		return (1);
	return (rc);
}
