/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:30 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:31 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_threads(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->data.number_of_coders)
	{
		if (pthread_create(&prog->coders[i].thread,
				NULL, coder_routine, &prog->coders[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&prog->monitor,
			NULL, monitor_routine, prog) != 0)
		return (0);
	return (1);
}
