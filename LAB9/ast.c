/*   Abstract syntax tree code

    This code is used to define an AST node, 
    routine for printing out the AST
    defining an enumerated type so we can figure out what we need to
    do with this.  The ENUM is basically going to be every non-terminal
    and terminal in our language.

    Shaun Cooper February 2020

    
    Program name: ast.c 
    Modified by Meagan Waldo 
    May 6, 2021 

    What is the purpose: The purpose of this code is to successfully print out the abstract syntax tree of given input.

    Algorithm used: I recursively made calls to the function ASTPrint to print out the entire abstract syntax tree. 

    Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 
    
    Output: If an error occurs then an error message is printed out to the user with the line number the error is found on. If everything works then a AST will print out if called at the end of the lab9.y file.

    Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 

    Changes made to ast.c code: 
    3/30/21: I Set the default value for next in ASTCreateNode. I Created the code for the function PT. I Added cases for A_Decaf_STRING, A_Decaf_Bool and A_Decaf_VOID in the function AST_Print_Type. I added cases and code for A_PROGRAM, A_PACKAGE, A_EXTERN, A_ExternType, A_METHODIDENTIFIER, A_METHODCALL, A_METHODPARMS, A_LVALUE, A_VAR_RVALUE, A_ASSIGN, A_BREAK, A_CONTINUE, A_RETURN, A_IF, A_IFBODY, A_CONSTANT_INT, A_CONSTANT_BOOL and A_CONSTANT_STRING. I Added code to cases A_VARDEC, A_METHODDEC and A_EXPR to print out more data and improve formatting. I removed the case A_NUMBER and function main.
    4/9/21: I added the function check_parameters as well as fixed errors in case A_LVALUE involving the array. I also removed any trace of A_PARAM due to it being dead code.
    5/6/21: No changes made.
*/


#include<stdio.h>
#include<malloc.h>
#include "ast.h" 


/* 
 * Uses malloc to create an ASTnode and passes back the heap address of the newley created node.
 * Parameters: A enum AST_Tree_Element_Type called mytype.
 * Returns: ASTnode heap address.
 * Preconditions: None.
 * Postconditions: The heap address of the newly created ASTnode is returned.
 */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype)
{
    ASTnode *p;
    
    // If mydebug is set to true then print out debug information.
    if (mydebug) fprintf(stderr,"Creating AST Node \n");
    
    p=(ASTnode *)malloc(sizeof(ASTnode)); // Used to get the heap address.
    
    p->type=mytype; // Set up the Element type.
    p->S1=NULL; // Default value.
    p->S2=NULL; // Default value. 
    p->next=NULL; // Default value added.
    p->value=0; // Default value.
    
    return(p); // Returns the ASTnode heap address.
} // end of ASTCreateNode


/*
 * Helper function to print tabbing.
 * Parameters: A int called howmany.
 * Returns: Nothing.
 * Preconditions: None.
 * Postconditions: Indentation printed.
 */
void PT(int howmany)
{
    int i;
    
    // Prints out indentation while i is less than howmany.
    for (i = 0; i < howmany; i++) {
	 printf(" ");
    } // end of for
} // end of PT


/*
 * Function used to print DECAF type.
 * Parameters: A enum AST_Decaf_Types called t.
 * Returns: Nothing.
 * Preconditions: None.
 * Postconditions: DECAF type is printed or the default print statement is printed.
 */
void AST_Print_Type( enum AST_Decaf_Types t)
{
	// The correct case is picked based on t.
	switch (t) {
		case  A_Decaf_INT : printf(" INT ");
	                            break;
		case  A_Decaf_STRING : printf(" STRING ");
		                       break;
		case  A_Decaf_BOOL : printf(" BOOLEAN ");
		                     break;
		case  A_Decaf_VOID : printf(" VOID ");
		                     break;		    
                default :  fprintf(stderr,"Unknown AST DECAF TYPE !!!\n");
	} // end of switch
}// of AST_Print_Type


/*
 * Prints out the abstract syntax tree.
 * Parameters: A int called level and a ASTnode pointer called p.
 * Returns: When p is NULL.
 * Preconditions: p must not equal NULL.
 * Postconditions: abstract syntax tree is printed.
 */
