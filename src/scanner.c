/* File name: scanner.c
** Compiler: MS Visual Studio 2015
** Authors:
**			John Pilon,		040822687
**			Daniel Brenot,	040837035
** Course: CST8152-Compilers, Lab 012
** Date: September 16th, 2017
** Professor: Sv. Ranev
** Purpose: Definitions of all functions declared in token.h and table.h; All functions relate to the tasks of a scanner
** Function list:
char_class()
get_next_state()
isKeyword()
atolh()
scanner_init()
malar_next_token()
get_next_state()
aa_func02()
aa_func03()
aa_func05()
aa_func08()
aa_func10()
aa_func12()
aa_func13()
*/


/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
* to suppress the warnings about using "unsafe" functions like fopen()
* and standard sting library functions defined in string.h.
* The define does not have any effect in Borland compiler projects.
*/
#define _CRT_SECURE_NO_WARNINGS
#define DEBUG  /* for conditional processing */
#undef DEBUG

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
#include <math.h>	 /**/
#include <assert.h>	 /**/

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"



/* Global objects - variables */
/* This buffer is used as a repository for string literals.
It is defined in platy_st.c */
extern Buffer * str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

						 /* Local(file) global objects - variables */
static Buffer *lex_buf;/*pointer to temporary lexeme buffer*/

					   /* No other global variable declarations/definitiond are allowed */

					   /* scanner.c static(local) function  prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char, int *); /* state machine function */
static int iskeyword(char * kw_lexeme); /*keywords lookup functuion */
static long atolh(char * lexeme); /* converts hexadecimal string to decimal value */

								  /* Purpose: Initialises a scanner that searches a buffer
								  ** Author: John Pilon
								  ** Version: 1.0
								  ** Called functions: malloc(), calloc(), free()
								  ** Parameters: sc_buf: a valid pointer to an initialised buffer
								  ** Return Value: EXIT_SUCCESS if the scanner was initialised properly;EXIT_FAILURE otherwise
								  ** Algorithm: Resets the buffer and prepares it to be read by the scanner
								  */
