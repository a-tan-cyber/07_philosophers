/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 22:27:56 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 17:46:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>

static void	fatal_post(sem_t *sem)
	{
	if (sem_post_retry(sem))
		exit(EXIT_ERR);
}

int	print_state(t_philo *philo, const char *msg)
{
	t_table	*table;
	long	ts;
	int		rc;

	if (!philo || !msg || !philo->table)
		return (1);
	table = philo->table;
	if (sem_wait_retry(table->sem_end))
		exit(EXIT_ERR);
	if (sem_wait_retry(table->sem_print))
	{
		fatal_post(table->sem_end);
		exit(EXIT_ERR);
	}
	rc = since_start_ms(table, &ts);
	if (!rc)
		rc = write_line(ts, philo->id, msg);
	fatal_post(table->sem_print);
	fatal_post(table->sem_end);
	return (rc);
}

int	print_death(t_philo *philo)
{
	t_table	*table;
	long	ts;

	if (!philo || !philo->table)
		exit(EXIT_ERR);
	table = philo->table;
	if (sem_wait_retry(table->sem_end))
		exit(EXIT_ERR);
	if (sem_wait_retry(table->sem_print))
		exit(EXIT_ERR);
	if (since_start_ms(table, &ts) || write_line(ts, philo->id, "died"))
	{
		fatal_post(table->sem_print);
		exit(EXIT_ERR);
	}
	fatal_post(table->sem_print);
	return (0);
}
