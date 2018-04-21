/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:31:19 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 15:31:23 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include <stdint.h>

# define CHAR_DIGIT						'0'
# define CHAR_ALPHA						'a'
# define CHAR_ALPHA_NO_L				'b'
# define CHAR_BLANK						' '
# define FIRST_ANT_ID					1
# define MAX_ANT						1000000
# define MAX_ROOM						1000000
# define MEMORY_SIZE					1000000
# define MAX_LINK						1000000
# define MAX_LINK_NODE					(MAX_LINK * 2)
# define MAX_LINE_SIZE					10000
# define PARSER_STACK_SIZE				1000000
# define SUERR_PART1					"Error : different rooms "
# define SUERR_PART2					"should not be at the"
# define SUERR_PART3					" exact same coordinates : "
# define SUPERPOSITION_ERROR_MESSAGE	(SUERR_PART1 SUERR_PART2 SUERR_PART3)
# define BAD_SE_PART1					"Error : both start and end "
# define BAD_SE_PART2					"rooms must exist and be different.\n"
# define BAD_START_END_MESSAGE			(BAD_SE_PART1 BAD_SE_PART2)
# define UNDEFINED_ROOM_MESSAGE			("Error : Undefined room\n")
# define SYNTAX_ERROR					("Error : Syntax error\n")
# define DISC_SEE_PART1					"Error : Start room and End room "
# define DISC_SEE_PART2					"must be connected together.\n"
# define DISC_START_END_ERROR_MESSAGE	(DISC_SEE_PART1 DISC_SEE_PART2)
# define TOO_MANY_ANTS_ERROR_MESSAGE	"Error : too many ants.\n"
# define TOO_MANY_ROOMS_ERROR_MESSAGE	"Error : Too many rooms.\n"
# define TOO_MANY_LINKS_ERROR_MESSAGE	"Error : Too many links.\n"
# define LONG_LINE_ERROR				"Error : A line is too long.\n"

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
	struct s_ant						*ant;
	int									connected;
	int									negative;
	int									positive;
	uint32_t							padding;
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
	void								*data;
	t_nt								nt;
	uint32_t							padding;
}										t_capture;

typedef struct							s_lemin
{
	t_room								*start;
	t_room								*end;
	char								*antnbr_string;
	t_state								cur_state;
	uint32_t							padding;
	unsigned long long					antnbr;
	union
	{
		char							value;
		unsigned long long				padding;
		struct
		{
			char						start:1;
			char						end:1;
			char						fu_norminette:6;
		}								f;
	}									flags;
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

extern t_link							g_link[];
extern t_room							g_room[];
extern int								g_room_index;
extern int								g_link_index;
extern t_capture						g_stack[];
extern t_capture						*g_stack_ptr;
extern char								*g_string_ptr;

#endif
