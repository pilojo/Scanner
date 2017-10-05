/* Filename: table.h
* Transition Table and function declarations necessary for the scanner implementation
* as required for CST8152 - Assignment #2.
* Version: 1.17.2
* Date: 1 October 2017
* Provided by: Svillen Ranev
* The file is incomplete. You are to complete it.
***************************************************
* REPLACE THIS HEADER WITH YOUR HEADER
***************************************************
*/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

#define SEOF 255

#define PLATY_INT_MAX SHRT_MAX

/*  Single-lexeme tokens processed separately one by one
*  in the token-driven part of the scanner
*  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' ,
*       space
*  !!comment , ',' , '"' , ';' , '-' , '+' , '*' , '/', '#' ,
*  .AND., .OR. , SEOF, 'wrong symbol',
*/

#define ES  -255 /* Error state */
#define IS -1    /* Inavalid state */

/* State transition table definition */

#define TABLE_COLUMNS 7
/*transition table - type of states defined in separate table */
int  st_table[][TABLE_COLUMNS] = {
	/* State 0  */  { INITIALIZE },
	/* State 1  */  { INITIALIZE },
	/* State 2  */	{ INITIALIZE },
	/* State 3  */	{ INITIALIZE },
	/* State 4  */	{ INITIALIZE },
	/* State 5  */	{ INITIALIZE },
	/* State 6  */	{ INITIALIZE },
	/* State 7  */	{ INITIALIZE },
	/* State 8  */	{ INITIALIZE },
	/* State 9  */	{ INITIALIZE },
	/* State 10 */	{ INITIALIZE },
	/* State 11 */	{ INITIALIZE },
	/* State 12 */	{ INITIALIZE },
};

	/* Accepting state table definition */
#define ASWR     1  /* accepting state with retract */
#define ASNR     2  /* accepting state with no retract */
#define NOAS     0  /* not accepting state */

	int as_table[] = {
		/* State 0  */	
		/* State 1  */
		/* State 2  */
		/* State 3  */
		/* State 4  */
		/* State 5  */
		/* State 6  */
		/* State 7  */
		/* State 8  */
		/* State 9  */
		/* State 10 */
		/* State 11 */
		/* State 12 */
	};

/* Accepting action function declarations */

FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE.THEY ALL RETURN Token AND TAKE
ONE ARGUMENT : A string REPRESENTING A TOKEN LEXEME.

Token aa_func02(char *lexeme);
Token aa_func03(char *lexeme);

Replace XX with the number of the accepting state : 02, 03 and so on.

/* defining a new type: pointer to function (of one char * argument)
returning Token
*/

typedef Token(*PTR_AAF)(char *lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
* Token (*aa_table[])(char lexeme[]) = {
*/

PTR_AAF aa_table[] = {


	HERE YOU MUST PROVIDE AN INITIALIZATION FOR AN ARRAY OF POINTERS
	TO ACCEPTING FUNCTIONS.THE ARRAY HAS THE SAME SIZE AS as_table[].
	YOU MUST INITIALIZE THE ARRAY ELEMENTS WITH THE CORRESPONDING
	ACCEPTING FUNCTIONS(FOR THE STATES MARKED AS ACCEPTING IN as_table[]).
	THE REST OF THE ELEMENTS MUST BE SET TO NULL.

};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  10

char * kw_table[] =
{
	"ELSE",
	"FALSE",
	"IF",
	"PLATYPUS",
	"READ",
	"REPEAT",
	"THEN",
	"TRUE",
	"WHILE",
	"WRITE"
};

#endif
 