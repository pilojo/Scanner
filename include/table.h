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

#ifndef TOKEN_H_
#include "token.h"
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

#define ES  12 /* Error state */
#define IS -1    /* Inavalid state */

/* State transition table definition */

#define TABLE_COLUMNS 7
/*transition table - type of states defined in separate table */
int  st_table[][TABLE_COLUMNS] = {
	/* State 0  */  { 1, 6, 4, 4, ES, ES, ES },
	/* State 1  */  { 1, 1, 1, 1, 2, 3, 2 },
	/* State 2  */	{ IS, IS, IS, IS, IS, IS, IS},
	/* State 3  */	{ IS, IS, IS, IS, IS, IS, IS },
	/* State 4  */	{ 5, 4, 4, 4, 7, 5, 5 },
	/* State 5  */	{ IS, IS, IS, IS, IS, IS, IS },
	/* State 6  */	{ ES, 9, 9, ES, 7, ES, 5 },
	/* State 7  */	{ 8, 7, 7, 7, 8, 8, 8 },
	/* State 8  */	{ IS, IS, IS, IS, IS, IS, IS },
	/* State 9  */	{ ES, ES, 11, ES, ES, ES, 10},
	/* State 10 */	{ IS, IS, IS, IS, IS, IS, IS },
	/* State 11 */	{ ES, 11, 11, ES ,ES, ES, 10 },
	/* State 12 */	{ IS, IS, IS, IS, IS, IS, IS },
	/* State 13 */	{ ES, 9, 9, ES, 7, ES, 5 }
};

	/* Accepting state table definition */
#define ASWR     1  /* accepting state with retract */
#define ASNR     2  /* accepting state with no retract */
#define NOAS     0  /* not accepting state */

	int as_table[] = {
		/* State 0  */	NOAS,
		/* State 1  */	NOAS,
		/* State 2  */	ASWR,
		/* State 3  */	ASNR,
		/* State 4  */	NOAS,
		/* State 5  */	ASWR,
		/* State 6  */	NOAS,
		/* State 7  */	NOAS,
		/* State 8  */	ASWR,
		/* State 9  */	NOAS,
		/* State 10 */	ASWR,	
		/* State 11 */	NOAS,
		/* State 12 */	ASNR,
		/* State 13 */	ASWR
	};

/* Accepting action function declarations */

Token aa_func02(char *lexeme);
Token aa_func03(char *lexeme);
Token aa_func05(char *lexeme);
Token aa_func08(char *lexeme);
Token aa_func10(char *lexeme);
Token aa_func12(char *lexeme);
Token aa_func13(char *lexeme);

/* defining a new type: pointer to function (of one char * argument)
returning Token
*/

typedef Token(*PTR_AAF)(char *lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
* Token (*aa_table[])(char lexeme[]) = {
*/

PTR_AAF aa_table[] = {
	
	NULL,
	aa_func02,
	aa_func03,
	NULL,
	aa_func05,
	NULL,
	NULL,
	aa_func08,
	NULL,
	aa_func10,
	NULL,
	aa_func12,
	aa_func13

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
 