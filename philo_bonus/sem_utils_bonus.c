/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:05:27 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 17:53:22 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <errno.h>
#include <fcntl.h>

int	sem_wait_retry(sem_t *sem)
{
	while (sem_wait(sem) == -1)
	{
		if (errno != EINTR)
			return (1);
	}
	return (0);
}

int	sem_post_retry(sem_t *sem)
{
	if (sem_post(sem) == -1)
		return (1);
	return (0);
}

int	sem_open_create(const char *name, unsigned int value, sem_t **out)
{
	sem_t	*sem;

	if (!name || !out)
		return (1);
	sem_unlink(name);
	sem = sem_open(name, O_CREAT | O_EXCL, 0644, value);
	if (sem == SEM_FAILED)
		return (1);
	*out = sem;
	return (0);
}

static int	append_id(int n, char *buf, int *i, int size)
{
	char	tmp[12];
	int		len;

	len = 0;
	while (n > 0 && len < 11)
	{
		tmp[len++] = (char)('0' + (n % 10));
		n /= 10;
	}
	while (len > 0 && *i < size - 1)
		buf[(*i)++] = tmp[--len];
	return (0);
}

void	build_meal_name(int id, char *buf, int buf_size)
{
	const char	*pre;
	int			i;

	pre = SEM_MEAL_PREFIX;
	i = 0;
	while (*pre && i < buf_size - 1)
		buf[i++] = *pre++;
	if (id > 0)
		append_id(id, buf, &i, buf_size);
	buf[i] = '\0';
}
