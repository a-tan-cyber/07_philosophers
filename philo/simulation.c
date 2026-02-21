/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 21:45:28 by amtan             #+#    #+#             */
/*   Updated: 2026/02/20 14:55:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	cleanup(t_table *table, int created, int gate_locked, int rc)
{
	if (gate_locked && pthread_mutex_unlock(&table->start_mtx))
		rc = 1;
	while (created > 0)
	{
		created--;
		if (pthread_join(table->philos[created].thread, NULL))
			rc = 1;
	}
	return (rc);
}

int	start_simulation(t_table *table)
{
	int	created;
	int	gate_locked;
	int	rc;

	if (!table)
		return (1);
	created = 0;
	gate_locked = 0;
	rc = sim_setup(table, &created, &gate_locked);
	if (!rc && created == table->philo_count && monitor_loop(table))
	{
		fatal_stop_best_effort(table);
		rc = 1;
	}
	return (cleanup(table, created, gate_locked, rc));
}
