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

#include "lem-in.h"
#include <stdlib.h>
#include <unistd.h>

extern t_link						g_link[];
extern t_ant						g_ant[];
extern t_room						g_room[];
extern int							g_room_index;
extern int							g_link_index;

t_room								*get_room_by_name(
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

void								combine_name_nbr(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	t_room							*room;

	room = get_room_by_name(first_capture->data);
	room = room ? room : g_room + g_room_index++;
	room->name = first_capture->data;
	room->x = second_capture->data;
	first_capture->data = room;
	first_capture->nt = NT_ROOM;
}

void								combine_room_nbr(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	((t_room *)first_capture->data)->y = second_capture->data;
	first_capture->nt = NT_DEFROOM_INCOMPLETE;
}

void								combine_link_name(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	t_link							*link;

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

t_nt									g_nt_combination[NT_END][NT_END] = {
	[NT_BEGIN][NT_NAME] = NT_NAME,
	[NT_BEGIN][NT_NBR] = NT_ANTNBR_INCOMPLETE,
	[NT_COMMAND_BEGIN][NT_NAME] = NT_COMMAND_INCOMPLETE,
	[NT_DEFROOM_INCOMPLETE][NT_LINEFEED] = NT_DEFROOM,
	[NT_DEFLINK_INCOMPLETE][NT_LINEFEED] = NT_DEFLINK,
	[NT_ANTNBR_INCOMPLETE][NT_LINEFEED] = NT_ANTNBR,
	[NT_COMMAND_INCOMPLETE][NT_LINEFEED] = NT_COMMAND,
	[NT_NAME][NT_DASH] = NT_LINK,
};

t_combine_func						g_nt_combination_func[NT_END][NT_END] =
{
	[NT_NAME][NT_NBR] = combine_name_nbr,
	[NT_ROOM][NT_NBR] = combine_room_nbr,
	[NT_LINK][NT_NAME] = combine_link_name,
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
