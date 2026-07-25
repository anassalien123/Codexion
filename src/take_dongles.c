/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:24:05 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:24:06 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long		order;

	order = next_request_order(coder->program);
	if (coder->left_dongle <= coder->right_dongle)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (!request_dongle(coder, first, order))
		return (0);
	if (!request_dongle(coder, second, order))
	{
		release_dongle(first);
		return (0);
	}
	return (1);
}
