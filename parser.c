#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char								g_buffer[10000];

void								check_room_superposition()
{
	int								i;
	int								e;
	int								same_x;
	int								same_y;
	t_room							*room;

	i = 0;
	if (g_room_index)
	{
		while (i < g_room_index - 1)
		{
			room = g_room + i + 1;
			while(room != g_room + g_room_index)
			{
				e = 0;
				while (room->x[e] && g_room[i].x[e] == room->x[e])
					++e;
				same_x = g_room[i].x[e] == room->x[e];
				e = 0;
				while (room->y[e] && g_room[i].y[e] == room->y[e])
					++e;
				same_y = g_room[i].y[e] == room->y[e];
				if (same_x && same_y)
				{
					write(2, SUPERPOSITION_ERROR_MESSAGE, sizeof(SUPERPOSITION_ERROR_MESSAGE));
					e = 0;
					while (room->name[e])
						++e;
					write(2, room->name, e);
					write(2, ", ", sizeof(", "));
					e = 0;
					while (g_room[i].name[e])
						++e;
					write(2, g_room[i].name, e);
					exit(1);
				}
				++room;
			}
			++i;
		}
	}
}

t_room								*get_room_by_name(
	const char *const restrict name)
{
	int								i;
	int								e;

	i = 0;
	while (i < g_room_index)
	{
		e = 0;
		while (name[e] && g_room[i].name[e] == name[e])
			++e;
		if (g_room[i].name[e] == name[e])
			return (g_room + i);
		++i;
	}
	return (0);
}

int									get_command_by_name(
	const char *const restrict name)
{
	int								i;
	int								e;

	i = 0;
	while (g_commands[i])
	{
		e = 0;
		while (name[e] && g_commands[i][e] == name[e])
			++e;
		if (g_commands[i][e] == name[e])
			return (i);
		++i;
	}
	return (-1);
}

int									combine2(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture)
{
	t_room							*room;
	t_link							*link;

	if (nt_combination[first_capture->nt][second_capture->nt])
	{
		first_capture->nt = nt_combination[first_capture->nt][second_capture->nt];
		first_capture->data = first_capture->data ?
			first_capture->data : second_capture->data;
	}
	else if (NT_NAME == first_capture->nt && NT_NBR == second_capture->nt)
	{
		room = get_room_by_name(first_capture->data);
		room = room ? room : g_room + g_room_index++;
		room->name = first_capture->data;
		room->x = second_capture->data;
		first_capture->data = room;
		first_capture->nt = NT_ROOM;
	}
	else if (NT_ROOM == first_capture->nt && NT_NBR == second_capture->nt)
	{
		((t_room *)first_capture->data)->y = second_capture->data;
		first_capture->nt = NT_DEFROOM_INCOMPLETE;
	}
	else if (NT_LINK == first_capture->nt && NT_NAME == second_capture->nt)
	{
		// TODO
		// [NT_LINK][NT_NAME] = NT_DEFLINK_INCOMPLETE,
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
	else
		return (0);
	return (1);
}

void								combine(
	t_lemin *const restrict lemin)
{
	while (lemin->stack != (lemin->stack_ptr - 1) &&
		combine2(lemin->stack_ptr - 2, lemin->stack_ptr - 1))
	{
		--lemin->stack_ptr;
	}
	if (lemin->stack != (lemin->stack_ptr - 1) &&
		forbidden_combination[(lemin->stack_ptr - 2)->nt]
			[(lemin->stack_ptr - 1)->nt])
	{
		write(2, SYNTAX_ERROR, sizeof(SYNTAX_ERROR));
		exit(1);
	}
}

char								*get_string(int use)
{
	char							*res;

	res = 0;
	if (use)
	{
		res = g_string_origin;
		*g_string_ptr = *(g_string_ptr - 1);
		*(g_string_ptr - 1) = 0;
		g_string_origin = g_string_ptr++;
	}
	else
	{
		*g_string_origin = *(g_string_ptr - 1);
		g_string_ptr = g_string_origin + 1;
	}
	return (res);
}

int									transit(
	t_lemin *const restrict lemin,
	const char *const restrict input)
{
	t_state							new_state;
	t_nt							new_nt;
	int								pushed_something;
	char							c;
	int								command;

	// printf("begin_loop\n");
	// printf("lemin->cur_state = %s ; *(lemin->stack_ptr - 1) = %s ; *input = [%c]\n", state_string[lemin->cur_state], nt_string[(lemin->stack_ptr - 1)->nt], *(input - 1));
	pushed_something = 0;
	c = character_class[*(input - 1)] ? character_class[*(input - 1)] : *(input - 1);
	new_state = transitions_stack[c][lemin->cur_state][(lemin->stack_ptr - 1)->nt];
	if (!new_state)
		new_state = transitions[c][lemin->cur_state];
	if (!new_state)
		new_state = defaults[lemin->cur_state];
	if (S_INITIAL != lemin->cur_state && !new_state)
	{
		// printf("lemin->cur_state = %s\n", state_string[lemin->cur_state]);
		// printf("new_nt = %s\n", nt_string[nt_to_keep[lemin->cur_state]]);
		lemin->stack_ptr->nt = nt_to_keep[lemin->cur_state];
		(lemin->stack_ptr++)->data =
			get_string(g_is_data[nt_to_keep[lemin->cur_state]]);
		combine(lemin);
		new_state = S_INITIAL;
		pushed_something = 1;
		if (NT_DEFROOM == (lemin->stack_ptr - 1)->nt && lemin->flags.f.start)
		{
			lemin->start = (lemin->stack_ptr - 1)->data;
			lemin->flags.f.start = 0;
		}
		if (NT_DEFROOM == (lemin->stack_ptr - 1)->nt && lemin->flags.f.end)
		{
			lemin->end = (lemin->stack_ptr - 1)->data;
			lemin->flags.f.end = 0;
		}
		if (NT_ANTNBR == (lemin->stack_ptr - 1)->nt)
			lemin->antnbr_string = (lemin->stack_ptr - 1)->data;
		if (NT_COMMAND == (lemin->stack_ptr - 1)->nt)
		{
			if (-1 != (command = get_command_by_name((lemin->stack_ptr - 1)->data)))
				lemin->flags.value |= 1 << command;
			--lemin->stack_ptr;
		}
	}
	else if (!new_state)
	{
		write(2, SYNTAX_ERROR, sizeof(SYNTAX_ERROR));
		exit(1);
	}
	else if (S_INITIAL == lemin->cur_state)
		get_string(0);
	// printf("lemin->cur_state = %s => new_state = %s\n", state_string[lemin->cur_state], state_string[new_state]);
	lemin->cur_state = new_state;
	// printf("end_loop\n");
	return (pushed_something);
}

int									check_start_end_connection(
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

void								create_link_nodes()
{
	t_link_node						*link_node;
	t_room							*room;
	t_link							*link;

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
				link_node->target = (link - 1)->a == (room - 1) ? (link - 1)->b : (link - 1)->a;
			}
			--link;
		}
		--room;
	}
}


