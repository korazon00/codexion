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
				log_state(sim, sim->coders[i].id, "burned out");
				sim->stop = 1;
				return NULL;
			}
			i++;
		}
		usleep(1000);
	}
	return NULL;
}
