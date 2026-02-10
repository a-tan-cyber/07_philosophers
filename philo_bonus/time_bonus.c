/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:44:23 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 18:59:43 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <sys/time.h>
#include <unistd.h>

int	now_ms(long *out)
{
	struct timeval	tv;

	if (!out)
		return (1);
	if (gettimeofday(&tv, NULL))
		return (1);
	*out = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
	return (0);
}

static int	pick_sleep_us(long remaining_ms)
{
	if (remaining_ms >= 10)
		return (5000);
	if (remaining_ms >= 2)
		return (1000);
	if (remaining_ms == 1)
		return (500);
	return (100);
}

int	ms_sleep(long ms)
{
	long	now;
	long	deadline;

	if (ms <= 0)
		return (0);
	if (now_ms(&now))
		return (1);
	deadline = now + ms;
	while (1)
	{
		if (now_ms(&now))
			return (1);
		if (now >= deadline)
			return (0);
		usleep(pick_sleep_us(deadline - now));
	}
}
