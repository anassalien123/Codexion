/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:50 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:51 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	request_stop(t_program *prog)
{
	pthread_mutex_lock(&prog->stop_mutex);
	prog->stop = 1;
	pthread_mutex_unlock(&prog->stop_mutex);
}

static int	check_coder(t_program *prog, int i, int *all_finished)
{
	int		finished;
	long	last;

	pthread_mutex_lock(&prog->state_mutex);
	last = prog->coders[i].last_compile_start;
	finished = (prog->coders[i].compile_count
			>= prog->data.number_of_compiles_required);
	pthread_mutex_unlock(&prog->state_mutex);
	if (!finished)
		*all_finished = 0;
	if (!finished && get_time_ms() - last >= prog->data.time_to_burnout)
	{
		print_status(prog, prog->coders[i].id, "burned out");
		request_stop(prog);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_program	*prog;
	int			i;
	int			all_finished;

	prog = (t_program *)arg;
	while (!simulation_stopped(prog))
	{
		i = 0;
		all_finished = 1;
		while (i < prog->data.number_of_coders)
			if (check_coder(prog, i++, &all_finished))
				return (NULL);
		if (all_finished)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
