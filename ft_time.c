/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-27 17:32:56 by faeljedd          #+#    #+#             */
/*   Updated: 2026-06-27 17:32:56 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	timestamp(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}

void	log_state(t_sim	*sim, int coder_id, char *msg)
{
	pthread_mutex_lock(&sim->print_mutex);
	if (!sim -> stop)
		printf("%ld %d %s\n", timestamp(sim), coder_id, msg);
	pthread_mutex_unlock(&sim->print_mutex);
}
