/*  
   Program name: emit.c 
   Created and modified by Meagan Waldo 
   May 6, 2021 
  
   What is the purpose: The purpose of this code is to successfully print out MIPS code of given input that is in DECAF.
    
   Algorithm used: Calls to different functions within this file to build upon MIPS code to generate a MIPS version of the DECAF file being read in as input. Some of the calls use recursion to traverse the AST to find things such as strings and global variables. 
   
   Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 
                                                             
   Output: If an error occurs then an error message is printed out to the user. If there are not errors, then MIPS code is successfully printed out to a file.
   
   Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. 
   
   Changes made to emit.c code:
   5/6/21: Since the last lab I have added this entire file. Everything in emit.c is new and used to create MIPS code from our AST tree.
 */

#include "emit.h"
#include <string.h>
#include <stdlib.h>

void emit_mips(ASTnode *p); /* Prototype for internal use. */
void emit_expr(ASTnode *p); /* Prototype for internal use. */
void emit_ident(ASTnode *p); /* Prototype for internal use. */
void emit_method_call(ASTnode *p); /* Prototype for internal use. */

static int GLABEL = 0; /* Global level counter. */

FILE *fp = NULL; /* A pointer to the file that the MIPS code is being written into. */

/* 
 * Sets up the start of the MIPS code and then calls emit_mips which gets the rest.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing.
 * Preconditions: A pointer to the top of AST for DECAF.
 * Postconditions: MIPS code which implements the DECAF representation.
 */
void emit_all(ASTnode *p) {
  
	fprintf(fp, "#         PACKAGE %s \n\n", p->S2->name);
	fprintf(fp, ".data\t# Data section starts here, strings first \n\n");
	
	emit_strings(p); /* Get MIPS code for any strings. */

        fprintf(fp, "_NL: .asciiz \"\\n\" # New line\n\n");
        fprintf(fp, ".align 2 # start all of global variables aligned \n\n");

        emit_global_vars(p); /* Get MIPS code for any global variables. */

	fprintf(fp, "\n\n.text  # start of text(code) segment \n\n");     
	fprintf(fp, ".globl main  # Force MIPS to start at main label \n\n");

	emit_mips(p); /* Get MIPS code for any AST node types needed. */
} // end of emit_all

/*
 * Used to create a label.
 * Parameters: None.
 * Returns: S1 which is the heap memory.
 * Preconditions: Nothing.
 * Postconditions: Creates a label and gets heap memory for the label name.
 */
char *GEN_LABEL() {
    char s[100];
    char *S1;
    sprintf(s, "_L%d", GLABEL++);
    S1 = strdup(s); /* Gets heap memory for label name. */
    return(S1);
} // end of *GEN_LABEL

/*
 * Used to take all the strings found in the AST and print them out in MIPS code.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing if pointer is NULL.
 * Preconditions: Pointer to an AST.
 * Postconditions: All strings found are given labels and proper MIPS being printed out for them. 
 */
void emit_strings(ASTnode *p) {

   /* If the pointer p is NULL then return. */	
   if(p == NULL) {  
       return;
   } // end of if

   /* If a string is found then print out the proper MIPS code. */
   if(p->type == A_CONSTANT_STRING) {
      p->label = GEN_LABEL(); 
      fprintf(fp, "%s:\t.asciiz\t%s\n", p->label, p->name);  
   } // end of if
  
   /* Recursive calls to S1, S2 and next. */
   emit_strings(p->S1);
   emit_strings(p->S2);
   emit_strings(p->next);
} // end of emit_strings

/*
 * Used to take all global variables found in the AST and print them out in MIPS code.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing if pointer is NULL.
 * Preconditions: Pointer to AST.
 * Postconditions: All gobal variables have proper MIPS code printed out for them.
 */
void emit_global_vars(ASTnode *p) {
  
   /* If the pointer p is NULL then return. */
   if(p == NULL) {		
      return;		
   } // end of if

   /* If a global variable is found then print out the proper MIPS code. */
   if(p->type == A_VARDEC && p->symbol->level == 0) {

      /* If the global variable has a value print out the proper code to handle that. Else print out code for just the global variable. */
      if(p->S2 != NULL) {
         fprintf(fp, "%s:\t.word %d\t#global var with value\n",p->name,p->S2->value);
      } // end of if	  
      else {
         fprintf(fp, "%s:\t.space %d\t#global var\n",p->name,p->symbol->mysize*WS); 	
      } // end of else
   } // end of if
  
   /* Recursive calls to S1, S2 and next. */
   emit_global_vars(p->S1);
   emit_global_vars(p->S2);
   emit_global_vars(p->next);
} // end of emit_global_vars

