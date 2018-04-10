/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:31:19 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:31:23 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# define CHAR_DIGIT						'0'
# define CHAR_ALPHA						'a'
# define CHAR_ALPHA_NO_L				'b'
# define CHAR_BLANK						' '
# define MAX_ANT						100000
# define MAX_ROOM						10000
# define MEMORY_SIZE					1000000
# define MAX_LINK						100000
# define MAX_LINK_NODE					100000
# define SUPERPOSITION_ERROR_MESSAGE	("Error : different rooms \
	should not be at the exact same coordinates : ")

# define BAD_START_END_MESSAGE			("Error : both start and end "\
	"rooms must exist and be differeeeeeeeeeeeent.\n")

# define UNDEFINED_ROOM_MESSAGE			("Error : Undefined room\n")
# define SYNTAX_ERROR					("Error : Syntax error\n")
# define DISC_START_END_ERROR_MESSAGE	("Error : Start room and End room "\
	"must be connected together.\n")

typedef enum							e_nt
{
	NT_BEGIN = 1,
	NT_ANTNBR,
	NT_DASH,
	NT_LINK,
	NT_COMMAND,
	NT_COMMAND_BEGIN,
	NT_DEFLINK_INCOMPLETE,
	NT_DEFROOM_INCOMPLETE,
	NT_ANTNBR_INCOMPLETE,
	NT_COMMAND_INCOMPLETE,
	NT_DEFLINK,
	NT_DEFROOM,
	NT_LINEFEED,
	NT_ROOM,
	NT_NAME,
	NT_NBR,
	NT_END,
}										t_nt;

typedef enum							e_state
{
	S_INITIAL = 1,
	S_LINE,
	S_BLANK,
	S_NAME,
	S_NBR,
	S_DASH,
	S_LINEFEED,
	S_NAME_DASH,
	S_SHARP,
	S_COMMENT,
	S_COMMAND,
	S_END,
}										t_state;

typedef struct							s_room
{
	char								*name;
	char								*x;
	char								*y;
	struct s_link_node					*first_link;
	int									connected;
	int									negative;
	int									positive;
	struct s_ant						*ant;
}										t_room;

typedef struct							s_ant
{
	t_room								*room;
}										t_ant;

typedef struct							s_link
{
	t_room								*a;
	t_room								*b;
}										t_link;

typedef struct							s_link_node
{
	struct s_link_node					*next;
	t_room								*target;
}										t_link_node;

typedef struct							s_capture
{
	t_nt								nt;
	void								*data;
}										t_capture;

typedef struct							s_lemin
{
	union
	{
		char							value;
		struct
		{
			char						start:1;
			char						end:1;
		}								f;
	}									flags;
	t_state								cur_state;
	t_capture							stack[10000];
	t_capture							*stack_ptr;
	t_room								*start;
	t_room								*end;
	unsigned long long					antnbr;
	char								*antnbr_string;
}										t_lemin;

typedef void							(*t_combine_func)(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture);

void									validate(
	t_lemin *lemin);

int										combine_loop(
	t_capture *const restrict first_capture,
	t_capture *const restrict second_capture);

int										ft_strcmp(
	const char *restrict str_a,
	const char *restrict str_b);

int										parse(
	t_lemin *const restrict lemin,
	const char *const restrict input);

void									resolve(
	t_lemin *lemin);

#endif
