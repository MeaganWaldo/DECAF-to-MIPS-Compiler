%{

/*
 *			**** CALC ****
 *
 * This routine will function like a desk calculator
 * There are 26 integer registers, named 'a' thru 'z'
 *
 */

/* This calculator depends on a LEX description which outputs either VARIABLE or INTEGER.
   The return type via yylval is integer 

   When we need to make yylval more complicated, we need to define a pointer type for yylval 
   and to instruct YACC to use a new type so that we can pass back better values
 
   The registers are based on 0, so we subtract 'a' from each single letter we get.

   based on context, we have YACC do the correct memory look up or the storage depending
   on position

   Shaun Cooper
    January 2015

   problems  fix unary minus, fix parenthesis, add multiplication.
   problems  make it so that verbose is on and off with an input argument instead of compiled in.

   Program name: lab9.y
   Modified by Meagan Waldo
   May 6, 2021

   What is the purpose: The purpose of this code is to take tokenized character strings given from LEX and use them to determine if they match the YACC grammar. If the input does not match the YACC grammar, then an error is stated along with the line number the error is found on. In the main function debug information can be printed out as requested and MIPS code is generated into a file based on the user given input. 
   
   Algorithm used: Tokenized character strings given from LEX are checked to see if they follow the YACC grammar. If they don't and error statement is given with the line number, the error is found on.
  
   Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.
   
   Output: If an error occurs then an error message is printed out to the user with the line number the error is found on. If everything works then debug information such as the AST will print out along with the symbol table if they are requested. MIPS code will also be generated and placed in a file based on the user input given. 

   Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.

   Changes made to YACC code:
   2/5/21: I added a declaration for yylex() to stop the compiler warning. I fixed spelling and spacing for the print statement that printed out the answer. I added a rule for multiplication between two expr in order to get multiplication to work. I removed the expr that was before ‘-’ to get the unary minus to work. I fixed spacing for the print statement that printed out the found variable value. I added the type int Infront of the main function. 
   2/26/21: I included "symtable.h" as well as defined the maximum number of variables allowed. I also changed the number of variables you can have in the regs array to be MAXSTACK. I added a global variable to keep track of the number of variables inserted. I changed the reg array calls to ask for the method FetchAddress. I changed start to be P instead of list. I created a UNION type to allow LEX to return integers or strings. I added the type, value for expr as well as created two tokens that match the two types, value and string in the UNION. I also created a token for instances of int. I added code so that in the declaration mode it is ensured that the variable is not duplicated. I also wrote code that made sure if the variable was not a duplicate it was inserted into the symbol table. I added code so that when a variable is in an expression the value is fetching by getting the address component out of the symbol table. I added code so that when a variable is on the left-hand side the value is set in the memory based on the assigned slot from the symbol table.  
   3/8/21: I added variable to keep track of line number as well as added a print statement for errors along with the line number the error occurs on. I changed start to be Program. I removed all the tokens in YACC and put in the tokens listed in the definition link. I replaced the GRAMMAR rules with the DECAF grammar rules in the section "Decaf Grammar". 
   3/30/21: I added #include "ast.h". I added a global struct that is a ASTnodetype  pointer called PROGRAM. I added enum AST_Decaf_Types asttype and enum AST_Operators operator to the %union. All the new types (astnode, asttype and operator) were added for all of the nonterminals. The tokens T_ID and T_STRINGCONSTANT both are updated to have the type string. The token T_INCONSTANT is updated to have the type value. I added semantic actions to every production rule in the YACC file. In the main function I added a function call to the function ASTprint which prints the abstract syntax tree. 
   4/9/21: I included stdlib.h and symtable.h. I added new global variables called LEVEL, OFFSET, MAXOFFSET and GOFFSET. I updated yyerror to display a new format of error message. I added code to maintain the level, size and offset of data stored in the symbol table. I added code to check if symbols where already defined. I added code to insert the symbols into the table if they were not defined and have proper type match. I added rudimentary type inheritance. I added code to ensure that every time a BLOCK is exited decrement occurs on the LEVEL global with the symbols defined at that level being removed and OFFSET reset to the previous level offset. TEMPs are created and placed into the symbol table whenever there is a calculation. The symbol table is printed at the end of the program before the abstract syntax tree is printed.  
   5/6/21: I included string.h and emit.h as well as added the variable mydebug. In Assign statement I set a declared type, created a label and other information for a calculation and inserted the label and other information for the calculation in the symbol table. In Term I made it so leftshift and rightshift could not use booleans in an arithmetic operation instead of an Integer. I Added code to main to check for mydebug flag needing to be set to 1 when “-d” is present. I also added code that caused MIPS code to be generated when “-o” is present. Code added uses the parameters argc and argv to make the new file for the MIPS code to be written into.
*/

	/* begin specs */
#include <stdio.h>
#include <stdlib.h> // Included stdlib.h.
#include <string.h> // Included string.h.
#include <ctype.h>
#include "ast.h" // Included ast.h.
#include "symtable.h" // Included symtable.h.
#include "emit.h" // Included emit.h.

int yylex(); /* Added this declaration to stop the compiler warning. */
int lineNumber; /* Added variable to keep track of line number. */
extern int mydebug; // Added mydebug.

/* AST Global */
struct ASTnodetype *PROGRAM; // Added a global struct.

int LEVEL = 0; // Added LEVEL with is the level of BLOCKS we have seen.
int OFFSET = 0; // Added OFFSET which is used to store the offset.
int MAXOFFSET; // Added MAXOFFSET which is the maximum offset.
int GOFFSET; // Added GOFFSET which is used to store offset during manipulation.

/* This yyerror function is used when there is an error. */
void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  fprintf(stderr, "Error on line %d : %s\n", lineNumber, s); // Updated print statement for errors along with the line number the error occurs on.
} // end of yyerror