int scanner_init(Buffer * sc_buf) {
	if (b_isempty(sc_buf)) return EXIT_FAILURE;/*1*/
											   /* in case the buffer has been read previously  */
	b_rewind(sc_buf);
	b_clear(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;/*0*/
}

/* Purpose: To retrieve the next token stored in the buffer
** Author: John Pilon, Daniel Brenot
** Version: 1.0
** Called functions: b_getc()
** Parameters: sc_buf : a valid pointer to an initialised buffer
** Return Value: A token with the propper error code and attribute value
** Algorithm: Gets the next token in the buffer
*/
Token malar_next_token(Buffer * sc_buf)
{
	Token t; /* token to return after recognition */
	unsigned char c; /* input symbol */
	int state = 0; /* initial state of the FSM */
	short lexstart;  /*start offset of a lexeme in the input char buffer (array) */
	short lexend;    /*end   offset of a lexeme in the input char buffer (array)*/
	int accept = NOAS; /* type of state - initially not accepting */



					   /*DECLARE YOUR LOCAL VARIABLES HERE IF NEEDED*/
	int lines = 0;
	char * lexeme;
	int i;

	if (sc_buf == NULL) {

		scerrnum = -1;

		return aa_table[ES]("RUN TIME ERROR: ");

	}

	while (1)
	{
		c = b_getc(sc_buf);
		switch (c)
		{
		case SEOF:
			t.code = SEOF_T;
			return t;
		case '\0':
			t.code = SEOF_T;
			return t;
		case ' ':
			continue;
		case '\t':
			continue;
		case '{':
			t.code = LBR_T;
			return t;
		case '}':
			t.code = RBR_T;
			return t;
		case '(':
			t.code = LPR_T;
			return t;
		case ')':
			t.code = RPR_T;
			return t;
		case ',':
			t.code = COM_T;
			return t;
		case ';':
			t.code = EOS_T;
			return t;
		case '+':
			t.code = ART_OP_T;
			t.attribute.arr_op = PLUS;
			return t;
		case '-':
			t.code = ART_OP_T;
			t.attribute.arr_op = MINUS;
			return t;
		case '*':
			t.code = ART_OP_T;
			t.attribute.arr_op = MULT;
			return t;
		case '/':
			t.code = ART_OP_T;
			t.attribute.arr_op = DIV;
			return t;
		case '>':
			t.code = REL_OP_T;
			t.attribute.rel_op = GT;
			return t;
		case '<':
			c = b_getc(sc_buf);
			if (c == '>') {
				t.code = LOG_OP_T;
				t.attribute.log_op = NE;
			}
			else {
				b_retract(sc_buf);
				t.code = REL_OP_T;
				t.attribute.rel_op = LT;
			}
			return t;
		case '=':
			c = b_getc(sc_buf);
			if (c == '=') {
				t.code = REL_OP_T;
				t.attribute.rel_op = EQ;
			}
			else {
				b_retract(sc_buf);
				t.code = ASS_OP_T;
			}
			return t;
		case '!':
			c = b_getc(sc_buf);
			if (c != '!') {
				t.code = ERR_T;
				t.attribute.err_lex[0] = '!';
				t.attribute.err_lex[1] = c;
				t.attribute.err_lex[2] = '\0';
				while (c = b_getc(sc_buf) != '\n') { /* Warning C4706 acknowledgement */
					continue;
				}
				
				return t;
			}
			while (c = b_getc(sc_buf) != '\n') { /* Warning C4706 acknowledgement */
				continue;
			}
			lines++;
			break;
			
		case '.':
			c = b_getc(sc_buf);
			if (c == 'A' && b_getc(sc_buf) == 'N' && b_getc(sc_buf) == 'D' && b_getc(sc_buf) == '.') {
				t.code = LOG_OP_T;
				t.attribute.log_op = AND;
			}
			else if (c == 'O' && b_getc(sc_buf) == 'R' && b_getc(sc_buf) == '.') {
				t.code = LOG_OP_T;
				t.attribute.log_op = OR;
			}
			else {
				b_retract(sc_buf);
				t.code = ERR_T;
				t.attribute.err_lex[0] = '.';
				t.attribute.err_lex[1] = '\0';
			}
			return t;
		case '#':
			t.code = SCC_OP_T;
			return t;
		case '\n':
			lines++;
			break;
		case '\"':
			lexstart = b_mark(sc_buf, b_getcoffset(sc_buf));
			b_mark(str_LTBL, b_getcoffset(str_LTBL));
			while ((c = b_getc(sc_buf)) != '\"' && !b_eob(sc_buf));
			lexend = b_getcoffset(sc_buf) - 1;
			if (b_eob(sc_buf)) {
				t.code = ERR_T;
				strncpy(t.attribute.err_lex, b_location(sc_buf, (short)lexstart), 17);
				for (i = 17; i < 20; i++) {
					t.attribute.err_lex[i] = '.';
				}
			}
			else {
				t.attribute.str_offset = b_limit(str_LTBL);
				b_reset(sc_buf);
				for (i = 0; i < lexend - lexstart; i++) {
					b_addc(str_LTBL, b_getc(sc_buf));
				}
				b_addc(str_LTBL, '\0');
				t.code = STR_T;
			}
			b_getc(sc_buf);
			return t;
		case '$':
			t.code = ERR_T;
			t.attribute.err_lex[0] = '$';
			t.attribute.err_lex[1] = '\0';
			return t;
		default:
			if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {

				b_retract(sc_buf);
				lexstart = b_mark(sc_buf, b_getcoffset(sc_buf));
				lexend = lexstart;
				state = 0;
				while (accept == NOAS) {
					state = get_next_state(state, b_getc(sc_buf), &accept);
					if (accept != NOAS) break;
				}
				if (accept == ASWR) b_retract(sc_buf);

				lexend = b_getcoffset(sc_buf);

				lex_buf = b_allocate(lexend - lexstart + 1, 10, 'a');
				b_reset(sc_buf);
				for (; lexstart < lexend; ++lexstart) {
					c = b_getc(sc_buf);
					if(c != '\n')	b_addc(lex_buf, c);
				}
				b_addc(lex_buf, '\0');
				if (aa_table[state] != NULL) {
					t = aa_table[state](b_location(lex_buf, 0));
					free(lex_buf);
				}

				else {
					scerrnum = -1;
					t = aa_table[ES]("RUN TIME ERROR: ");
					return t;
				}
				return t;
			}
			else {
				t.code = ERR_T;
				t.attribute.err_lex[0] = c;
				t.attribute.err_lex[1] = '\0';
				return t;
			}
		}

		/*CHECK OTHER CHARS HERE if NEEDED, SET A TOKEN AND RETURN IT.
		FOR ILLEGAL CHARACTERS SET ERROR TOKEN.
		THE ILLEGAL CHAR IS THE ATTRIBUTE OF THE ERROR TOKEN
		IN A CASE OF RUNTIME ERROR, THE FUNCTION MUST STORE
		A NON-NEGATIVE NUMBER INTO THE GLOBAL VARIABLE scerrnum
		AND RETURN AN ERROR TOKEN. THE ERROR TOKEN ATTRIBUTE MUST
		BE THE STRING "RUN TIME ERROR: "
		}*/

	}
}

/* Purpose: To find the next token state given the current state and the next character
** Author: Svillen Ranev
** Version: 1.0
** Called functions: printf(), exit()
** Parameters:
** Return Value:
** Algorithm:
*/
int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
	assert(next != IS);
#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}

/* Purpose:
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
int char_class(char c)
{
	
	if ((c >= 'a'&&c <= 'z') && c != 'x' || (c >= 'G' && c <= 'Z'))
	{return 0;}
	else if (c == '0')
	{return 1;}
	else if (c == 'x')
	{return 2;}
	else if (c >='A' && c <='F')
	{return 3;}
	else if (c >= '1'&&c <= '9')
	{return 4;}
	else if (c == '.')
	{return 5;}
	else if (c == '$')
	{return 6;}
	else
	{return 7;}
}

/* Purpose: Checks if the lexeme is a keyword and sets the token accordingly
** Author: John Pilon
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func02(char lexeme[])
{
	Token token;
	int i;

	if (iskeyword(lexeme)) {
		for (i = 0; i < KWT_SIZE; i++) {
			if (strcmp(lexeme, kw_table[i]) == 0) {
				token.attribute.kwt_idx = i;
				token.code = KW_T;
				break;
			}
		}
	}
	else {
		if (strlen(lexeme) > VID_LEN+1) {
			strncpy(token.attribute.vid_lex, lexeme, VID_LEN);
			token.attribute.vid_lex[VID_LEN ] = '\0';
		}
		else {
			strncpy(token.attribute.vid_lex, lexeme, strlen(lexeme));
			token.attribute.vid_lex[strlen(lexeme)] = '\0';
		}
		token.code = AVID_T;
	}

	return token; /* Warning C4701 acknowledgement: token will always be used */
}