/*
 * Used to print out proper MIPS code with or without a label and a line of code instructions and comment.
 * Parameters: A char pointers call L which is a label, A char pointer called I which is a line of code instructions and lastly a char pointer call C which is a comment.
 * Returns: Nothing.
 * Preconditions: Pointers that point to a instruction and comment with a label being optional.
 * Postconditions: Properly formatted MIPS code is printed out.
 */
void emit(char *L, char *I, char *C) {
 
    /* If there is a label then print out a label with instructions and a comment. Else just print the instructions and comment. */	
    if(L != NULL) {
       fprintf(fp, "%s:\t%s\t\t#%s\n",L,I,C);
    } // end of if
    else { 
       fprintf(fp, "\t%s\t\t#%s\n",I,C);
    } // end of else
} // end of emit

/*
 * Used to print out proper MIPS code for global and local variables that can be arrays or scalars.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing.
 * Preconditions: Pointer to and identifier (LVALUE or RVALUE).
 * Postconditions: All identifiers have proper MIPS code printed out for them and $a0 is the memory address of the identifiers.
 */
void emit_ident(ASTnode *p) {

     char s[100];   

     /* Check to see if the type is a LVALUE or a RVALUE. Else print error and exit. */
     if(p->type != A_LVALUE && p->type != A_VAR_RVALUE) {
	printf("illegal use of emit_ident\n");
        exit(1);     
     } // end of if

     /* If identifier is an array. Else it is a scalar. */
     if(p->S1 != NULL) {
  
        /* If identifier is Global use the following MIPS print out. Else it is a local and use the other MIPS print out. */	     
        if(p->symbol->level == 0) {
           emit_expr(p->S1);
           emit(NULL, "sll $t3, $a0, 2", "Shift left by 2");
    	   sprintf(s, "la $a0, %s", p->name);
       	   emit(NULL, s, "Load global address of variable");
	   emit(NULL, "add $a0, $a0, $t3", "Array reference");
	} // end of if
        else {
           emit_expr(p->S1);
	   emit(NULL, "sll $t3, $a0, 2", "Shift left by 2");
           sprintf(s, "li $a0, %d", p->symbol->offset*WS);
	   emit(NULL, s, "offset for variable address");	
	   emit(NULL, "add $a0, $a0, $sp", "Exact location for stack variable");
	   emit(NULL, "add $a0, $a0, $t3", "Array reference");	
	} // end of else
     } // end of if
     else { 

        /* If identifier is Global use the following MIPS print out. Else it is a local and use the other MIPS print out. */
        if(p->symbol->level == 0) {
  	   sprintf(s, "la $a0, %s", p->name);
	   emit(NULL, s, "Load global address of variable");	
   	} // end of if
      	else {	  
           sprintf(s, "li $a0, %d", p->symbol->offset*WS);
	   emit(NULL, s, "offset for variable address");
	   emit(NULL, "add $a0, $a0, $sp", "Exact location for stack variable");		
        } // end of if
     } // end of else
} // end of emit_ident

/*
 * Used to take all methods found in the AST and printing out their labels along with the needed MIPS code.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing.
 * Preconditions: A pointer to a METHOD DEC.
 * Postconditions: Mips code is printed for the METHOD.
 */
void emit_method_dec(ASTnode *p) {
    char s[100];
   
    fprintf(fp, "\n%s:\t\t# METHOD LABEL\n\n",p->name);
    sprintf(s, "subu $t0  $sp %d",p->symbol->mysize *WS); 
    emit(NULL,s,"set up $t0 to be the new spot for SP");

    emit(NULL,"sw $ra ($t0)","Store the return address");
    emit(NULL,"sw $sp 4($t0)","Store the old stack pointer");
    emit(NULL,"move $sp $t0","set the stack pointer to the new value");

    fprintf(fp, "\n\n"); 
    emit_mips(p->S2);  /* Call to emit_mips for the block. */

    fprintf(fp, "\n\n");
    emit(NULL,"li $v0 0","return  NULL zero (0)");
    emit(NULL,"lw $ra ($sp)","reset return address");
    emit(NULL,"lw $sp 4($sp)","reset stack pointer");

    /* If the name of the METHOD is main then print MIPS code for a METHOD ending and exit. */
    if(strcmp(p->name,"main") == 0) {
       emit(NULL,"li $v0, 10","Main function ends");
       emit(NULL,"syscall","MAIN FUNCTION EXITS");
    } // end of if
    fprintf(fp,"\n");
} // end of emit_method_dec

