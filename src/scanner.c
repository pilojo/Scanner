/* Filename: scanner.c
/* PURPOSE:
*    SCANNER.C: Functions implementing a Lexical Analyzer (Scanner)
*    as required for CST8152, Assignment #2
*    scanner_init() must be called before using the scanner.
*    The file is incomplete;
*    Provided by: Svillen Ranev
*    Version: 1.17.2
*    Date: 1 October 2017
*******************************************************************
*    REPLACE THIS HEADER WITH YOUR HEADER
*******************************************************************
*/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
* to suppress the warnings about using "unsafe" functions like fopen()
* and standard sting library functions defined in string.h.
* The define does not have any effect in Borland compiler projects.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
#include <math.h>

/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

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

/*Initializes scanner */
int scanner_init(Buffer * sc_buf) {
	if (b_isempty(sc_buf)) return EXIT_FAILURE;/*1*/
											   /* in case the buffer has been read previously  */
	b_rewind(sc_buf);
	b_clear(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;/*0*/
	/*   scerrnum = 0;  *//*no need - global ANSI C */
}

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

	while (1)
	{ 
		c = b_getc(sc_buf);
		switch (c)
		{
			case ' ':
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
				t.code = REL_OP_T;
				t.attribute.rel_op = LT;
				return t;
			case '=':
				c = b_getc(sc_buf);
				if (c == '=') {
					t.code = REL_OP_T;
					t.attribute.rel_op = EQ;
				}
				else if (c == ' ') {
					t.code = ASS_OP_T;
				}
				else {
					t.code = ERR_T;
					strcpy(t.attribute.err_lex, '=' + c);
				}
				return t;
			case '!':
				c = b_getc(sc_buf);
				if (c == '=') {
					t.code = REL_OP_T;
					t.attribute.rel_op = NE;
				}
				else if (c == '!') {
					while (c = b_getc(sc_buf) != '\n') {
						continue;
					}
					lines++;
				}
				else {
					t.code = ERR_T;
					strcpy(t.attribute.err_lex, '!' + c);
				}
				return t;
			case '.':
				t.code = ERR_T;

				c = b_getc(sc_buf);
				if (c == 'A') {
					c = b_getc(sc_buf);
					if (c == 'N') {
						c = b_getc(sc_buf);
						if (c == 'D') {
							c = b_getc(sc_buf);
							if (c == '.') {
								t.code = LOG_OP_T;
								t.attribute.log_op = AND;
							}
							else t.attribute.err_lex[0] = c;
						}
						else t.attribute.err_lex[0] = c;
					}
					else t.attribute.err_lex[0] = c;
				}
				else if (c == 'O') {
					c = b_getc(sc_buf);
					if (c == 'R') {
						c = b_getc(sc_buf);
						if (c == '.') {
							t.code = LOG_OP_T;
							t.attribute.log_op = OR;
						}
						else t.attribute.err_lex[0] = c;
					}
					else t.attribute.err_lex[0] = c;
				}
				else t.attribute.err_lex[0] = c;
				return t;
			case '#':
				t.code = SCC_OP_T;
				return t;
			case '\n':
				lines++;
				break;
		}
		

			
	}//while
		
}//malar_next_token

/*
* author:
* version: 
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
}//get_next_state

/*
** author: Daniel Brenot
** version: 1.0
*/
int char_class(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A' && c <= 'Z'))
	{return 0;}
	else if (c=='0')
	{return 1;}
	else if (c>='1'&&c<='9')
	{return 2;}
	else if (c=='.')
	{return 3;}
	else if (c=='$')
	{return 4;}
	else
	{return 5;}
}//char_class