/* Purpose: Sets the token as a string token and ensures it does not exceed max string size
** Author: John Pilon
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func03(char lexeme[]) {
	Token token;
	int i;

	token.code = SVID_T;

	if (strlen(lexeme) > VID_LEN) {
		strncpy(token.attribute.vid_lex, lexeme, VID_LEN - 1);
		token.attribute.vid_lex[VID_LEN - 1] = '$';
		token.attribute.vid_lex[VID_LEN] = '\0';
	}
	else {
		for (i = 0; i < strlen(lexeme); i++) {
			token.attribute.vid_lex[i] = lexeme[i];
		}
		token.attribute.vid_lex[strlen(lexeme)] = '\0';
	}

	return token;

}

/* Purpose:  Converts a decimal lexeme to a floating point value token
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func05(char lexeme[]) {
	Token t;
	char i, base;
	long dec = 0;
	for (i = (char)strlen(lexeme) - 1, base = 0; i >= 0; i--, base++)
	{
		dec += ((short)pow(10, base))*((short)(lexeme[i] - '0'));
		if (dec < PLATY_INT_MIN || dec > PLATY_INT_MAX)
		{
			memcpy(t.attribute.err_lex, lexeme, ERR_LEN - 3);
			t.attribute.err_lex[ERR_LEN - 3] = '.';
			t.attribute.err_lex[ERR_LEN - 2] = '.';
			t.attribute.err_lex[ERR_LEN - 1] = '.';
			t.attribute.err_lex[ERR_LEN] = '\0';
			t.code = ERR_T;
			return t;
		}
	}
	t.code = INL_T;
	t.attribute.int_value = dec;
	return t;
}

/* Purpose: Converts a decimal into an integer token
** Author: Svillen Ranev
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func08(char lexeme[])
{
	Token t;
	char decimal = (char)(strchr(lexeme, '.') - lexeme);
	char i, base;
	char pass = 0;
	float flt = 0;

	for (i = (char)strlen(lexeme) - 1, base = decimal - (i); i >= 0; i--)
	{
		if (lexeme[i] == '.') { continue; }
		else
		{
			flt += ((float)(pow(10, base)*(lexeme[i] - '0')));
			if (flt == INFINITY || flt == NAN || flt < PLATY_INT_MIN || flt > PLATY_INT_MAX)
			{
				memcpy(t.attribute.err_lex, lexeme, ERR_LEN - 3);
				t.attribute.err_lex[ERR_LEN - 3] = '.';
				t.attribute.err_lex[ERR_LEN - 2] = '.';
				t.attribute.err_lex[ERR_LEN - 1] = '.';
				t.attribute.err_lex[ERR_LEN] = '\0';
				t.code = ERR_T;
				return t;
			}
			base++;
		}
	}
	t.code = FPL_T;
	t.attribute.flt_value = flt;
	return t;
}

/* Purpose: Converts a hexidecimal integer literal lexeme to a integer token
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func10(char lexeme[])
{
	Token t;
	t.attribute.int_value = atolh(lexeme);
	t.code = INL_T;
	if (t.attribute.int_value < PLATY_INT_MIN || t.attribute.int_value > PLATY_INT_MAX)
	{
		memcpy(t.attribute.err_lex, lexeme, ERR_LEN - 3);
		t.attribute.err_lex[ERR_LEN - 3] = '.';
		t.attribute.err_lex[ERR_LEN - 2] = '.';
		t.attribute.err_lex[ERR_LEN - 1] = '.';
		t.attribute.err_lex[ERR_LEN] = '\0';
		t.code = ERR_T;
	}

	return t;
}

/* Purpose: Returns an error token with a truncated lexeme as the attribute
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func12(char lexeme[])
{
	return aa_table[ESWR](lexeme);
}

/* Purpose: Returns an error token with a truncated lexeme as the attribute; Retracts the buffer
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
Token aa_func13(char lexeme[])
{
	Token t;
	if (strlen(lexeme)>ERR_LEN)
	{
		memcpy(t.attribute.err_lex, lexeme, ERR_LEN - 3);
		t.attribute.err_lex[ERR_LEN - 3] = '.';
		t.attribute.err_lex[ERR_LEN - 2] = '.';
		t.attribute.err_lex[ERR_LEN - 1] = '.';
		t.attribute.err_lex[ERR_LEN] = '\0';
	}
	else
	{
		strncpy(t.attribute.err_lex, lexeme, strlen(lexeme));
		t.attribute.err_lex[strlen(lexeme)] = '\0';
	}
	t.code = ERR_T;
	return t;
}

/* Purpose:
** Author: John Pilon
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
long atolh(char * lexeme) {
	unsigned char i, base;  /* counters for base exponent and lexeme index */
	long hex = 0; /* integer conversion of hex value */
				  /* Determines integer value of ASCII represented hex value. A,B,C,D,E,F are defined in an enum in table.h */
	for (i = (char)strlen(lexeme)-1, base = 0; i >= 0 && hex >= 0; i--, base++) {
		/* conversion between ASCII chars and hex integer values. one-time literals are used to complete this calculation */
		hex += (short)pow(16, base)*(lexeme[i] <= '9' ? (short)(lexeme[i] - '0') : (short)(lexeme[i] - 'A' + 10));
	}
	return hex;
}

/* Purpose:
** Author: John Pilon
** Version: 1.0
** Called functions:
** Parameters:
** Return Value:
** Algorithm:
*/
int iskeyword(char * kw_lexeme) {
	int i;
	for (i = 0; i < KWT_SIZE; i++) {
		if (strcmp(kw_lexeme, kw_table[i]) == 0) return 1;
	}
	return 0;
}