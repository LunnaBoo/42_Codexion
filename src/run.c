/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	spawn_coders(t_simulation *sim);
static void	join_coders(t_simulation *sim);
static void	stop_sim(t_simulation *sim);

int	sim_run(t_simulation *sim)
{
	if (spawn_coders(sim) == 0)
	{
		stop_sim(sim);
		join_coders(sim);
		return (0);
	}
	if (pthread_create(&sim->thread, NULL, monitor_routine, sim) != 0)
	{
		stop_sim(sim);
		join_coders(sim);
		return (0);
	}
	join_coders(sim);
	pthread_join(sim->thread, NULL);
	return (1);
}

static int	spawn_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		sim->coders[i].last_compile = sim->start_time;
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

static void	join_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

static void	stop_sim(t_simulation *sim)
{
	pthread_mutex_lock(&sim->dead_lock);
	sim->dead_flag = 1;
	pthread_mutex_unlock(&sim->dead_lock);
}
