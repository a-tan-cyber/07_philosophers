/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:40:58 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 17:46:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdlib.h>
#include <string.h>

void	destroy_all(t_table *table)
{
	int	i;

	if (!table)
		return ;
	if (table->forks)
	{
		i = 0;
		while (i < table->forks_init_count)
			pthread_mutex_destroy(&table->forks[i++]);
		free(table->forks);
		table->forks = NULL;
	}
	table->forks_init_count = 0;
	if (table->print_mtx_inited)
		pthread_mutex_destroy(&table->print_mtx);
	table->print_mtx_inited = 0;
	if (table->state_mtx_inited)
		pthread_mutex_destroy(&table->state_mtx);
	table->state_mtx_inited = 0;
	free(table->philos);
	table->philos = NULL;
}

static int	die_init(t_table *table, const char *msg)
{
	destroy_all(table);
	return (error_msg(msg));
}

static int	init_forks(t_table *table)
{
	int	i;

	table->forks_init_count = 0;
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
			return (1);
		table->forks_init_count++;
		i++;
	}
	return (0);
}

static void	init_philos(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->philo_count)
	{
		philo = &table->philos[i];
		memset(philo, 0, sizeof(*philo));
		philo->id = i + 1;
		philo->left_fork = &table->forks[i];
		philo->right_fork = &table->forks[(i + 1) % table->philo_count];
		philo->table = table;
		i++;
	}
}

int	init_table(t_table *table)
{
	if (!table)
		return (error_msg("internal error - init_table()"));
	if (pthread_mutex_init(&table->print_mtx, NULL))
		return (die_init(table, "failed to init print_mtx"));
	table->print_mtx_inited = 1;
	if (pthread_mutex_init(&table->state_mtx, NULL))
		return (die_init(table, "failed to init state_mtx"));
	table->state_mtx_inited = 1;
	table->forks = malloc(table->philo_count * sizeof(pthread_mutex_t));
	if (!table->forks)
		return (die_init(table, "failed to malloc forks"));
	if (init_forks(table))
		return (die_init(table, "failed to init fork"));
	table->philos = malloc(table->philo_count * sizeof(t_philo));
	if (!table->philos)
		return (die_init(table, "failed to malloc philos"));
	init_philos(table);
	return (0);
}
