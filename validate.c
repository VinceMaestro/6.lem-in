/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:10:58 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:10:59 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <unistd.h>
#include <stdlib.h>

static inline int					superposition_error(
	const char *const restrict name_a,
	const char *const restrict name_b)
{
	size_t							e;

	write(2, SUPERPOSITION_ERROR_MESSAGE, sizeof(SUPERPOSITION_ERROR_MESSAGE));
	e = 0;
	while (name_a[e])
		++e;
	write(2, name_a, e);
	write(2, ", ", sizeof(", "));
	e = 0;
	while (name_b[e])
		++e;
	write(2, name_b, e);
	return (1);
}

static inline void					check_room_superposition(void)
{
	int								i;
	t_room							*room;

	i = 0;
	if (g_room_index)
	{
		while (i < g_room_index - 1)
		{
			room = g_room + i + 1;
			while (room != g_room + g_room_index)
			{
				if (ft_strcmp(room->x, g_room[i].x) &&
					ft_strcmp(room->y, g_room[i].y))
					exit(superposition_error(room->name, g_room[i].name));
				++room;
			}
			++i;
		}
	}
}

static inline int					check_start_end_connection(
	t_lemin *lemin)
{
	int								updated_something;
	t_link							*link;

	updated_something = 1;
	while (updated_something)
	{
		updated_something = 0;
		link = g_link + g_link_index;
		while (link != g_link)
		{
			if ((link - 1)->a->connected ^ (link - 1)->b->connected)
			{
				(link - 1)->a->connected = 1;
				(link - 1)->b->connected = 1;
				updated_something = 1;
			}
			--link;
		}
	}
	return (lemin->end->connected);
}

void								validate(
	t_lemin *lemin)
{
	if (!lemin->start || !lemin->end || lemin->start == lemin->end)
		exit((int)write(2, BAD_START_END_MESSAGE,
			sizeof(BAD_START_END_MESSAGE)));
	check_room_superposition();
	lemin->start->connected = 1;
	if (!g_link_index || !check_start_end_connection(lemin))
		exit((int)write(2, DISC_START_END_ERROR_MESSAGE,
			sizeof(DISC_START_END_ERROR_MESSAGE)));
}
