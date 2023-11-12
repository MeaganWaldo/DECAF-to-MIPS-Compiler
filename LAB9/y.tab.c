/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "lab9.y" /* yacc.c:339  */


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


#line 146 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 86 "lab9.y" /* yacc.c:355  */

int value;
char * string;
struct ASTnodetype *astnode;
enum AST_Decaf_Types asttype; // AST_Decaf_Types called asttype.
enum AST_Operators operator; // AST_Operators called operator.

#line 266 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 283 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   168

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    50,     2,     2,     2,    49,     2,     2,
      39,    40,    47,    45,    42,    46,     2,    48,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    41,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   142,   142,   153,   154,   158,   176,   177,   181,   185,
     193,   194,   201,   219,   239,   258,   259,   266,   267,   266,
     294,   295,   299,   314,   334,   333,   355,   356,   363,   381,
     401,   402,   409,   413,   417,   435,   459,   487,   491,   526,
     527,   531,   539,   551,   556,   563,   573,   581,   582,   583,
     590,   595,   599,   603,   604,   624,   625,   626,   627,   628,
     629,   633,   634,   655,   656,   660,   661,   693,   694,   695,
     696,   697,   698,   699,   703,   727,   728,   753,   754,   755,
     760,   776,   788,   789,   793,   794,   798,   799,   803,   808,
     816,   824,   829
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_AND", "T_ASSIGN", "T_BOOLTYPE",
  "T_BREAK", "T_CONTINUE", "T_DOT", "T_ELSE", "T_EQ", "T_EXTERN",
  "T_FALSE", "T_FOR", "T_FUNC", "T_GEQ", "T_GT", "T_LT", "T_IF",
  "T_INTTYPE", "T_LEFTSHIFT", "T_LEQ", "T_NEQ", "T_NULL", "T_OR",
  "T_PACKAGE", "T_RETURN", "T_RIGHTSHIFT", "T_STRINGTYPE", "T_TRUE",
  "T_VOID", "T_WHILE", "T_VAR", "T_ID", "T_INTCONSTANT", "T_CHARCONSTANT",
  "T_STRINGCONSTANT", "'{'", "'}'", "'('", "')'", "';'", "','", "'['",
  "']'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "$accept", "Program",
  "Externs", "ExternDefn", "ExternParmList", "FullExternParmList",
  "FieldDecls", "FieldDecl", "MethodDecls", "MethodDecl", "$@1", "$@2",
  "MethodDeclParmList", "FullMethodDeclParmList", "Block", "$@3",
  "VarDecls", "VarDecl", "Statements", "Statement", "Assign", "Lvalue",
  "MethodCall", "MethodCallParmList", "FullMethodCallParmList",
  "MethodArg", "Expr", "Simpleexpression", "Relop", "Additiveexpression",
  "Addop", "Term", "Multop", "Factor", "ExternType", "Type", "MethodType",
  "BoolConstant", "ArrayType", "Constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   123,   125,    40,
      41,    59,    44,    91,    93,    43,    45,    42,    47,    37,
      33
};
# endif

