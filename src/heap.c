/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int capacity, t_heap_cmp cmp)
{
	h->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (h->nodes == NULL)
		return (0);
	h->size = 0;
	h->capacity = capacity;
	h->seq = 0;
	h->cmp = cmp;
	return (1);
}

static void	sift_down(t_heap *h, int idx)
{
	t_heap_node	*nodes;
	t_heap_node	tmp;
	int			left;
	int			right;
	int			smallest;

	nodes = h->nodes;
	while (1)
	{
		left = idx * 2 + 1;
		right = left + 1;
		smallest = idx;
		if (left < h->size && h->cmp(&nodes[left], &nodes[smallest]) < 0)
			smallest = left;
		if (right < h->size && h->cmp(&nodes[right], &nodes[smallest]) < 0)
			smallest = right;
		if (smallest == idx)
			break ;
		tmp = nodes[idx];
		nodes[idx] = nodes[smallest];
		nodes[smallest] = tmp;
		idx = smallest;
	}
}

void	heap_destroy(t_heap *h)
{
	free(h->nodes);
	h->nodes = NULL;
	h->size = 0;
	h->capacity = 0;
	h->seq = 0;
	h->cmp = NULL;
}

int	heap_push(t_heap *h, t_heap_node item)
{
	t_heap_node	tmp;
	int			idx;
	int			parent;

	if (h->size >= h->capacity)
		return (0);
	item.seq = h->seq;
	h->seq++;
	h->nodes[h->size] = item;
	idx = h->size;
	h->size++;
	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (h->cmp(&h->nodes[idx], &h->nodes[parent]) < 0)
		{
			tmp = h->nodes[idx];
			h->nodes[idx] = h->nodes[parent];
			h->nodes[parent] = tmp;
			idx = parent;
		}
		else
			break ;
	}
	return (1);
}

int	heap_pop(t_heap *h, t_heap_node *out)
{
	if (h->size == 0)
		return (0);
	*out = h->nodes[0];
	h->size--;
	if (h->size > 0)
	{
		h->nodes[0] = h->nodes[h->size];
		sift_down(h, 0);
	}
	return (1);
}
