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
	if (!sim.coders)
		return (1);

	sim.start_time = get_time_ms();
	init_coders(&sim);

	// printf("%d\n", params.number_of_coders);
	// printf("%d\n", params.time_to_burnout);
	// printf("%d\n", params.time_to_compile);
	// printf("%d\n", params.time_to_debug);
	// printf("%d\n", params.number_of_compiles_required);
	// printf("%d\n", params.dongle_cooldown);
	// printf("%d", params.scheduler);
	return (0);
}
