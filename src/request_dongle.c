/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_dongle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:24:03 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:24:04 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_ready(t_coder *coder, t_dongle *dongle)
{
	t_request	top;
	long		now;

	if (dongle->heap.size == 0)
		return (0);
	top = heap_top(&dongle->heap);
	if (top.coder_id != coder->id)
		return (0);
	if (dongle->in_use)
		return (0);
	now = get_time_ms();
	if (now - dongle->last_release < coder->program->data.dongle_cooldown)
		return (0);
	return (1);
}

static void	init_request(t_coder *coder, t_request *req, long order)
{
	req->coder_id = coder->id;
	req->order = order;
	pthread_mutex_lock(&coder->program->state_mutex);
	req->key = coder->last_compile_start
		+ coder->program->data.time_to_burnout;
	pthread_mutex_unlock(&coder->program->state_mutex);
}

static int	wait_dongle(t_coder *coder, t_dongle *dongle)
{
	while (!simulation_stopped(coder->program)
		&& !dongle_ready(coder, dongle))
	{
		pthread_mutex_unlock(&dongle->mutex);
		usleep(DONGLE_POLL_US);
		pthread_mutex_lock(&dongle->mutex);
	}
	if (simulation_stopped(coder->program))
	{
		heap_remove_by_id(&dongle->heap, coder->id);
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	return (1);
}

int	request_dongle(t_coder *coder, t_dongle *dongle, long order)
{
	t_request	req;

	init_request(coder, &req, order);
	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->heap, req);
	if (!wait_dongle(coder, dongle))
		return (0);
	heap_pop(&dongle->heap);
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->mutex);
	print_status(coder->program, coder->id, "has taken a dongle");
	return (1);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->last_release = get_time_ms();
	pthread_mutex_unlock(&dongle->mutex);
}
