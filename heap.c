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

void bubbledown(int heap[], int size, int i)
{
	int	smalleast;
	int	left;
	int	right;

	while (1)
	{
		smalleast = i;
		left = i * 2 + 1;
		right = i * 2 + 2;
		if (left < size && heap[smalleast] > heap[left])
			smalleast = left;
		if (right < size && heap[smalleast] > heap[right])
			smalleast = right;
		if (smalleast == i)
			break;
		swap(&heap[i], &heap[smalleast]);
		i = smalleast;
	}
}

void	push(int heap[], int *size, int value)
{
	int	i;

	heap[*size] = value;
	i = *size;
	(*size)++;

	while (i > 0 && heap[(i - 1) / 2] > heap[i])
	{
		swap(&heap[i], &heap[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

int	pop(int heap[], int *size)
{
	int	i;
	int	min;

	i = 0;
	if (*size == 0)
		return (-1);
	min = heap[0];
	heap[0] = heap[*size - 1];
	(*size)--;
	bubbledown(heap, *size, 0);
	return (min);
}

void	swap(int *a, int *b)
{
	int	i;

	i = *a;
	*a = *b;
	*b = i;
}
