/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_simulation *sim);
static int	check_completion(t_simulation *sim);
static void	wake_all(t_simulation *sim);

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				burn_id;

	sim = (t_simulation *)arg;
	burn_id = 0;
	while (1)
	{
		burn_id = check_burnout(sim);
		if (burn_id != 0 || check_completion(sim) == 1)
			break ;
		usleep(1000);
	}
	pthread_mutex_lock(&sim->dead_lock);
	sim->dead_flag = 1;
	pthread_mutex_unlock(&sim->dead_lock);
	wake_all(sim);
	if (burn_id != 0)
		log_burnout(sim, burn_id);
	return (NULL);
}

static int	check_burnout(t_simulation *sim)
{
	long	now;
	int		i;

	now = get_current_time();
	i = 0;
	while (i < sim->nb_of_coders)
	{
		if (now - sim->coders[i].last_compile >= sim->time_to_burnout)
			return (sim->coders[i].id);
		i++;
	}
	return (0);
}

static int	check_completion(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		if (sim->coders[i].compiling_score < sim->compiling_quota)
			return (0);
		i++;
	}
	return (1);
}

static void	wake_all(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_cond_broadcast(&sim->dongles[i].cond);
		i++;
	}
}
