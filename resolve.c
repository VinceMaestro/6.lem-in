/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:10:48 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:10:50 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <unistd.h>

static t_ant							g_ant[MAX_ANT];

static inline void						print_ant_movement(
	int ant_id,
	const char *const restrict room_name)
{
	int									e;
	char								nbr[30];

	write(1, "L", 1);
	e = 29;
	nbr[e--] = '0' + ant_id % 10;
	while ((ant_id /= 10))
		nbr[e--] = '0' + ant_id % 10;
	write(1, nbr + e + 1, 29 - e);
	write(1, "-", 1);
	e = 0;
	while (room_name[e])
		++e;
	write(1, room_name, e);
	write(1, " ", 1);
}

static inline int						move_ant(
	t_ant *const restrict ant,
	const int ant_id,
	t_room *const restrict best_room,
	t_room *const restrict end_room)
{
	ant->room->negative++;
	if (best_room->positive > ant->room->positive)
		ant->room->positive = best_room->positive - 1;
	if (end_room != best_room)
		best_room->ant = ant;
	ant->room->ant = 0;
	ant->room = best_room;
	print_ant_movement(ant_id, best_room->name);
	return (1);
}

static inline void						try_moving_ant(
	t_ant *const restrict ant,
	const int ant_id,
	int *const restrict something_moved,
	t_room *const restrict end_room
)
{
	t_room								*best_room;
	t_link_node							*link_node;
	int									score;
	int									best_score;

	link_node = ant->room->first_link;
	best_room = 0;
	while (link_node)
	{
		score = link_node->target->positive ?
			link_node->target->positive : -link_node->target->negative;
		if (!link_node->target->ant && (!best_room || best_score < score))
		{
			best_room = link_node->target;
			best_score = score;
		}
		link_node = link_node->next;
	}
	if (best_room)
		*something_moved = move_ant(ant, ant_id, best_room, end_room);
}

void									resolve(
	t_lemin *lemin)
{
	int									ant_id;
	t_ant								*ant;
	int									something_moved;

	ant_id = 0;
	write(1, "\n", 1);
	while (ant_id < lemin->antnbr)
		g_ant[ant_id++].room = lemin->start;
	something_moved = 1;
	lemin->start->negative = 0;
	lemin->end->positive = 999999999;
	while (something_moved)
	{
		something_moved = 0;
		ant_id = 0;
		while (ant_id < lemin->antnbr)
		{
			ant = g_ant + ant_id;
			if (lemin->end != ant->room)
				try_moving_ant(ant, ant_id, &something_moved, lemin->end);
			++ant_id;
		}
		if (something_moved)
			write(1, "\n", 1);
	}
}
