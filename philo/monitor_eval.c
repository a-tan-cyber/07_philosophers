/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_eval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:15:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/19 10:15:00 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_eval(t_mon_eval *out)
{
	out->ended = 0;
	out->dead_id = 0;
	out->print_death = 0;
}

static int	adjust_full_if_dead(t_table *table, int full,
			t_philo *dead, long now)
{
	long	elapsed;

	if (!table->must_eat_enabled || !full || !dead)
		return (full);
	elapsed = now - dead->last_meal_ms;
	if (elapsed < 0)
		elapsed = 0;
	if (elapsed > table->time_die)
		return (0);
	return (full);
}

int	monitor_eval_locked(t_table *table, long now, t_mon_eval *out)
{
	t_philo	*dead;
	int		full;

	if (!table || !out)
		return (1);
	dead = NULL;
	full = 0;
	init_eval(out);
	if (table->must_eat_enabled && monitor_all_full_locked(table, &full))
		return (1);
	if (monitor_find_dead_locked(table, &dead, now))
		return (1);
	full = adjust_full_if_dead(table, full, dead, now);
	if (table->must_eat_enabled && full)
		out->ended = 1;
	else if (dead)
	{
		out->ended = 1;
		out->dead_id = dead->id;
		out->print_death = 1;
	}
	return (0);
}
