/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:35 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:36 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	heap_top(t_heap *heap)
{
	return (heap->data[0]);
}

t_request	heap_pop(t_heap *heap)
{
	t_request	top;

	top = heap->data[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->data[0] = heap->data[heap->size];
		heapify_down(heap, 0);
	}
	return (top);
}
