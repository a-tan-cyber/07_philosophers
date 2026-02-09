/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:10:32 by amtan             #+#    #+#             */
/*   Updated: 2026/02/09 16:59:13 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <string.h>

static int	set_counts(t_table *table, int argc, char **argv)
{
	if (ft_atoi_strict(argv[1], &table->philo_count))
		return (error_msg("invalid number_of_philosophers"));
	if (argc == 6)
	{
		table->must_eat_enabled = 1;
		if (ft_atoi_strict(argv[5], &table->must_eat_count))
			return (error_msg("invalid must_eat"));
	}
	else
	{
		table->must_eat_enabled = 0;
		table->must_eat_count = 0;
	}
	return (0);
}

static int	set_times(t_table *table, char **argv)
{
	if (ft_atol_strict(argv[2], &table->time_die))
		return (error_msg("invalid time_to_die"));
	if (ft_atol_strict(argv[3], &table->time_eat))
		return (error_msg("invalid time_to_eat"));
	if (ft_atol_strict(argv[4], &table->time_sleep))
		return (error_msg("invalid time_to_sleep"));
	return (0);
}

static int	check_rules(t_table *table)
{
	if (table->philo_count < 1)
		return (error_msg("number_of_philosophers must be >= 1"));
	if (table->time_die < 1 || table->time_eat < 1 || table->time_sleep < 1)
		return (error_msg("time values must be >= 1"));
	if (table->must_eat_enabled && table->must_eat_count < 1)
		return (error_msg("must_eat must be >= 1"));
	return (0);
}

int	parse_args(t_table *table, int argc, char **argv)
{
	if (!table)
		return (error_msg("internal error - parse_args()"));
	if (argc != 5 && argc != 6)
		return (error_msg("usage: ./philo n die eat sleep [must_eat]"));
	memset(table, 0, sizeof(*table));
	if (set_counts(table, argc, argv))
		return (1);
	if (set_times(table, argv))
		return (1);
	return (check_rules(table));
}
