/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combination.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:10:33 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:10:34 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <stdlib.h>
#include <unistd.h>

static inline t_room				*get_room_by_name(
	const char *const restrict name)
{
	int								i;

	i = 0;
	while (i < g_room_index)
	{
		if (ft_strcmp(g_room[i].name, name))
			return (g_room + i);
		++i;
	}
	return (0);
}

static inline void					combine_name_nbr(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	t_room							*room;

	if (g_room_index == MAX_ROOM)
		exit((int)write(2, TOO_MANY_ROOMS_ERROR_MESSAGE,
			sizeof(TOO_MANY_ROOMS_ERROR_MESSAGE)));
	room = get_room_by_name(first_capture->data);
	room = room ? room : g_room + g_room_index++;
	room->name = first_capture->data;
	room->x = second_capture->data;
	first_capture->data = room;
	first_capture->nt = NT_ROOM;
}

static inline void					combine_room_nbr(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	((t_room *)first_capture->data)->y = second_capture->data;
	first_capture->nt = NT_DEFROOM_INCOMPLETE;
}

static inline void					combine_link_name(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	t_link							*link;

	if (g_link_index == MAX_LINK)
		exit((int)write(2, TOO_MANY_LINKS_ERROR_MESSAGE,
			sizeof(TOO_MANY_LINKS_ERROR_MESSAGE)));
	link = g_link + g_link_index++;
	link->a = get_room_by_name(first_capture->data);
	link->b = get_room_by_name(second_capture->data);
	if (!link->a | !link->b)
	{
		write(2, UNDEFINED_ROOM_MESSAGE, sizeof(UNDEFINED_ROOM_MESSAGE));
		exit(1);
	}
	first_capture->data = link;
	first_capture->nt = NT_DEFLINK_INCOMPLETE;
}

static t_combine_func				g_nt_combination_func[NT_END][NT_END] =
{
	[NT_NAME][NT_NBR] = combine_name_nbr,
	[NT_ROOM][NT_NBR] = combine_room_nbr,
	[NT_LINK][NT_NAME] = combine_link_name,
};

static t_nt							g_nt_combination[NT_END][NT_END] = {
	[NT_BEGIN][NT_NAME] = NT_NAME,
	[NT_BEGIN][NT_NBR] = NT_ANTNBR_INCOMPLETE,
	[NT_COMMAND_BEGIN][NT_NAME] = NT_COMMAND_INCOMPLETE,
	[NT_DEFROOM_INCOMPLETE][NT_LINEFEED] = NT_DEFROOM,
	[NT_DEFLINK_INCOMPLETE][NT_LINEFEED] = NT_DEFLINK,
	[NT_ANTNBR_INCOMPLETE][NT_LINEFEED] = NT_ANTNBR,
	[NT_COMMAND_INCOMPLETE][NT_LINEFEED] = NT_COMMAND,
	[NT_NAME][NT_DASH] = NT_LINK,
};

int									combine_loop(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	if (g_nt_combination[first_capture->nt][second_capture->nt])
	{
		first_capture->nt =
			g_nt_combination[first_capture->nt][second_capture->nt];
		first_capture->data = first_capture->data ?
			first_capture->data : second_capture->data;
	}
	else if (g_nt_combination_func[first_capture->nt][second_capture->nt])
		g_nt_combination_func[first_capture->nt][second_capture->nt](
			first_capture, second_capture);
	else
		return (0);
	return (1);
}
