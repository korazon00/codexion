/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 09:11:41 by faeljedd          #+#    #+#             */
/*   Updated: 2026-07-03 09:11:41 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	comp_finished(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->sim_mtx);
	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (sim->coders[i].compile_count < \
sim->args.number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->sim_mtx);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&sim->sim_mtx);
	return (1);
}

static int	is_not_burnout(t_sim *sim, t_coder *coder)
{
	long	now;
	long	last_comp_start;

	now = get_time_ms();
	pthread_mutex_lock(&coder->coder_mtx);
	last_comp_start = coder->last_comp_start;
	pthread_mutex_unlock(&coder->coder_mtx);
	if (now - last_comp_start >= sim->args.time_to_burnout)
		return (1);
	return (0);
}

void	if_is_burnout(t_sim *sim, int i)
{
	pthread_mutex_lock(&sim->print_mtx);
	pthread_mutex_lock(&sim->sim_mtx);
	sim->stop = 1;
	printf("%ld %d burned out\n", timestamp(sim), sim->coders[i].id);
	pthread_mutex_unlock(&sim->sim_mtx);
	pthread_mutex_unlock(&sim->print_mtx);
}

int	if_comp_finished(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->sim_mtx);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	sim->start_time = get_time_ms();
	is_ready(sim);
	broadcast(sim);
	while (!should_stop(sim))
	{
		i = 0;
		while (i < sim->args.number_of_coders)
		{
			if (is_not_burnout(sim, &sim->coders[i]))
			{
				if_is_burnout(sim, i);
				return (NULL);
			}
			i++;
		}
		if (comp_finished(sim))
			if (if_comp_finished(sim))
				return (NULL);
		custum_usleep(sim, 1);
	}
	return (NULL);
}
