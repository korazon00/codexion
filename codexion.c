/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-17 21:16:15 by faeljedd          #+#    #+#             */
/*   Updated: 2026-06-17 21:16:15 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_positive(char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	if (s[0] == '+')
		i = 1;
	else
		i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	printf("i = %d\n", i);
	return (1);
}

int	check_atoi(char *s)
{
	long	result;
	int		i;

	result = 0;
	if (s[0] == '+')
		i = 1;
	else
		i = 0;
	while (s[i])
	{
		result = result * 10 + (s[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	return ((int)result);
}

int	parse_args(int argc, char **argv, t_args *p)
{
	int	i;

	if (argc != 9)
		return (0);
	i = 1;
	while (i <= 7)
	{
		if (!is_positive(argv[i]))
			return (0);
		if (check_atoi(argv[i]) < 0)
			return (0);
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (0);
	p->number_of_coders = atoi(argv[1]);
	p->time_to_burnout = atoi(argv[2]);
	p->time_to_compile = atoi(argv[3]);
	p->time_to_debug = atoi(argv[4]);
	p->time_to_refactor = atoi(argv[5]);
	p->number_of_compiles_required = atoi(argv[6]);
	p->dongle_cooldown = atoi(argv[7]);
	p->scheduler = (strcmp("fifo", argv[8]) == 0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_args	params;

	if (!parse_args(argc, argv, &params))
		return (1);
	printf("%d\n", params.number_of_coders);
	printf("%d\n", params.time_to_burnout);
	printf("%d\n", params.time_to_compile);
	printf("%d\n", params.time_to_debug);
	printf("%d\n", params.number_of_compiles_required);
	printf("%d\n", params.dongle_cooldown);
	printf("%d", params.scheduler);
	return (0);
}


/*
	struct pqueue{
	
		t_waiter *waiters;
		int hsize;
		int size;
	
	}
	
	pqueue_push()
	pqueue_bubble_up()

	pqueue_pop()
	pqueue_bubble_down()

	pqueue_seek()


*/
