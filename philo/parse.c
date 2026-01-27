/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:10:32 by amtan             #+#    #+#             */
/*   Updated: 2026/01/28 00:24:16 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <string.h>

static int	set_counts(t_table *t, int argc, char **argv)
{
	if (ft_atoi_strict(argv[1], &t->philo_count))
		return (error_msg("invalid number_of_philosophers"));
	if (argc == 6)
	{
		t->must_eat_enabled = 1;
		if (ft_atoi_strict(argv[5], &t->must_eat_count))
			return (error_msg("invalid must_eat"));
	}
	else
	{
		t->must_eat_enabled = 0;
		t->must_eat_count = 0;
	}
	return (0);
}

static int	set_times(t_table *t, char **argv)
{
	if (ft_atol_strict(argv[2], &t->time_die))
		return (error_msg("invalid time_to_die"));
	if (ft_atol_strict(argv[3], &t->time_eat))
		return (error_msg("invalid time_to_eat"));
	if (ft_atol_strict(argv[4], &t->time_sleep))
		return (error_msg("invalid time_to_sleep"));
	return (0);
}

static int	check_rules(t_table *t)
{
	if (t->philo_count < 1)
		return (error_msg("number_of_philosophers must be >= 1"));
	if (t->time_die < 1 || t->time_eat < 1 || t->time_sleep < 1)
		return (error_msg("time values must be >= 1"));
	if (t->must_eat_enabled && t->must_eat_count < 1)
		return (error_msg("must_eat must be >= 1"));
	return (0);
}

int	parse_args(t_table *t, int argc, char **argv)
{
	if (!t)
		return (error_msg("internal error"));
	if (argc != 5 && argc != 6)
		return (error_msg("usage: ./philo n die eat sleep [must_eat]"));
	memset(t, 0, sizeof(*t));
	if (set_counts(t, argc, argv))
		return (1);
	if (set_times(t, argv))
		return (1);
	return (check_rules(t));
}
