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
	p->dongle_cooldown = atoi(argv[7]);
	p->scheduler = (strcmp("fifo", argv[8]) == 0);
}

int	print_errors(char *msg, int i)
{
	char *arg_names[] = {
	"number_of_coders", "time_to_burnout", "time_to_compile",
	"time_to_debug", "time_to_refactor", "number_of_compiles_required",
	"dongle_cooldown"
};
	if (i)
		fprintf(stderr, "Error: %s (go \"%s\")\n", msg, arg_names[i]);
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
			if (print_errors("must be a positive integer", i - 1))
				return (0);
		if (check_atoi(argv[i]) < 0)
			if (print_errors("value overflows int", i - 1))
			return (0);
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		if(print_errors("Error: scheduler must be \"fifo\" or \"edf\"", NULL))
			return (0);
	init_args(argv, p);
	return (1);
}
