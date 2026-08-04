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
	{
		if (strlen(s) == 1)
			return (0);
		i = 1;
	}
	else
		i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
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

void	init_args(char **argv, t_args *p)
{
	p->number_of_coders = atoi(argv[1]);
	p->number_of_dongles = atoi(argv[1]);
	p->time_to_burnout = atoi(argv[2]);
	p->time_to_compile = atoi(argv[3]);
	p->time_to_debug = atoi(argv[4]);
	p->time_to_refactor = atoi(argv[5]);
	p->number_of_compiles_required = atoi(argv[6]);
	p->dongle_coldown = atoi(argv[7]);
	p->scheduler = (strcmp("fifo", argv[8]) == 0);
}

int	print_errors(char *msg, int i)
{
	char	*arg_names[8];

	arg_names[0] = "error";
	arg_names[1] = "number_of_coders";
	arg_names[2] = "time_to_burnout";
	arg_names[3] = "time_to_compile";
	arg_names[4] = "time_to_debug";
	arg_names[5] = "time_to_refactor";
	arg_names[6] = "number_of_compiles_required";
	arg_names[7] = "dongle_coldown";
	if (i)
		fprintf(stderr, "Error: the %s %s\n", arg_names[i], msg);
	else
		fprintf(stderr, "Error: %s\n", msg);
	return (1);
}

int	parse_args(int argc, char **argv, t_args *p)
{
	int	i;

	if (argc != 9)
		if (print_errors("expected 8 arguments", 0))
			return (0);
	i = 1;
	while (i <= 7)
	{
		if (!is_positive(argv[i]))
			if (print_errors("must be a positive integer", i))
				return (0);
		if (check_atoi(argv[i]) < 0)
			if (print_errors("is larger than INT_MAX", i))
				return (0);
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		if (print_errors("scheduler must be \"fifo\" or \"edf\"", 0))
			return (0);
	init_args(argv, p);
	return (1);
}