void ASTprint(int level,ASTnode *p)
{
   int i;
   
   // If p is Null then return else print out the abstract syntax tree data.
   if (p == NULL ) {
       return;
   } // end of if
   else { 
       PT(level);
       
       // Switch that picks the correct case based on p->type.
       switch (p->type) {
              
	case A_PROGRAM : printf("PROGRAM\n");       
                         ASTprint(level, p->S1);
                         ASTprint(level, p->S2);
	                 break; 

	case A_PACKAGE : printf("Package : %s \n", p->name);	                       
	                 ASTprint(level + 1, p->S1);
		         ASTprint(level + 1, p->S2);
		         break;
 
	case A_EXTERN : printf("EXTERN FUNC %s ", p->name);
	                printf("(");
		        ASTprint(level+1, p->S1);
		        printf(")");
	                AST_Print_Type(p->A_Declared_Type);
	                printf("\n");
	                break;
	
	case A_ExternType : printf("externType");
		            AST_Print_Type(p->A_Declared_Type);
	                    break; 

        case A_VARDEC : printf("Variable ");
                        printf("%s ",p->name);
		     
			// If the variable is an array, then print out the formatting with the correct value else print out the correct formatting for a non-array variable.
			if (p->S1 != NULL){
		            printf("[");
		            printf("%d",p->S1->value);
		            printf("] with type");
		        } // end of if
		        else{
                            printf("with type");
		        } // end of else
                     
			AST_Print_Type(p->A_Declared_Type);
                     
		        // If the Variable has a declared value print the value.
			if (p->S2 != NULL){
		    	   
		            // If A_Declared_Type is equal to A_Decaf_BOOLprint out true or false based on the value.		
		            if (p->A_Declared_Type == A_Decaf_BOOL){
		           
				// If the value is 1 then print true else print false.    
			        if (p->S2->value = 1){
			            printf("= true");
			        } // end of if
			        else {
	                   	    printf("= false");
			        } // end of else
			    } // end of if
			    else { 
	              		printf("= %d ",p->S2->value);
		            } // end of else
			} // end of if

			printf("\n");
   		        break;

        case A_METHODDEC : printf("Method FUNCTION");
		           printf(" %s",p->name);
                           AST_Print_Type(p->A_Declared_Type);
		           printf( "\n");  
		     
                           // If there are no parameters print out that there are none else print out eh parameters.
                           if (p->S1 == NULL ) { 
	                       PT(level+2); 
	                       printf( "( \n");
			       PT(level+2);
		               printf ("NONE \n");
		               PT(level+2);
			       printf( ")");
		           } // end of if
                           else { 
                               PT(level+2);
                               printf( "( \n");
                               ASTprint(level+2, p->S1);
                               PT(level+2);
                               printf( ") ");
                           } // end of else

                           printf("\n");
                           ASTprint(level+2, p->S2); // print out the block
                           break;

        case A_METHODIDENTIFIER : printf("PARAMETER");
		                  AST_Print_Type(p->A_Declared_Type);
	                          printf("%s",p->name);
		                  printf("\n");
		                  break; 

	case A_METHODCALL: printf("METHOD CALL name: %s\n",p->name);
	                   
			   // If there are no parameters then print none else print the parameters.
			   if (p->S1 == NULL ) { 
		               PT(level+2);
	    	       	       printf( "( \n");
		               PT(level+2);
			       printf (" NONE \n");
			       PT(level+2);
			       printf( ")");
			   } // end of if
	   	           else {
			       PT(level+2);
 			       printf( "( \n");
     			       ASTprint(level+2, p->S1);
	  		       PT(level+2);
	       		       printf( ") ");  
	     	           } // end of else
			   
                           printf("\n");
                           break;

        case A_METHODPARMS : printf(" Method Arg \n");
                             ASTprint(level+1, p->S1);
                             break;

        case A_EXPR : printf("EXPR ");
                       
		      // This switch chooses the correct case based on p->operator.
		      switch(p->operator) {
   		
		       case A_PLUS : printf("+ ");
                                     break;
   		
	               case A_MINUS: printf("- ");
                                     break;
		       
		       case A_DIVIDE: printf("/ ");
		                      break;
		
		       case A_TIMES: printf("* ");
		                     break;
		
		       case A_UMINUS: printf("UMINUS ");
	      	                      break;
		
		       case A_MOD: printf("%% ");
                                   break;
		
	               case A_AND: printf("&& ");
                                   break;
		
		       case A_OR: printf("|| ");
                                  break;
		 
		       case A_LEFTSHIFT: printf("<< ");
	     	       	        	 break;
		
		       case A_RIGHTSHIFT: printf(">> ");			
  		                          break;
		
		       case A_LEQ: printf("<= ");			
                                   break;
		
		       case A_LT: printf("< ");			
		                  break;
		
		       case A_GT: printf("> ");	
	                          break;
		
		       case A_GE: printf(">= ");
	                          break;
		
		       case A_EQ: printf("== ");			
	              	          break;
		
		       case A_NEQ: printf("!= ");			
			           break;
		
		       case A_NOT: printf("NOT ");			
		                   break;
		
		       default: printf("unknown EXPR operator ");	       
		      } // end of switch

                      printf("\n");
                      ASTprint(level+1, p->S1);
		       
		      // When the operator is not A_Not recursive call for p->S2.
		      if (p->operator != A_NOT) { 
                          ASTprint(level+1, p->S2);
		      } // end of if
                    
	              break;

        case A_BLOCK : printf("BLOCK STATEMENT \n",p->name);
                       ASTprint(level+1, p->S1);
                       ASTprint(level+1, p->S2);
                       break;

        case A_LVALUE : printf("VARIABLE %s\n",p->name);
		      
			// If S1 is not NULL then recursive call for p->S1.
			if(p->S1 != NULL){
		           PT(level);
			   printf("[\n");	
		           ASTprint(level+2, p->S1);
	                   PT(level);
			   printf("]\n");		   
	                } // end of if

		        break;

	case A_VAR_RVALUE : printf("VARIABLE %s\n",p->name);
	                    
			    // If S1 is not NULL print out the variable with an array format.
			    if(p->S1 != NULL){
			       PT(level);      
			       printf("[\n");
		               ASTprint(level+2, p->S1);
		               PT(level);
                               printf("]\n");			
   		            } // end of if		     
		            
			    break;	     

        case A_ASSIGN : printf("ASSIGNMENT STATEMENT \n");
                        ASTprint(level, p->S1);
		        ASTprint(level+2, p->S2);
	                break;

        case A_BREAK : printf("BREAK STATEMENT  \n");
		       break;

	case A_CONTINUE : printf("CONTINUE STATEMENT  \n");
		          break;

	case A_RETURN : printf("RETURN STATEMENT  \n");
                        ASTprint(level+1, p->S1); 
		        break;

        case A_IF : printf("IF STATEMENT  \n");
		    ASTprint(level+1, p->S1);
		    ASTprint(level+1, p->S2);
		    break;

	case A_IFBODY :	printf("\n");    
	                ASTprint(level+1, p->S1);
	              
			// If S2 is not NULL then print out the else statement.
			if(p->S2 != NULL){
		           PT(level+1); 
		           printf("ELSE STATEMENT \n");
	                   ASTprint(level+2, p->S2);	
	                } // end of if

		        break;

        case A_WHILESTMT : printf("WHILE STATEMENT \n");
                           ASTprint(level+1, p->S1);
                           ASTprint(level+2, p->S2);
                           break;

        case A_CONSTANT_INT : printf("CONSTANT INTEGER %d\n",p->value);
                              break;

        case A_CONSTANT_BOOL : printf("CONSTANT BOOLEAN ");
                               
			       // If the value is 1 then print out true else print out false.
			       if (p->value == 1){
                                   printf("true\n");
                               } // end of if
                               else{
                                   printf("false\n");
                               } // end of else		     
	 	     
			       break;

        case A_CONSTANT_STRING : printf("CONSTANT STRING %s\n",p->name);
	                         break;

        default: printf("unknown type in ASTprint\n");
       } // end of switch
       ASTprint(level, p->next);
     } // end of else
} // end of ASTprint


/*
 * This method is used to check formal and actual parameters.
 * Parameters: Two ASTnode pointers called formal and actual.
 * Pre-condition: None.
 * Post condition: Formal and actual parameters are checked to see if they are the same length and type.
 */
int check_parameters(ASTnode *formal, ASTnode *actual) {

   /* Checks if formal and actual are NULL. */
   if ((formal == NULL) && (actual == NULL)) {		 
      return 1;	
   } // end of if

   /* Checks if formal or actual are NULL. */
   if ((formal == NULL) || (actual == NULL)) {
      return 0;
   } // end of if
			   
   /* Checks if formal and actual have the same type. */
   if (formal->A_Declared_Type != actual->A_Declared_Type) {
      return 0;	
   } // end of if

   /* Recursion. */
   return check_parameters(formal->next, actual->next);
} // end of check_parameters