#define YYPACT_NINF -99

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-99)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      43,     3,    64,    42,    43,    37,   -99,    41,   -99,    36,
      40,    15,    46,   -99,   -99,   -99,    39,   -99,    44,   -99,
      48,    68,    46,    -3,    15,     4,    50,    49,    68,   -99,
     -99,   -99,    53,   -99,    55,     0,    59,   -99,   -99,   -99,
     -99,    57,    24,   -99,   -99,    63,     9,   -99,   -99,   -99,
     -99,    65,    70,   -99,   -99,     9,    67,   -99,    66,    -3,
      70,   -99,   -99,    72,   -99,   -99,    73,    71,    62,    73,
       4,    69,    74,    77,    32,    78,   -31,   -99,    76,    62,
      79,   107,    80,   -99,    81,    82,   -99,   -99,     6,    26,
     -99,     6,     6,     6,   -99,   -99,   -99,     6,   -99,   -99,
     -99,    18,   -99,     6,     6,     6,   -99,    84,    75,   -15,
       2,   -99,   -99,    85,    87,    88,    89,   -99,    83,   -99,
      86,   -99,     6,    91,   -99,   -99,    72,   -99,   -99,   -99,
     -99,   -99,   -99,     6,   -99,   -99,     6,   -99,   -99,   -99,
     -99,   -99,   -99,   -99,     6,   -99,    92,    72,   -99,     6,
     -99,    90,   -99,   103,   -15,     2,   -99,   -99,   -99,   -99,
     -99,    72,   -99
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     3,     0,     1,     0,     4,     0,
       0,     6,    10,    85,    84,    82,     0,     7,     8,    83,
       0,    15,    10,     0,     0,     0,     0,     0,    15,    11,
      86,    87,     0,     9,     0,     0,     0,    17,     2,    16,
       5,     0,     0,    12,    13,     0,     0,    89,    88,    91,
      92,     0,    20,    90,    14,     0,     0,    21,    22,     0,
       0,    18,    23,     0,    24,    19,    26,     0,    30,    26,
       0,     0,     0,     0,     0,     0,    35,    32,     0,    30,
       0,     0,     0,    27,     0,     0,    50,    51,     0,     0,
      47,     0,    39,     0,    25,    31,    33,     0,    37,    28,
      29,    74,    79,     0,     0,     0,    75,     0,    52,    53,
      61,    65,    77,     0,     0,     0,     0,    40,    41,    43,
       0,    34,     0,     0,    81,    80,     0,    59,    58,    56,
      57,    55,    60,     0,    63,    64,     0,    70,    72,    71,
      73,    67,    68,    69,     0,    48,     0,     0,    38,     0,
      36,     0,    78,    45,    54,    62,    66,    49,    46,    42,
      76,     0,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -99,   -99,   109,   -99,   -99,    94,    97,   -99,   104,   -99,
     -99,   -99,   -99,    93,   -63,   -99,    95,   -99,    56,   -99,
     -99,   -99,   -47,   -99,   -13,   -99,   -78,   -99,   -99,     5,
     -99,     1,   -99,   -98,   -99,   -22,    96,   -99,    98,    99
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    16,    17,    21,    22,    27,    28,
      45,    63,    56,    57,    77,    66,    68,    69,    78,    79,
      80,    81,   106,   116,   117,   118,   119,   108,   133,   109,
     136,   110,   144,   111,    18,    19,    32,    50,    36,   112
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      65,    31,    13,    35,    42,   137,   124,   125,    92,    13,
     107,   114,    93,   115,    13,   120,    14,     5,    47,   121,
      13,    82,   138,    14,    53,   123,   139,    30,    14,   140,
     134,   135,    82,    58,    14,    48,    47,    31,    47,   101,
      49,    43,   102,    15,   151,   103,   156,    34,    84,   141,
     142,   143,   104,    48,     1,    48,   105,    92,    49,   101,
      49,   122,   102,   153,     6,   103,   113,     7,    71,    72,
       9,    89,   104,    90,    10,    11,   105,    12,    20,    23,
      73,    25,    26,    37,   158,   127,    24,    38,    74,    41,
     128,   129,   130,    75,    40,    76,   131,   132,   162,    64,
      44,    46,    52,    55,    70,    67,    54,    59,    60,    64,
      86,    97,   161,     8,    94,    87,    88,    91,    33,    29,
      96,    98,    99,   100,   126,   149,   145,   146,   147,   148,
     150,   152,    39,   157,   160,    95,   159,   155,   154,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,     0,    85
};

