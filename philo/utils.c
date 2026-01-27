/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:10:08 by amtan             #+#    #+#             */
/*   Updated: 2026/01/28 00:25:42 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <limits.h>
#include <unistd.h>

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

void	putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

int	error_msg(char *msg)
{
	putstr_fd("Error\n", 2);
	if (msg)
		putstr_fd(msg, 2);
	putstr_fd("\n", 2);
	return (1);
}

int	ft_atol_strict(char *s, long *out)
{
	long	n;
	int		i;
	int		d;

	if (!s || !*s || !out)
		return (1);
	i = 0;
	if (s[i] == '+')
		i++;
	if (s[i] == '\0')
		return (1);
	n = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		d = s[i] - '0';
		if (n > (LONG_MAX - d) / 10)
			return (1);
		n = (n * 10) + d;
		i++;
	}
	*out = n;
	return (0);
}

int	ft_atoi_strict(char *s, int *out)
{
	long	n;

	if (!out)
		return (1);
	if (ft_atol_strict(s, &n))
		return (1);
	if (n > INT_MAX)
		return (1);
	*out = (int)n;
	return (0);
}
