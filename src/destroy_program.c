/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_program.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:32 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:33 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_program(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->data.number_of_coders)
	{
		heap_destroy(&prog->dongles[i].heap);
		pthread_mutex_destroy(&prog->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&prog->order_mutex);
	pthread_mutex_destroy(&prog->state_mutex);
	pthread_mutex_destroy(&prog->stop_mutex);
	pthread_mutex_destroy(&prog->print_mutex);
	free(prog->dongles);
	free(prog->coders);
}
