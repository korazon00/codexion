/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-27 16:52:53 by faeljedd          #+#    #+#             */
/*   Updated: 2026-06-27 16:52:53 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	params;
	t_sim	sim;

	if (!parse_args(argc, argv, &params))
		return (1);

	sim.coders = malloc(sizeof(t_coder) * params.number_of_coders);
	sim.dongles = malloc(sizeof(t_dongle) * params.number_of_dongles);
	if (!sim.coders || !sim.dongles)
		return (1);
	
	sim.args = params;
	sim.stop = 0;
	sim.start_time = get_time_ms();

	pthread_mutex_init(&sim.sim_mtx, NULL);
	pthread_cond_init(&sim.cond, NULL);

	init_mutex(&sim);
	init_dongles(&sim);
	init_coders(&sim);

	pthread_create(&sim.monitor, NULL, monitor_routine, &sim);
	pthread_join(sim.monitor, NULL);
	join_threads(&sim);

	pthread_mutex_destroy(&sim.sim_mtx);
	pthread_cond_destroy(&sim.cond);
	free(sim.coders);
	free(sim.dongles);
	return (0);
}
