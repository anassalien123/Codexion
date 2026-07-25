/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:24:10 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:24:11 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	error_msg(char *str)
{
	int	i;
	int	len;

	len = 0;
	while (str[len])
		len++;
	i = 0;
	while (i < len)
	{
		if (write(1, &str[i], 1) < 0)
			break ;
		i++;
	}
	return (i);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}

void	build_timeout(struct timespec *ts, long ms_from_now)
{
	long	nsec;

	clock_gettime(CLOCK_REALTIME, ts);
	nsec = ts->tv_nsec + (ms_from_now * 1000000L);
	ts->tv_sec += nsec / 1000000000L;
	ts->tv_nsec = nsec % 1000000000L;
}

void	print_status(t_program *prog, int id, char *msg)
{
	pthread_mutex_lock(&prog->stop_mutex);
	if (prog->stop)
	{
		pthread_mutex_unlock(&prog->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&prog->stop_mutex);
	pthread_mutex_lock(&prog->print_mutex);
	printf("%ld %d %s\n",
		get_time_ms() - prog->start_time,
		id,
		msg);
	pthread_mutex_unlock(&prog->print_mutex);
}
