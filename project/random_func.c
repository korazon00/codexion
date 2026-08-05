/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_of_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-08-03 17:29:03 by faeljedd          #+#    #+#             */
/*   Updated: 2026-08-03 17:29:03 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	life_of_coder(t_sim *sim, t_coder *coder)
{
	compiling(sim, coder);
	release_dongles(coder);
	debuging(sim, coder);
	refactoring(sim, coder);
}

int	create_coders(t_sim *sim, int i)
{
	if (pthread_create(&sim->coders[i].thread, NULL,
			coder_routine, &sim->coders[i]) != 0)
	{
		pthread_mutex_lock(&sim->sim_mtx);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->sim_mtx);
		broadcast(sim);
		fprintf(stderr, "Error at thread creation: thread %d has failed\n", i);
		while (--i >= 0)
		{
			pthread_join(sim->coders[i].thread, NULL);
		}
		return (0);
	}
	return (1);
}

void	init_sim_flag(t_sim *sim)
{
	sim->stop = 0;
	sim->is_ready = 0;
}

void	is_ready(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	sim->is_ready = 1;
	pthread_mutex_unlock(&sim->sim_mtx);
}
