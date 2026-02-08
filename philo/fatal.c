/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fatal.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:48:10 by amtan             #+#    #+#             */
/*   Updated: 2026/02/08 23:54:18 by amtan            ###   ########.fr       */
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
	table->fatal = 1;
	if (pthread_mutex_unlock(&table->stop_mtx))
		fatal_stop_no_lock(table);
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
