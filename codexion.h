/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-17 21:16:21 by faeljedd          #+#    #+#             */
/*   Updated: 2026-06-17 21:16:21 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct s_args
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    int scheduler;
} t_args;

typedef struct s_dongle
{
	pthread_mutex_t mutex;
	long last_released;
	// queue (heap)

} t_dongle;

typedef struct s_coder
{
	int	id;
	pthread_t thread;
	long last_comp_start;
	int compile_count;
	t_dongle	*left;
	t_dongle	*right;
	pthread_mutex_t	mtx;
} t_coder;

typedef struct s_simulation
{
	t_args args;
	t_coder *coders;
	t_dongle *dongles;
	int stop;
	pthread_mutex_t print_mutex;
	pthread_t monitor;
} t_sim;



#endif