void								error_exit(
	const char *const restrict error_code,
	const size_t size)
{
	write(2, error_code, size);
	exit(1);
}

void								validate_and_preprocess(
	t_lemin *lemin)
{
	int								i;

	if (!lemin->start || !lemin->end || lemin->start == lemin->end)
		error_exit(BAD_START_END_MESSAGE, sizeof(BAD_START_END_MESSAGE));
	check_room_superposition();
	lemin->start->connected = 1;
	if (!g_link_index || !check_start_end_connection(lemin))
		error_exit(DISC_START_END_ERROR_MESSAGE, sizeof(DISC_START_END_ERROR_MESSAGE));
	create_link_nodes();
	i = 0;
	while (lemin->antnbr_string[i])
		lemin->antnbr = lemin->antnbr * 10 + lemin->antnbr_string[i++] - '0';
	i = 0;
	while (i < g_room_index)
	{
		printf("[%s] [%s] : [%s]\n", g_room[i].name, g_room[i].x, g_room[i].y);
		++i;
	}
	i = 0;
	while (i < g_link_index)
	{
		// printf("[%d]-[%d]\n", i, g_link_index);
		// printf("[%p]-[%p]\n", g_link[i].a, g_link[i].b);
		printf("[%s]-[%s]\n", g_link[i].a->name, g_link[i].b->name);
		++i;
	}
	printf("[%s] => [%s]\n", lemin->start->name, lemin->end->name);
	printf("%p\n", lemin->start->first_link);
	t_link_node					*tmp;
	tmp = lemin->start->first_link;
	while (tmp)
	{
		printf("%s\n", tmp->target->name);
		tmp = tmp->next;
	}
	printf("antnbr : %llu\n", lemin->antnbr);
}

void								resolve(
	t_lemin *lemin)
{
	int								i;
	t_ant							*ant;
	t_room							*best_room;
	t_link_node						*link_node;
	int								score;
	int								best_score;
	int								something_moved;

	i = 0;
	while (i < lemin->antnbr)
		g_ant[i++].room = lemin->start;
	something_moved = 1;
	lemin->start->negative = 0;
	lemin->end->positive = 999999999;
	while (something_moved)
	{
		something_moved = 0;
		i = 0;
		// printf("yop %d %d\n", something_moved, i);
		while (i < lemin->antnbr)
		{
			ant = g_ant + i;
			if (lemin->end != ant->room)
			{
				link_node = ant->room->first_link;
				best_room = 0;
				// printf("lol %p %p\n", link_node, best_room);
				while (link_node)
				{
					score = link_node->target->positive ?
						link_node->target->positive : -link_node->target->negative;
					// printf("mdr %s %d\n", link_node->target->name, score);
					if (!link_node->target->ant && (!best_room || best_score < score))
					{
						best_room = link_node->target;
						best_score = score;
					}
					link_node = link_node->next;
				}
				if (best_room)
				{
					ant->room->negative++;
					// best_room->negative += ant->room->negative / 100;
					// if (ant->room->negative > best_room->negative)
					// 	best_room->negative = ant->room->negative - 1;
					if (best_room->positive > ant->room->positive)
						ant->room->positive = best_room->positive - 1;
					if (lemin->end != best_room)
						best_room->ant = ant;
					ant->room->ant = 0;
					ant->room = best_room;
					something_moved = 1;
					printf("L%d-%s ", i, best_room->name);
				}
			}
			++i;
		}
		printf("YOLO\n");
	}
}

int									main(void)
{
	t_lemin							lemin;
	char							*input;

	lemin.stack[0].nt = NT_BEGIN;
	lemin.stack[0].data = 0;
	lemin.stack_ptr = lemin.stack + 1;
	input = g_buffer;
	lemin.cur_state = S_INITIAL;
	// printf("lmao\n");
	while (0 < read(0, input, 1))
	{
		*g_string_ptr++ = *input++;
		if (transit(&lemin, input))
			transit(&lemin, input);
		if ('\n' == *(input -1))
		{
			write(1, g_buffer, input - g_buffer);
			input = g_buffer;
		}
	}
	*input = 0;
	transit(&lemin, input);
	validate_and_preprocess(&lemin);
	resolve(&lemin);
	return (0);
}