static const yytype_int16 yycheck[] =
{
      63,    23,     5,    25,     4,     3,   104,   105,    39,     5,
      88,    89,    43,    91,     5,    93,    19,    14,    12,    97,
       5,    68,    20,    19,    46,   103,    24,    30,    19,    27,
      45,    46,    79,    55,    19,    29,    12,    59,    12,    33,
      34,    41,    36,    28,   122,    39,   144,    43,    70,    47,
      48,    49,    46,    29,    11,    29,    50,    39,    34,    33,
      34,    43,    36,   126,     0,    39,    40,    25,     6,     7,
      33,    39,    46,    41,    33,    39,    50,    37,    32,    40,
      18,    33,    14,    33,   147,    10,    42,    38,    26,    34,
      15,    16,    17,    31,    41,    33,    21,    22,   161,    37,
      41,    44,    39,    33,    33,    32,    41,    40,    42,    37,
      41,     4,     9,     4,    38,    41,    39,    39,    24,    22,
      41,    41,    41,    41,    40,    42,    41,    40,    40,    40,
      44,    40,    28,    41,    44,    79,   149,   136,   133,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    70
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    11,    52,    53,    54,    14,     0,    25,    53,    33,
      33,    39,    37,     5,    19,    28,    55,    56,    85,    86,
      32,    57,    58,    40,    42,    33,    14,    59,    60,    57,
      30,    86,    87,    56,    43,    86,    89,    33,    38,    59,
      41,    34,     4,    41,    41,    61,    44,    12,    29,    34,
      88,    90,    39,    86,    41,    33,    63,    64,    86,    40,
      42,    87,    64,    62,    37,    65,    66,    32,    67,    68,
      33,     6,     7,    18,    26,    31,    33,    65,    69,    70,
      71,    72,    73,    67,    86,    89,    41,    41,    39,    39,
      41,    39,    39,    43,    38,    69,    41,     4,    41,    41,
      41,    33,    36,    39,    46,    50,    73,    77,    78,    80,
      82,    84,    90,    40,    77,    77,    74,    75,    76,    77,
      77,    77,    43,    77,    84,    84,    40,    10,    15,    16,
      17,    21,    22,    79,    45,    46,    81,     3,    20,    24,
      27,    47,    48,    49,    83,    41,    40,    40,    40,    42,
      44,    77,    40,    65,    80,    82,    84,    41,    65,    75,
      44,     9,    65
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    58,    59,    59,    61,    62,    60,
      63,    63,    64,    64,    66,    65,    67,    67,    68,    68,
      69,    69,    70,    70,    71,    72,    72,    70,    73,    74,
      74,    75,    75,    76,    70,    70,    70,    70,    70,    70,
      70,    70,    77,    78,    78,    79,    79,    79,    79,    79,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    83,
      83,    83,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    84,    85,    85,    86,    86,    87,    87,    88,    88,
      89,    90,    90
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     7,     0,     2,     8,     0,     1,     1,     3,
       0,     2,     4,     4,     6,     0,     2,     0,     0,     9,
       0,     1,     2,     4,     0,     5,     0,     2,     4,     4,
       0,     2,     1,     2,     3,     1,     4,     2,     4,     0,
       1,     1,     3,     1,     7,     5,     5,     2,     4,     5,
       2,     2,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 143 "lab9.y" /* yacc.c:1646  */
    { PROGRAM = ASTCreateNode(A_PROGRAM); 
            PROGRAM->S1 = (yyvsp[-6].astnode);
            PROGRAM->S2 = ASTCreateNode(A_PACKAGE);
            PROGRAM->S2->name = (yyvsp[-4].string); 
            PROGRAM->S2->S1 = (yyvsp[-2].astnode); 
            PROGRAM->S2->S2 = (yyvsp[-1].astnode);
           }
#line 1490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 153 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 154 "lab9.y" /* yacc.c:1646  */
    { (yyvsp[-1].astnode)->next = (yyvsp[0].astnode); }
#line 1502 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 159 "lab9.y" /* yacc.c:1646  */
    { 
               /* This if statement is used to check if the symbol is already defined. If it is then the program is exited an error is printed. */
               if (Search((yyvsp[-5].string), LEVEL, 0) != NULL) {
                   yyerror("Symbol already defined");
                   yyerror((yyvsp[-5].string));
                   exit (1);
               } // end of if

               (yyval.astnode) = ASTCreateNode(A_EXTERN); 
               (yyval.astnode)->name = (yyvsp[-5].string); 
               (yyval.astnode)->A_Declared_Type = (yyvsp[-1].asttype); 
               (yyval.astnode)->S1 = (yyvsp[-3].astnode); 
               (yyval.astnode)->symbol = Insert((yyvsp[-5].string), (yyvsp[-1].asttype), ID_Sub_Type_Method, LEVEL, 0, 0, (yyvsp[-3].astnode)); /* Inserts the symbol into the symbol table. */
             }
#line 1521 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 176 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 177 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 1533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 182 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_ExternType);
                       (yyval.astnode)->A_Declared_Type = (yyvsp[0].asttype); 
                     }
#line 1541 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 186 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_ExternType);
                       (yyval.astnode)->A_Declared_Type = (yyvsp[-2].asttype); 
                       (yyval.astnode)->next = (yyvsp[0].astnode); 
                     }
#line 1550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 193 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 195 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode);
               (yyval.astnode)->next = (yyvsp[0].astnode); 
             }
#line 1564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 202 "lab9.y" /* yacc.c:1646  */
    { 
             /* Checks if the variable has already been declared. If it has already been added, then an error is printed. */ 
             if (Search((yyvsp[-2].string), LEVEL, 0) != NULL) {
                 yyerror("Variable name already used");
                 yyerror((yyvsp[-2].string));
                 exit (1);
             } // end of if

             (yyval.astnode) = ASTCreateNode(A_VARDEC);
             (yyval.astnode)->name = (yyvsp[-2].string);
             (yyval.astnode)->A_Declared_Type = (yyvsp[-1].asttype); 
             (yyval.astnode)->symbol = Insert((yyvsp[-2].string), (yyvsp[-1].asttype), ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the variable into the symbol table. */
             OFFSET += 1;
            }
#line 1583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 220 "lab9.y" /* yacc.c:1646  */
    { 
              /* Checks to see if the variable was already declared. If it has been declared, then an error is printed. */
              if (Search((yyvsp[-2].string), LEVEL, 0) != NULL) {
                  yyerror("Variable name already used");
                  yyerror((yyvsp[-2].string));
                  exit (1);
              } // end of if 
             
              (yyval.astnode) = ASTCreateNode(A_VARDEC);
              (yyval.astnode)->name = (yyvsp[-2].string);
              (yyval.astnode)->S1 = (yyvsp[-1].astnode);
              (yyval.astnode)->size = (yyvsp[-1].astnode)->value;
              (yyval.astnode)->A_Declared_Type = (yyvsp[-1].astnode)->A_Declared_Type;
              (yyval.astnode)->symbol = Insert((yyvsp[-2].string), (yyvsp[-1].astnode)->A_Declared_Type, ID_Sub_Type_Array, LEVEL, (yyval.astnode)->size, OFFSET, NULL); /* Inserts the variable into the symbol table. */
              OFFSET += (yyval.astnode)->size; 
            }
#line 1604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 240 "lab9.y" /* yacc.c:1646  */
    { 
              /* Checks to see if the variable was already declared. If it has been declared, then an error is printed. */
              if (Search((yyvsp[-4].string), LEVEL, 0) != NULL) {
                  yyerror("Variable name already used");
                  yyerror((yyvsp[-4].string));
                  exit (1);
              } // end of if
   
      	      (yyval.astnode) = ASTCreateNode(A_VARDEC);
              (yyval.astnode)->name = (yyvsp[-4].string);
              (yyval.astnode)->A_Declared_Type = (yyvsp[-3].asttype); 
              (yyval.astnode)->S2 = (yyvsp[-1].astnode);
              (yyval.astnode)->symbol = Insert((yyvsp[-4].string), (yyvsp[-3].asttype), ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the variable into the symbol table. */
              OFFSET += 1;
            }
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 258 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 260 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode);
                (yyval.astnode)->next = (yyvsp[0].astnode); 
              }
