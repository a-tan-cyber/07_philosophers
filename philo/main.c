/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:23:14 by amtan             #+#    #+#             */
/*   Updated: 2026/01/29 12:37:37 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_table	table;

	if (parse_args(&table, argc, argv))
		return (1);
	printf("n=%d die=%ld eat=%ld sleep=%ld must=%d enabled=%d\n",
		table.philo_count, table.time_die, table.time_eat, table.time_sleep,
		table.must_eat_count, table.must_eat_enabled);
	return (0);
}
