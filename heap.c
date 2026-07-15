/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:43:13 by faeljedd          #+#    #+#             */
/*   Updated: 2026/07/11 12:18:46 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//parent == (i - 1)// 2
// left == i * 2 + 1
// right == i * 2 + 2

void bubbledown(t_coder *heap[], int size, int i)
{
	int	smallest;
	int	left;
	int	right;

	while (1)
	{
		smallest = i;
		left = i * 2 + 1;
		right = i * 2 + 2;
		if (left < size && heap[smallest]->priority > heap[left]->priority)
			smallest = left;
		if (right < size && heap[smallest]->priority > heap[right]->priority)
			smallest = right;
		if (smallest == i)
			break;
		swap(&heap[i], &heap[smallest]);
		i = smallest;
	}
}

void	push(t_coder *heap[], int *size, t_coder *coder)
{
	int	i;

	heap[*size] = coder;
	i = *size;
	(*size)++;

	while (i > 0 && heap[(i - 1) / 2]->priority > heap[i]->priority)
	{
		swap(&heap[i], &heap[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

t_coder	*pop(t_coder *heap[], int *size)
{
	t_coder	*min;

    if (*size == 0)
    {
        min->id = -1;
        min->priority = 0;
        return (min);
    }
	min = heap[0];
	heap[0] = heap[*size - 1];
	(*size)--;
	bubbledown(heap, *size, 0);
	return (min);
}

void	swap(t_coder *a, t_coder *b)
{
	t_coder	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
