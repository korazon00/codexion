/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-28 14:06:33 by faeljedd          #+#    #+#             */
/*   Updated: 2026-07-28 14:06:33 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	is_odd(t_sim *sim, t_coder *coder)
{
	long	time_to_sleep;

	time_to_sleep = (sim->args.time_to_compile + sim->args.dongle_cooldown) / 2;
	if (coder->id % 2 == 0)
	{
		custum_usleep(sim, time_to_sleep);
	}
}

int	one_coder(t_sim *sim, t_coder *coder)
{
	log_state(sim, coder->id, "has taken a dongle");
	custum_usleep(sim, sim->args.time_to_burnout);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	waiting_station(sim, coder);
	if (sim->args.number_of_coders == 1)
		if (one_coder(sim, coder))
			return (NULL);
	is_odd(sim, coder);
	while (!should_stop(sim))
	{
		dongles_request(coder);
		if (should_stop(sim))
		{
			release_dongles_if_not_avail(sim, coder);
			return (NULL);
		}
		compiling(sim, coder);
		release_dongles(coder);
		debuging(sim, coder);
		refactoring(sim, coder);
	}
	return (NULL);
}

int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->dongles[i].id = i;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
		sim->coders[i].last_comp_start = get_time_ms();
		sim->coders[i].left = sim->coders[i].id - 1;
		sim->coders[i].right = sim->coders[i].id % sim->args.number_of_coders;
		pthread_mutex_init(&sim->coders[i].coder_mtx, NULL);
		if (pthread_create(
				&sim->coders[i].thread,
				NULL,
				coder_routine,
				&sim->coders[i]) != 0)
			return (0);
		i++;
	}
	
	return (1);
}

int	join_threads(t_sim	*sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	return (0);
}
