/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:11:27 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:11:33 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <stdlib.h>
#include <unistd.h>

static char							g_character_class[255] = {
	['0'] = CHAR_DIGIT,
	['1'] = CHAR_DIGIT,
	['2'] = CHAR_DIGIT,
	['3'] = CHAR_DIGIT,
	['4'] = CHAR_DIGIT,
	['5'] = CHAR_DIGIT,
	['6'] = CHAR_DIGIT,
	['7'] = CHAR_DIGIT,
	['8'] = CHAR_DIGIT,
	['9'] = CHAR_DIGIT,
	[' '] = CHAR_BLANK,
	['\t'] = CHAR_BLANK,
	['\r'] = CHAR_BLANK,
	['\v'] = CHAR_BLANK,
	['\f'] = CHAR_BLANK,
	['a'] = CHAR_ALPHA,
	['b'] = CHAR_ALPHA,
	['c'] = CHAR_ALPHA,
	['d'] = CHAR_ALPHA,
	['e'] = CHAR_ALPHA,
	['f'] = CHAR_ALPHA,
	['g'] = CHAR_ALPHA,
	['h'] = CHAR_ALPHA,
	['i'] = CHAR_ALPHA,
	['j'] = CHAR_ALPHA,
	['k'] = CHAR_ALPHA,
	['l'] = CHAR_ALPHA,
	['m'] = CHAR_ALPHA,
	['n'] = CHAR_ALPHA,
	['o'] = CHAR_ALPHA,
	['p'] = CHAR_ALPHA,
	['q'] = CHAR_ALPHA,
	['r'] = CHAR_ALPHA,
	['s'] = CHAR_ALPHA,
	['t'] = CHAR_ALPHA,
	['u'] = CHAR_ALPHA,
	['v'] = CHAR_ALPHA,
	['w'] = CHAR_ALPHA,
	['x'] = CHAR_ALPHA,
	['y'] = CHAR_ALPHA,
	['z'] = CHAR_ALPHA,
	['A'] = CHAR_ALPHA,
	['B'] = CHAR_ALPHA,
	['C'] = CHAR_ALPHA,
	['D'] = CHAR_ALPHA,
	['E'] = CHAR_ALPHA,
	['F'] = CHAR_ALPHA,
	['G'] = CHAR_ALPHA,
	['H'] = CHAR_ALPHA,
	['I'] = CHAR_ALPHA,
	['J'] = CHAR_ALPHA,
	['K'] = CHAR_ALPHA,
	['M'] = CHAR_ALPHA,
	['N'] = CHAR_ALPHA,
	['O'] = CHAR_ALPHA,
	['P'] = CHAR_ALPHA,
	['Q'] = CHAR_ALPHA,
	['R'] = CHAR_ALPHA,
	['S'] = CHAR_ALPHA,
	['T'] = CHAR_ALPHA,
	['U'] = CHAR_ALPHA,
	['V'] = CHAR_ALPHA,
	['W'] = CHAR_ALPHA,
	['X'] = CHAR_ALPHA,
	['Y'] = CHAR_ALPHA,
	['Z'] = CHAR_ALPHA,
};

static int							g_is_data[NT_END] = {
	[NT_NBR] = 1,
	[NT_NAME] = 1,
};

static t_nt							g_nt_to_keep[S_END] = {
	[S_NBR] = NT_NBR,
	[S_NAME] = NT_NAME,
	[S_DASH] = NT_DASH,
	[S_LINEFEED] = NT_LINEFEED,
	[S_COMMAND] = NT_COMMAND_BEGIN,
};

static t_state						g_defaults[S_END] = {
	[S_SHARP] = S_COMMENT,
	[S_COMMENT] = S_COMMENT,
};

static t_state						g_transitions[255][S_END] = {
	[CHAR_DIGIT][S_NBR] = S_NBR,
	['L'][S_NAME] = S_NAME,
	[CHAR_ALPHA][S_NAME] = S_NAME,
	[CHAR_DIGIT][S_NAME] = S_NAME,
	[CHAR_BLANK][S_INITIAL] = S_INITIAL,
	['#'][S_SHARP] = S_COMMAND,
	['\n'][S_INITIAL] = S_LINEFEED,
	['\n'][S_COMMENT] = S_INITIAL,
	[0][S_INITIAL] = S_INITIAL,
};

