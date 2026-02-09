/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 22:27:56 by amtan             #+#    #+#             */
/*   Updated: 2026/02/09 16:58:48 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <errno.h>
#include <unistd.h>

static int	append_num(char *buf, int *i, long n)
{
	char	tmp[32];
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
	j--;
	while (j >= 0)
	{
		buf[*i] = tmp[j];
		(*i)++;
		j--;
	}
	buf[*i] = ' ';
	(*i)++;
	return (0);
}

static int	append_str(char *buf, int *i, const char *s)
{
	while (*s)
	{
		buf[*i] = *s;
		(*i)++;
		s++;
	}
	buf[*i] = '\n';
	(*i)++;
	return (0);
}

static int	write_line(long ts, int id, const char *msg)
{
	char	buf[128];
	int		i;
	ssize_t	off;
	ssize_t	n;

	i = 0;
	append_num(buf, &i, ts);
	append_num(buf, &i, id);
	append_str(buf, &i, msg);
	off = 0;
	while (off < (ssize_t)i)
	{
		n = write(1, buf + off, (size_t)(i - off));
		if (n < 0)
		{
			if (errno == EINTR)
				continue ;
			return (1);
		}
		if (n == 0)
			return (1);
		off += n;
	}
	return (0);
}

int	print_state(t_philo *philo, char *msg)
{
	t_table	*table;
	long	ts;
	int		stop;
	int		rc;

	if (!philo || !philo->table || !msg)
		return (1);
	table = philo->table;
	if (since_start_ms(table, &ts))
		return (1);
	if (pthread_mutex_lock(&table->print_mtx))
		return (1);
	rc = get_stop(table, &stop);
	if (!rc && !stop)
		rc = write_line(ts, philo->id, msg);
	if (pthread_mutex_unlock(&table->print_mtx))
		return (1);
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}

int	print_death(t_philo *philo)
{
	t_table	*table;
	long	now;
	int		rc;

	if (!philo || !philo->table)
		return (1);
	table = philo->table;
	if (now_ms(&now))
		return (fatal_return(table));
	if (pthread_mutex_lock(&table->print_mtx))
		return (fatal_return(table));
	rc = set_stop(table, 1);
	if (!rc)
		rc = write_line(now - table->start_ms, philo->id, "died");
	if (pthread_mutex_unlock(&table->print_mtx))
		return (fatal_return_no_lock(table));
	if (rc)
		fatal_stop_best_effort(table);
	return (rc);
}