/*
 * Used to print out MIPS code that prints out a number.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing.
 * Preconditions: A pointer to a METHOD_CALL that is print_int.
 * Postconditions: Mips code is printed for the method.
 */
void emit_print_int(ASTnode *p) {
 
    emit_expr(p->S1->S1); /* Call to emit_expr to get proper MIPS code for the expr. */	

    emit(NULL,"li $v0 1","set up write call");
    emit(NULL,"syscall","print a number");

    emit(NULL,"li $v0, 4","print NEWLINE");
    emit(NULL,"la $a0, _NL","print NEWLINE string location");    
    emit(NULL,"syscall","call print a NEWLINE"); 
    fprintf(fp,"\n");
 } // end of emit_print_int

/*
 * Used to print out MIPS code that prints out a string.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing.
 * Preconditions: A pointer to a METHOD_CALL that is print_string.
 * Postconditions: Mips code is printed for the method.
 */
void emit_print_string(ASTnode *p) {
	  
     emit_expr(p->S1->S1); /* Call to emit_expr to get proper MIPS code for the expr. */

     emit(NULL,"li $v0 4","set up write call");
     emit(NULL,"syscall","print a string");

     emit(NULL,"li $v0, 4","print NEWLINE");
     emit(NULL,"la $a0, _NL","print NEWLINE string location");
     emit(NULL,"syscall","call print a NEWLINE");
     fprintf(fp,"\n");
} // end of emit_print_string

/*
 * Used to print out MIPS code that handles a given expr.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing after a case for the type entered and the MIPS code is printed out.
 * Preconditions: A pointer to a expr.
 * Postconditions: Mips code is printed that handles a expr.
 */