#line 1638 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 266 "lab9.y" /* yacc.c:1646  */
    { GOFFSET = OFFSET; OFFSET = 2; MAXOFFSET = OFFSET;}
#line 1644 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 267 "lab9.y" /* yacc.c:1646  */
    {
              /* Checks to see if the method was already declared. If it has been declared, then an error is printed. */
              if (Search((yyvsp[-5].string), LEVEL, 0) != NULL) {
                  yyerror("Method name already used");
                  yyerror((yyvsp[-5].string));
                  exit(1);
	      } // end of if
             
              Insert((yyvsp[-5].string), (yyvsp[0].asttype), ID_Sub_Type_Method, LEVEL, 0, 0, (yyvsp[-2].astnode)); /* Inserts the method into the symbol table. */
             }
#line 1659 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 278 "lab9.y" /* yacc.c:1646  */
    { 
              (yyval.astnode) = ASTCreateNode(A_METHODDEC); 
              (yyval.astnode)->name = (yyvsp[-7].string);
              (yyval.astnode)->A_Declared_Type = (yyvsp[-2].asttype);
              (yyval.astnode)->S1 = (yyvsp[-4].astnode); // will be $5
              (yyval.astnode)->S2 = (yyvsp[0].astnode); // will be $9
              (yyval.astnode)->symbol = Search((yyvsp[-7].string), LEVEL, 0);

              /* Mantipulation of the MAXOFFSET, OFFSET and GOFFSET globals. */
              (yyval.astnode)->size = MAXOFFSET;
              (yyval.astnode)->symbol->mysize = MAXOFFSET;
              OFFSET = GOFFSET;
             }
#line 1677 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 294 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 295 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 300 "lab9.y" /* yacc.c:1646  */
    { 
                          /* Checks to see if the method arg was already declared. If it has been declared, then an error is printed. */
                          if (Search ((yyvsp[-1].string), LEVEL +1, 0) != NULL) {
                              yyerror("Duplicate method arg name");
                              yyerror((yyvsp[-1].string));
                              exit (1);
                          } // end of if

                          (yyval.astnode) = ASTCreateNode(A_METHODIDENTIFIER);
                          (yyval.astnode)->name = (yyvsp[-1].string);
                          (yyval.astnode)->A_Declared_Type = (yyvsp[0].asttype);
                          (yyval.astnode)->symbol = Insert((yyvsp[-1].string), (yyvsp[0].asttype), ID_Sub_Type_Scalar, LEVEL + 1, 1, OFFSET, NULL); /* Inserts the method arg into the symbol table. */
                          OFFSET += 1;
                         }
#line 1708 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 315 "lab9.y" /* yacc.c:1646  */
    { 
                          /* Checks to see if the method arg was already declared. If it has been declared, then an error is printed. */
                          if (Search ((yyvsp[-3].string), LEVEL +1, 0) != NULL) {
                              yyerror("Duplicate method arg name");
                              yyerror((yyvsp[-3].string));
                              exit (1);
                          } // end of if

                          (yyval.astnode) = ASTCreateNode(A_METHODIDENTIFIER);
                          (yyval.astnode)->name = (yyvsp[-3].string);
                          (yyval.astnode)->A_Declared_Type = (yyvsp[-2].asttype);
                          (yyval.astnode)->next = (yyvsp[0].astnode);
                          (yyval.astnode)->symbol = Insert((yyvsp[-3].string), (yyvsp[-2].asttype), ID_Sub_Type_Scalar, LEVEL + 1, 1, OFFSET, NULL); /* Inserts the method arg into the symbol table. */
                          OFFSET += 1;
                         }
