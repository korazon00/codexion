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
	t_coder	*monitor;
	t_sim	*sim;
	long	now;
	int		i;

	monitor = (t_coder *)arg;
	sim = monitor->sim;
	now = get_time_ms();
	while (!sim->stop)
	{
		i = 0;
		while (i < sim->args.number_of_coders)
		{
			if (now - sim->coders[i].last_comp_start > sim->args.time_to_burnout)
			{
				log_state(&sim, &sim->coders[i], "burned out");
				sim->stop = 1;
				return NULL;
			}
			i++;
		}
		usleep(1000);
	}
}