void emit_expr(ASTnode *p) {
    char s[100];
 
    /* If the pointer is null print a error and exit. */
    if(p==NULL) { 
       printf("Illegal use of exit_expr NULL Pointer provided\n");
       exit(1);
    } // end of if

    /* Chooses the correct case that prints out MIPS code as well as any needed calls to a function for a specific type. */
    switch(p->type) {
	   case A_CONSTANT_INT:
                A_CONSTANT_BOOL:
		   sprintf(s,"li $a0, %d", p->value);
	           emit(NULL,s,"expr constant value");
	           return;
	           break;

	   case A_CONSTANT_STRING:
	  	   sprintf(s,"la $a0, %s", p->label);		 
	  	   emit(NULL,s,"expr constant is a string");			    
	  	   return;
	  	   break;
	    
	   case A_METHODCALL:
  		   emit_method_call(p); /* Call to emit_method_call. */
  		   return;
  		   break;
 
	   case A_VAR_RVALUE:
  		   emit_ident(p); /* Call to emit_ident. */
  		   emit(NULL, "lw $a0, ($a0)", "Load from memory a value");
  		   return;
		   break;
    } // end of switch

    emit_expr(p->S1); /* Call emit_expr again but with p pointing to S1. */

    sprintf(s, "sw $a0, %d($sp)", p->symbol->offset*WS);
    emit(NULL, s, "Store LHS from expr temporarily");

    /* if p pointing to S2 is not null then call emit_expr for p pointing to S2 and then print out needed MIPS code. */
    if(p->S2 != NULL) {
       emit_expr(p->S2); /* Call to emit_expr with p pointing to S2. */
       emit(NULL, "move $a1, $a0", "copy RHS to a1");
       sprintf(s, "lw $a0, %d($sp)", p->symbol->offset*WS);
       emit(NULL, s, "retrive LHS from storage");
    } // end of if

    /* Prints out proper MIPS code for the operator found. Exit if no case matches the operator provided. */
    switch(p->operator) {
   
	    case A_PLUS: emit(NULL, "add $a0, $a0, $a1", "EXPR Add");
			 break;

	    case A_MINUS: emit(NULL, "sub $a0, $a0, $a1", "EXPR Subtract");
		  	  break;

            case A_TIMES: emit(NULL, "mult $a0, $a1","EXPR Multiply");    		       
			  emit(NULL, "mflo $a0","The low");
		          break;

            case A_DIVIDE: emit(NULL, "div $a0 $a1","EXPR Divide"); 		    
			 emit(NULL, "mflo $a0","The low");
			 break;
            
	    case A_UMINUS: emit(NULL, "neg $a0 $a1","EXPR Divide");
			   break;

            case A_MOD: emit(NULL, "div $a0, $a1", "EXPR Mod");
			emit(NULL, "mfhi $a0","The high");
			break;

	    case A_AND: emit(NULL, "and $a0, $a0, $a1", "EXPR And");
			break;

	    case A_OR: emit(NULL, "or $a0, $a0, $a1", "EXPR Or");
		       break;
		
            case A_RIGHTSHIFT: emit(NULL, "srlv $a0, $a0, $a1", "EXPR Right Shift"); 
		               break;

            case A_LEFTSHIFT: emit(NULL, "sllv $a0, $a0, $a1", "EXPR Left Shift");	
	                      break;

	    case A_LEQ: emit(NULL, "sle $a0, $a0, $a1", "EXPR Less Than Or Equal");    
			break;

	    case A_LT: emit(NULL, "slt $a0, $a0, $a1", "EXPR Less Than");
		       break;
			
	    case A_GE: emit(NULL, "sge $a0, $a0, $a1", "EXPR Greater Than Or Equal");				      
		       break;

	    case A_GT: emit(NULL, "sgt $a0, $a0, $a1", "EXPR Greater Than");	
		       break;

	    case A_EQ: emit(NULL, "seq $a0, $a0, $a1", "EXPR Equal"); 
	               break;
			
	    case A_NEQ: emit(NULL, "sne $a0, $a0, $a1", "EXPR Not Equal");	
	                break;

	    case A_NOT: emit(NULL, "sltiu $a0, $a0, 1", "EXPR Not");
			break;		      

	    default: printf("Emit EXPR operator unknown \n");
		     exit(1);
		     break;
    } // end of switch
} // end of emit_expr

/*
 * Used to print out proper MIPS code when dealing with a if statement.
 * Parameters: A ASTnode pointer called p.
 * Returns: None.
 * Preconditions: Pointer to a A_IF.
 * Postconditions: The MIPS code needed to complete an if statement is printed out.
 */
void emit_if(ASTnode *p) {
	   
       	char s[100];   
	char *L1, *L2;

	/* Generate two labels. */
	L1 = GEN_LABEL();
	L2 = GEN_LABEL();
	
	emit_expr(p->S1); /* emit_expr for the if statement. */
	sprintf(s, "beq $a0, $0, %s", L1 );
	emit(NULL, s, "Branch out ");		 

        emit_mips(p->S2->S1); /* emit_mips for block. */
	sprintf(s, "j %s", L2);
	emit(NULL, s, "Jump");

	emit(L1, "", "Else part of if"); 
	emit_mips(p->S2->S2); /* emit_mips for block. */
	emit(L2, "", "End of if statement");
        fprintf(fp,"\n");
} // end of emit_if

/*
 * Used to print out proper MIPS code when dealing with a while statement.
 * Parameters: A ASTnode pointer called p.
 * Returns: None.
 * Preconditions: Pointer to a A_WHILE.
 * Postconditions: The MIPS code needed to complete an while statement is printed out.
 */
void emit_while(ASTnode *p) {
	   
       	char s[100];
	char *L1, *L2;
      
	/* Generate two labels. */
        L1 = GEN_LABEL();
        L2 = GEN_LABEL();

	emit(L1, "", "Start of while");
	emit_expr(p->S1); /* emit_expr for the while statement. */

	sprintf(s, "beq $a0, $0, %s", L2);
	emit(NULL, s, "While branch out");

	emit_mips(p->S2); /* emit_mips for block. */
	sprintf(s, "j %s", L1);
	emit(NULL, s, "Jump back to top of while");    
	emit(L2, "", "End of while");
        fprintf(fp,"\n");
} // end of while