%}
/*  defines the start symbol, what values come back from LEX and how the operators are associated  */

%start Program /* Changed start to be Program. */

/* Created a UNION type to allow LEX to return integer or string. */
%union
{
int value;
char * string;
struct ASTnodetype *astnode;
enum AST_Decaf_Types asttype; // AST_Decaf_Types called asttype.
enum AST_Operators operator; // AST_Operators called operator.
} /* end of union */

/* All new types added. */
%type <astnode> Externs ExternParmList FullExternParmList ExternDefn FieldDecl FieldDecls
%type <astnode> ArrayType MethodDecl MethodDecls MethodDeclParmList FullMethodDeclParmList
%type <astnode> Block VarDecl VarDecls Constant BoolConstant Statement Statements
%type <astnode> Factor Term Additiveexpression Simpleexpression Expr MethodArg
%type <astnode> FullMethodCallParmList MethodCallParmList MethodCall Lvalue Assign
%type <asttype> MethodType Type ExternType
%type <operator> Multop Addop Relop 

/* Tokens used within the rules. */
%token T_AND
%token T_ASSIGN
%token T_BOOLTYPE
%token T_BREAK
%token T_CONTINUE
%token T_DOT
%token T_ELSE
%token T_EQ
%token T_EXTERN
%token T_FALSE
%token T_FOR
%token T_FUNC
%token T_GEQ
%token T_GT
%token T_LT
%token T_IF
%token T_INTTYPE
%token T_LEFTSHIFT
%token T_LEQ
%token T_NEQ
%token T_NULL
%token T_OR
%token T_PACKAGE
%token T_RETURN
%token T_RIGHTSHIFT
%token T_STRINGTYPE
%token T_TRUE
%token T_VOID
%token T_WHILE
%token T_VAR
%token <string> T_ID /* Updated to have the type string. */
%token <value> T_INTCONSTANT /* Updated to have the type value. */
%token T_CHARCONSTANT
%token <string> T_STRINGCONSTANT /* Updated to have the type string. */

%%	/* end specs, begin rules */

/* This grammar rule is for defining the start of the program. The pointers are set to the terminals and nonterminals properly and new nodes are created. */
Program : Externs T_PACKAGE T_ID '{' FieldDecls MethodDecls '}' 
	  { PROGRAM = ASTCreateNode(A_PROGRAM); 
            PROGRAM->S1 = $1;
            PROGRAM->S2 = ASTCreateNode(A_PACKAGE);
            PROGRAM->S2->name = $3; 
            PROGRAM->S2->S1 = $5; 
            PROGRAM->S2->S2 = $6;
           }
        ;

/* This grammar shows that it has 0 to n repetitions. This grammar is used for Extern functions. The pointers are set to the nonterminals properly. */
Externs : /* empty */ { $$ = NULL; }
        | ExternDefn Externs { $1->next = $2; }
        ;

/* This grammar is for defining Extern functions. The pointers are set to the terminals and nonterminals properly and new nodes are created. If the symbol is already defined an error is printed. If the symbol is not defined, then it is created and added to the symbol table. */
ExternDefn : T_EXTERN T_FUNC T_ID '(' ExternParmList')' MethodType ';'
	     { 
               /* This if statement is used to check if the symbol is already defined. If it is then the program is exited an error is printed. */
               if (Search($3, LEVEL, 0) != NULL) {
                   yyerror("Symbol already defined");
                   yyerror($3);
                   exit (1);
               } // end of if

               $$ = ASTCreateNode(A_EXTERN); 
               $$->name = $3; 
               $$->A_Declared_Type = $7; 
               $$->S1 = $5; 
               $$->symbol = Insert($3, $7, ID_Sub_Type_Method, LEVEL, 0, 0, $5); /* Inserts the symbol into the symbol table. */
             }
           ;

/* This grammar shows that it has 0 to 1 repetitions. This grammar is used for the Extern parameters. The pointers are set to the nonterminal properly. */
ExternParmList : /* empty */ { $$ = NULL; }
               | FullExternParmList { $$ = $1; }
               ;