#line 1728 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 334 "lab9.y" /* yacc.c:1646  */
    { 
          LEVEL++; 
         }
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 338 "lab9.y" /* yacc.c:1646  */
    { 
           (yyval.astnode) = ASTCreateNode(A_BLOCK);
           (yyval.astnode)->S1 = (yyvsp[-2].astnode);
           (yyval.astnode)->S2 = (yyvsp[-1].astnode);
           if (mydebug) Display(); /* Prints out the symbol table. */

           /* Update the MAXOFFSET if OFFSET is larger. */
           if (OFFSET > MAXOFFSET) { 
              MAXOFFSET = OFFSET;
           } // end of if

           OFFSET -= Delete(LEVEL); /* Remove all the symbols added to the symbol table from the block. */
           LEVEL--;
         }
#line 1755 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 355 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1761 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 357 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode);
             (yyval.astnode)->next = (yyvsp[0].astnode);
           }
#line 1769 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 364 "lab9.y" /* yacc.c:1646  */
    { 
           /* Checks to see if the variable was already declared. If it has been declared, then an error is printed. */	
           if (Search((yyvsp[-2].string), LEVEL, 0) != NULL) {
               yyerror("Variable already defined at this level");
               yyerror((yyvsp[-2].string));
               exit (1);
           } // end of if

           (yyval.astnode) = ASTCreateNode(A_VARDEC);
           (yyval.astnode)->name = (yyvsp[-2].string);
           (yyval.astnode)->A_Declared_Type = (yyvsp[-1].asttype);
           (yyval.astnode)->symbol = Insert((yyvsp[-2].string), (yyvsp[-1].asttype), ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the variable into the symbol table. */
           OFFSET += 1;
          }
#line 1788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 382 "lab9.y" /* yacc.c:1646  */
    { 
           /* Checks to see if the variable was already declared. If it has been declared then an error is printed. */
           if (Search((yyvsp[-2].string), LEVEL, 0) != NULL) {
               yyerror("Variable already defined at this level");
               yyerror((yyvsp[-2].string));
               exit (1);
           } // end of if

           (yyval.astnode) = ASTCreateNode(A_VARDEC);
           (yyval.astnode)->name = (yyvsp[-2].string);
           (yyval.astnode)->S1 = (yyvsp[-1].astnode);
           (yyval.astnode)->size = (yyvsp[-1].astnode)->value;
           (yyval.astnode)->A_Declared_Type = (yyval.astnode)->S1->A_Declared_Type;
           (yyval.astnode)->symbol = Insert((yyvsp[-2].string), (yyval.astnode)->S1->A_Declared_Type, ID_Sub_Type_Array, LEVEL, (yyval.astnode)->size, OFFSET, NULL); /* Inserts the variable into the symbol table. */
           OFFSET += (yyval.astnode)->size;
          }
#line 1809 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 401 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 403 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode);
               (yyval.astnode)->next = (yyvsp[0].astnode);
             }
