/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 22:27:56 by amtan             #+#    #+#             */
/*   Updated: 2026/01/30 00:06:22 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdio.h>

int	print_state(t_philo *philo, char *msg)
{
	t_table	*table;
	int		rc;
	int		stop;
	long	timestamp;

	if (!philo || !philo->table || !msg)
		return (1);
	table = philo->table;
	rc = 0;
	stop = 0;
	timestamp = 0;
	if (pthread_mutex_lock(&table->print_mtx))
		return (1);
	if (get_stop(table, &stop))
		rc = 1;
	if (!rc && !stop && since_start_ms(table, &timestamp))
		rc = 1;
	if (!rc && !stop)
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	if (pthread_mutex_unlock(&table->print_mtx))
		rc = 1;
	return (rc);
}
