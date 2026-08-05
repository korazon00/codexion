/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpful_functs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-28 14:35:34 by faeljedd          #+#    #+#             */
/*   Updated: 2026-07-28 14:35:34 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_mtx(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].coder_mtx);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
}

int	dongle_available(t_dongle *dongle, t_coder *coder, int coldown, long now)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->waiters->waiters[0] != coder)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	if (!dongle->is_available)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	if (now - dongle->last_released < coldown)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	init_my_dongles(t_coder *coder, int *first, int *second)
{
	if (coder->left < coder->right)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

void	coder_waiting(t_sim *sim)
{
	custum_usleep(sim, 1);
}

void	coder_request(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = &coder->sim->dongles[coder->left];
	right = &coder->sim->dongles[coder->right];
	pthread_mutex_lock(&coder->coder_mtx);
	if (coder->sim->args.scheduler == 1)
		coder->priority = get_time_ms();
	else
	{
		coder->priority = coder->last_comp_start + \
coder->sim->args.time_to_burnout;
	}
	pthread_mutex_unlock(&coder->coder_mtx);
	pthread_mutex_lock(&left->mutex);
	push(left->waiters->waiters, &left->waiters->size, coder);
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&right->mutex);
	push(right->waiters->waiters, &right->waiters->size, coder);
	pthread_mutex_unlock(&right->mutex);
}
