/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:37 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:38 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_before(t_heap *heap, t_request a, t_request b)
{
	if (strcmp(heap->program->data.scheduler, "fifo") == 0)
		return (a.order < b.order);
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.order < b.order);
}

static void	rebuild_heap(t_heap *heap)
{
	int	i;

	i = heap->size / 2 - 1;
	while (i >= 0)
	{
		heapify_down(heap, i);
		i--;
	}
}

void	heap_remove_by_id(t_heap *heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->data[i].coder_id != coder_id)
		i++;
	if (i == heap->size)
		return ;
	heap->size--;
	while (i < heap->size)
	{
		heap->data[i] = heap->data[i + 1];
		i++;
	}
	rebuild_heap(heap);
}
