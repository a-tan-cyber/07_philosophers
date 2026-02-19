/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:42:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:43:10 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monitor_loop(t_table *table)
{
	int	ended;

	if (!table)
		return (1);
	if (pthread_mutex_lock(&table->start_mtx))
		return (fatal_return(table));
	if (pthread_mutex_unlock(&table->start_mtx))
		return (fatal_return(table));
	ended = 0;
	while (!ended)
	{
		if (monitor_step(table, &ended))
			return (1);
	}
	return (0);
}
