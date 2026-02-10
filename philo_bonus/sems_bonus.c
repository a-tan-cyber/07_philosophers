/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:05:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 23:15:34 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SEM_FORKS "/philo_bonus_forks"
#define SEM_ROOM  "/philo_bonus_room"
#define SEM_PRINT "/philo_bonus_print"
#define SEM_STATE "/philo_bonus_state"

static void	unlink_all(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_ROOM);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STATE);
}

int	sems_open(t_table *table)
{
	unsigned int	room;

	if (!table)
		return (1);
	unlink_all();
	table->sem_forks = sem_open(SEM_FORKS, O_CREAT, 0644, table->philo_count);
	table->sem_print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	table->sem_state = sem_open(SEM_STATE, O_CREAT, 0644, 1);
	room = 1;
	if (table->philo_count >= 2)
		room = (unsigned int)(table->philo_count / 2);
	table->sem_room = sem_open(SEM_ROOM, O_CREAT, 0644, room);
	if (table->sem_forks == SEM_FAILED || table->sem_print == SEM_FAILED
		|| table->sem_state == SEM_FAILED || table->sem_room == SEM_FAILED)
		return (1);
	return (0);
}

void	sems_close_unlink(t_table *table)
{
	if (!table)
		return ;
	if (table->sem_forks && table->sem_forks != SEM_FAILED)
		sem_close(table->sem_forks);
	if (table->sem_room && table->sem_room != SEM_FAILED)
		sem_close(table->sem_room);
	if (table->sem_print && table->sem_print != SEM_FAILED)
		sem_close(table->sem_print);
	if (table->sem_state && table->sem_state != SEM_FAILED)
		sem_close(table->sem_state);
	unlink_all();
}

int	sem_wait_retry(sem_t *sem)
{
	while (sem_wait(sem) == -1)
	{
		if (errno != EINTR)
			return (1);
	}
	return (0);
}