#line 1823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 409 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 1829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 413 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode); }
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 418 "lab9.y" /* yacc.c:1646  */
    { 
          /* Checks if the types match or not. If they don't then an error is printed. */
          if ((yyvsp[-2].astnode)->A_Declared_Type != (yyvsp[0].astnode)->A_Declared_Type) {
              yyerror("Type mismatch on assignment");
              exit(1);
          } // end of if

          (yyval.astnode) = ASTCreateNode(A_ASSIGN);
          (yyval.astnode)->S1 = (yyvsp[-2].astnode);
          (yyval.astnode)->S2 = (yyvsp[0].astnode);
          (yyval.astnode)->A_Declared_Type = (yyvsp[-2].astnode)->A_Declared_Type; /* Declare the type. */
          (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
          (yyval.astnode)->symbol = Insert((yyval.astnode)->name, (yyval.astnode)->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET++, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
         }
#line 1854 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 436 "lab9.y" /* yacc.c:1646  */
    { 
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search((yyvsp[0].string), LEVEL, 1);

           /* Checks if the symbol on LVALUE is defined or not. If it is not defined, then an error message is printed. */ 
           if (p == NULL) {
               yyerror("Symbol on LVALUE not defined");
               yyerror((yyvsp[0].string));
               exit(1);	   
           } // end of if

           /* Checks if the type is a scalar. If it is not a scalar, then an error is printed. */
           if (p->SubType != ID_Sub_Type_Scalar) {
               yyerror("Needs to be a scalar, wrong subtype");
               exit(1);
           } // end of if
              
           (yyval.astnode) = ASTCreateNode(A_LVALUE); 
           (yyval.astnode)->name = (yyvsp[0].string);
           (yyval.astnode)->symbol = p;
           (yyval.astnode)->A_Declared_Type = p->Type;
         }
#line 1882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 460 "lab9.y" /* yacc.c:1646  */
    {  
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search((yyvsp[-3].string), LEVEL, 1);
         
           /* Checks if the symbol on LVALUE is defined or not. If it is not defined, then an error message is printed. */
           if (p == NULL) {
               yyerror("Symbol on LVALUE not defined");
               yyerror((yyvsp[-3].string));
               exit(1);
           } // end of if
       
           /* Checks if the type is an array. If it is not an array then an error is printed. */
           if (p->SubType != ID_Sub_Type_Array) {
               yyerror("Needs to be an array, wrong subtype");
               exit(1);
           } // end of if

           (yyval.astnode) = ASTCreateNode(A_LVALUE);
           (yyval.astnode)->name = (yyvsp[-3].string);
           (yyval.astnode)->S1 = (yyvsp[-1].astnode);
           (yyval.astnode)->symbol = p;
           (yyval.astnode)->A_Declared_Type = p->Type;
         }
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 487 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode); }
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 492 "lab9.y" /* yacc.c:1646  */
    { 
              /* Creates and sets up a pointer called p. */
              struct SymbTab *p;
              p = Search((yyvsp[-3].string), LEVEL, 1);
 
              /* Checks if the symbol is defined or not. If it is not defined, then an error message is printed. */
              if (p == NULL) {
                  yyerror("Symbol not defined");
                  yyerror((yyvsp[-3].string));
                  exit(1);
              } // end of if
       
              /* Checks if the type is a method. If it is not a method, then an error is printed. */
              if (p->SubType != ID_Sub_Type_Method) {
                  yyerror("Needs to be a method, wrong subtype");
                 exit(1);
              } // end of if

              /* Checks if the formal and actual parameters match or not. If they do not match, then an error is printed. */
              if (check_parameters(p->fparms, (yyvsp[-1].astnode)) == 0) {
                  yyerror("Formal and actual parameters to not match");
                  yyerror((yyvsp[-3].string));
                  exit(1);
              } // end of if

              (yyval.astnode) = ASTCreateNode(A_METHODCALL);
              (yyval.astnode)->name = (yyvsp[-3].string);
              (yyval.astnode)->S1 = (yyvsp[-1].astnode);
              (yyval.astnode)->symbol = p;
              (yyval.astnode)->A_Declared_Type = p->Type;
            }
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 526 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = NULL; }
#line 1959 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 527 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 1965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 532 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_METHODPARMS);
		           (yyval.astnode)->S1 = (yyvsp[0].astnode);
                           (yyval.astnode)->A_Declared_Type = (yyvsp[0].astnode)->A_Declared_Type;
                           (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                           (yyval.astnode)->symbol = Insert((yyval.astnode)->name, (yyval.astnode)->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                           OFFSET++;
                         }
#line 1977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 540 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_METHODPARMS);
                           (yyval.astnode)->S1 = (yyvsp[-2].astnode);
                           (yyval.astnode)->next = (yyvsp[0].astnode);  
                           (yyval.astnode)->A_Declared_Type = (yyvsp[-2].astnode)->A_Declared_Type;
                           (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                           (yyval.astnode)->symbol = Insert((yyval.astnode)->name, (yyval.astnode)->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                           OFFSET++; 
                         }
#line 1990 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 551 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); 
                 }
#line 1997 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 557 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_IF);
              (yyval.astnode)->S1 = (yyvsp[-4].astnode);
              (yyval.astnode)->S2 = ASTCreateNode(A_IFBODY);
              (yyval.astnode)->S2->S1 = (yyvsp[-2].astnode);
              (yyval.astnode)->S2->S2 = (yyvsp[0].astnode); 
            }
#line 2008 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 564 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_IF);
              (yyval.astnode)->S1 = (yyvsp[-2].astnode);
              (yyval.astnode)->S2 = ASTCreateNode(A_IFBODY);
              (yyval.astnode)->S2->S1 = (yyvsp[0].astnode);
              (yyval.astnode)->S2->S2 = NULL;
             }
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 574 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_WHILESTMT);
              (yyval.astnode)->S1 = (yyvsp[-2].astnode);
              (yyval.astnode)->S2 = (yyvsp[0].astnode);
            }
#line 2028 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 581 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_RETURN); }
#line 2034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 582 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_RETURN); }
#line 2040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 584 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_RETURN);
              (yyval.astnode)->S1 = (yyvsp[-2].astnode); 
            }
