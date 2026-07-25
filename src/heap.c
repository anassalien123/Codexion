/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:23:39 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:23:40 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, int capacity, t_program *prog)
{
	heap->data = malloc(sizeof(t_request) * capacity);
	if (!heap->data)
		return (0);
	heap->size = 0;
	heap->capacity = capacity;
	heap->program = prog;
	return (1);
}

void	heap_destroy(t_heap *heap)
{
	free(heap->data);
}

void	heapify_up(t_heap *heap, int index)
{
	int			parent;
	t_request	tmp;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(heap, heap->data[index], heap->data[parent]))
			break ;
		tmp = heap->data[index];
		heap->data[index] = heap->data[parent];
		heap->data[parent] = tmp;
		index = parent;
	}
}

void	heapify_down(t_heap *heap, int index)
{
	int			left;
	int			right;
	int			smallest;
	t_request	tmp;

	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		smallest = index;
		if (left < heap->size
			&& request_before(heap, heap->data[left], heap->data[smallest]))
			smallest = left;
		if (right < heap->size
			&& request_before(heap, heap->data[right], heap->data[smallest]))
			smallest = right;
		if (smallest == index)
			break ;
		tmp = heap->data[index];
		heap->data[index] = heap->data[smallest];
		heap->data[smallest] = tmp;
		index = smallest;
	}
}

void	heap_push(t_heap *heap, t_request req)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->data[heap->size] = req;
	heap->size++;
	heapify_up(heap, heap->size - 1);
}
