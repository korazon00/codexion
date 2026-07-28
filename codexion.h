/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 21:16:21 by faeljedd          #+#    #+#             */
/*   Updated: 2026/07/11 12:12:19 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

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
	//
	int number_of_dongles;

} t_args;

typedef struct s_coder
{
	int	id;
	pthread_t thread;
	long last_comp_start;
	int compile_count;
	int	left;
	int	right;
	pthread_mutex_t	coder_mtx;
	struct s_simulation *sim;
	long long priority;
} t_coder;

typedef struct s_heap
{
	t_coder **waiters;
	int size;
	
} t_heap;

typedef struct s_dongle
{
	int	id;
	pthread_mutex_t mutex;
	long last_released;
	t_heap	*waiters;
	int is_available;
	
} t_dongle;

typedef struct s_simulation
{
	t_args args;
	t_coder *coders;
	t_dongle *dongles;
	long	start_time;
	int stop;
	pthread_cond_t cond;
	pthread_mutex_t sim_mtx;
	pthread_mutex_t print_mtx;
	pthread_mutex_t monitor_mtx;
	pthread_t monitor;
} t_sim;


//prsing
int	parse_args(int argc, char **argv, t_args *p);
int	check_atoi(char *s);
int	is_positive(char *s);

//coders
int init_coders(t_sim *sim);
void *coder_routine(void *arg);
int	join_threads(t_sim	*sim);

//ft_time
void	log_state(t_sim	*sim, int coder_id, char *msg);
long	timestamp(t_sim *sim);
long	get_time_ms(void);
int	should_stop(t_sim *sim);
void custum_usleep(t_sim *sim, long time_to_sleep);

//dongles
int	init_dongles(t_sim *sim);
void	init_mutex(t_sim *sim);
void	dongles_request(t_coder *coder);
void	release_dongles(t_coder *coder);
void	destroy_mtx(t_sim *sim);

//monitor
void	*monitor_routine(void *arg);

//heap
void bubbledown(t_sim *sim,t_coder *heap[], int size, int i);
void	push(t_coder *heap[], int *size, t_coder *coder);
t_coder	*pop(t_sim *sim,t_coder *heap[], int *size);
void	swap(t_coder **a, t_coder **b);

//cdr_rtn_func
void compiling(t_sim *sim, t_coder *coder);
void debuging(t_sim *sim, t_coder *coder);
void refactoring(t_sim *sim, t_coder *coder);
void waiting_station(t_sim *sim, t_coder *coder);
void release_dongles_if_not_avail(t_sim *sim, t_coder *coder);

//helpful_functs
void	destroy_mtx(t_sim *sim);
int	dongle_available(t_dongle *dongle, int cooldonw, long now);
void	coder_request(t_coder *coder);
void	coder_waiting(t_sim *sim, t_coder *coder);
void	init_my_dongles(t_coder *coder, int *first, int *second);
#endif