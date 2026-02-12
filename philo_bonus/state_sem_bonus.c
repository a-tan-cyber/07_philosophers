/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_sem_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 21:20:58 by amtan             #+#    #+#             */
/*   Updated: 2026/02/12 21:39:10 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

static void	append_int(char *buf, int *i, int n)
{
	char	tmp[16];
	int		j;

	j = 0;
	if (n == 0)
	{
		tmp[j] = '0';
		j++;
	}
	while (n > 0)
	{
		tmp[j] = (char)('0' + (n % 10));
		n /= 10;
		j++;
	}
	while (j > 0)
	{
		buf[(*i)] = tmp[j - 1];
		(*i)++;
		j--;
	}
}

static void	make_state_name(char *buf, int id)
{
	const char	*p = "/philo_bonus_state_";
	int			i;

	i = 0;
	while (p[i])
	{
		buf[i] = p[i];
		i++;
	}
	append_int(buf, &i, id);
	buf[i] = '\0';
}

void	sem_state_child_init(t_table *table, int id)
{
	char	name[64];
	sem_t	*local;
	sem_t	*old;

	make_state_name(name, id);
	sem_unlink(name);
	local = sem_open(name, O_CREAT, 0644, 1);
	if (local == SEM_FAILED)
		exit(1);
	sem_unlink(name);
	old = table->sem_state;
	table->sem_state = local;
	if (old && old != SEM_FAILED)
		sem_close(old);
}
