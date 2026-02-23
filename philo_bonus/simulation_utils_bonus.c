/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:37:09 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	sim_kill_all(t_table *table, int created)
{
	int	i;

	i = 0;
	while (i < created)
	{
		if (table->pids[i] > 0)
			kill(table->pids[i], SIGTERM);
		i++;
	}
	usleep(20000);
	i = 0;
	while (i < created)
	{
		if (table->pids[i] > 0)
			kill(table->pids[i], SIGKILL);
		i++;
	}
}

int	sim_wait_all(void)
{
	while (1)
	{
		if (waitpid(-1, 0, 0) > 0)
			continue ;
		if (errno == EINTR)
			continue ;
		if (errno == ECHILD)
			break ;
		return (1);
	}
	return (0);
}

int	sim_post_start_all(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		if (sem_post_retry(table->sem_start))
			return (1);
		i++;
	}
	return (0);
}

void	sim_close_parent_meals(t_table *table, int created)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		if (table->philos[i].meal_sem
			&& table->philos[i].meal_sem != SEM_FAILED)
		{
			if (sem_close(table->philos[i].meal_sem))
			{
				sim_kill_all(table, created);
				sim_wait_all();
				destroy_all(table);
				exit(EXIT_ERR);
			}
			table->philos[i].meal_sem = NULL;
		}
		i++;
	}
}

void	sim_close_start_sem(t_table *table, int created)
{
	if (!table || !table->sem_start || table->sem_start == SEM_FAILED)
		return ;
	if (sem_close(table->sem_start))
	{
		table->sem_start = NULL;
		sim_kill_all(table, created);
		sim_wait_all();
		destroy_all(table);
		exit(EXIT_ERR);
	}
	table->sem_start = NULL;
}