/*
 * Used to print out MIPS code that reads in a number.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing.
 * Preconditions: A pointer to a METHOD_CALL that is read_int.
 * Postconditions: Mips code is printed for the method.
 */
void emit_read_int(ASTnode *p) {
     
     emit(NULL, "li $v0, 5", "read int from user");
     emit(NULL, "syscall", "to read a number");
     emit(NULL, "move $a0, $v0", "Make the read in number $a0");
     fprintf(fp,"\n"); 
} // end of emit_read_int

/*
 * Used to call functions that print MIPS code for specific actions such a printing an int or string and reading an int.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing after a function is called.
 * Preconditions: Pointer to a method_call
 * Postconditions: The correct function calls are made to get the needed MIPS code printed.
 */
void emit_method_call(ASTnode *p) {
     
      /* The name of the method is print_int then call the function. */
      if(strcmp(p->name,"print_int") == 0) {
 	 emit_print_int(p);	   
       	 return;
      } // end of if

      /* The name of the method is print_string then call the function. */
      if(strcmp(p->name,"print_string") == 0) {
	 emit_print_string(p);		  	 
     	 return;		     
      } // end of if

      /* The name of the method is read_int then call the function. */
      if(strcmp(p->name,"read_int") == 0) {
	 emit_read_int(p);	 
    	 return;
      } // end of if

      printf("NO METHOD CALL implemented for %s at this point \n", p->name);
      fprintf(fp,"\n");
} // end of emit_method_call

/*
 * Used to print out proper MIPS code when wanting to assign a value.
 * Parameters: A ASTnode pointer called p.
 * Returns: None.
 * Preconditions: Pointer to a A_ASSIGN.
 * Postconditions: The MIPS code needed to complete an assignment statement is printed out.
 */
void emit_assign(ASTnode *p) {
     
     char s[100];

     /* If the type is not a A_ASSIGN then print and error and exit. */
     if(p->type != A_ASSIGN) { 
	printf("Illegal use of emit_assign");
	exit(1);
     } // end of if

     emit_expr(p->S2);
     sprintf(s, "sw $a0 %d($sp)", p->symbol->offset*WS);
     emit(NULL, s, "store RHS temporarily");
    
     emit_ident(p->S1);
     sprintf(s, "lw $a1, %d($sp)", p->symbol->offset*WS);
     emit(NULL, s, "load back RHS into $a1");
     emit(NULL, "sw $a1, ($a0)", "Store assign value");
     fprintf(fp,"\n");
} // end of emit_assign

/*
 * Used to call functions to get the MIPS code for specific AST node types.
 * Parameters: A ASTnode pointer called p.
 * Returns: Nothing if pointer is NULL.
 * Preconditions: Pointer to AST
 * Postconditions: The correct function calls are made to get the needed MIPS code printed.
 */
void emit_mips(ASTnode *p) {

    /* If the pointer p is NULL then return. */
    if(p == NULL) {
       return;		  
    } // end of if

    /* Switch that calls the correct function depending on the node type. */
    switch(p->type) {

	    case A_PACKAGE:
	    	    emit_mips(p->S2); /* Call to emit_mips with p pointing to S2. */  
		    break;		  
	   	
 	    case A_PROGRAM :
      		    emit_mips(p->S2);  /* Call to emit_mips with p pointing to S2. */
    		    break;

 	    case A_METHODDEC:
    		    emit_method_dec(p);  /* Call to emit_method_dec. */
    		    break;

     	    case A_BLOCK:
    		    emit_mips(p->S2);  /* Call to emit_mips with p pointing to S2. */
    		    break;

 	    case A_METHODCALL:
    		    emit_method_call(p);  /* Call to emit_method_call. */
    		    break;	
           
 	    case A_ASSIGN:
    		    emit_assign(p);  /* Call to emit_assign. */
    		    break;
    
     	    case A_IF:
    		    emit_if(p);  /* Call to emit_if. */
    		    break;

 	    case A_WHILESTMT:
    		    emit_while(p);  /* Call to emit_while. */
    		    break;
 	   
	    default: printf("emit_mips UNKNOWN AST type\n");
    } // end of switch

    emit_mips(p->next);
} // end of emit_mips
