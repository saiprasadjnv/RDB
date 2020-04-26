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
#line 2 "Parser.y" /* yacc.c:339  */


	#include "ParseTree.h" 
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <iostream>

	extern "C" int yylex();
	extern "C" int yyparse();
	extern "C" void yyerror(char *s);
  
	// these data structures hold the result of the parsing
	struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
	struct TableList *tables; // the list of tables and aliases in the query
	struct AndList *boolean; // the predicate in the WHERE clause
	struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
	struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
	int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
	int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query
	struct SchemaAttributes *schemaAttributes; // list of the attributes to create
	char *fileType; //fileType
	char *fileName; // file from which data is loaded in insert command.
	struct SortAttributes *sortAttributes; // attributes to sort on, used for sortedfile
	int operationType; //Operation type like create|select|insert..


#line 94 "y.tab.c" /* yacc.c:339  */

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
    Name = 258,
    Float = 259,
    Int = 260,
    String = 261,
    SELECT = 262,
    GROUP = 263,
    DISTINCT = 264,
    BY = 265,
    FROM = 266,
    WHERE = 267,
    SUM = 268,
    AS = 269,
    AND = 270,
    OR = 271,
    CREATE = 272,
    TABLE = 273,
    HEAP = 274,
    SORTED = 275,
    ON = 276,
    INTEGER = 277,
    DBL = 278,
    STR = 279,
    INSERT = 280,
    INTO = 281,
    DROP = 282,
    SET = 283,
    OUTPUT = 284,
    STDOUT = 285,
    NONE = 286,
    UPDATE = 287,
    STATISTICS = 288,
    FOR = 289
  };
#endif
/* Tokens.  */
#define Name 258
#define Float 259
#define Int 260
#define String 261
#define SELECT 262
#define GROUP 263
#define DISTINCT 264
#define BY 265
#define FROM 266
#define WHERE 267
#define SUM 268
#define AS 269
#define AND 270
#define OR 271
#define CREATE 272
#define TABLE 273
#define HEAP 274
#define SORTED 275
#define ON 276
#define INTEGER 277
#define DBL 278
#define STR 279
#define INSERT 280
#define INTO 281
#define DROP 282
#define SET 283
#define OUTPUT 284
#define STDOUT 285
#define NONE 286
#define UPDATE 287
#define STATISTICS 288
#define FOR 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 31 "Parser.y" /* yacc.c:355  */

 	struct FuncOperand *myOperand;
	struct FuncOperator *myOperator; 
	struct TableList *myTables;
	struct ComparisonOp *myComparison;
	struct Operand *myBoolOperand;
	struct OrList *myOrList;
	struct AndList *myAndList;
	struct NameList *myNames;
	char *actualChars;
	char whichOne;
	struct SchemaAttributes *myCreateAtts;
	struct SortAttributes *mySortAtts;

#line 217 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 234 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   105

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      37,    38,    41,    40,    36,    39,     2,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
      43,    45,    44,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   106,   106,   118,   130,   145,   160,   176,   192,   207,
     222,   237,   252,   267,   282,   297,   312,   327,   343,   349,
     354,   361,   369,   375,   381,   388,   395,   403,   413,   426,
     436,   442,   451,   462,   467,   472,   477,   483,   497,   506,
     514,   523,   532,   539,   546,   554,   562,   570,   578,   590,
     598,   606,   615,   620,   627,   635,   642,   649,   657,   665,
     673,   681,   688,   694,   702,   707
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "Name", "Float", "Int", "String",
  "SELECT", "GROUP", "DISTINCT", "BY", "FROM", "WHERE", "SUM", "AS", "AND",
  "OR", "CREATE", "TABLE", "HEAP", "SORTED", "ON", "INTEGER", "DBL", "STR",
  "INSERT", "INTO", "DROP", "SET", "OUTPUT", "STDOUT", "NONE", "UPDATE",
  "STATISTICS", "FOR", "';'", "','", "'('", "')'", "'-'", "'+'", "'*'",
  "'/'", "'<'", "'>'", "'='", "$accept", "SQL", "WhatIWant", "Function",
  "Atts", "Tables", "CompoundExp", "Op", "AndList", "OrList", "Condition",
  "BoolComp", "Literal", "SimpleExp", "WhatCWant", "CAtts", "CTables",
  "sAtts", "fleType", YY_NULLPTR
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
     285,   286,   287,   288,   289,    59,    44,    40,    41,    45,
      43,    42,    47,    60,    62,    61
};
# endif

