/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:36:07 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>

static void	close_one_sem(sem_t **sem)
{
	if (sem && *sem && *sem != SEM_FAILED)
	{
		if (sem_close(*sem))
			exit(EXIT_ERR);
	}
	if (sem)
		*sem = NULL;
}

static void	close_meal_sems(t_table *table)
{
	int	i;

	if (!table || !table->philos)
		return ;
	i = 0;
	while (i < table->philo_count)
	{
		close_one_sem(&table->philos[i].meal_sem);
		i++;
	}
}

static void	unlink_meal_sems(t_table *table)
{
	char	name[64];
	int		i;

	if (!table)
		return ;
	i = 0;
	while (i < table->philo_count)
	{
		build_meal_name(i + 1, name, (int) sizeof(name));
		sem_unlink(name);
		i++;
	}
}

void	destroy_all(t_table *table)
{
	if (!table)
		return ;
	close_one_sem(&table->sem_forks);
	close_one_sem(&table->sem_room);
	close_one_sem(&table->sem_print);
	close_one_sem(&table->sem_end);
	close_one_sem(&table->sem_start);
	close_meal_sems(table);
	sem_unlink(SEM_FORKS_NAME);
	sem_unlink(SEM_ROOM_NAME);
	sem_unlink(SEM_PRINT_NAME);
	sem_unlink(SEM_END_NAME);
	sem_unlink(SEM_START_NAME);
	unlink_meal_sems(table);
	free(table->pids);
	table->pids = NULL;
	free(table->philos);
	table->philos = NULL;
}
