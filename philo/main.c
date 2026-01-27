/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:23:14 by amtan             #+#    #+#             */
/*   Updated: 2026/01/28 00:23:38 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_table	t;

	if (parse_args(&t, argc, argv))
		return (1);
	printf("n=%d die=%ld eat=%ld sleep=%ld must=%d enabled=%d\n",
		t.philo_count, t.time_die, t.time_eat, t.time_sleep,
		t.must_eat_count, t.must_eat_enabled);
	return (0);
}
