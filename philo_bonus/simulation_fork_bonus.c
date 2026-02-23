/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_fork_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:37:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>

int	sim_fork_children(t_table *table, int *created)
{
	pid_t	pid;

	while (*created < table->philo_count)
	{
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
			philo_process(&table->philos[*created]);
		table->pids[*created] = pid;
		(*created)++;
	}
	return (0);
}
