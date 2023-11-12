/*  Symbol Table --linked list
    Used for Compilers class

    Modified Spring 2015 to allow for name to pointed to by symtable, instead of copied, since the name is copied
    into the heap

    Modified to have levels.  A level 0 means global variable, other levels means in range of the function.  We
    start out our offsets at 0 (from stack pointer) for level 1,,,when we enter a functional declaration.
    We increment offset each time we insert a new variable.  A variable is considered to be valid if it is found in
    the symbol table at our level or lesser level.  If at 0, then it is global.  

    We return a pointer to the symbol table when a variable matches our creteria.

    We add a routine to remove variables at our level and above.


    Program name: symtable.c       
    Modified by Meagan Waldo 
    May 6, 2021 

    What is the purpose: The purpose of this code is to contain methods that can be used to successfully print out a symbol table of given input. 

    Algorithm used:  Traversal of the symbol table. All the methods in this program are used to modify or display the symbol table using the pointer. 

    Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 
    Output: If an error occurs then an error message is printed out to the user with the line number the error is found on. If everything works then a symbol table will be printed out if requested at the end of the lab9.y file. 

    Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 

    Changes made to symtable.c code: 
    4/9/21: I added the method TEMP_CREATE. I added two new cases for method and array in the switch statement for SubType. I added three new cases for boolean, string and void in the switch statement Type. I added mysize to the printout of the symbol table (header and data part). I added the variable GTEMP.
    5/6/21: No changes made.
*/
    
#include <string.h>
#include "ast.h"
#include "symtable.h"

int GTEMP = 0; // One up number for creating temps.

/* 
 * Creates a TEMP for every time we have a calculation.
 * Parameters: None.
 * Returns: It returns strdup(s).
 * Preconditions: None.
 * Postconditions: A TEMP is created.
 */
char *TEMP_CREATE() {
	char s[100];
	sprintf(s, "_T%d", GTEMP);
	GTEMP++;
	return(strdup(s));
} // end of TEMP_CREATE

struct SymbTab * first=NULL; /* SymbTab pointer called first. */

/*
 * Simple Insert into the symbol table with the size, type and level.
 * Parameters: A char pointer called name, enum AST_Decaf_Types called Type, enum ID_Sub_Type called subtype, ASTnode pointer called fparms and three int variables called level, offset and mysize.
 * Returns: NULL or a SymbTab pointer called p.
 * Preconditions: If the symbol is already in the table it can not be added.
 * Postconditions: The symbol is added to the table.
 */
struct SymbTab * Insert(char *name, enum AST_Decaf_Types Type, enum ID_Sub_Type subtype , int  level, int mysize, int offset , ASTnode * fparms) {
   
    struct SymbTab * n;
   
    n=Search(name,level, 0);
   
    /* This checks to see if the symbol has already been inserted into the table. If it already inserted a statement is printed alerting the user. If it is not then it is added to the table. */
    if(n!=NULL) {
       printf("\n\tThe name %s exists at level %d already in the symbol table\n\tDuplicate can't be inserted",name, level);
       return (NULL);
    } // end of if
    else {
      struct SymbTab *p;
      p=malloc(sizeof(struct SymbTab));
      p->name=name;
      p->offset=offset;  /* assign the offset */
      p->level=level;  /* assign the level */
      p->mysize=mysize;  /* assign the size */
      p->Type=Type;  /* assign the Type */
      p->SubType=subtype;  /* assign the Function  */
      p->fparms=fparms;  /* assign the Method parameter list  */
      p->next=NULL;

      /* Check on how many elements we have in the symbol table. */
      if(first==NULL) {
         first=p;
      } // end of if
      else {
         p->next=first;
         first=p;
      } // end of else
      
      return (p);
    } // end of else
     
    printf("\n\tLabel inserted\n");
} // end of Insert

