/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:28 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:29 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
		return (error_msg("Wrong number of arguments"), 0);
	i = 1;
	while (i < argc - 1)
	{
		if (!is_numeric(argv[i]))
			return (error_msg("Invalid input"), 0);
		i++;
	}
	return (1);
}

static void	fill_data(t_data *data, char **argv)
{
	data->number_of_coders = ft_atol(argv[1]);
	data->time_to_burnout = ft_atol(argv[2]);
	data->time_to_compile = ft_atol(argv[3]);
	data->time_to_debug = ft_atol(argv[4]);
	data->time_to_refactor = ft_atol(argv[5]);
	data->number_of_compiles_required = ft_atol(argv[6]);
	data->dongle_cooldown = ft_atol(argv[7]);
	data->scheduler = argv[8];
}

static int	validate_data(t_data *data)
{
	if (strcmp(data->scheduler, "fifo") != 0
		&& strcmp(data->scheduler, "edf") != 0)
		return (error_msg("Invalid scheduler"), 0);
	if (data->number_of_coders <= 0
		|| data->time_to_burnout <= 0
		|| data->time_to_compile <= 0
		|| data->time_to_debug <= 0
		|| data->time_to_refactor <= 0
		|| data->number_of_compiles_required <= 0
		|| data->dongle_cooldown <= 0)
		return (error_msg("Overflow"), 0);
	return (1);
}

t_data	*init_data(char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (error_msg("Malloc failed"), NULL);
	fill_data(data, argv);
	if (!validate_data(data))
		return (free(data), NULL);
	return (data);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_program	prog;

	if (!check_args(argc, argv))
		return (1);
	data = init_data(argv);
	if (!data)
		return (1);
	prog.data = *data;
	free(data);
	if (!init_program(&prog))
		return (1);
	if (!init_coders(&prog))
		return (destroy_program(&prog), 1);
	if (!create_threads(&prog))
		return (destroy_program(&prog), 1);
	join_threads(&prog);
	destroy_program(&prog);
	return (0);
}
