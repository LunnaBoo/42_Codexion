/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/02 10:00:00 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	t_dongle	*right;
	t_dongle	*left;

	right = coder->r_dongle;
	left = coder->l_dongle;
	if (right == left)
		return ;
	if (right->id < left->id)
	{
		request_dongle(right, coder, coder->sim);
		request_dongle(left, coder, coder->sim);
		log_action(coder->sim, coder->id, "has taken a dongle");
		log_action(coder->sim, coder->id, "has taken a dongle");
	}
	else
	{
		request_dongle(left, coder, coder->sim);
		request_dongle(right, coder, coder->sim);
		log_action(coder->sim, coder->id, "has taken a dongle");
		log_action(coder->sim, coder->id, "has taken a dongle");
	}
}

void	release_dongles(t_coder *coder)
{
	if (coder->r_dongle == coder->l_dongle)
		return ;
	release_dongle(coder->r_dongle, coder->sim->dongle_cooldown);
	release_dongle(coder->l_dongle, coder->sim->dongle_cooldown);
}
