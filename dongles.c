/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 09:55:52 by faeljedd          #+#    #+#             */
/*   Updated: 2026/07/11 12:25:39 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		sim->coders[i].left = sim->coders[i].id - 1;
		sim->coders[i].right = sim->coders[i].id % sim->args.number_of_coders;
		i++;
	}
}

void	init_mutex(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_dongles)
	{
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		sim->dongles[i].last_released = 0;
		sim->dongles[i].is_available = 1;
		i++;
	}
}

void	take_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		first;
	int		second;
	long long	now;

	sim = coder->sim;
	first = coder->left < coder->right ? coder->left : coder->right;
	second = coder->left < coder->right ? coder->right : coder->left;
	coder_request(coder);
	pthread_mutex_lock(&sim->sim_mtx);
	while(1)
	{
		now = get_time_ms();
		if (&sim->dongles[first].waiters->waiters[0] == coder
			&& dongle_available(&sim->dongles[first], sim->args.dongle_cooldown, now))
		{
			if (&sim->dongles[second].waiters->waiters[0] == coder 
			&& dongle_available(&sim->dongles[second], sim->args.dongle_cooldown, now))
			{
				pop(&sim->dongles[first].waiters->waiters, &sim->dongles[first].waiters->size);
				sim->dongles[first].is_available = 0;
				pthread_mutex_lock(&sim->dongles[first].mutex);
				log_state(sim, coder->id, "has taken a dongle");

				pop(&sim->dongles[second].waiters->waiters, &sim->dongles[second].waiters->size);
				sim->dongles[second].is_available = 0;
				pthread_mutex_lock(&sim->dongles[second].mutex);
				log_state(sim, coder->id, "has taken a dongle");

				pthread_mutex_unlock(&sim->sim_mtx);
			}
			return;
		}
		pthread_cond_wait(&sim->cond, &sim->sim_mtx);
	}
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	//first_dongle
	pthread_mutex_unlock(&sim->dongles[coder->left].mutex);
	sim->dongles[coder->left].last_released = get_time_ms();
	sim->dongles[coder->left].is_available = 1;

	//second_dongle
	pthread_mutex_unlock(&sim->dongles[coder->right].mutex);
	sim->dongles[coder->right].last_released = get_time_ms();
	sim->dongles[coder->right].is_available = 1;

	pthread_cond_broadcast(&sim->cond);

}

void	coder_request(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = &coder->sim->dongles[coder->left];
	right = &coder->sim->dongles[coder->right];

	if (coder->sim->args.scheduler == 1)
		coder->priority = get_time_ms();
	else
		coder->priority = coder->last_comp_start + coder->sim->args.time_to_burnout;
	
	push(left->waiters->waiters, &left->waiters->size, *coder);
	push(right->waiters->waiters, &right->waiters->size, *coder);
}

int	dongle_available(t_dongle *dongle, int cooldonw, long now)
{
	if (!dongle->is_available)
		return (0);
	if (now - dongle->last_released < cooldonw)
		return (0);
	return (1);
}
