/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cdr_rtn_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-28 14:06:46 by faeljedd          #+#    #+#             */
/*   Updated: 2026-07-28 14:06:46 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiling(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mtx);
	coder->last_comp_start = get_time_ms();
	coder->compile_count ++;
	pthread_mutex_unlock(&coder->coder_mtx);
	log_state(sim, coder->id, "is compiling");
	custum_usleep(sim, sim->args.time_to_compile);
}

void	debuging(t_sim *sim, t_coder *coder)
{
	log_state(sim, coder->id, "is debugging");
	custum_usleep(sim, sim->args.time_to_debug);
}

void	refactoring(t_sim *sim, t_coder *coder)
{
	log_state(sim, coder->id, "is refactoring");
	custum_usleep(sim, sim->args.time_to_refactor);
}

void	broadcast(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->sim_mtx);
}

void	waiting_station(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	while (!sim->is_ready)
		pthread_cond_wait(&sim->cond, &sim->sim_mtx);
	pthread_mutex_unlock(&sim->sim_mtx);
}

