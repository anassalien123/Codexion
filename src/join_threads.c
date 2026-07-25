/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:48 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:49 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	join_threads(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->data.number_of_coders)
	{
		pthread_join(prog->coders[i].thread, NULL);
		i++;
	}
	pthread_join(prog->monitor, NULL);
	return (1);
}
