/* 
   Program name: emit.h
   Modified and created by Meagan Waldo
   May 6, 2021

   What is the purpose: The purpose of this code is to be a header file for emit.c.
 
   Algorithm used: No algorithm as this is a header file for emit.c. emit.c however uses and algorithm of calls to different functions within the file to build upon MIPS code to generate a MIPS version of the DECAF file being read in as input. Some of the calls use recursion to traverse the AST to find things such as strings and global variables.
 
   Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.
 
   Output: If an error occurs then an error message is printed out to the user. If there are not errors, then MIPS code is successfully printed out to a file.
 
   Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.
 
   Changes made to emit.h code:
   5/6/21: Since the last lab I have added this entire file. Everything in emit.h is new and used to create MIPS code from our AST tree.
 */

#include <stdio.h>
#include "ast.h"
#include "symtable.h"

#ifndef EMIT_H
#define EMIT_H
#define WS 4 // number of bytes that are in our word.

extern FILE *fp;

void emit_strings(ASTnode *p); /* Used to take all the strings found in the AST and print them out in MIPS code. */

char *GEN_LABEL(); /* Used to create a label. */

void emit_all(ASTnode *p); /* Sets up the start of the MIPS code and then calls emit_mips which gets the rest. */

void emit_global_vars(ASTnode *p); /* Used to take all global variables found in the AST and print them out in MIPS code. */

#endif // end of EMIT_H
