/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:53 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:54 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	next_request_order(t_program *prog)
{
	long	order;

	pthread_mutex_lock(&prog->order_mutex);
	order = prog->request_order++;
	pthread_mutex_unlock(&prog->order_mutex);
	return (order);
}
