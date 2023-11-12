/*   Abstract syntax tree code

 Header file   
 Shaun Cooper January 2020

 Program name: ast.h 
 Modified by Meagan Waldo        
 May 6, 2021
 
 What is the purpose: The purpose of this code is to be a header file for ast.c. 

 Algorithm used: This is a header file for ast.c which does use as algorithm of recursively making calls to the function ASTPrint to print out the entire abstract syntax tree. 

 Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 

 Output: If an error occurs then an error message is printed out to the user with the line number the error is found on. If everything works then a AST will print out at the end of the lab9.y file if requested. 
			   
 Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 
			       
 Changes made to ast.h code: 
 3/30/21: I added A_PROGRAM, A_ARRAYTYPE, A_EXTERN, A_METHODIDENTIFIER, A_METHODCALL, A_METHODPARMS, A_NUMBER, A_LVALUE, A_ASSIGN, A_IF, A_IFBODY, A_WHILESTMT, A_CONSTANT_INT, A_CONSTANT_BOOL, A_CONSTANT_STRING, A_BREAK, A_CONTINUE, A_RETURN, A_VAR_RVALUE and A_ExternType to AST_Tree_Element_Type. I Added A_UMINUS, A_DIVIDE, A_MOD, A_AND, A_OR, A_LEFTSHIFT, A_RIGHTSHIFT, A_LEQ, A_LT, A_GT, A_GE, A_EQ and A_NEQ to AST_Operators. I added A_Decaf_STRING to AST_Decaf_Types. I added enum AST_Decaf_Types A_Declared_Type to ASTnodetype.
 4/9/21: I added an int variable called size and a SymTab struct called symbol into the ASTnodetype struct. I included symtable.h. I added a method prototype for the method check_parameters. I removed any trace of A_PARAM due to it being dead code.  
 5/6/21: I changed mydebug from static to extern. I also added a char pointer called label into the struct ASTnodetype.
*/

#include<stdio.h>
#include<malloc.h>

#ifndef AST_H
#define AST_H
extern int mydebug;

/* Define the enumerated types for the AST. This is used to tell us what sort of production rule we came across. */
enum AST_Tree_Element_Type {
   A_PROGRAM,
   A_PACKAGE,
   A_ARRAYTYPE,
   A_EXTERN,
   A_METHODDEC,
   A_METHODIDENTIFIER,
   A_METHODCALL,
   A_METHODPARMS,
   A_NUMBER,
   A_LVALUE,
   A_ASSIGN,
   A_IF,
   A_IFBODY,
   A_WHILESTMT,
   A_BLOCK,
   A_EXPR,
   A_VARDEC,
   A_CONSTANT_INT,
   A_CONSTANT_BOOL,
   A_CONSTANT_STRING,
   A_BREAK,
   A_CONTINUE,
   A_RETURN,
   A_VAR_RVALUE,
   A_ExternType
};

/* Define the enumerated operators for the AST. This is used to tell us what sort of production rule we came across. */
enum AST_Operators {
   A_PLUS,
   A_MINUS,
   A_UMINUS,
   A_TIMES,
   A_DIVIDE,
   A_MOD,
   A_AND,
   A_OR,
   A_LEFTSHIFT,
   A_RIGHTSHIFT,
   A_LEQ,
   A_LT,
   A_GT,
   A_GE,
   A_EQ,
   A_NEQ,
   A_NOT
};

/* Define the enumerated Decaf types for the AST. This is used to tell us what sort of decaf type we came across. */
enum AST_Decaf_Types {
   A_Decaf_INT,
   A_Decaf_STRING,
   A_Decaf_BOOL,
   A_Decaf_VOID
};

/* Define a type AST node which will hold pointers to AST structs that will allow us to represent the parsed code */
typedef struct ASTnodetype {
     enum AST_Tree_Element_Type type;
     enum AST_Operators operator;
     char * name;
     char * label; // Used for string labels.
     int value;
     int size; // Added new variable.
     struct SymbTab *symbol; // Added new struct.
     enum AST_Decaf_Types A_Declared_Type;
     struct ASTnodetype *S1,*S2, *next ;
} ASTnode;

#include "symtable.h"

/* Uses malloc to create an ASTnode and passes back the heap address of the newly created node. */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype);

/* Prints howmany number of spaces. */
void PT(int howmany);

/* Global Pointer for connection between YACC and backend. */
ASTnode *program;

/* Prints out the abstract syntax tree with level horizontal spacing. */
void ASTprint(int level,ASTnode *p);

/* Used to check the length and type of the formal and actual parameters. */
int check_parameters(ASTnode *formal, ASTnode *actual);

#endif // of AST_H
