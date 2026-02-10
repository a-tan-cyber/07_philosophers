/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:23:14 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 19:36:40 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	kill_all(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGKILL);
		i++;
	}
}

static int	wait_children(pid_t *pids, int count)
{
	int		i;
	int		status;
	pid_t	pid;

	i = 0;
	while (i < count)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0)
			return (1);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			kill_all(pids, count);
			while (++i < count)
				waitpid(-1, 0, 0);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	fork_all(t_table *table, pid_t *pids)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < table->philo_count)
	{
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
			philo_process(table, i + 1);
		pids[i] = pid;
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	table;
	pid_t	*pids;
	int		rc;

	if (parse_args(&table, argc, argv))
		return (1);
	if (now_ms(&table.start_ms))
		return (1);
	if (sems_open(&table))
		return (1);
	pids = (pid_t *)malloc(sizeof(pid_t) * table.philo_count);
	if (!pids)
		return (sems_close_unlink(&table), 1);
	rc = 0;
	if (fork_all(&table, pids))
		rc = 1;
	if (!rc && wait_children(pids, table.philo_count))
		rc = 1;
	free(pids);
	sems_close_unlink(&table);
	return (rc);
}
