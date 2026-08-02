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

void	init_dongles_2(t_sim *sim, int i)
{
	sim->dongles[i].waiters->size = 0;
	sim->dongles[i].last_released = 0;
	sim->dongles[i].is_available = 1;
}

int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_dongles)
	{
		sim->dongles[i].waiters = NULL;
		sim->dongles[i].waiters = malloc(sizeof(t_heap));
		if (!sim->dongles[i].waiters)
			return (0);
		sim->dongles[i].waiters->waiters = malloc(sizeof(t_coder *) * 2);
		if (!sim->dongles[i].waiters->waiters)
		{
			while (--i >= 0)
			{
				free(sim->dongles[i].waiters->waiters);
				free(sim->dongles[i].waiters);
			}
			sim->dongles[i].waiters = NULL;
			return (0);
		}
		init_dongles_2(sim, i);
		//protection
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		i++;
	}
	return (1);
}

void	take_dongles(t_sim *sim, t_coder *coder, int first, int second)
{
	pthread_mutex_lock(&sim->dongles[first].mutex);
	pthread_mutex_lock(&sim->dongles[second].mutex);
	pop(sim->dongles[first].waiters->waiters,
		&sim->dongles[first].waiters->size);
	sim->dongles[first].is_available = 0;
	log_state(sim, coder->id, "has taken a dongle");
	pop(sim->dongles[second].waiters->waiters,
		&sim->dongles[second].waiters->size);
	sim->dongles[second].is_available = 0;
	log_state(sim, coder->id, "has taken a dongle");
	pthread_mutex_unlock(&sim->dongles[first].mutex);
	pthread_mutex_unlock(&sim->dongles[second].mutex);
	pthread_mutex_unlock(&sim->sim_mtx);
}

void	dongles_request(t_coder *coder)
{
	t_sim		*sim;
	int			first;
	int			second;
	long long	now;

	sim = coder->sim;
	init_my_dongles(coder, &first, &second);
	pthread_mutex_lock(&sim->sim_mtx);
	coder_request(coder);
	while (!sim->stop)
	{
		now = get_time_ms();
		if (sim->dongles[first].waiters->waiters[0] == coder \
&& dongle_available(&sim->dongles[first], sim->args.dongle_cooldown, now))
		{
			if (sim->dongles[second].waiters->waiters[0] == coder \
&& dongle_available(&sim->dongles[second], sim->args.dongle_cooldown, now))
			{
				take_dongles(sim, coder, first, second);
				return ;
			}
		}
		coder_waiting(sim, coder);
	}
	pthread_mutex_unlock(&sim->sim_mtx);
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->sim_mtx);
	sim->dongles[coder->left].last_released = get_time_ms();
	sim->dongles[coder->left].is_available = 1;
	sim->dongles[coder->right].last_released = get_time_ms();
	sim->dongles[coder->right].is_available = 1;
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->sim_mtx);
}
