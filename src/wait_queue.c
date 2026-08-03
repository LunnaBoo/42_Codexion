/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 10:00:00 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/02 10:00:00 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take(t_dongle *d, t_coder *c, t_simulation *sim);
static void	grant_dongle(t_dongle *d, t_coder *c);
static void	wait_for_grant(t_dongle *d, t_coder *c, t_simulation *sim);

void	request_dongle(t_dongle *d, t_coder *c, t_simulation *sim)
{
	t_heap_node	request;

	pthread_mutex_lock(&d->mutex);
	if (can_take(d, c, sim) == 1)
	{
		grant_dongle(d, c);
		pthread_mutex_unlock(&d->mutex);
		return ;
	}
	request.coder = c;
	request.priority = get_current_time();
	if (strcmp(sim->scheduler, "edf") == 0)
		request.priority = c->last_compile + sim->time_to_burnout;
	if (heap_push(d->wait_queue, request) == 0)
	{
		pthread_mutex_unlock(&d->mutex);
		return ;
	}
	wait_for_grant(d, c, sim);
	pthread_mutex_unlock(&d->mutex);
}

/* dead_flag is read lock-free: the monitor writes it under dead_lock before
   broadcasting, and the cond wait/acquire orders the read after that. */
static int	can_take(t_dongle *d, t_coder *c, t_simulation *sim)
{
	if (sim->dead_flag == 1)
		return (0);
	if (d->taken == 1)
		return (0);
	if (get_current_time() < d->available_after)
		return (0);
	if (d->wait_queue->size == 0)
		return (1);
	return (d->wait_queue->nodes[0].coder == c);
}

static void	grant_dongle(t_dongle *d, t_coder *c)
{
	(void)c;
	d->taken = 1;
}

static void	wait_for_grant(t_dongle *d, t_coder *c, t_simulation *sim)
{
	t_heap_node		head;
	struct timespec	ts;
	long			wait;

	while (1)
	{
		if (sim->dead_flag == 1)
			return ;
		if (can_take(d, c, sim) == 1)
		{
			heap_pop(d->wait_queue, &head);
			grant_dongle(d, c);
			return ;
		}
		wait = d->available_after - get_current_time();
		if (wait <= 0)
			pthread_cond_wait(&d->cond, &d->mutex);
		else
		{
			ts = ms_to_timespec((size_t)wait);
			pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
		}
	}
}

void	release_dongle(t_dongle *d, long cooldown)
{
	pthread_mutex_lock(&d->mutex);
	d->taken = 0;
	d->available_after = get_current_time() + cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