/*
 * Prints out a single symbol table entry for debugging.
 * Parameters: A SymbTab pointer called s.
 * Returns: None.
 * Preconditions: None.
 * Postconditions: A formatted print out with two of the cases being picked based on the SubType of s and Type of s.
 */
void PrintSym(struct SymbTab *s) {
	
	char *string,*display_type; 
        
	/* Switch statement to pick the correct SubType for the print. */
	switch(s->SubType) {
	       case ID_Sub_Type_Scalar: string = "Scalar " ; break;
       	       case ID_Sub_Type_Method: string = "Method " ; break;
               case ID_Sub_Type_Array: string = "Array" ; break;					  
	       default:  string ="Unknown subtype in print symbol";
	 } // end of switch statement for SubType

	 /* Switch statement to pick the correct Type for the print. */
	 switch(s->Type) {
	        case A_Decaf_INT: display_type = "INT" ; break;
	        case A_Decaf_BOOL: display_type = "BOOLEAN" ; break;
	        case A_Decaf_STRING: display_type = "STRING" ; break;
                case A_Decaf_VOID: display_type = "VOID" ; break;
		default:  string ="Unknown Declared TYPE in print symbol";
	 } // end of switch statement for Type

         printf("\t%s\t\t%d\t%d\t%d\t%s\t%s\n",s->name,s->offset,s->mysize,s->level,display_type,string);
} // end of PrintSym

/*
 * Prints out the symbol table.
 * Parameters: None.
 * Returns: None.
 * Preconditions: None.
 * Postconditions: The symbol table is printed.
 */
void Display() {
   int i;
   struct SymbTab *p;
   p=first;
   printf("\n\tLABEL\t\tOffset\tSIZE\tLEVEL\ttype\tsubtype\n");
   
   /* If the pointer is not NULL print out the information and point to the next node. */
   while (p!=NULL) {
          PrintSym(p);
          p=p->next;
   } // end of while
} // end of Display

/*
 * Checks if the symbol is found in the table.
 * Parameters: A char array called name and two int variables called level and recur.
 * Returns: A SymbTab pointer called p or NULL.
 * Preconditions: None.
 * Postconditions: Pointer returned if symbol is found. If symbol is not found then NULL is returned.
 */
struct SymbTab * Search(char name[], int level, int recur) {

   int i,flag=0;
   struct SymbTab *p;

   /* For each level, try to find the symbol. */
   while (level >= 0) {
          p=first;
      
          /* While p is not null check for the symbol. */
          while (p!=NULL) {
         
	         /* If the symbol is found. */
	         if((strcmp(p->name,name)==0) && (p->level == level))
                    return p;
              
	      	 p=p->next;
          } // end of while
          
	  /* If the symbol is not found at the level. */
	  if (recur == 0) 
	      return (NULL);
          
	  
	  level--; /* Check the next level up. */
    } // end of while
    
    return  NULL;  /* Did not find it, return 0. */
} // end of Search


/*
 * Remove all entries that have the indicated level.
 * Parameters: A int variable called level.
 * Returns: A int called SIZE.
 * Preconditions: None.
 * Postconditions: All entries that have the indicated level are removed.
 */
int Delete(int level) {
    
    struct SymbTab *p,*f=NULL;  /* We follow with pointer f. */
    int SIZE=0;
    p=first;

    /* While not null cruise through the list. */
    while (p != NULL) {

        /* Check if there is a match. */
        if (p->level>= level ) { 
          
           SIZE+=p->mysize;
           
	   /* If it is the first in the list we have to update first, we know this by f being NULL. If not NULL then it is not the first element. */
	   if ( f==NULL) first=p->next;
           else{
	       f->next=p->next;
           } // end of else
           
	   p=p->next;
           
        } // end of if
        else {
          
	   /* Update follow pointer, move the p pointer. */
           f=p;
           p=p->next;
        } // end of else
    } // end of while
    
    return(SIZE);
} // end of Delete
