/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 21:45:28 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 13:00:17 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	sim_abort(t_table *table, int created, int close_start)
{
	if (close_start)
		sim_close_start_sem(table, created);
	sim_kill_all(table, created);
	sim_wait_all();
	return (EXIT_ERR);
}

static int	sim_finalize(t_table *table, int created, int rc)
{
	if (rc != EXIT_FULL)
		sim_kill_all(table, created);
	if (sim_wait_all())
		return (EXIT_ERR);
	return (rc);
}

int	start_simulation(t_table *table)
{
	int	created;
	int	rc;

	if (!table)
		return (1);
	created = 0;
	if (now_ms(&table->start_ms))
		return (1);
	if (sim_fork_children(table, &created))
		return (sim_abort(table, created, 0));
	sim_close_parent_meals(table, created);
	if (sim_post_start_all(table))
		return (sim_abort(table, created, 1));
	sim_close_start_sem(table, created);
	rc = sim_wait_until_end(table, created);
	return (sim_finalize(table, created, rc));
}
