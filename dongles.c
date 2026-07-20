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
	while (i < sim->args.number_of_dongles)
	{
		sim->dongles[i].waiters = malloc(sizeof(t_heap));
		sim->dongles[i].waiters->waiters = malloc(sizeof(t_coder) * 2);
		sim->dongles[i].waiters->size = 0;
		sim->dongles[i].last_released = 0;
		sim->dongles[i].is_available = 1;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		i++;
	}
}

void	destroy_mtx_dngls(t_sim *sim)
{
	int	i;

	i = 0;
	while(i < sim->args.number_of_dongles)
	{
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

void	take_dongles(t_coder *coder)
{
	t_sim		*sim;
	int			first;
	int			second;
	long long	now;

	sim = coder->sim;
	first = coder->left < coder->right ? coder->left : coder->right;
	second = coder->left < coder->right ? coder->right : coder->left;
	pthread_mutex_lock(&sim->sim_mtx);
	coder_request(coder);
	while(1)
	{
		if(should_stop(sim))
		{
			pthread_mutex_lock(&sim->sim_mtx);
			return;
		}
		now = get_time_ms();
		if (sim->dongles[first].waiters->waiters[0] == coder
			&& dongle_available(&sim->dongles[first], sim->args.dongle_cooldown, now))
		{
			if (sim->dongles[second].waiters->waiters[0] == coder 
			&& dongle_available(&sim->dongles[second], sim->args.dongle_cooldown, now))
			{
				pop(sim->dongles[first].waiters->waiters, &sim->dongles[first].waiters->size);
				sim->dongles[first].is_available = 0;
				pthread_mutex_lock(&sim->dongles[first].mutex);
				log_state(sim, coder->id, "has taken a dongle");

				pop(sim->dongles[second].waiters->waiters, &sim->dongles[second].waiters->size);
				sim->dongles[second].is_available = 0;
				pthread_mutex_lock(&sim->dongles[second].mutex);
				log_state(sim, coder->id, "has taken a dongle");

				pthread_mutex_unlock(&sim->sim_mtx);
				return;
			}
		}
		pthread_cond_wait(&sim->cond, &sim->sim_mtx);
		if(should_stop(sim))
		{
			pthread_mutex_lock(&sim->sim_mtx);
			return;
		}
		printf("coder %d woke up\n", coder->id);
	}
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->sim_mtx);

	//first_dongle
	pthread_mutex_unlock(&sim->dongles[coder->left].mutex);
	sim->dongles[coder->left].last_released = get_time_ms();
	sim->dongles[coder->left].is_available = 1;

	//second_dongle
	pthread_mutex_unlock(&sim->dongles[coder->right].mutex);
	sim->dongles[coder->right].last_released = get_time_ms();
	sim->dongles[coder->right].is_available = 1;

	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->sim_mtx);
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
	
	push(left->waiters->waiters, &left->waiters->size, coder);
	push(right->waiters->waiters, &right->waiters->size, coder);
	// printf("Coder %d requests %d and %d\n", coder->id, coder->left, coder->right);
}