#define YYPACT_NINF -71

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-71)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      15,     5,    13,    27,    16,   -10,     8,    46,   -71,    56,
      -2,    49,    25,    26,    60,    38,    60,    14,    31,   -71,
      26,    29,    -1,    64,    56,    65,    33,    34,    60,    35,
      37,    39,    40,    60,    -1,   -71,   -71,   -71,    -1,    -1,
      41,     9,    59,     1,    26,   -71,    60,    73,    63,    43,
     -71,   -71,   -71,   -71,    45,    44,    47,   -71,   -71,   -71,
     -71,   -71,   -71,    -1,    78,    50,    80,   -71,   -12,    48,
      -3,   -71,   -71,   -71,     9,   -71,   -71,    52,    76,    74,
      53,    54,    55,   -71,    57,    72,   -71,    -1,   -71,   -71,
     -71,   -71,    58,    79,   -20,    84,    94,    73,    73,    73,
     -71,    95,   -71,    86,    52,   -71,   -71,   -71,    52,    56,
     -71,   -71,   -71,   -71,    17,   -71,    50,   -71,   -71,    26,
     -71,    95,   -71,   -71
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,     0,    19,    20,     0,     0,     0,     0,     0,     1,
      21,     0,     0,     0,     0,     0,    59,     0,     0,     8,
      10,    12,    14,     0,     0,    51,    49,    50,     0,     0,
       0,    31,     0,     0,    18,    25,     0,     0,     0,     7,
       9,    11,    13,    15,    16,     0,     0,    32,    22,    33,
      34,    35,    36,     0,     0,     0,     0,    60,     0,     0,
       0,     6,    17,    23,    30,    28,    26,     0,     2,     0,
      53,    54,    55,    52,    65,     0,     4,     0,    48,    46,
      47,    45,     0,    40,     0,     0,     0,     0,     0,     0,
      64,     0,    29,    38,     0,    42,    43,    44,     0,     0,
      27,    56,    57,    58,    63,     5,     0,    39,    41,     3,
      61,     0,    37,    62
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -71,   -71,   -71,   -71,    -9,   -71,   -33,    28,   -17,     0,
     -71,   -71,    -5,   -71,   -71,   -70,    -7,   -16,   -71
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,    11,    12,    13,    43,    40,    63,    78,    92,
      93,   108,    94,    41,    48,    69,    27,   115,    86
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      20,    55,    35,    36,    37,    56,    57,    21,     8,    29,
      80,    81,    82,    65,     9,    44,    84,    85,    10,    17,
      30,    49,     1,   105,   106,   107,    54,   111,   112,   113,
      75,    14,     2,    15,    16,    22,    38,    66,    39,    67,
       3,    18,     4,     5,    31,    32,    19,     6,    59,    60,
      61,    62,   120,   121,   102,    88,    89,    90,    91,     8,
      23,    24,    25,    26,    28,    33,    34,    42,    45,    46,
      50,    47,    51,    64,    52,    53,    68,    70,    71,    58,
      72,    76,    73,    79,    95,    74,    83,    77,    96,    97,
      98,    99,   100,   101,   109,   104,   103,   110,   114,   122,
     119,   116,    87,   118,   117,   123
};

