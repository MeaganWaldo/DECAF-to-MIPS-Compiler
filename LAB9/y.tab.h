/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_AND = 258,
    T_ASSIGN = 259,
    T_BOOLTYPE = 260,
    T_BREAK = 261,
    T_CONTINUE = 262,
    T_DOT = 263,
    T_ELSE = 264,
    T_EQ = 265,
    T_EXTERN = 266,
    T_FALSE = 267,
    T_FOR = 268,
    T_FUNC = 269,
    T_GEQ = 270,
    T_GT = 271,
    T_LT = 272,
    T_IF = 273,
    T_INTTYPE = 274,
    T_LEFTSHIFT = 275,
    T_LEQ = 276,
    T_NEQ = 277,
    T_NULL = 278,
    T_OR = 279,
    T_PACKAGE = 280,
    T_RETURN = 281,
    T_RIGHTSHIFT = 282,
    T_STRINGTYPE = 283,
    T_TRUE = 284,
    T_VOID = 285,
    T_WHILE = 286,
    T_VAR = 287,
    T_ID = 288,
    T_INTCONSTANT = 289,
    T_CHARCONSTANT = 290,
    T_STRINGCONSTANT = 291
  };
#endif
/* Tokens.  */
#define T_AND 258
#define T_ASSIGN 259
#define T_BOOLTYPE 260
#define T_BREAK 261
#define T_CONTINUE 262
#define T_DOT 263
#define T_ELSE 264
#define T_EQ 265
#define T_EXTERN 266
#define T_FALSE 267
#define T_FOR 268
#define T_FUNC 269
#define T_GEQ 270
#define T_GT 271
#define T_LT 272
#define T_IF 273
#define T_INTTYPE 274
#define T_LEFTSHIFT 275
#define T_LEQ 276
#define T_NEQ 277
#define T_NULL 278
#define T_OR 279
#define T_PACKAGE 280
#define T_RETURN 281
#define T_RIGHTSHIFT 282
#define T_STRINGTYPE 283
#define T_TRUE 284
#define T_VOID 285
#define T_WHILE 286
#define T_VAR 287
#define T_ID 288
#define T_INTCONSTANT 289
#define T_CHARCONSTANT 290
#define T_STRINGCONSTANT 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 86 "lab9.y" /* yacc.c:1909  */

int value;
char * string;
struct ASTnodetype *astnode;
enum AST_Decaf_Types asttype; // AST_Decaf_Types called asttype.
enum AST_Operators operator; // AST_Operators called operator.

#line 134 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