/*
** author: John Pilon
** version: 1.0
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
		if (strlen(lexeme) > VID_LEN) {
			char temp[VID_LEN];
			strcpy(temp, lexeme);
			strcpy(token.attribute.vid_lex, temp);
			token.attribute.vid_lex[VID_LEN+1] = '\0';
		}
		else {
			strcpy(token.attribute.vid_lex, lexeme);
			token.attribute.vid_lex[strlen(lexeme)] = '\0';
		}
		token.code = AVID_T;
	}

	return token;
}//aa_func02

/* author: john pilon
** version: 1.0
*/
Token aa_func03(char lexeme[]) {
	Token token;

	token.code = SVID_T;

	if (strlen(lexeme) > VID_LEN-1) {
		char temp[VID_LEN - 1];
		strcpy(temp, lexeme);
		strcpy(token.attribute.vid_lex, temp);
		token.attribute.vid_lex[VID_LEN] = '$';
		token.attribute.vid_lex[VID_LEN+1] = '\0';
	}
	else {
		strcpy(token.attribute.vid_lex, lexeme);
		token.attribute.vid_lex[strlen(lexeme)] = '\0';
	}

	return token;

}//aa_func03

/*
* author:
* version:
*/
Token aa_func08(char lexeme[]) {
	Token t;
	/*THE FUNCTION MU CONVERT THE LEXEME TO A FLOATING POINT VALUE,
		WHICH IS THE ATTRIBUTE FOR THE TOKEN.
		THE VALUE MUST BE IN THE SAME RANGE AS the value of 4 - byte float in C.
		IN CASE OF ERROR(OUT OF RANGE) THE FUNCTION MUST RETURN ERROR TOKEN
		THE ERROR TOKEN ATTRIBUTE IS  lexeme.IF THE ERROR lexeme IS LONGER
		than ERR_LEN characters, ONLY THE FIRST ERR_LEN - 3 characters ARE
		STORED IN err_lex.THEN THREE DOTS ... ARE ADDED TO THE END OF THE
		err_lex C - type string.
		BEFORE RETURNING THE FUNCTION MUST SET THE APROPRIATE TOKEN CODE
	return t;*/
}//aa_func08

/*
* author: Daniel Brenot
*
* converts a base 10 string to a token storing an integer;TODO hat about negatives?
*/
Token aa_func05(char lexeme[])
{
	Token t;
	short i;
	short size = sizeof(lexeme);
	for (i=0; i<size; i++)
	{
		t.attribute.int_value += (lexeme[i]-'0') *pow(10,size-i);
		if (t.attribute.int_value < 0) 
		{ 
			if (sizeof(lexeme) > ERR_LEN)
			{
				strcpy(t.attribute.err_lex, lexeme, ERR_LEN - 3);
				t.attribute.err_lex[ERR_LEN - 3] = '.';
				t.attribute.err_lex[ERR_LEN - 2] = '.';
				t.attribute.err_lex[ERR_LEN - 1] = '.';
				t.attribute.err_lex[ERR_LEN] = '\0';
				t.code = ERR_T;
			}
			else
			{
				strcpy(t.attribute.err_lex, lexeme);
				t.attribute.err_lex[size] = '\0';
				t.code = INL_T;
			}
			break; 
		}
	}
	return t;
}//aa_func05

/*
* author:
* version:
* TODO
*/
Token aa_func10(char lexeme[])
{
	Token t;

	return t;
}//aa_func10

/*
* author:
* version:
* TODO
*/
Token aa_func12(char lexeme[])
{
	Token t;
	return t;
}//aa_func12

/*
* author:
* version:
* TODO
*/
Token aa_func13(char lexeme[])
{
	Token t;
	return t;
}//aa_func13

/* author: john pilon
* version: 1.1
*/
long atolh(char * lexeme) {
	unsigned char i, base;  /* counters for base exponent and lexeme index */
	long hex = 0; /* integer conversion of hex value */
	/* Determines integer value of ASCII represented hex value. A,B,C,D,E,F are defined in an enum in table.h */
	for (i = (char)strlen(lexeme), base = 0; i > 1 && hex >= 0; i--, base++) {
		/* conversion between ASCII chars and hex integer values. one-time literals are used to complete this calculation */
		hex += (short)pow(16, base)*(lexeme[i] <= '9' ? (short)(lexeme[i]-'0') : (short)(lexeme[i]-'A'+10));
	}
	return hex;
}//atolh

/* author: john pilon
* version: 1.0
*/
int iskeyword(char * kw_lexeme) {
	int i;
	for (i = 0; i < KWT_SIZE; i++) {
		if (strcmp(kw_lexeme, kw_table[i]) == 0) return 1;
	}
	return 0;
}//isKeyword