static const yytype_uint8 yycheck[] =
{
       9,    34,     3,     4,     5,    38,    39,     9,     3,    16,
      22,    23,    24,    12,     9,    24,    19,    20,    13,    29,
       6,    28,     7,    43,    44,    45,    33,    97,    98,    99,
      63,    18,    17,     6,    18,    37,    37,    36,    39,    46,
      25,    33,    27,    28,    30,    31,     0,    32,    39,    40,
      41,    42,    35,    36,    87,     3,     4,     5,     6,     3,
      11,    36,    36,     3,    26,    34,    37,     3,     3,    36,
      35,    37,    35,    14,    35,    35,     3,    14,    35,    38,
      35,     3,    38,     3,     8,    38,    38,    37,    14,    36,
      36,    36,    35,    21,    10,    16,    38,     3,     3,   116,
     109,    15,    74,   108,   104,   121
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    17,    25,    27,    28,    32,    47,     3,     9,
      13,    48,    49,    50,    18,     6,    18,    29,    33,     0,
      50,     9,    37,    11,    36,    36,     3,    62,    26,    62,
       6,    30,    31,    34,    37,     3,     4,     5,    37,    39,
      52,    59,     3,    51,    50,     3,    36,    37,    60,    62,
      35,    35,    35,    35,    62,    52,    52,    52,    38,    39,
      40,    41,    42,    53,    14,    12,    36,    62,     3,    61,
      14,    35,    35,    38,    38,    52,     3,    37,    54,     3,
      22,    23,    24,    38,    19,    20,    64,    53,     3,     4,
       5,     6,    55,    56,    58,     8,    14,    36,    36,    36,
      35,    21,    52,    38,    16,    43,    44,    45,    57,    10,
       3,    61,    61,    61,     3,    63,    15,    55,    58,    50,
      35,    36,    54,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    48,    48,
      48,    48,    49,    49,    50,    50,    51,    51,    52,    52,
      52,    52,    52,    53,    53,    53,    53,    54,    54,    55,
      55,    56,    57,    57,    57,    58,    58,    58,    58,    59,
      59,    59,    60,    61,    61,    61,    61,    61,    61,    62,
      62,    63,    63,    63,    64,    64
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     6,     9,     6,     8,     5,     4,     3,     4,
       3,     4,     3,     4,     3,     4,     4,     5,     3,     1,
       1,     2,     4,     5,     1,     3,     3,     5,     3,     5,
       3,     1,     2,     1,     1,     1,     1,     5,     3,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     2,     2,     4,     4,     4,     1,
       3,     2,     3,     1,     2,     1
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
#line 107 "Parser.y" /* yacc.c:1646  */
    {
	fileType =NULL;
	groupingAtts = NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileName = NULL;
	tables = (yyvsp[-2].myTables);
	boolean = (yyvsp[0].myAndList);	
	operationType=1;
}
#line 1398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 119 "Parser.y" /* yacc.c:1646  */
    {
	fileType =NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileName = NULL;
	tables = (yyvsp[-5].myTables);
	boolean = (yyvsp[-3].myAndList);	
	groupingAtts = (yyvsp[0].myNames);
	operationType=1;
}
#line 1413 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 131 "Parser.y" /* yacc.c:1646  */
    {
	boolean = NULL;	
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	sortAttributes=NULL;
	attsToSelect=NULL;
	fileName = NULL;
	tables = (yyvsp[-3].myTables);
	operationType=2;
	fileType= strdup("HEAP");
}
#line 1431 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 146 "Parser.y" /* yacc.c:1646  */
    {
	boolean = NULL;	
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	fileName = NULL;
	tables = (yyvsp[-5].myTables);
	fileType = (char*) malloc(sizeof ("SORTED"));
	strcpy(fileType,"SORTED");
	sortAttributes=(yyvsp[0].mySortAtts);
	operationType=2;
}
#line 1450 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 161 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables = (yyvsp[-1].myTables);
	fileName = (yyvsp[-3].actualChars);
	operationType=3;

}
#line 1470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 177 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables = (yyvsp[0].myTables);
	fileName = (yyvsp[-2].actualChars);
	operationType=3;

}
#line 1490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 193 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = (yyvsp[0].myTables);
	operationType=4;
}
#line 1509 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 208 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = (yyvsp[-1].myTables);
	operationType=4;
}
#line 1528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 223 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup((yyvsp[0].actualChars));
	operationType=5;
}
#line 1547 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 238 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup((yyvsp[-1].actualChars));
	operationType=5;
}
#line 1566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 253 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName=strdup("STDOUT");
	operationType=5;
}
#line 1585 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 268 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName=strdup("STDOUT");
	operationType=5;
}
#line 1604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 283 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup("NONE");
	operationType=5;
}
#line 1623 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 298 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup("NONE");
	operationType=5;
}
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 313 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = (yyvsp[0].myTables);
	operationType=6;
}
#line 1661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 328 "Parser.y" /* yacc.c:1646  */
    {
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = (yyvsp[-1].myTables);
	operationType=6;
}
#line 1680 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 344 "Parser.y" /* yacc.c:1646  */
    {
	attsToSelect = (yyvsp[0].myNames);
	distinctAtts = 0;
}
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 350 "Parser.y" /* yacc.c:1646  */
    {
	attsToSelect = NULL;
}
#line 1697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 355 "Parser.y" /* yacc.c:1646  */
    {
	distinctAtts = 0;
	finalFunction = NULL;
	attsToSelect = (yyvsp[0].myNames);
}
#line 1707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 362 "Parser.y" /* yacc.c:1646  */
    {
	distinctAtts = 1;
	finalFunction = NULL;
	attsToSelect = (yyvsp[0].myNames);
	finalFunction = NULL;
}
#line 1718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 370 "Parser.y" /* yacc.c:1646  */
    {
	distinctFunc = 0;
	finalFunction = (yyvsp[-1].myOperator);
}
#line 1727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 376 "Parser.y" /* yacc.c:1646  */
    {
	distinctFunc = 1;
	finalFunction = (yyvsp[-1].myOperator);
}
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 382 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myNames) = (struct NameList *) malloc (sizeof (struct NameList));
	(yyval.myNames)->name = (yyvsp[0].actualChars);
	(yyval.myNames)->next = NULL;
}
#line 1746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 389 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myNames) = (struct NameList *) malloc (sizeof (struct NameList));
	(yyval.myNames)->name = (yyvsp[0].actualChars);
	(yyval.myNames)->next = (yyvsp[-2].myNames);
}
#line 1756 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 396 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myTables) = (struct TableList *) malloc (sizeof (struct TableList));
	(yyval.myTables)->tableName = (yyvsp[-2].actualChars);
	(yyval.myTables)->aliasAs = (yyvsp[0].actualChars);
	(yyval.myTables)->next = NULL;
}
#line 1767 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 404 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myTables) = (struct TableList *) malloc (sizeof (struct TableList));
	(yyval.myTables)->tableName = (yyvsp[-2].actualChars);
	(yyval.myTables)->aliasAs = (yyvsp[0].actualChars);
	(yyval.myTables)->next = (yyvsp[-4].myTables);
}
#line 1778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 414 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));
	(yyval.myOperator)->leftOperator->leftOperator = NULL;
	(yyval.myOperator)->leftOperator->leftOperand = (yyvsp[-2].myOperand);
	(yyval.myOperator)->leftOperator->right = NULL;
	(yyval.myOperator)->leftOperand = NULL;
	(yyval.myOperator)->right = (yyvsp[0].myOperator);
	(yyval.myOperator)->code = (yyvsp[-1].whichOne);	

}
#line 1794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 427 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = (yyvsp[-3].myOperator);
	(yyval.myOperator)->leftOperand = NULL;
	(yyval.myOperator)->right = (yyvsp[0].myOperator);
	(yyval.myOperator)->code = (yyvsp[-1].whichOne);	

}
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 437 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myOperator) = (yyvsp[-1].myOperator);

}
#line 1816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 443 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = NULL;
	(yyval.myOperator)->leftOperand = (yyvsp[0].myOperand);
	(yyval.myOperator)->right = NULL;	

}
#line 1828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 452 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = (yyvsp[0].myOperator);
	(yyval.myOperator)->leftOperand = NULL;
	(yyval.myOperator)->right = NULL;	
	(yyval.myOperator)->code = '-';

}
#line 1841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 463 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.whichOne) = '-';
}
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 468 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.whichOne) = '+';
}
#line 1857 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 473 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.whichOne) = '*';
}
#line 1865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 478 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.whichOne) = '/';
}
#line 1873 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 484 "Parser.y" /* yacc.c:1646  */
    {
        // here we need to pre-pend the OrList to the AndList
        // first we allocate space for this node
        (yyval.myAndList) = (struct AndList *) malloc (sizeof (struct AndList));

        // hang the OrList off of the left
        (yyval.myAndList)->left = (yyvsp[-3].myOrList);

        // hang the AndList off of the right
        (yyval.myAndList)->rightAnd = (yyvsp[0].myAndList);

}
#line 1890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 498 "Parser.y" /* yacc.c:1646  */
    {
        // just return the OrList!
        (yyval.myAndList) = (struct AndList *) malloc (sizeof (struct AndList));
        (yyval.myAndList)->left = (yyvsp[-1].myOrList);
        (yyval.myAndList)->rightAnd = NULL;
}
#line 1901 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 507 "Parser.y" /* yacc.c:1646  */
    {
        // here we have to hang the condition off the left of the OrList
        (yyval.myOrList) = (struct OrList *) malloc (sizeof (struct OrList));
        (yyval.myOrList)->left = (yyvsp[-2].myComparison);
        (yyval.myOrList)->rightOr = (yyvsp[0].myOrList);
}
#line 1912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 515 "Parser.y" /* yacc.c:1646  */
    {
        // nothing to hang off of the right
        (yyval.myOrList) = (struct OrList *) malloc (sizeof (struct OrList));
        (yyval.myOrList)->left = (yyvsp[0].myComparison);
        (yyval.myOrList)->rightOr = NULL;
}
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 524 "Parser.y" /* yacc.c:1646  */
    {
        // in this case we have a simple literal/variable comparison
        (yyval.myComparison) = (yyvsp[-1].myComparison);
        (yyval.myComparison)->left = (yyvsp[-2].myBoolOperand);
        (yyval.myComparison)->right = (yyvsp[0].myBoolOperand);
}
#line 1934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 533 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the comparison
        (yyval.myComparison) = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        (yyval.myComparison)->code = LESS_THAN;
}
#line 1944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 540 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the comparison
        (yyval.myComparison) = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        (yyval.myComparison)->code = GREATER_THAN;
}
#line 1954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 547 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the comparison
        (yyval.myComparison) = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        (yyval.myComparison)->code = EQUALS;
}
#line 1964 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 555 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the string
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = STRING;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1975 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 563 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the FP number
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = DOUBLE;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 571 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the integer
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = INT;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1997 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 579 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the name
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = NAME;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 2008 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 591 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the FP number
        (yyval.myOperand) = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        (yyval.myOperand)->code = DOUBLE;
        (yyval.myOperand)->value = (yyvsp[0].actualChars);
}
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 599 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the integer
        (yyval.myOperand) = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        (yyval.myOperand)->code = INT;
        (yyval.myOperand)->value = (yyvsp[0].actualChars);
}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 607 "Parser.y" /* yacc.c:1646  */
    {
        // construct and send up the operand containing the name
        (yyval.myOperand) = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        (yyval.myOperand)->code = NAME;
        (yyval.myOperand)->value = (yyvsp[0].actualChars);
}
#line 2041 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 616 "Parser.y" /* yacc.c:1646  */
    {
	schemaAttributes=(yyvsp[-1].myCreateAtts);
}
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 621 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myCreateAtts) = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	(yyval.myCreateAtts)->name=(yyvsp[-1].actualChars);
	(yyval.myCreateAtts)->type=2;
	(yyval.myCreateAtts)->next=NULL;
}
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 628 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myCreateAtts) = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	(yyval.myCreateAtts)->name=(yyvsp[-1].actualChars);
	(yyval.myCreateAtts)->type=1;
	(yyval.myCreateAtts)->next=NULL;

}
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 636 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myCreateAtts) = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	(yyval.myCreateAtts)->name=(yyvsp[-1].actualChars);
	(yyval.myCreateAtts)->type=4;
	(yyval.myCreateAtts)->next=NULL;
}
#line 2083 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 643 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myCreateAtts) = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	(yyval.myCreateAtts)->name=(yyvsp[-3].actualChars);
	(yyval.myCreateAtts)->type=2;
	(yyval.myCreateAtts)->next=(yyvsp[0].myCreateAtts);
}
#line 2094 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 650 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myCreateAtts) = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	(yyval.myCreateAtts)->name=(yyvsp[-3].actualChars);
	(yyval.myCreateAtts)->type=1;
	(yyval.myCreateAtts)->next=(yyvsp[0].myCreateAtts);

}
#line 2106 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 658 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myCreateAtts) = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	(yyval.myCreateAtts)->name=(yyvsp[-3].actualChars);
	(yyval.myCreateAtts)->type=4;
	(yyval.myCreateAtts)->next=(yyvsp[0].myCreateAtts);
}
#line 2117 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 666 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myTables) = (struct TableList *) malloc (sizeof (struct TableList));
	(yyval.myTables)->tableName = (yyvsp[0].actualChars);
	(yyval.myTables)->aliasAs=NULL;
	(yyval.myTables)->next = NULL;
}
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 674 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.myTables) = (struct TableList *) malloc (sizeof (struct TableList));
	(yyval.myTables)->tableName = (yyvsp[-2].actualChars);
	(yyval.myTables)->aliasAs=NULL;
	(yyval.myTables)->next = (yyvsp[0].myTables);
}
#line 2139 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 682 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.mySortAtts) = (struct SortAttributes*) malloc (sizeof (struct SortAttributes));
	(yyval.mySortAtts)->name=(yyvsp[-1].actualChars);
	(yyval.mySortAtts)->next=NULL;

}
#line 2150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 689 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.mySortAtts) = (struct SortAttributes*) malloc (sizeof (struct SortAttributes));
	(yyval.mySortAtts)->name=(yyvsp[-2].actualChars);
	(yyval.mySortAtts)->next=(yyvsp[0].mySortAtts);
}
#line 2160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 695 "Parser.y" /* yacc.c:1646  */
    {
	(yyval.mySortAtts) = (struct SortAttributes*) malloc (sizeof (struct SortAttributes));
	(yyval.mySortAtts)->name=(yyvsp[0].actualChars);
	(yyval.mySortAtts)->next=NULL;

}
#line 2171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 703 "Parser.y" /* yacc.c:1646  */
    {
	fileType = (char*) malloc(sizeof ("HEAP"));
	strcpy(fileType,"HEAP");
}
#line 2180 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 708 "Parser.y" /* yacc.c:1646  */
    {
	fileType = (char*) malloc(sizeof ("HEAP"));
	strcpy(fileType,"HEAP");
}
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2193 "y.tab.c" /* yacc.c:1646  */
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
#line 713 "Parser.y" /* yacc.c:1906  */


