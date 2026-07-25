/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:45 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:46 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_partial_dongles(t_program *prog, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		heap_destroy(&prog->dongles[i].heap);
		pthread_mutex_destroy(&prog->dongles[i].mutex);
		i++;
	}
}

static int	init_one_dongle(t_program *prog, int i)
{
	prog->dongles[i].in_use = 0;
	prog->dongles[i].last_release = 0;
	if (pthread_mutex_init(&prog->dongles[i].mutex, NULL) != 0)
		return (0);
	if (!heap_init(&prog->dongles[i].heap,
			prog->data.number_of_coders, prog))
	{
		pthread_mutex_destroy(&prog->dongles[i].mutex);
		return (0);
	}
	return (1);
}

static int	init_dongles(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->data.number_of_coders)
	{
		if (!init_one_dongle(prog, i))
			return (destroy_partial_dongles(prog, i), 0);
		i++;
	}
	return (1);
}

static int	init_mutexes(t_program *prog)
{
	if (!init_dongles(prog))
		return (0);
	if (pthread_mutex_init(&prog->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&prog->stop_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&prog->state_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&prog->order_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	init_program(t_program *prog)
{
	prog->stop = 0;
	prog->request_order = 0;
	prog->start_time = get_time_ms();
	if (!allocate_memory(prog))
		return (0);
	if (!init_mutexes(prog))
	{
		free(prog->coders);
		free(prog->dongles);
		return (0);
	}
	return (1);
}