static t_state						g_transitions_stack[255][S_END][NT_END] =
{
	[CHAR_DIGIT][S_INITIAL][NT_NAME] = S_NBR,
	['-'][S_INITIAL][NT_NAME] = S_DASH,
	[CHAR_DIGIT][S_INITIAL][NT_BEGIN] = S_NBR,
	[CHAR_DIGIT][S_INITIAL][NT_ROOM] = S_NBR,
	[CHAR_ALPHA][S_INITIAL][NT_LINK] = S_NAME,
	['#'][S_INITIAL][NT_BEGIN] = S_SHARP,
	[CHAR_ALPHA][S_INITIAL][NT_COMMAND_BEGIN] = S_NAME,
	[CHAR_DIGIT][S_INITIAL][NT_COMMAND_BEGIN] = S_NAME,
	[CHAR_ALPHA][S_INITIAL][NT_ANTNBR] = S_NAME,
	[CHAR_DIGIT][S_INITIAL][NT_ANTNBR] = S_NAME,
	['#'][S_INITIAL][NT_ANTNBR] = S_SHARP,
	[CHAR_ALPHA][S_INITIAL][NT_DEFROOM] = S_NAME,
	[CHAR_DIGIT][S_INITIAL][NT_DEFROOM] = S_NAME,
	['#'][S_INITIAL][NT_DEFROOM] = S_SHARP,
	[CHAR_ALPHA][S_INITIAL][NT_DEFLINK] = S_NAME,
	[CHAR_DIGIT][S_INITIAL][NT_DEFLINK] = S_NAME,
	['#'][S_INITIAL][NT_DEFLINK] = S_SHARP,
	[CHAR_DIGIT][S_INITIAL][NT_LINK] = S_NAME,
};

static int							g_forbidden_combination[NT_END][NT_END] =
{
	[NT_DEFLINK][NT_DEFROOM] = 1,
	[NT_NBR][NT_LINEFEED] = 1,
	[NT_NAME][NT_LINEFEED] = 1,
	[NT_LINK][NT_LINEFEED] = 1,
	[NT_ROOM][NT_LINEFEED] = 1,
	[NT_COMMAND_BEGIN][NT_LINEFEED] = 1,
	[NT_BEGIN][NT_LINEFEED] = 1,
	[NT_ANTNBR][NT_LINEFEED] = 1,
	[NT_DEFLINK][NT_LINEFEED] = 1,
	[NT_DEFROOM][NT_LINEFEED] = 1,
};

static char							*g_commands[] = {
	"start",
	"end",
	0,
};

static char							g_string[MEMORY_SIZE];
static char							*g_string_origin = g_string;
char								*g_string_ptr = g_string;

static inline int					get_command_by_name(
	const char *const restrict name)
{
	int								i;

	i = 0;
	while (g_commands[i])
	{
		if (ft_strcmp(g_commands[i], name))
			return (i);
		++i;
	}
	return (-1);
}

static inline char					*get_string(int use)
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

static inline void					combine(void)
{
	while (g_stack != (g_stack_ptr - 1) &&
		combine_loop(g_stack_ptr - 2, g_stack_ptr - 1))
		--g_stack_ptr;
	if (g_stack != (g_stack_ptr - 1) &&
		g_forbidden_combination[(g_stack_ptr - 2)->nt]
			[(g_stack_ptr - 1)->nt])
		exit((int)write(2, SYNTAX_ERROR, sizeof(SYNTAX_ERROR)));
}

static inline void					push_non_terminal(
	t_lemin *const restrict lemin)
{
	int								command;

	g_stack_ptr->nt = g_nt_to_keep[lemin->cur_state];
	(g_stack_ptr++)->data =
		get_string(g_is_data[g_nt_to_keep[lemin->cur_state]]);
	combine();
	if (NT_DEFROOM == (g_stack_ptr - 1)->nt && lemin->flags.f.start)
	{
		lemin->start = (g_stack_ptr - 1)->data;
		lemin->flags.f.start = 0;
	}
	if (NT_DEFROOM == (g_stack_ptr - 1)->nt && lemin->flags.f.end)
	{
		lemin->end = (g_stack_ptr - 1)->data;
		lemin->flags.f.end = 0;
	}
	if (NT_ANTNBR == (g_stack_ptr - 1)->nt)
		lemin->antnbr_string = (g_stack_ptr - 1)->data;
	if (NT_COMMAND == (g_stack_ptr - 1)->nt)
	{
		if (-1 != (command = get_command_by_name((g_stack_ptr - 1)->data)))
			lemin->flags.value |= 1 << command;
		--g_stack_ptr;
	}
}

int									parse(
	t_lemin *const restrict lemin,
	const char *const restrict input)
{
	t_state							new_state;
	int								pushed_something;
	char							c;

	pushed_something = 0;
	c = g_character_class[(int)*(input - 1)] ?
		g_character_class[(int)*(input - 1)] : *(input - 1);
	new_state = g_transitions_stack[(int)c]
		[lemin->cur_state][(g_stack_ptr - 1)->nt];
	if (!new_state)
		new_state = g_transitions[(int)c][lemin->cur_state];
	if (!new_state)
		new_state = g_defaults[lemin->cur_state];
	if (S_INITIAL != lemin->cur_state && !new_state)
	{
		push_non_terminal(lemin);
		pushed_something = 1;
		new_state = S_INITIAL;
	}
	else if (!new_state)
		exit((int)write(2, SYNTAX_ERROR, sizeof(SYNTAX_ERROR)));
	else if (S_INITIAL == lemin->cur_state)
		get_string(0);
	lemin->cur_state = new_state;
	return (pushed_something);
}