/* This grammar shows that it has 1 to n repetitions. This grammar is used to list off Extern parameters. The pointers are set to the nonterminals properly and new nodes are created. */
FullExternParmList : ExternType
                     { $$ = ASTCreateNode(A_ExternType);
                       $$->A_Declared_Type = $1; 
                     }
                   | ExternType ',' FullExternParmList
                     { $$ = ASTCreateNode(A_ExternType);
                       $$->A_Declared_Type = $1; 
                       $$->next = $3; 
                     }
                   ;

/* This grammar shows that is has 0 to n repetitions. This grammar is used to declare variables. The pointers are set to the nonterminals properly. */
FieldDecls : /* empty */ { $$ = NULL; }
	   | FieldDecl FieldDecls
             { $$ = $1;
               $$->next = $2; 
             }
	   ;

/* This grammar is used to declare variables. The pointers are set to the terminals and nonterminal properly and new nodes are created. If the variable has not been declared already it is inserted into the symbol table. If the variable is already declared, then an error prints out. */
FieldDecl : T_VAR T_ID Type ';'
	    { 
             /* Checks if the variable has already been declared. If it has already been added, then an error is printed. */ 
             if (Search($2, LEVEL, 0) != NULL) {
                 yyerror("Variable name already used");
                 yyerror($2);
                 exit (1);
             } // end of if

             $$ = ASTCreateNode(A_VARDEC);
             $$->name = $2;
             $$->A_Declared_Type = $3; 
             $$->symbol = Insert($2, $3, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the variable into the symbol table. */
             OFFSET += 1;
            }
	  ;

/* This grammar is for defining an array declaration. The pointers are set to the terminals and nonterminal properly and new nodes are created. If the variable is not declared it is added to the symbol table. If it is already declared, then an error is printed out. */
FieldDecl : T_VAR T_ID ArrayType ';'
	    { 
              /* Checks to see if the variable was already declared. If it has been declared, then an error is printed. */
              if (Search($2, LEVEL, 0) != NULL) {
                  yyerror("Variable name already used");
                  yyerror($2);
                  exit (1);
              } // end of if 
             
              $$ = ASTCreateNode(A_VARDEC);
              $$->name = $2;
              $$->S1 = $3;
              $$->size = $3->value;
              $$->A_Declared_Type = $3->A_Declared_Type;
              $$->symbol = Insert($2, $3->A_Declared_Type, ID_Sub_Type_Array, LEVEL, $$->size, OFFSET, NULL); /* Inserts the variable into the symbol table. */
              OFFSET += $$->size; 
            }  
          ;

/* This grammar is for defining an assignment. The pointers are set to the terminals and nonterminals properly and new nodes are created. If the variable is not declared it is added to the symbol table. If it is already declared, then an error is printed out. */
FieldDecl : T_VAR T_ID Type T_ASSIGN Constant ';'
	    { 
              /* Checks to see if the variable was already declared. If it has been declared, then an error is printed. */
              if (Search($2, LEVEL, 0) != NULL) {
                  yyerror("Variable name already used");
                  yyerror($2);
                  exit (1);
              } // end of if
   
      	      $$ = ASTCreateNode(A_VARDEC);
              $$->name = $2;
              $$->A_Declared_Type = $3; 
              $$->S2 = $5;
              $$->symbol = Insert($2, $3, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the variable into the symbol table. */
              OFFSET += 1;
            }
          ;

/* This grammar shows that it has 0 to n repetitions of the expression. This grammar is used to declare a method. The pointers are set to the nonterminals properly. */
MethodDecls : /* empty */ { $$ = NULL; }
	    | MethodDecl MethodDecls
              { $$ = $1;
                $$->next = $2; 
              }
            ;

/* This grammar is for defining a method. The pointers are set to the terminals and nonterminals properly and new nodes are created. If the method is not declared it is added to the symbol table. If it is already declared, then an error is printed out. */
MethodDecl : T_FUNC T_ID { GOFFSET = OFFSET; OFFSET = 2; MAXOFFSET = OFFSET;}'(' MethodDeclParmList ')' MethodType 
	     {
              /* Checks to see if the method was already declared. If it has been declared, then an error is printed. */
              if (Search($2, LEVEL, 0) != NULL) {
                  yyerror("Method name already used");
                  yyerror($2);
                  exit(1);
	      } // end of if
             
              Insert($2, $7, ID_Sub_Type_Method, LEVEL, 0, 0, $5); /* Inserts the method into the symbol table. */
             }
             Block
	     { 
              $$ = ASTCreateNode(A_METHODDEC); 
              $$->name = $2;
              $$->A_Declared_Type = $7;
              $$->S1 = $5; // will be $5
              $$->S2 = $9; // will be $9
              $$->symbol = Search($2, LEVEL, 0);

              /* Mantipulation of the MAXOFFSET, OFFSET and GOFFSET globals. */
              $$->size = MAXOFFSET;
              $$->symbol->mysize = MAXOFFSET;
              OFFSET = GOFFSET;
             }
           ;

/* This grammar shows that it has 0 to 1 repetitions. This grammar is used to declare the parameters of a method. The pointers are set to the nonterminal properly. */
MethodDeclParmList : /* empty */ { $$ = NULL; }
		   | FullMethodDeclParmList { $$ = $1; }
		   ;

/* This grammar shows that it has 1 to n repetitions. This grammar is used to declare the parameters of a method. The pointers are set to the terminals and nonterminals properly and new nodes are created. If the method arg is not declared it is added to the symbol table. If it is already declared, then an error is printed out. */
FullMethodDeclParmList : T_ID Type
		         { 
                          /* Checks to see if the method arg was already declared. If it has been declared, then an error is printed. */
                          if (Search ($1, LEVEL +1, 0) != NULL) {
                              yyerror("Duplicate method arg name");
                              yyerror($1);
                              exit (1);
                          } // end of if

                          $$ = ASTCreateNode(A_METHODIDENTIFIER);
                          $$->name = $1;
                          $$->A_Declared_Type = $2;
                          $$->symbol = Insert($1, $2, ID_Sub_Type_Scalar, LEVEL + 1, 1, OFFSET, NULL); /* Inserts the method arg into the symbol table. */
                          OFFSET += 1;
                         } 
		       | T_ID Type ',' FullMethodDeclParmList
                         { 
                          /* Checks to see if the method arg was already declared. If it has been declared, then an error is printed. */
                          if (Search ($1, LEVEL +1, 0) != NULL) {
                              yyerror("Duplicate method arg name");
                              yyerror($1);
                              exit (1);
                          } // end of if

                          $$ = ASTCreateNode(A_METHODIDENTIFIER);
                          $$->name = $1;
                          $$->A_Declared_Type = $2;
                          $$->next = $4;
                          $$->symbol = Insert($1, $2, ID_Sub_Type_Scalar, LEVEL + 1, 1, OFFSET, NULL); /* Inserts the method arg into the symbol table. */
                          OFFSET += 1;
                         }
                       ;

/* This grammar is for defining a block statement. The pointers are set to the nonterminals properly and new nodes are created. Update LEVEL when the block is entered. Update LEVEL, OFFSET and MAXOFFSET when exiting the block. Print out the symbol table. */
Block : '{' 
         { 
          LEVEL++; 
         }
         VarDecls Statements '}'
         { 
           $$ = ASTCreateNode(A_BLOCK);
           $$->S1 = $3;
           $$->S2 = $4;
           if (mydebug) Display(); /* Prints out the symbol table. */

           /* Update the MAXOFFSET if OFFSET is larger. */
           if (OFFSET > MAXOFFSET) { 
              MAXOFFSET = OFFSET;
           } // end of if

           OFFSET -= Delete(LEVEL); /* Remove all the symbols added to the symbol table from the block. */
           LEVEL--;
         }
      ;

/* This grammar shows that it has 0 to n repetitions. This grammar declares variables. The pointers are set to the nonterminals properly. */
VarDecls : /* empty */ { $$ = NULL; }
	 | VarDecl VarDecls
           { $$ = $1;
             $$->next = $2;
           }
	 ;

/* This grammar is for defining a variable declaration. The pointers are set to the terminals and nonterminal properly and new nodes are created. If the variable is not declared it is added to the symbol table. If it is already declared, then an error is printed out. */
VarDecl : T_VAR T_ID Type ';'
	  { 
           /* Checks to see if the variable was already declared. If it has been declared, then an error is printed. */	
           if (Search($2, LEVEL, 0) != NULL) {
               yyerror("Variable already defined at this level");
               yyerror($2);
               exit (1);
           } // end of if

           $$ = ASTCreateNode(A_VARDEC);
           $$->name = $2;
           $$->A_Declared_Type = $3;
           $$->symbol = Insert($2, $3, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the variable into the symbol table. */
           OFFSET += 1;
          }
        ;

/* This grammar is for defining an array declaration. The pointers are set to the terminals and nonterminal properly and new nodes are created. If the variable is not declared it is added to the symbol table. If it is already declared, then an error is printed out. */
VarDecl : T_VAR T_ID ArrayType ';'
	  { 
           /* Checks to see if the variable was already declared. If it has been declared then an error is printed. */
           if (Search($2, LEVEL, 0) != NULL) {
               yyerror("Variable already defined at this level");
               yyerror($2);
               exit (1);
           } // end of if

           $$ = ASTCreateNode(A_VARDEC);
           $$->name = $2;
           $$->S1 = $3;
           $$->size = $3->value;
           $$->A_Declared_Type = $$->S1->A_Declared_Type;
           $$->symbol = Insert($2, $$->S1->A_Declared_Type, ID_Sub_Type_Array, LEVEL, $$->size, OFFSET, NULL); /* Inserts the variable into the symbol table. */
           OFFSET += $$->size;
          }
	;

/* This grammar shows that it has 0 to n repetitions. This grammar is used to declare a statement. The pointers are set to the nonterminals properly. */
Statements : /* empty */ { $$ = NULL; }
	   | Statement Statements
             { $$ = $1;
               $$->next = $2;
             }
           ;

/* This grammar is for defining a block statement. The pointers are set to the nonterminal properly. */
Statement : Block { $$ = $1; }
	  ;

/* This grammar is for defining an assignment statement. The pointers are set to the nonterminal properly. */
Statement : Assign ';' { $$ = $1; }
	  ;

/* This grammar is for defining an assignment statement. The pointers are set to the terminal and nonterminals properly and new nodes are created. Checks for type matching and if a match is not found then an error is printed. */
Assign : Lvalue T_ASSIGN Expr
	 { 
          /* Checks if the types match or not. If they don't then an error is printed. */
          if ($1->A_Declared_Type != $3->A_Declared_Type) {
              yyerror("Type mismatch on assignment");
              exit(1);
          } // end of if

          $$ = ASTCreateNode(A_ASSIGN);
          $$->S1 = $1;
          $$->S2 = $3;
          $$->A_Declared_Type = $1->A_Declared_Type; /* Declare the type. */
          $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
          $$->symbol = Insert($$->name, $$->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET++, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
         }
       ;

/* This grammar is for defining if the left value ID is a value or array. The pointers are set to the terminals and nonterminal properly and new nodes are created. Pointer called p is created. Checks done to see if symbol is defined and if the types match. */
Lvalue : T_ID 
         { 
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search($1, LEVEL, 1);

           /* Checks if the symbol on LVALUE is defined or not. If it is not defined, then an error message is printed. */ 
           if (p == NULL) {
               yyerror("Symbol on LVALUE not defined");
               yyerror($1);
               exit(1);	   
           } // end of if

           /* Checks if the type is a scalar. If it is not a scalar, then an error is printed. */
           if (p->SubType != ID_Sub_Type_Scalar) {
               yyerror("Needs to be a scalar, wrong subtype");
               exit(1);
           } // end of if
              
           $$ = ASTCreateNode(A_LVALUE); 
           $$->name = $1;
           $$->symbol = p;
           $$->A_Declared_Type = p->Type;
         }
       | T_ID '[' Expr ']'
         {  
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search($1, LEVEL, 1);
         
           /* Checks if the symbol on LVALUE is defined or not. If it is not defined, then an error message is printed. */
           if (p == NULL) {
               yyerror("Symbol on LVALUE not defined");
               yyerror($1);
               exit(1);
           } // end of if
       
           /* Checks if the type is an array. If it is not an array then an error is printed. */
           if (p->SubType != ID_Sub_Type_Array) {
               yyerror("Needs to be an array, wrong subtype");
               exit(1);
           } // end of if

           $$ = ASTCreateNode(A_LVALUE);
           $$->name = $1;
           $$->S1 = $3;
           $$->symbol = p;
           $$->A_Declared_Type = p->Type;
         }
       ;

/* This grammar is for defining a method call. The pointers are set to the nonterminal properly. */
Statement : MethodCall ';' { $$ = $1; }
	  ;

/* This grammar is for defining a method call. The pointers are set to the terminal and nonterminal properly and new nodes are created. Pointer called p is created. Checks done to see if symbol is defined and if the types match. A check is also done on the formal and actual parameters to see if the match. If matches are not found or the symbol is not defined, then an error is printed. */
MethodCall : T_ID '(' MethodCallParmList ')'
	    { 
              /* Creates and sets up a pointer called p. */
              struct SymbTab *p;
              p = Search($1, LEVEL, 1);
 
              /* Checks if the symbol is defined or not. If it is not defined, then an error message is printed. */
              if (p == NULL) {
                  yyerror("Symbol not defined");
                  yyerror($1);
                  exit(1);
              } // end of if
       
              /* Checks if the type is a method. If it is not a method, then an error is printed. */
              if (p->SubType != ID_Sub_Type_Method) {
                  yyerror("Needs to be a method, wrong subtype");
                 exit(1);
              } // end of if

              /* Checks if the formal and actual parameters match or not. If they do not match, then an error is printed. */
              if (check_parameters(p->fparms, $3) == 0) {
                  yyerror("Formal and actual parameters to not match");
                  yyerror($1);
                  exit(1);
              } // end of if

              $$ = ASTCreateNode(A_METHODCALL);
              $$->name = $1;
              $$->S1 = $3;
              $$->symbol = p;
              $$->A_Declared_Type = p->Type;
            }
	   ;

/* This grammar shows that it has 0 to 1 repetitions. This grammar is used to define the method call parameters. The pointers are set to the nonterminal properly. */
MethodCallParmList : /* empty */ { $$ = NULL; }
		   | FullMethodCallParmList { $$ = $1; }
		   ;

/* This grammar shows that it has 1 to n repetitions. This grammar is used to define the method call parameters. The pointers are set to the nonterminals properly and new nodes are created. Creates a label and other information for calculations then places them in the symbol table. */
FullMethodCallParmList : MethodArg 
		         { $$ = ASTCreateNode(A_METHODPARMS);
		           $$->S1 = $1;
                           $$->A_Declared_Type = $1->A_Declared_Type;
                           $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                           $$->symbol = Insert($$->name, $$->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                           OFFSET++;
                         }
		       | MethodArg ',' FullMethodCallParmList
                         { $$ = ASTCreateNode(A_METHODPARMS);
                           $$->S1 = $1;
                           $$->next = $3;  
                           $$->A_Declared_Type = $1->A_Declared_Type;
                           $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                           $$->symbol = Insert($$->name, $$->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                           OFFSET++; 
                         }
		       ;

/* This grammar is for defining a method argument. The pointers are set to the nonterminal properly. */
MethodArg : Expr { $$ = $1; 
                 }
	  ;

/* This grammar is for defining a if statement and if else statement. The pointers are set to the terminals and nonterminals properly and new nodes are created. */
Statement : T_IF '(' Expr ')' Block T_ELSE Block 
	    { $$ = ASTCreateNode(A_IF);
              $$->S1 = $3;
              $$->S2 = ASTCreateNode(A_IFBODY);
              $$->S2->S1 = $5;
              $$->S2->S2 = $7; 
            }
	  | T_IF '(' Expr ')' Block	    
            { $$ = ASTCreateNode(A_IF);
              $$->S1 = $3;
              $$->S2 = ASTCreateNode(A_IFBODY);
              $$->S2->S1 = $5;
              $$->S2->S2 = NULL;
             }
          ;

/* This grammar is for defining a while statement. The pointers are set to the terminal and nonterminals properly and new nodes are created. */
Statement : T_WHILE '(' Expr ')' Block 
	    { $$ = ASTCreateNode(A_WHILESTMT);
              $$->S1 = $3;
              $$->S2 = $5;
            }
	  ;

/* This grammar is for defining different types of return statements. The pointers are set to the terminals and nonterminals properly and new nodes are created. */
Statement : T_RETURN ';' { $$ = ASTCreateNode(A_RETURN); }
          | T_RETURN '(' ')' ';' { $$ = ASTCreateNode(A_RETURN); }
          | T_RETURN '(' Expr ')' ';'
            { $$ = ASTCreateNode(A_RETURN);
              $$->S1 = $3; 
            }
          ;

/* This grammar is for defining a break. The pointers are set to the terminal properly and new nodes are created. */
Statement : T_BREAK ';'
	    { $$ = ASTCreateNode(A_BREAK); }
          ;

/* This grammar is for defining a continue. The pointers are set to the terminal properly and new nodes are created. */
Statement : T_CONTINUE ';' { $$ = ASTCreateNode(A_CONTINUE); }
	  ;

/* This grammar is for defining an expression. The pointers are set to the nonterminal properly. */
Expr : Simpleexpression { $$ = $1; }
     ;
     
/* This grammar is for defining two types of expressions with the possibility of n repetitions of the expression. The pointers are set to the nonterminals properly and new nodes are created. Checks for type matches and prints an error if needed. Creates a label and other information for calculations then places them in the symbol table. */
Simpleexpression : Additiveexpression { $$ = $1; }
                 | Simpleexpression Relop Additiveexpression
                   { 
                     /* Checks if the type matches and if it does not then an error is printed. */
                     if ($1->A_Declared_Type != $3->A_Declared_Type ) {
                         yyerror ("Both sides need to be the same type");
                         exit(1);
                     } // end of if

                     $$ = ASTCreateNode(A_EXPR);
                     $$->S1 = $1;
                     $$->S2 = $3;
                     $$->operator = $2;
                     $$->A_Declared_Type = A_Decaf_BOOL;
                     $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                     $$->symbol = Insert($$->name, $$->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                     OFFSET++;
                   }
                 ;

/* This grammar is for defining relational operators. The pointers are set to the terminals properly. */
Relop : T_LEQ { $$ = A_LEQ; }
      | T_GT { $$ = A_GT; }
      | T_LT { $$ = A_LT; }
      | T_GEQ { $$ = A_GE; }
      | T_EQ { $$ = A_EQ; }
      | T_NEQ { $$ = A_NEQ; }
      ;

/* This grammar is for defining two types of expressions with the possibility of n repetitions of the expression. The pointers are set to the nonterminals properly and new nodes are created. Checks for type matches and prints an error if needed. Creates a label and other information for calculations then places them in the symbol table. */
Additiveexpression : Term { $$ = $1; }
		   | Additiveexpression Addop Term
                     { 
               
                      /* Checks if the type matches and if it does not then an error is printed. */       
                      if(($1->A_Declared_Type != $3->A_Declared_Type) || ($1->A_Declared_Type != A_Decaf_INT)){
                         yyerror ("Addition and subtraction need INTS only");
                         exit(1);
                      } // end of if

                      $$ = ASTCreateNode(A_EXPR);
                      $$->S1 = $1;
                      $$->S2 = $3;
                      $$->operator = $2;
                      $$->A_Declared_Type;
                      $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                      $$->symbol = Insert($$->name, $$->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                      OFFSET++;
                     }
                   ;

/* This grammar is for defining the add and subtract operators. The pointers are set to the terminals properly. */
Addop : '+' { $$ = A_PLUS; }
      | '-' { $$ = A_MINUS; }
      ;

/* This grammar is for defining two types of terms with the possibility of n repetitions of the expression. The pointers are set to the nonterminals properly and new nodes are created. Checks for matching types and useage of the correct operation. If the match or operation is not correct, then an error is printed. Creates a label and other information for calculations then places them in the symbol table. */
Term : Factor { $$ = $1; }
     | Term Multop Factor
       { 
         /* Checks if the type matches and if it does not then an error is printed. */
         if ($1->A_Declared_Type != $3->A_Declared_Type) {
             yyerror ("Type mismatch");
             exit(1);
         } // end of if

         /* Checks if the type matches with the correct operation. If it does not, then an error is printed. Updated to include leftshift and rightshift. */
         if ($1->A_Declared_Type == A_Decaf_BOOL && (($2 == A_TIMES) || ($2 == A_DIVIDE) || ($2 == A_MOD) || ($2 == A_LEFTSHIFT) || ($2 == A_RIGHTSHIFT))) {
             yyerror("Cannot use Booleans in an arithmetic operation");
             exit(1);     
         } // end of if

         /* Checks if the type matches with the correct operation. If it does not, then an error is printed. Updated to not include leftshift and rightshift. */
         if ($1->A_Declared_Type == A_Decaf_INT && (($2 == A_AND) || ($2 == A_OR))) {
             yyerror("Cannot use INTEGER in boolean operation");
             exit(1);
         } // end of if

         $$ = ASTCreateNode(A_EXPR);
         $$->S1 = $1;
         $$->A_Declared_Type = $1->A_Declared_Type;
         $$->S2 = $3;
         $$->operator = $2;
         $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
         $$->symbol = Insert($$->name, $$->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
         OFFSET++;
       }
     ;

/* This grammar is for defining operators such as leftshift, rightshift, or, and, mod, multiply and divide. The pointers are set to the terminals properly. */
Multop : '*' { $$ = A_TIMES; }
       | '/' { $$ = A_DIVIDE; }
       | '%' { $$ = A_MOD; }
       | T_AND { $$ = A_AND; }
       | T_OR { $$ = A_OR; }
       | T_LEFTSHIFT { $$ = A_LEFTSHIFT; }
       | T_RIGHTSHIFT { $$ = A_RIGHTSHIFT; }
       ;

/* This grammar is for defining different factors. The pointers are set to the terminals and nonterminals properly and new nodes are created. Pointer called p is created. Checks done to see if symbol is defined and if the types match. Creates a label and other information for calculations then places them in the symbol table. */
Factor : T_ID 
       	 {  
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search($1, LEVEL, 1);

           /* Checks if the symbol on RVALUE is defined or not. If it is not defined, then an error message is printed. */
           if (p == NULL) {
               yyerror("Symbol on RVALUE not defined");
               yyerror($1);
               exit(1);
           } // end of if

           /* Checks if the type is a scalar. If it is not a scalar, then an error is printed. */
           if (p->SubType != ID_Sub_Type_Scalar) {
               yyerror("Needs to be a scalar, wrong subtype");
               exit(1);
           } // end of if

           $$ = ASTCreateNode(A_VAR_RVALUE);
           $$->name = $1;
           $$->symbol = p;
           $$->A_Declared_Type = p->Type;
         }
       | MethodCall { $$ = $1; }
       | T_ID '[' Expr ']'
         { 
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search($1, LEVEL, 1);

           /* Checks if the symbol on RVALUE is defined or not. If it is not defined, then an error message is printed. */
           if (p == NULL) {
               yyerror("Symbol on RVALUE not defined");
               yyerror($1);
               exit(1);
           } // end of if

           /* Checks if the type is an array. If it is not an array, then an error is printed. */
           if (p->SubType != ID_Sub_Type_Array) {
               yyerror("Needs to be a array, wrong subtype");
               exit(1);
           } // end of if

           $$ = ASTCreateNode(A_VAR_RVALUE);
           $$->name = $1;
           $$->S1 = $3;
           $$->symbol = p;
           $$->A_Declared_Type = p->Type;
         }
       | Constant { $$ = $1; }
       | '(' Expr ')' { $$ = $2; }
       | T_STRINGCONSTANT
         { $$ = ASTCreateNode(A_CONSTANT_STRING);
           $$->name = $1;
           $$->A_Declared_Type = A_Decaf_STRING;
         }
       | '!' Factor 
         {  
          /* Checks if the type matches and if it does not then an error is printed. */
          if ($2->A_Declared_Type != A_Decaf_BOOL ) {
              yyerror ("Type mismatch , expecting a boolean");
              exit(1);
          } // end of if

          $$ = ASTCreateNode(A_EXPR);
          $$-> operator = A_NOT;
          $$->S1 = $2;
          $$->A_Declared_Type = A_Decaf_BOOL;
          $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
          $$->symbol = Insert($$->name, A_Decaf_BOOL, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
          OFFSET++;
         }
       | '-' Factor
         { $$ = ASTCreateNode(A_EXPR);
           $$-> operator = A_UMINUS;
           $$->S1 = $2;
           $$->A_Declared_Type = A_Decaf_INT;
           $$->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
           $$->symbol = Insert($$->name, A_Decaf_INT, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
           OFFSET++;
         }
       ;

/* This grammar is for defining a type. The pointers are set to the terminal and nonterminal properly. */
ExternType : T_STRINGTYPE { $$ = A_Decaf_STRING; } 
	   | Type { $$ = $1; }
           ;

/* This grammar is for defining a type. The pointers are set to the terminals properly. */
Type : T_INTTYPE { $$ = A_Decaf_INT; }
     | T_BOOLTYPE { $$ = A_Decaf_BOOL; }
     ;

/* This grammar is for defining a method type. The pointers are set to the terminal and nonterminal properly. */
MethodType : T_VOID { $$ = A_Decaf_VOID; } 
	   | Type { $$ = $1; }
           ;

/* This grammar is for defining a boolean as true or false. The pointers are set to the terminals properly and new nodes are created. */
BoolConstant : T_TRUE 
	       { $$ = ASTCreateNode(A_CONSTANT_BOOL);
                 $$->value = 1;
                 $$->A_Declared_Type = A_Decaf_BOOL;
               }
	     | T_FALSE
               { $$ = ASTCreateNode(A_CONSTANT_BOOL);
                 $$->value = 0;
                 $$->A_Declared_Type = A_Decaf_BOOL;
               }
             ;

/* This grammar is for defining a array. The pointers are set to the terminal and nonterminal properly and new nodes are created. */
ArrayType : '[' T_INTCONSTANT ']' Type
            { $$ = ASTCreateNode(A_ARRAYTYPE);
              $$->value = $2;
              $$->A_Declared_Type = $4;
            }
	  ;

/* This grammar is for defining a constant. The pointers are set to the terminal and nonterminal properly and new nodes are created. */
Constant : T_INTCONSTANT
	   { $$ = ASTCreateNode(A_CONSTANT_INT);
             $$->value = $1;
             $$->A_Declared_Type = A_Decaf_INT;
           }
	 | BoolConstant { $$ = $1; }
         ;

%%	/* end of rules, start of program */

/*  
 * This main function was fixed when I added int before main. This function is used to determine if debug information such as the symbol table or AST needed to be printed. The function also creates a file that will be filled with MIPS code when the function emit_all is called at the end of the function. 
 * Parameters: An int called argc and a char array pointer called argv. 
 * Returns: Nothing. 
 * Preconditions: None. 
 * Postconditions: Any debug information that was requested is printed and a file is created that will be filled with MIPS code from user input. 
 */ 
int main(int argc, char *argv[]) {

printf("argc is %d\n", argc);

/* Prints out args value found and then checks if the debug flag needs to be fliped. */
for (int i=1; i < argc; i++) {
     printf("argv[%d] is %s \n", i, argv[i]);

     /* Update debug flag to be 1. */
     if (strcmp(argv[i],"-d") == 0){
         mydebug = 1;
     } // end of if

     /* Open the next args value and make sure everything is fine with the open. Output file will be the input file with suffix ".asm". */
     if (strcmp(argv[i],"-o") == 0){
         char filename[100]; /* For our new file name. */
         strcpy(filename, argv[i+1]); /* Sets the file name. */
         strcat(filename,".asm"); /* Places the correct ending on the file. */
         printf("file name is %s\n", filename);
         fp = fopen(filename, "w"); /* Open file. */
        
         /* If the file can not be opened then print error and exit. */
         if (fp == NULL) {
             printf("unable to open %s \n", filename);
             exit(1);
         } // end of if
     } // end of if
} // end of for

yyparse();

printf("Parsing completed\n");

/* Prints symbol table if mydebug is set to 1. */
if (mydebug) Display();

/* Prints new lines if mydebug is set to 1. */
if (mydebug) printf("\n\n\n");

/* Prints AST if mydebug is set to 1. */
if (mydebug) ASTprint(0,PROGRAM);

/* Prints out that AST had finished printing if mydebug is set to 1. */
if (mydebug) printf("\nFinished printing AST \n");

/* Writes MIPS code to the file based on input. */
emit_all(PROGRAM);
} // end of main
