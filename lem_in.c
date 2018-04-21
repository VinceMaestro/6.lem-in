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
#include <limits.h>

static char								g_line_buffer[MAX_LINE_SIZE];
static t_link_node						g_link_node[MAX_LINK_NODE];
static int								g_link_node_index;
t_link									g_link[MAX_LINK];
t_room									g_room[MAX_ROOM];
int										g_room_index;
int										g_link_index;
t_capture								g_stack[PARSER_STACK_SIZE];
t_capture								*g_stack_ptr;

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

static inline void						create_link_nodes(void)
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

static inline void						preprocess(
	t_lemin *lemin)
{
	int									i;
	t_link_node							*tmp;

	create_link_nodes();
	i = 0;
	while (lemin->antnbr_string[i])
		lemin->antnbr = lemin->antnbr * 10 +
			(unsigned long long)(lemin->antnbr_string[i++] - '0');
	if (lemin->antnbr > MAX_ANT)
		exit((int)write(2, TOO_MANY_ANTS_ERROR_MESSAGE,
			sizeof(TOO_MANY_ANTS_ERROR_MESSAGE)));
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

	g_stack[0].nt = NT_BEGIN;
	g_stack_ptr = g_stack + 1;
	input = g_line_buffer;
	lemin.cur_state = S_INITIAL;
	while (0 < read(0, input, 1))
	{
		if (MAX_LINE_SIZE < (input - g_line_buffer))
			exit((int)write(2, LONG_LINE_ERROR, sizeof(LONG_LINE_ERROR)));
		*g_string_ptr++ = *input++;
		parse(&lemin, input) && parse(&lemin, input);
		if ('\n' == *(input - 1))
		{
			write(1, g_line_buffer, (size_t)(input - g_line_buffer));
			input = g_line_buffer;
		}
	}
	*input = 0;
	parse(&lemin, input);
	validate(&lemin);
	preprocess(&lemin);
	resolve(&lemin);
	return (0);
}