#line 2048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 591 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_BREAK); }
#line 2054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 595 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_CONTINUE); }
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 599 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 603 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 605 "lab9.y" /* yacc.c:1646  */
    { 
                     /* Checks if the type matches and if it does not then an error is printed. */
                     if ((yyvsp[-2].astnode)->A_Declared_Type != (yyvsp[0].astnode)->A_Declared_Type ) {
                         yyerror ("Both sides need to be the same type");
                         exit(1);
                     } // end of if

                     (yyval.astnode) = ASTCreateNode(A_EXPR);
                     (yyval.astnode)->S1 = (yyvsp[-2].astnode);
                     (yyval.astnode)->S2 = (yyvsp[0].astnode);
                     (yyval.astnode)->operator = (yyvsp[-1].operator);
                     (yyval.astnode)->A_Declared_Type = A_Decaf_BOOL;
                     (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                     (yyval.astnode)->symbol = Insert((yyval.astnode)->name, (yyval.astnode)->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                     OFFSET++;
                   }
#line 2093 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 624 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_LEQ; }
#line 2099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 625 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_GT; }
#line 2105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 626 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_LT; }
#line 2111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 627 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_GE; }
#line 2117 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 628 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_EQ; }
#line 2123 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 629 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_NEQ; }
#line 2129 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 633 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2135 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 635 "lab9.y" /* yacc.c:1646  */
    { 
               
                      /* Checks if the type matches and if it does not then an error is printed. */       
                      if(((yyvsp[-2].astnode)->A_Declared_Type != (yyvsp[0].astnode)->A_Declared_Type) || ((yyvsp[-2].astnode)->A_Declared_Type != A_Decaf_INT)){
                         yyerror ("Addition and subtraction need INTS only");
                         exit(1);
                      } // end of if

                      (yyval.astnode) = ASTCreateNode(A_EXPR);
                      (yyval.astnode)->S1 = (yyvsp[-2].astnode);
                      (yyval.astnode)->S2 = (yyvsp[0].astnode);
                      (yyval.astnode)->operator = (yyvsp[-1].operator);
                      (yyval.astnode)->A_Declared_Type;
                      (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
                      (yyval.astnode)->symbol = Insert((yyval.astnode)->name, (yyval.astnode)->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
                      OFFSET++;
                     }
#line 2157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 655 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_PLUS; }
#line 2163 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 656 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_MINUS; }
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 660 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 662 "lab9.y" /* yacc.c:1646  */
    { 
         /* Checks if the type matches and if it does not then an error is printed. */
         if ((yyvsp[-2].astnode)->A_Declared_Type != (yyvsp[0].astnode)->A_Declared_Type) {
             yyerror ("Type mismatch");
             exit(1);
         } // end of if

         /* Checks if the type matches with the correct operation. If it does not, then an error is printed. Updated to include leftshift and rightshift. */
         if ((yyvsp[-2].astnode)->A_Declared_Type == A_Decaf_BOOL && (((yyvsp[-1].operator) == A_TIMES) || ((yyvsp[-1].operator) == A_DIVIDE) || ((yyvsp[-1].operator) == A_MOD) || ((yyvsp[-1].operator) == A_LEFTSHIFT) || ((yyvsp[-1].operator) == A_RIGHTSHIFT))) {
             yyerror("Cannot use Booleans in an arithmetic operation");
             exit(1);     
         } // end of if

         /* Checks if the type matches with the correct operation. If it does not, then an error is printed. Updated to not include leftshift and rightshift. */
         if ((yyvsp[-2].astnode)->A_Declared_Type == A_Decaf_INT && (((yyvsp[-1].operator) == A_AND) || ((yyvsp[-1].operator) == A_OR))) {
             yyerror("Cannot use INTEGER in boolean operation");
             exit(1);
         } // end of if

         (yyval.astnode) = ASTCreateNode(A_EXPR);
         (yyval.astnode)->S1 = (yyvsp[-2].astnode);
         (yyval.astnode)->A_Declared_Type = (yyvsp[-2].astnode)->A_Declared_Type;
         (yyval.astnode)->S2 = (yyvsp[0].astnode);
         (yyval.astnode)->operator = (yyvsp[-1].operator);
         (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
         (yyval.astnode)->symbol = Insert((yyval.astnode)->name, (yyval.astnode)->A_Declared_Type, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
         OFFSET++;
       }
#line 2208 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 693 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_TIMES; }
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 694 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_DIVIDE; }
#line 2220 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 695 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_MOD; }
#line 2226 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 696 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_AND; }
#line 2232 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 697 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_OR; }
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 698 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_LEFTSHIFT; }
#line 2244 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 699 "lab9.y" /* yacc.c:1646  */
    { (yyval.operator) = A_RIGHTSHIFT; }
#line 2250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 704 "lab9.y" /* yacc.c:1646  */
    {  
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search((yyvsp[0].string), LEVEL, 1);

           /* Checks if the symbol on RVALUE is defined or not. If it is not defined, then an error message is printed. */
           if (p == NULL) {
               yyerror("Symbol on RVALUE not defined");
               yyerror((yyvsp[0].string));
               exit(1);
           } // end of if

           /* Checks if the type is a scalar. If it is not a scalar, then an error is printed. */
           if (p->SubType != ID_Sub_Type_Scalar) {
               yyerror("Needs to be a scalar, wrong subtype");
               exit(1);
           } // end of if

           (yyval.astnode) = ASTCreateNode(A_VAR_RVALUE);
           (yyval.astnode)->name = (yyvsp[0].string);
           (yyval.astnode)->symbol = p;
           (yyval.astnode)->A_Declared_Type = p->Type;
         }
#line 2278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 727 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 729 "lab9.y" /* yacc.c:1646  */
    { 
           /* Creates and sets up a pointer called p. */
           struct SymbTab *p;
           p = Search((yyvsp[-3].string), LEVEL, 1);

           /* Checks if the symbol on RVALUE is defined or not. If it is not defined, then an error message is printed. */
           if (p == NULL) {
               yyerror("Symbol on RVALUE not defined");
               yyerror((yyvsp[-3].string));
               exit(1);
           } // end of if

           /* Checks if the type is an array. If it is not an array, then an error is printed. */
           if (p->SubType != ID_Sub_Type_Array) {
               yyerror("Needs to be a array, wrong subtype");
               exit(1);
           } // end of if

           (yyval.astnode) = ASTCreateNode(A_VAR_RVALUE);
           (yyval.astnode)->name = (yyvsp[-3].string);
           (yyval.astnode)->S1 = (yyvsp[-1].astnode);
           (yyval.astnode)->symbol = p;
           (yyval.astnode)->A_Declared_Type = p->Type;
         }
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 753 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 754 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[-1].astnode); }
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 756 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_CONSTANT_STRING);
           (yyval.astnode)->name = (yyvsp[0].string);
           (yyval.astnode)->A_Declared_Type = A_Decaf_STRING;
         }
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 761 "lab9.y" /* yacc.c:1646  */
    {  
          /* Checks if the type matches and if it does not then an error is printed. */
          if ((yyvsp[0].astnode)->A_Declared_Type != A_Decaf_BOOL ) {
              yyerror ("Type mismatch , expecting a boolean");
              exit(1);
          } // end of if

          (yyval.astnode) = ASTCreateNode(A_EXPR);
          (yyval.astnode)-> operator = A_NOT;
          (yyval.astnode)->S1 = (yyvsp[0].astnode);
          (yyval.astnode)->A_Declared_Type = A_Decaf_BOOL;
          (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
          (yyval.astnode)->symbol = Insert((yyval.astnode)->name, A_Decaf_BOOL, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
          OFFSET++;
         }
#line 2354 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 777 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_EXPR);
           (yyval.astnode)-> operator = A_UMINUS;
           (yyval.astnode)->S1 = (yyvsp[0].astnode);
           (yyval.astnode)->A_Declared_Type = A_Decaf_INT;
           (yyval.astnode)->name = TEMP_CREATE(); /* Creates a label and other information for a calculation. */
           (yyval.astnode)->symbol = Insert((yyval.astnode)->name, A_Decaf_INT, ID_Sub_Type_Scalar, LEVEL, 1, OFFSET, NULL); /* Inserts the label and other information for the calculation in the symbol table. */
           OFFSET++;
         }
#line 2367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 788 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = A_Decaf_STRING; }
#line 2373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 789 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = (yyvsp[0].asttype); }
#line 2379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 793 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = A_Decaf_INT; }
#line 2385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 794 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = A_Decaf_BOOL; }
#line 2391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 798 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = A_Decaf_VOID; }
#line 2397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 799 "lab9.y" /* yacc.c:1646  */
    { (yyval.asttype) = (yyvsp[0].asttype); }
#line 2403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 804 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_CONSTANT_BOOL);
                 (yyval.astnode)->value = 1;
                 (yyval.astnode)->A_Declared_Type = A_Decaf_BOOL;
               }
#line 2412 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 809 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_CONSTANT_BOOL);
                 (yyval.astnode)->value = 0;
                 (yyval.astnode)->A_Declared_Type = A_Decaf_BOOL;
               }
#line 2421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 817 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_ARRAYTYPE);
              (yyval.astnode)->value = (yyvsp[-2].value);
              (yyval.astnode)->A_Declared_Type = (yyvsp[0].asttype);
            }
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 825 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = ASTCreateNode(A_CONSTANT_INT);
             (yyval.astnode)->value = (yyvsp[0].value);
             (yyval.astnode)->A_Declared_Type = A_Decaf_INT;
           }
#line 2439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 829 "lab9.y" /* yacc.c:1646  */
    { (yyval.astnode) = (yyvsp[0].astnode); }
#line 2445 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2449 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 832 "lab9.y" /* yacc.c:1906  */
	/* end of rules, start of program */

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
