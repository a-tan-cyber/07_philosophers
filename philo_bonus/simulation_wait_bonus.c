/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_wait_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 12:37:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <errno.h>
#include <sys/wait.h>

static void	mark_reaped(t_table *table, pid_t pid)
{
	int	i;

	if (!table || !table->pids || pid <= 0)
		return ;
	i = 0;
	while (i < table->philo_count)
	{
		if (table->pids[i] == pid)
		{
			table->pids[i] = 0;
			return ;
		}
		i++;
	}
}

static int	handle_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_DIED);
}

int	sim_wait_until_end(t_table *table, int created)
{
	int		status;
	int		full;
	int		code;
	pid_t	pid;

	full = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1 && errno == EINTR)
			continue ;
		if (pid <= 0)
			return (1);
		mark_reaped(table, pid);
		code = handle_exit_status(status);
		if (code == EXIT_DIED)
			return (EXIT_DIED);
		if (!table->must_eat_enabled)
			return (code);
		if (code != EXIT_FULL)
			return (EXIT_ERR);
		full++;
		if (full >= created)
			return (EXIT_FULL);
	}
}
