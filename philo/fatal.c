/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fatal.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:48:10 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 22:53:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fatal_stop_no_lock(t_table *table)
{
	if (!table)
		return ;
	table->stop = 1;
	table->fatal = 1;
}

void	fatal_stop_best_effort(t_table *table)
{
	if (!table)
		return ;
	if (pthread_mutex_lock(&table->stop_mtx))
	{
		fatal_stop_no_lock(table);
		return ;
	}
	table->stop = 1;
	if (pthread_mutex_unlock(&table->stop_mtx))
	{
		fatal_stop_no_lock(table);
		return ;
	}
	if (pthread_mutex_lock(&table->state_mtx))
	{
		table->fatal = 1;
		return ;
	}
	table->fatal = 1;
	if (pthread_mutex_unlock(&table->state_mtx))
		table->fatal = 1;
}

int	fatal_return(t_table *table)
{
	fatal_stop_best_effort(table);
	return (1);
}

int	fatal_return_no_lock(t_table *table)
{
	fatal_stop_no_lock(table);
	return (1);
}
