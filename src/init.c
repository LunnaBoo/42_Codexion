/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle(t_simulation *sim, t_dongle *dongle);

int	init_dongles(t_simulation *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->nb_of_coders);
	if (sim->dongles == NULL)
		return (0);
	i = 0;
	while (i < sim->nb_of_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].taken = 0;
		sim->dongles[i].available_after = 0;
		sim->dongles[i].wait_queue = NULL;
		if (init_dongle(sim, &sim->dongles[i]) == 0)
		{
			destroy_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_dongle(t_simulation *sim, t_dongle *dongle)
{
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	dongle->wait_queue = malloc(sizeof(t_heap));
	if (dongle->wait_queue == NULL)
	{
		pthread_cond_destroy(&dongle->cond);
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	if (heap_init(dongle->wait_queue, sim->nb_of_coders, cmp_node) == 0)
	{
		free(dongle->wait_queue);
		pthread_cond_destroy(&dongle->cond);
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

void	destroy_dongle_partial(t_dongle *dongle)
{
	if (dongle->wait_queue != NULL)
	{
		heap_destroy(dongle->wait_queue);
		free(dongle->wait_queue);
		dongle->wait_queue = NULL;
	}
	pthread_mutex_destroy(&dongle->mutex);
	pthread_cond_destroy(&dongle->cond);
}

void	destroy_dongles(t_simulation *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		destroy_dongle_partial(&sim->dongles[i]);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

int	init_coders(t_simulation *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->nb_of_coders);
	if (sim->coders == NULL)
		return (0);
	i = 0;
	while (i < sim->nb_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].is_compiling = 0;
		sim->coders[i].compiling_score = 0;
		sim->coders[i].last_compile = 0;
		sim->coders[i].r_dongle = &sim->dongles[i];
		sim->coders[i].l_dongle = &sim->dongles[(i + 1) % sim->nb_of_coders];
		sim->coders[i].dead_lock = &sim->dead_lock;
		sim->coders[i].compile_lock = &sim->compile_lock;
		sim->coders[i].print_lock = &sim->print_lock;
		sim->coders[i].sim = sim;
		i++;
	}
	return (1);
}
