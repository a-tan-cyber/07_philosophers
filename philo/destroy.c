/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 10:15:00 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdlib.h>

static void	destroy_forks(t_table *table)
{
	int	i;

	if (!table->forks)
	{
		table->forks_init_count = 0;
		return ;
	}
	i = 0;
	while (i < table->forks_init_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
	table->forks = NULL;
	table->forks_init_count = 0;
}

static void	destroy_mutexes(t_table *table)
{
	if (table->print_mtx_inited)
		pthread_mutex_destroy(&table->print_mtx);
	table->print_mtx_inited = 0;
	if (table->stop_mtx_inited)
		pthread_mutex_destroy(&table->stop_mtx);
	table->stop_mtx_inited = 0;
	if (table->state_mtx_inited)
		pthread_mutex_destroy(&table->state_mtx);
	table->state_mtx_inited = 0;
	if (table->start_mtx_inited)
		pthread_mutex_destroy(&table->start_mtx);
	table->start_mtx_inited = 0;
	if (table->ready_mtx_inited)
		pthread_mutex_destroy(&table->ready_mtx);
	table->ready_mtx_inited = 0;
}

void	destroy_all(t_table *table)
{
	if (!table)
		return ;
	destroy_forks(table);
	destroy_mutexes(table);
	free(table->philos);
	table->philos = NULL;
}
