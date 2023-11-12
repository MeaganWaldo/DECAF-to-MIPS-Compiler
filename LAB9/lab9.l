/*  Small LEX routine which returns two formal tokens (INTEGER and VARIABLE)
    along with single string elements like '+'.
 
    This LEX definition is the companion to the docalc.y YACC routine which
    is a simple calculator

    Shaun Cooper
    January 2015

    Program name: lab9.l
    Modified by Meagan Waldo
    May 06, 2021

    What is the purpose: The purpose of this code is to receive input from the user and determine if the input is a letter, number, whitespace, newline, comment, hex, or one of the many tokens. The input could also be determined to be one of the single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. Once the input is determined then a debug statement is printed out stating what was found. If mydeug is set to zero, then there will be no printed-out messages. A return token is then sent to the YACC file. If a hex is found, then the function HexToInt is called, and the hex is transformed into an integer.

    Algorithm used: Input is read in and is checked to see if it is a letter, number, whitespace, newline, comment, hex, or one of the many tokens. The input could also be determined to be one of the single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >. If input is found to be a letter, then the message "Letter found" is printed out. That string for the variable is then stored in yylval.string and a return token is returned to YACC. If input is found to be a number, then the message "Digit found" is printed out. That number for the Integer is then stored in yylval.value and a return token is returned to YACC. If whitespace is found, then the message "Whitespace found" is printed out. If one of the other many tokens are found their return token is returned to YACC If a single string element is found, then a message is printed out stating what the single string element read in was. The yytext pointer is then returned. If there is a newline then a message is printed stating that there was one found. The yytext pointer is then returned. If a hex number is found the function HexToInt is called transforming the hex to an integer which is then stored in yylval.value with a return token being returned to YACC.Input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.
  
    Output: When mydebug is set to 1 then messages for each letter, number, hex number, whitespace, newline or single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or > will be printed out along with statements printed out from YACC which gives the answer to the given input as well as if any integers or digits were found. If mydeug is set to 0 then only the YACC printed statements will show. If everything works then a AST will print out along with a symbol table. If an error is found, then a statement is printed out stating the error with its line. An output MIPS file will also be generated if no errors are present within the input file.

    Assumptions about the input: User input that may contain letters, numbers, hex numbers, many other tokens, whitespace, newlines and single string elements such as {, }, [, ], -, +, *, /, %, &, |, ;, (, ), ,, !, < or >.

    Changes made to LEX code:
    02/05/21: I set mydebug to be 0 so that the debug information does not print out. I added the single string elements ‘(‘ and ‘)’ to the fourth LEX directive that returns tokens so that parenthesis work.
    02/26/21: I Added a rule for "int" that returns T_INT. I updated the variable rule so that there could be longer variable names that may include uppercase letters, lowercase letters and numbers after the initial capital or lowercase letter. I also updated the yylaval in the variable rule to be yylaval.string and equal to the strdup of yytext. I updated the yylaval in the integer rule to be yylabal.value. Lastly, I added a semicolon to the list of single string elements.   
    03/08/21: I added a line number count variable. I included math.h for the HexToInt function. I created a HexToInt function to convert hex numbers to integers. I added tokens with lex directives as well as added the single string elements '{', '}', '[', ']', ',', '<', '>' and '!' to the list but removed '=' and '&'. I also added a regular expression with lex directives for a hex number, comments, constant string and constant char. I also updated the return tokens for the current regular expressions and added underscore to the available characters for a ID. 
    03/30/21: I included ast.h.
    04/09/21: No changes made.
    05/06/21: I changed mydebug to no longer be static.
*/


%{
int lineNumber = 1; // Added a line number count variable.
int mydebug = 0; /* Switched off debug information. */
#include "ast.h" // Included ast.h.
#include "y.tab.h"
#include "math.h" // Included math.h for the HexToInt function.

/*
 * This method called HexToInt is used to read in a hex number and convert it to a integer.
 * Parameters: A char pointer called hex.
 * Returns: The given hex number is turned into a integer.
 * Preconditions: None.
 * Postconditions: Given hex number is turned into a integer and returned.
 */
int HexToInt(const char * hex ){
        int intAnswer = 0;
        const char * temp = hex;
       
        temp = temp + 2; // Removes the "0x" from the string.

        int length = strlen(temp) - 1; // Gets the length of the string.

        int value = 0;

        char character = '\0';

        // While the string has not reached the end grab the next character and check it to see if it is a letter or number. Once that is determined the operation is completed and the hex number is now written as a integer.
        for (int i = 0 ; temp[i] != '\0'; i++){

                character = temp[i];

		// Checks if it is the letter then gets the correct value.
                if(character == 'a' || character == 'A'){
                        value = 10;
                        character = '\0';
                } // end of if
                // Checks if it is the letter then gets the correct value.
                else if(character == 'b' || character == 'B'){
                        value = 11;
                        character = '\0';
                } // end of if
                // Checks if it is the letter then gets the correct value.
                else if(character == 'c' || character == 'C'){
                        value = 12;
                        character = '\0';
                } // end of if
                // Checks if it is the letter then gets the correct value.
                else if(character == 'd' || character == 'D'){
                        value = 13;
                        character = '\0';
                } // end of if
                // Checks if it is the letter then gets the correct value.
                else if(character == 'e' || character == 'E'){
                        value = 14;
                        character = '\0';
                } // end of if
                // Checks if it is the letter then gets the correct value.
                else if(character == 'f' || character == 'F'){
                        value = 15;
                        character = '\0';
                } // end of if
		// Gets the correct value for the number.
                else{
                        value = character - '0';
                } // end of if

                intAnswer = intAnswer + ( pow(16,length) * value ); // Converts from hex to integer.
                length--;
        } // end of for

        return(intAnswer);
} // end of HexToInt


%}


