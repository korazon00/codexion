/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-29 09:55:52 by faeljedd          #+#    #+#             */
/*   Updated: 2026-06-29 09:55:52 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		sim->coders[i].left = sim->coders[i].id - 1;
		sim->coders[i].right = sim->coders[i].id % sim->args.number_of_coders;
		i++;
	}
}

void	init_mutex(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_dongles)
	{
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		sim->dongles[i].last_released = 0;
		i++;
	}
}

void	take_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		first;
	int		second;

	sim = coder->sim;
	first = coder->left < coder->right ? coder->left : coder->right;
	second = coder->left < coder->right ? coder->right : coder->left;

	pthread_mutex_lock(&sim->dongles[first].mutex);
	log_state(sim, coder->id, "has taken a dongle");

	pthread_mutex_lock(&sim->dongles[second].mutex);
	log_state(sim, coder->id, "has taken a dongle");
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_unlock(&sim->dongles[coder->left].mutex);
	pthread_mutex_unlock(&sim->dongles[coder->right].mutex);
}
