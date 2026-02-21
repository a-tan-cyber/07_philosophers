/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:23:14 by amtan             #+#    #+#             */
/*   Updated: 2026/02/20 15:57:38 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <string.h>

int	main(int argc, char **argv)
{
	t_table	table;
	int		rc;

	memset(&table, 0, sizeof(table));
	if (parse_args(&table, argc, argv))
		return (1);
	rc = 0;
	if (init_table(&table))
		rc = 1;
	if (!rc && start_simulation(&table))
		rc = 1;
	if (table.fatal)
	{
		error_msg("fatal: internal error");
		rc = 1;
	}
	destroy_all(&table);
	return (rc);
}