%%


"&&"             {return(T_AND); /* Token for and. */ }
"="              {return(T_ASSIGN); /* Token for assign. */ }
"bool"           {return(T_BOOLTYPE); /* Token for boolean. */}
"break"          {return(T_BREAK); /* Token for break. */ } 
"continue"       {return(T_CONTINUE); /* Token for continue. */ }
"\."             {return(T_DOT); /* Token for a dot. */ }
"else"           {return(T_ELSE); /* Token for else. */ }
"=="             {return(T_EQ); /* Token for equals. */ }
"extern"         {return(T_EXTERN); /* Token for extern. */ }
"false"          {return(T_FALSE); /* Token for false. */ }
"for"            {return(T_FOR); /* Token for for. */ }
"func"           {return(T_FUNC); /* Token for function. */ }
">="             {return(T_GEQ); /* Token for greater than or equal. */ }
">"              {return(T_GT); /* Token for greater than. */ }
"<"              {return(T_LT); /* Token for less than. */ }
"if"             {return(T_IF); /* Token for if. */ }
"int"            {return(T_INTTYPE); /* Token for int. */ }
"<<"             {return(T_LEFTSHIFT); /* Token for leftshift. */ }
"<="             {return(T_LEQ); /* Token for less than or equal. */ }
"!="             {return(T_NEQ); /* Token for not equal. */ }
"null"           {return(T_NULL); /* Token for null. */ }
"||"             {return(T_OR); /* Token for or. */ }
"package"        {return(T_PACKAGE);/* Token for package. */ }
"return"         {return(T_RETURN);/* Token for return. */ }
">>"             {return(T_RIGHTSHIFT);/* Token for rightshift. */ }
"string"         {return(T_STRINGTYPE);/* Token for string. */ }
"true"           {return(T_TRUE);/* Token for true. */ }
"void"           {return(T_VOID);/* Token for void. */ }
"while"          {return(T_WHILE);/* Token for while. */ }
"var"            {return(T_VAR);/* Token for var. */ }


\'.\'    {// The constant char is stored in yylaval.value and a token is returned to YACC.
           yylval.value = yytext[1]; return(T_CHARCONSTANT);}


\".*\"   {// The constant string is stored in yylaval.string and a token is returned to YACC.
           yylval.string = strdup(yytext); return(T_STRINGCONSTANT);}


\/\/.* { /* This is a comment line for double slash. Return nothing, keep going. */ }


[a-zA-Z][0-9A-Za-z_]*		{// Updated for longer variable names that may include uppercase letters, lowercase letters, underscore and numbers after the initial capital or lowercase letter. Also updated the yylval to be yylaval.string and equal to the strdup of yytext. When the mydebug is set to 1 and input is found to be a letter the message "Letter found" is printed out. The string for the variable is then stored in yylval.string and the token T_ID is returned to YACC.
                  if (mydebug) fprintf(stderr,"ID found\n"); 
                       yylval.string=strdup(yytext); return(T_ID);}


[0-9][0-9]*	{// Updated the yylval to be yylval.value. When the mydebug is set to 1 and input is found to be a number the message "Digit found" is printed out. The number for the integer is then stored in yylval.value and the token T_INTCONSTANT is returned to YACC.
     		  if (mydebug) fprintf(stderr,"Digit found\n"); 
                       yylval.value=atoi((const char *)yytext); return(T_INTCONSTANT);}


0[xX][0-9a-fA-F]+ {// Created regular expression and lex directive for hex number. When the mydebug is set to 1 and input is found to be a number the message "Hex found" is printed out. The number for the integer returned by HexToInt is then stored in yylval.value and the token T_INTCONSTANT is returned to YACC.
                    if (mydebug) fprintf(stderr,"Hex found\n");
                       yylval.value=HexToInt((const char *)yytext); return(T_INTCONSTANT);}


[ \t]		{// When the mydebug is set to 1 and input is found to be whitespace the message "Whitespace found" is printed out.
		  if (mydebug) fprintf(stderr,"Whitespace found\n");}


[\-+*/%{}()\[\];,!] 	{// When the mydebug is set to 1 and input is found to be a single string element then a message is printed out stating what the single string element read in was. The yytext pointer is then returned.
                  if (mydebug) fprintf(stderr,"return a token %c\n",*yytext); /* I added the single string elements '(', ')', '{', '}', '[', ']', ',', '<', '>', '!' and ';' to the list but removed '=' and '&'. */ 
                       return (*yytext);}


\n 		{// Updated with a line number counter and removed the yytext pointer. When the mydebug is set to 1 and input is found to be a newline then a message is printed out stating that one had occurred.
		  if (mydebug) fprintf(stderr,"cariage return %c\n",*yytext); 
                       lineNumber++;}


%%


/* 
    This yywrap function is called by lex after the input is exhausted to see if there will be more input or if the input is complete.
*/
int yywrap(void)
{ 
return 1;
} // end of yywrap
