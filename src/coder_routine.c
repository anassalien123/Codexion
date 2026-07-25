/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:23 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:24 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_stopped(t_program *prog)
{
	int	stop;

	pthread_mutex_lock(&prog->stop_mutex);
	stop = prog->stop;
	pthread_mutex_unlock(&prog->stop_mutex);
	return (stop);
}

static int	coder_done(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->program->state_mutex);
	done = (coder->compile_count
			>= coder->program->data.number_of_compiles_required);
	pthread_mutex_unlock(&coder->program->state_mutex);
	return (done);
}

static void	compile_phase(t_coder *coder)
{
	t_program	*prog;

	prog = coder->program;
	pthread_mutex_lock(&prog->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&prog->state_mutex);
	print_status(prog, coder->id, "is compiling");
	usleep(prog->data.time_to_compile * 1000);
	pthread_mutex_lock(&prog->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&prog->state_mutex);
	release_dongles(coder);
}

static int	work_phase(t_program *prog, t_coder *coder,
	char *msg, long long time)
{
	if (simulation_stopped(prog))
		return (0);
	print_status(prog, coder->id, msg);
	usleep(time * 1000);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_program	*prog;

	coder = (t_coder *)arg;
	prog = coder->program;
	while (!simulation_stopped(prog) && !coder_done(coder))
	{
		if (!take_dongles(coder))
			break ;
		compile_phase(coder);
		if (!work_phase(prog, coder,
				"is debugging", prog->data.time_to_debug))
			break ;
		if (!work_phase(prog, coder,
				"is refactoring", prog->data.time_to_refactor))
			break ;
	}
	return (NULL);
}
