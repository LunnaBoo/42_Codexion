/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_cmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cmp_node(t_heap_node *a, t_heap_node *b)
{
	if (a->priority < b->priority)
		return (-1);
	if (a->priority > b->priority)
		return (1);
	if (a->seq < b->seq)
		return (-1);
	if (a->seq > b->seq)
		return (1);
	return (0);
}
