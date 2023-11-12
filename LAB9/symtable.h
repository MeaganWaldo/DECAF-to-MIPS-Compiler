/*  Symbol Table --linked list  headers
    Used for Compilers class

    Modified Spring 2015 to allow for name to pointed to by symtable, instead of copied, since the name is copied
    into the heap

    Modified to have levels.  A level 0 means global variable, other levels means in range of the function.  We
    start out our offsets at 0 (from stack pointer) for level 1,,,when we enter a functional declaration.
    We increment offset each time we insert a new variable.  A variable is considered to be valid if it is found in
    the symbol table at our level or lesser level.  If at 0, then it is global.  

    We return a pointer to the symbol table when a variable matches our creteria.

    We add a routine to remove variables at our level and above.

  
    Program name: symtable.h 
    Modified by Meagan Waldo 
    May 6, 2021 
	      
    What is the purpose: The purpose of this code is to be a header file for symtable.c. 
		  
    Algorithm used: This is a header file for symtable.c which does use an algorithm of traversal of the symbol table. All the methods in this program are used to modify or display the symbol table using the pointer. 

    Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.  
    Output: If an error occurs then an error message is printed out to the user with the line number the error is found on. If everything works then a symbol table will be printed out if requested at the end of the lab9.y file. 

    Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 

    Changes made to symtable.h code: 
    4/9/21: I added ID_Sub_Type_Method and ID_Sub_Type_Array in ID_Sub_Type. I added the prototype for the method TEMP_CREATE.
    5/6/21: No changes made.  
*/

#include "ast.h"

#ifndef _SYMTAB 
#define _SYMTAB

/* Prints out the symbol table. */
void Display();

/* Remove all entries that have the indicated level. */
int Delete(int level);

/* Fetches the address. */
int FetchAddr (char *lab);

/* The types an ID can have. */
enum ID_Sub_Type {
	ID_Sub_Type_Scalar,
	ID_Sub_Type_Method,
        ID_Sub_Type_Array	
};

/* Defines the SymbTab struct information. */
struct SymbTab
{
     char *name;
     int offset; /* from activation record boundary */
     int mysize;  /* number of words this item is 1 or more */
     int level;  /* the level where we found the variable */
     enum AST_Decaf_Types Type;  /* the type of the symbol */
     enum ID_Sub_Type  SubType;  /* the element is a function */
     ASTnode * fparms; /* pointer to parameters of the function in the AST */
     struct SymbTab *next;
};

/* Simple Insert into the symbol table with the size, type and level. */
struct SymbTab * Insert(char *name, enum AST_Decaf_Types Type, enum ID_Sub_Type subtype, int  level, int mysize, int offset, ASTnode * fparms );

/* Checks if the symbol is found in the table. */
struct SymbTab * Search(char name[], int level, int recur);

/* Creates a TEMP for everytime we have a calculation. */
char *TEMP_CREATE();

#endif
