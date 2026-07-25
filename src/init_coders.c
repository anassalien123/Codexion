/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:42 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:43 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	assign_dongles(t_program *prog, int i, int n)
{
	if (prog->coders[i].id % 2 == 0)
	{
		prog->coders[i].left_dongle = &prog->dongles[i];
		prog->coders[i].right_dongle = &prog->dongles[(i + 1) % n];
	}
	else
	{
		prog->coders[i].right_dongle = &prog->dongles[i];
		prog->coders[i].left_dongle = &prog->dongles[(i + 1) % n];
	}
}

int	init_coders(t_program *prog)
{
	int	i;
	int	n;

	i = 0;
	n = prog->data.number_of_coders;
	while (i < n)
	{
		prog->coders[i].id = i + 1;
		prog->coders[i].compile_count = 0;
		prog->coders[i].last_compile_start = prog->start_time;
		prog->coders[i].program = prog;
		assign_dongles(prog, i, n);
		i++;
	}
	return (1);
}
