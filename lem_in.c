/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:11:13 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:11:14 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char									g_buffer[10000];

t_link_node								g_link_node[MAX_LINK_NODE];
t_link									g_link[MAX_LINK];
t_ant									g_ant[MAX_ANT];
t_room									g_room[MAX_ROOM];
int										g_room_index;
int										g_link_index;
int										g_link_node_index;

extern char								*g_string_ptr;

int										ft_strcmp(
	const char *restrict str_a,
	const char *restrict str_b)
{
	while (*str_a && *str_b == *str_a)
	{
		++str_a;
		++str_b;
	}
	return (*str_a == *str_b);
}

void									create_link_nodes(void)
{
	t_link_node							*link_node;
	t_room								*room;
	t_link								*link;

	room = g_room + g_room_index;
	while (g_room != room)
	{
		link = g_link + g_link_index;
		while (g_link != link)
		{
			if ((link - 1)->a == (room - 1) || (link - 1)->b == (room - 1))
			{
				link_node = g_link_node + g_link_node_index++;
				link_node->next = (room - 1)->first_link;
				(room - 1)->first_link = link_node;
				link_node->target =
					(link - 1)->a == (room - 1) ? (link - 1)->b : (link - 1)->a;
			}
			--link;
		}
		--room;
	}
}

void									preprocess(
	t_lemin *lemin)
{
	int									i;
	t_link_node							*tmp;

	create_link_nodes();
	i = 0;
	while (lemin->antnbr_string[i])
		lemin->antnbr = lemin->antnbr * 10 + lemin->antnbr_string[i++] - '0';
	i = 0;
	while (i < g_room_index)
		++i;
	i = 0;
	while (i < g_link_index)
		++i;
	tmp = lemin->start->first_link;
	while (tmp)
		tmp = tmp->next;
}

int										main(void)
{
	t_lemin								lemin;
	char								*input;

	lemin.stack[0].nt = NT_BEGIN;
	lemin.stack[0].data = 0;
	lemin.stack_ptr = lemin.stack + 1;
	input = g_buffer;
	lemin.cur_state = S_INITIAL;
	while (0 < read(0, input, 1))
	{
		*g_string_ptr++ = *input++;
		if (parse(&lemin, input))
			parse(&lemin, input);
		if ('\n' == *(input - 1))
		{
			write(1, g_buffer, input - g_buffer);
			input = g_buffer;
		}
	}
	*input = 0;
	parse(&lemin, input);
	validate(&lemin);
	preprocess(&lemin);
	resolve(&lemin);
	return (0);
}
