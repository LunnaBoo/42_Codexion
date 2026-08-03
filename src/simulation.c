/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_shared_locks(t_simulation *sim);
static int	init_shared_locks(t_simulation *sim);

int	sim_init(t_simulation *sim)
{
	if (init_shared_locks(sim) == 0)
		return (0);
	if (init_dongles(sim) == 0)
	{
		destroy_shared_locks(sim);
		return (0);
	}
	if (init_coders(sim) == 0)
	{
		destroy_dongles(sim, sim->nb_of_coders);
		destroy_shared_locks(sim);
		return (0);
	}
	sim->start_time = get_current_time();
	sim->dead_flag = 0;
	return (1);
}

void	sim_destroy(t_simulation *sim)
{
	destroy_dongles(sim, sim->nb_of_coders);
	free(sim->coders);
	sim->coders = NULL;
	destroy_shared_locks(sim);
}

static void	destroy_partial_locks(t_simulation *sim, int level)
{
	if (level >= 3)
		pthread_mutex_destroy(&sim->print_lock);
	if (level >= 2)
		pthread_mutex_destroy(&sim->compile_lock);
	if (level >= 1)
		pthread_mutex_destroy(&sim->dead_lock);
}

static void	destroy_shared_locks(t_simulation *sim)
{
	destroy_partial_locks(sim, 3);
}

static int	init_shared_locks(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->dead_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->compile_lock, NULL) != 0)
	{
		destroy_partial_locks(sim, 1);
		return (0);
	}
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
	{
		destroy_partial_locks(sim, 2);
		return (0);
	}
	return (1);
}
