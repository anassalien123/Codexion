/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:19 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:20 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	allocate_memory(t_program *prog)
{
	prog->coders = malloc(sizeof(t_coder)
			* prog->data.number_of_coders);
	if (!prog->coders)
		return (0);
	prog->dongles = malloc(sizeof(t_dongle)
			* prog->data.number_of_coders);
	if (!prog->dongles)
	{
		free(prog->coders);
		return (0);
	}
	return (1);
}
