enum		e_state
{
	NT_BEGIN = 1,
	NT_LINK,
	NT_ROOM,
	NT_NAME,
	NT_END,
}

enum		e_state
{
	S_INITIAL = 1,
	S_LINE,
	S_BLANK,
	S_NAME,
	S_NBR,
	S_NAME_DASH,
	S_COMMENT,
	S_COMMAND,
	S_END,
}			t_state;

// enum		e_input
// {
// 	CHAR_DIGIT = '0',
//	CHAR_ALPHA = 'a',
// CHAR_BLANK = ' ',
// 	INPUT_END,
// }			t_input;

typedef s_replacer		character_class[255] = {
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
}						t_replacer;

	transition[replacer[*input]][cur_state]

t_state					defaults[S_END] = {
	[S_SHARP] = S_COMMENT,
	[S_COMMENT] = S_COMMENT,
	[S_COMMAND] = S_COMMAND,
};

t_state					transitions[255][S_END] = {
	[CHAR_DIGIT][S_NBR] = S_NBR,
	[CHAR_ALPHA][S_NAME] = S_NAME,
	[CHAR_DIGIT][S_NAME] = S_NAME,
	['#'][S_SHARP] = S_COMMAND,
};

t_state					transitions_stack[255][S_END][NT_END] = {
	[CHAR_DIGIT][S_INITIAL][NT_NAME] = S_NBR,
	['-'][S_INITIAL][NT_NAME] = S_NAME,
	[CHAR_ALPHA][S_INITIAL][NT_BEGIN] = S_NAME,
	[CHAR_ALPHA][S_INITIAL][NT_LINK] = S_NAME,
	[CHAR_DIGIT][S_INITIAL][NT_BEGIN] = S_NAME,
	[CHAR_DIGIT][S_INITIAL][NT_LINK] = S_NAME,
	['#'][S_INITIAL][NT_BEGIN] = S_SHARP,
};
