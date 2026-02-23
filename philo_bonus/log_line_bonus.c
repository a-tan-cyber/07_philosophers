/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_line_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by amtan             #+#    #+#             */
/*   Updated: 2026/02/21 00:00:00 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <errno.h>
#include <unistd.h>

static int	fill_digits_rev(char *tmp, long n)
{
	int	len;

	if (n < 0)
		n = 0;
	len = 0;
	if (n == 0)
	{
		tmp[len++] = '0';
		return (len);
	}
	while (n > 0)
	{
		tmp[len++] = (char)('0' + (n % 10));
		n /= 10;
	}
	return (len);
}

static void	append_num(char *buf, int *i, long n)
{
	char	tmp[32];
	int		j;
	int		len;

	len = fill_digits_rev(tmp, n);
	j = len - 1;
	while (j >= 0)
		buf[(*i)++] = tmp[j--];
	buf[(*i)++] = ' ';
}

static void	append_str(char *buf, int *i, const char *s)
{
	while (*s)
		buf[(*i)++] = *s++;
	buf[(*i)++] = '\n';
}

int	write_line(long ts, int id, const char *msg)
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
