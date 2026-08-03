/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_dead(t_coder *coder);
static void	compile_cycle(t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		if (is_dead(coder) == 1)
			break ;
		if (coder->r_dongle == coder->l_dongle)
		{
			ft_usleep(1);
			continue ;
		}
		compile_cycle(coder);
	}
	return (NULL);
}

static int	is_dead(t_coder *coder)
{
	int	dead;

	pthread_mutex_lock(coder->dead_lock);
	dead = coder->sim->dead_flag;
	pthread_mutex_unlock(coder->dead_lock);
	return (dead);
}

void	log_action(t_simulation *sim, int id, char *msg)
{
	long	now;

	pthread_mutex_lock(&sim->print_lock);
	pthread_mutex_lock(&sim->dead_lock);
	if (sim->dead_flag == 1)
	{
		pthread_mutex_unlock(&sim->dead_lock);
		pthread_mutex_unlock(&sim->print_lock);
		return ;
	}
	pthread_mutex_unlock(&sim->dead_lock);
	now = get_current_time() - sim->start_time;
	printf("%ld %d %s\n", now, id, msg);
	pthread_mutex_unlock(&sim->print_lock);
}

void	log_burnout(t_simulation *sim, int id)
{
	long	now;

	pthread_mutex_lock(&sim->print_lock);
	now = get_current_time() - sim->start_time;
	printf("%ld %d burned out\n", now, id);
	pthread_mutex_unlock(&sim->print_lock);
}

static void	compile_cycle(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	take_dongles(coder);
	pthread_mutex_lock(coder->compile_lock);
	coder->last_compile = get_current_time();
	coder->compiling_score++;
	pthread_mutex_unlock(coder->compile_lock);
	log_action(sim, coder->id, "is compiling");
	ft_usleep(sim->time_to_compile);
	release_dongles(coder);
	log_action(sim, coder->id, "is debugging");
	ft_usleep(sim->time_to_debug);
	log_action(sim, coder->id, "is refactoring");
	ft_usleep(sim->time_to_refactor);
}
