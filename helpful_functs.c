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

int	dongle_available(t_dongle *dongle, int cooldonw, long now)
{
	if (!dongle->is_available)
		return (0);
	if (now - dongle->last_released < cooldonw)
		return (0);
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

void	coder_waiting(t_sim *sim, t_coder *coder)
{
	struct timespec	ts;
	long			wake_up;

	// pthread_mutex_lock(&coder->coder_mtx);
	wake_up = coder->last_comp_start + sim->args.dongle_cooldown;
	ts.tv_sec = wake_up / 1000;
	ts.tv_nsec = (wake_up % 1000) * 1000000L;
	pthread_cond_timedwait(&sim->cond, &sim->sim_mtx, &ts);
	// pthread_mutex_unlock(&coder->sim->sim_mtx);
}

void	coder_request(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	pthread_mutex_lock(&coder->coder_mtx);
	left = &coder->sim->dongles[coder->left];
	right = &coder->sim->dongles[coder->right];
	if (coder->sim->args.scheduler == 1)
		coder->priority = get_time_ms();
	else
	{
		coder->priority = coder->last_comp_start + \
coder->sim->args.time_to_burnout;
	}
	push(left->waiters->waiters, &left->waiters->size, coder);
	push(right->waiters->waiters, &right->waiters->size, coder);
	pthread_mutex_unlock(&coder->coder_mtx);
}
