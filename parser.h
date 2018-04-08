# define CHAR_DIGIT						'0'
# define CHAR_ALPHA						'a'
# define CHAR_BLANK						' '
# define MAX_ANT						100000
# define MAX_ROOM						10000
# define MEMORY_SIZE					1000000
# define MAX_LINK						100000
# define MAX_LINK_NODE					100000
# define SUPERPOSITION_ERROR_MESSAGE	("Error : different rooms "\
	"should not be at the exact same coordinates : ")
# define BAD_START_END_MESSAGE			("Error : both start and end "\
	"rooms must exist and be differeeeeeeeeeeeent.\n")
# define UNDEFINED_ROOM_MESSAGE			("Error : Undefined room\n")
# define SYNTAX_ERROR					("Error : Syntax error\n")
# define DISC_START_END_ERROR_MESSAGE	("Error : Start room and End room"\
	"must be connected together. ")

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

char									*nt_string[NT_END] = {
	[NT_BEGIN] = "NT_BEGIN",
	[NT_ANTNBR] = "NT_ANTNBR",
	[NT_DASH] = "NT_DASH",
	[NT_LINK] = "NT_LINK",
	[NT_COMMAND] = "NT_COMMAND",
	[NT_COMMAND_BEGIN] = "NT_COMMAND_BEGIN",
	[NT_DEFLINK_INCOMPLETE] = "NT_DEFLINK_INCOMPLETE",
	[NT_DEFROOM_INCOMPLETE] = "NT_DEFROOM_INCOMPLETE",
	[NT_ANTNBR_INCOMPLETE] = "NT_ANTNBR_INCOMPLETE",
	[NT_COMMAND_INCOMPLETE] = "NT_COMMAND_INCOMPLETE",
	[NT_DEFLINK] = "NT_DEFLINK",
	[NT_DEFROOM] = "NT_DEFROOM",
	[NT_LINEFEED] = "NT_LINEFEED",
	[NT_ROOM] = "NT_ROOM",
	[NT_NAME] = "NT_NAME",
	[NT_NBR] = "NT_NBR",
};

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

char									*state_string[S_END] = {
	[S_INITIAL] = "S_INITIAL",
	[S_LINE] = "S_LINE",
	[S_BLANK] = "S_BLANK",
	[S_NAME] = "S_NAME",
	[S_NBR] = "S_NBR",
	[S_DASH] = "S_DASH",
	[S_LINEFEED] = "S_LINEFEED",
	[S_NAME_DASH] = "S_NAME_DASH",
	[S_SHARP] = "S_SHARP",
	[S_COMMENT] = "S_COMMENT",
	[S_COMMAND] = "S_COMMAND",
};

char									character_class[255] = {
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
	['L'] = CHAR_ALPHA,
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

char									*g_commands[] = {
	"start",
	"end",
	0,
};

int										forbidden_combination[NT_END][NT_END] =
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

int										g_is_data[NT_END] = {
	[NT_NBR] = 1,
	[NT_NAME] = 1,
};

t_nt									nt_combination[NT_END][NT_END] = {
	[NT_BEGIN][NT_NAME] = NT_NAME,
	[NT_BEGIN][NT_NBR] = NT_ANTNBR_INCOMPLETE,
	[NT_COMMAND_BEGIN][NT_NAME] = NT_COMMAND_INCOMPLETE,
	[NT_DEFROOM_INCOMPLETE][NT_LINEFEED] = NT_DEFROOM,
	[NT_DEFLINK_INCOMPLETE][NT_LINEFEED] = NT_DEFLINK,
	[NT_ANTNBR_INCOMPLETE][NT_LINEFEED] = NT_ANTNBR,
	[NT_COMMAND_INCOMPLETE][NT_LINEFEED] = NT_COMMAND,
	[NT_NAME][NT_DASH] = NT_LINK,
};

t_nt									nt_to_keep[S_END] = {
	[S_NBR] = NT_NBR,
	[S_NAME] = NT_NAME,
	[S_DASH] = NT_DASH,
	[S_LINEFEED] = NT_LINEFEED,
	[S_COMMAND] = NT_COMMAND_BEGIN,
};

t_state									defaults[S_END] = {
	[S_SHARP] = S_COMMENT,
	[S_COMMENT] = S_COMMENT,
};

t_state									transitions[255][S_END] = {
	[CHAR_DIGIT][S_NBR] = S_NBR,
	[CHAR_ALPHA][S_NAME] = S_NAME,
	[CHAR_DIGIT][S_NAME] = S_NAME,
	[CHAR_BLANK][S_INITIAL] = S_INITIAL,
	['#'][S_SHARP] = S_COMMAND,
	['\n'][S_INITIAL] = S_LINEFEED,
	['\n'][S_COMMENT] = S_INITIAL,
	[0][S_INITIAL] = S_INITIAL,
};

t_state									transitions_stack[255][S_END][NT_END] =
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

char									g_string[MEMORY_SIZE];
t_room									g_room[MAX_ROOM];
t_ant									g_ant[MAX_ANT];
t_link									g_link[MAX_LINK];
t_link_node								g_link_node[MAX_LINK_NODE];

char									*g_string_origin = g_string;
char									*g_string_ptr = g_string;
int										g_room_index;
int										g_ant_index;
int										g_link_index;
int										g_link_node_index;
