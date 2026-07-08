/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-05 14:43:13 by faeljedd          #+#    #+#             */
/*   Updated: 2026-07-05 14:43:13 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//parent == (i - 1)// 2
// left == i * 2 + 1
// right == i * 2 + 2

void bubbledown(t_waiter heap[], int size, int i)
{
	int	smalleast;
	int	left;
	int	right;

	while (1)
	{
		smalleast = i;
		left = i * 2 + 1;
		right = i * 2 + 2;
		if (left < size && heap[smalleast].spawn_time > heap[left].spawn_time)
			smalleast = left;
		if (right < size && heap[smalleast].spawn_time > heap[right].spawn_time)
			smalleast = right;
		if (smalleast == i)
			break;
		swap(&heap[i], &heap[smalleast]);
		i = smalleast;
	}
}

void	push(t_waiter heap[], int *size, t_waiter coder)
{
	int	i;

	heap[*size] = coder;
	i = *size;
	(*size)++;

	while (i > 0 && heap[(i - 1) / 2].spawn_time > heap[i].spawn_time)
	{
		swap(&heap[i], &heap[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

t_waiter	pop(t_waiter heap[], int *size)
{
	int			i;
	t_waiter	min;

	i = 0;
    if (*size == 0)
    {
        min.coder_id = -1;
        min.spawn_time = 0;
        return (min);
    }
	min = heap[0];
	heap[0] = heap[*size - 1];
	(*size)--;
	bubbledown(heap, *size, 0);
	return (min);
}

void	swap(t_waiter *a, t_waiter *b)
{
	t_waiter	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
