/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:40:58 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:36:10 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>
#include <string.h>

static int	alloc_arrays(t_table *table)
{
	table->pids = malloc(sizeof(*table->pids) * table->philo_count);
	if (!table->pids)
		return (error_msg("failed to malloc pids"));
	memset(table->pids, 0, sizeof(*table->pids) * table->philo_count);
	table->philos = malloc(sizeof(*table->philos) * table->philo_count);
	if (!table->philos)
	{
		free(table->pids);
		table->pids = NULL;
		return (error_msg("failed to malloc philos"));
	}
	return (0);
}

static void	init_philos(t_table *table)
{
	int		i;
	t_philo	*p;

	i = 0;
	while (i < table->philo_count)
	{
		p = &table->philos[i];
		memset(p, 0, sizeof(*p));
		p->id = i + 1;
		p->table = table;
		i++;
	}
}

static int	open_base_sems(t_table *table)
{
	unsigned int	room;

	room = 1;
	if (table->philo_count > 1)
		room = (unsigned int)(table->philo_count - 1);
	if (sem_open_create(SEM_FORKS_NAME, (unsigned int)table->philo_count,
			&table->sem_forks))
		return (error_msg("failed to open forks semaphore"));
	if (sem_open_create(SEM_ROOM_NAME, room, &table->sem_room))
		return (error_msg("failed to open room semaphore"));
	if (sem_open_create(SEM_PRINT_NAME, 1, &table->sem_print))
		return (error_msg("failed to open print semaphore"));
	if (sem_open_create(SEM_END_NAME, 1, &table->sem_end))
		return (error_msg("failed to open end semaphore"));
	if (sem_open_create(SEM_START_NAME, 0, &table->sem_start))
		return (error_msg("failed to open start semaphore"));
	return (0);
}

static int	open_meal_sems(t_table *table)
{
	char	name[64];
	int		i;

	i = 0;
	while (i < table->philo_count)
	{
		build_meal_name(i + 1, name, (int) sizeof(name));
		if (sem_open_create(name, 1, &table->philos[i].meal_sem))
			return (error_msg("failed to open meal semaphore"));
		i++;
	}
	return (0);
}

int	init_table(t_table *table)
{
	if (!table)
		return (error_msg("internal error - init_table()"));
	if (alloc_arrays(table))
		return (1);
	init_philos(table);
	if (open_base_sems(table) || open_meal_sems(table))
	{
		destroy_all(table);
		return (1);
	}
	return (0);
}
