/* File name: platy_scantest.c
*  Purpose:This is the test program for Assignment #2 - Scanner
*  CST8152 - Compilers
*  Version: 1.0
*  Author: Daniel Brenot
*  Date: 18 October 2017
*/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
* to suppress the warnings about using "unsafe" functions like fopen()
* and standard sting library functions defined in string.h.
* The define does not have any effect in other compiler projects.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "buffer.h"
#include "token.h"

/*check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/* Global objects - variables */
int scerrnum;     /* run-time error number = 0 by default (ANSI) */

				  /*external objects */
extern int line; /* source code line numbers - defined in scanner.c */
extern int scanner_init(Buffer * sc_buf);
extern Token aa_func02(char []);
extern Token aa_func03(char[]);
extern Token aa_func05(char[]);
scanner_init(Buffer * sc_buf);
extern Token malar_next_token(Buffer * sc_buf);

/*  main function takes a PLATYPUS source file as
*  an argument at the command line.
*  usage: scanner source_file_name"
*/
int main(int argc, char ** argv) {
	char c;
	int dec=0;
	printf("aa_func02()\n");
	printf("Keyword ELSE:\t\t%s\n",(aa_func02("ELSE").code == KW_T ? "Pass":"Fail"));
	printf("keyword FALSE:\t\t%s\n", (aa_func02("FALSE").code == KW_T ? "Pass" : "Fail"));
	printf("keyword IF:\t\t%s\n", (aa_func02("IF").code == KW_T ? "Pass" : "Fail"));
	printf("keyword PLATYPUS:\t%s\n", (aa_func02("PLATYPUS").code == KW_T ? "Pass" : "Fail"));
	printf("keyword READ:\t\t%s\n", (aa_func02("READ").code == KW_T ? "Pass" : "Fail"));
	printf("keyword REPEAT:\t\t%s\n", (aa_func02("REPEAT").code == KW_T ? "Pass" : "Fail"));
	printf("keyword THEN:\t\t%s\n", (aa_func02("THEN").code == KW_T ? "Pass" : "Fail"));
	printf("keyword TRUE:\t\t%s\n", (aa_func02("TRUE").code == KW_T ? "Pass" : "Fail"));
	printf("keyword WHILE:\t\t%s\n", (aa_func02("WHILE").code == KW_T ? "Pass" : "Fail"));
	printf("keyword WRITE:\t\t%s\n", (aa_func02("WRITE").code == KW_T ? "Pass" : "Fail"));
	printf("large input:\t\t%s\n", (aa_func02("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa").code == KW_T ? "FAIL" : "Pass"));
	printf("non keyword:\t\t%s\n", (aa_func02("aaa").code == KW_T ? "Fail" : "Pass"));
	printf("\n\naa_func03()\n");

	printf("String normal length:\t\t%s\n", (aa_func02("aaa$").attribute.vid_lex));
	printf("String normal length:\t\t%s\n", (aa_func02("aaaaaaaa$").attribute.vid_lex));
	
	printf("\n\naa_func05()\n");
	printf("Int test:\t\t%d\n",aa_func05("99"));
	printf("long test:\t\t%d\n", aa_func05("99999999999999999999999999999999999999999999"));
	scanf("%c", &c);
}