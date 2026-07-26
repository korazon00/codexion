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

static int	init_all(t_sim *sim)
{
	if (pthread_mutex_init(&sim->sim_mtx, NULL))
		return (0);
	if (pthread_mutex_init(&sim->print_mtx, NULL))
		return (0);
	if (pthread_mutex_init(&sim->monitor_mtx, NULL))
		return (0);
	if (pthread_cond_init(&sim->cond, NULL))
		return (0);
	if (!init_dongles(sim))
		return (0);
	if (!init_coders(sim))
		return (0);

	return (1);
}

static void destroy_all(t_sim *sim)
{
	pthread_mutex_destroy(&sim->sim_mtx);
	pthread_mutex_destroy(&sim->print_mtx);
	pthread_mutex_destroy(&sim->monitor_mtx);
	pthread_cond_destroy(&sim->cond);
	destroy_mtx(sim);
}

static void free_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_dongles)
	{
		if (sim->dongles[i].waiters)
		{
			free(sim->dongles[i].waiters->waiters);
			free(sim->dongles[i].waiters);
		}
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
}

int	main(int argc, char **argv)
{
	t_args	params;
	t_sim	sim;

	if (!parse_args(argc, argv, &params))
		return (1);
	sim.coders = malloc(sizeof(t_coder) * params.number_of_coders);
	if (!sim.coders)
		return (1);
	sim.dongles = malloc(sizeof(t_dongle) * params.number_of_dongles);
	if (!sim.dongles)
	{
		free(sim.coders);
		return (1);
	}
	sim.args = params;
	sim.stop = 0;
	sim.start_time = get_time_ms();
	if (!init_all(&sim))
	{
		destroy_all(&sim);
		free_all(&sim);
		return (1);
	}
	pthread_create(&sim.monitor, NULL, monitor_routine, &sim);
	join_threads(&sim);
	pthread_join(sim.monitor, NULL);
	destroy_all(&sim);
	free_all(&sim);
	return (0);
}
