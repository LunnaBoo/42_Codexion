/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_number(char *arg, long *dest);
int	parse_numbers(char **argv, long *values);
int	valid_scheduler(char *arg);
int	parse_args(int argc, char **argv, t_simulation *sim);

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (parse_args(argc, argv, &sim) == 0)
	{
		fprintf(stderr, "Error: invalid arguments.\n");
		return (1);
	}
	if (sim_init(&sim) == 0)
	{
		fprintf(stderr, "Error: simulation init failed.\n");
		return (1);
	}
	if (sim_run(&sim) == 0)
	{
		fprintf(stderr, "Error: simulation run failed.\n");
		sim_destroy(&sim);
		return (1);
	}
	sim_destroy(&sim);
	return (0);
}

int	parse_number(char *arg, long *dest)
{
	int	i;

	i = 0;
	if (arg[0] == '\0')
		return (0);
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (0);
		i++;
	}
	*dest = (long)atoi(arg);
	return (1);
}

int	parse_numbers(char **argv, long *values)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (parse_number(argv[i], &values[i - 1]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	valid_scheduler(char *arg)
{
	if (strcmp(arg, "fifo") == 0 || strcmp(arg, "edf") == 0)
		return (1);
	return (0);
}

int	parse_args(int argc, char **argv, t_simulation *sim)
{
	long	values[7];

	if (argc != 9)
		return (0);
	if (parse_numbers(argv, values) == 0)
		return (0);
	if (valid_scheduler(argv[8]) == 0)
		return (0);
	sim->nb_of_coders = (int)values[0];
	sim->time_to_burnout = values[1];
	sim->time_to_compile = values[2];
	sim->time_to_debug = values[3];
	sim->time_to_refactor = values[4];
	sim->compiling_quota = (int)values[5];
	sim->dongle_cooldown = values[6];
	sim->scheduler = argv[8];
	return (1);
}
