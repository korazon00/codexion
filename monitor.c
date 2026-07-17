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

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	long	now;
	int		i;

	sim = (t_sim *)arg;
	while (!sim->stop)
	{
		i = 0;
		while (i < sim->args.number_of_coders)
		{
			now = get_time_ms();
			if (now - sim->coders[i].last_comp_start > sim->args.time_to_burnout)
			{
				pthread_mutex_lock(&sim->sim_mtx);
				log_state(sim, sim->coders[i].id, "burned out");
				sim->stop = 1;
				pthread_cond_broadcast(&sim->cond);
				pthread_mutex_unlock(&sim->sim_mtx);
				return NULL;
			}
			i++;
		}
		if (comp_finished(&sim))
		{
			sim->stop = 1;
		}
		usleep(1000);
	}
	return NULL;
}

int	comp_finished(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (sim->coders[i].compile_count < sim->args.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}