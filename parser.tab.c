/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 8 "parser.y"

#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
int yylex();

#line 83 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LC = 3,                         /* LC  */
  YYSYMBOL_RC = 4,                         /* RC  */
  YYSYMBOL_LP = 5,                         /* LP  */
  YYSYMBOL_RP = 6,                         /* RP  */
  YYSYMBOL_SEMI = 7,                       /* SEMI  */
  YYSYMBOL_LB = 8,                         /* LB  */
  YYSYMBOL_RB = 9,                         /* RB  */
  YYSYMBOL_COMMA = 10,                     /* COMMA  */
  YYSYMBOL_INT = 11,                       /* INT  */
  YYSYMBOL_CHAR = 12,                      /* CHAR  */
  YYSYMBOL_FLOAT = 13,                     /* FLOAT  */
  YYSYMBOL_ID = 14,                        /* ID  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_FOR = 18,                       /* FOR  */
  YYSYMBOL_BREAK = 19,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 20,                  /* CONTINUE  */
  YYSYMBOL_SCAN = 21,                      /* SCAN  */
  YYSYMBOL_PRINT = 22,                     /* PRINT  */
  YYSYMBOL_TYPE_INT = 23,                  /* TYPE_INT  */
  YYSYMBOL_TYPE_FLOAT = 24,                /* TYPE_FLOAT  */
  YYSYMBOL_TYPE_CHAR = 25,                 /* TYPE_CHAR  */
  YYSYMBOL_ASSIGNOP = 26,                  /* ASSIGNOP  */
  YYSYMBOL_PLUS = 27,                      /* PLUS  */
  YYSYMBOL_MINUS = 28,                     /* MINUS  */
  YYSYMBOL_STAR = 29,                      /* STAR  */
  YYSYMBOL_DIV = 30,                       /* DIV  */
  YYSYMBOL_PLUS_ASSIGN = 31,               /* PLUS_ASSIGN  */
  YYSYMBOL_MINUS_ASSIGN = 32,              /* MINUS_ASSIGN  */
  YYSYMBOL_STAR_ASSIGN = 33,               /* STAR_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 34,                /* DIV_ASSIGN  */
  YYSYMBOL_INCREMENT = 35,                 /* INCREMENT  */
  YYSYMBOL_DECREMENT = 36,                 /* DECREMENT  */
  YYSYMBOL_EQ = 37,                        /* EQ  */
  YYSYMBOL_NE = 38,                        /* NE  */
  YYSYMBOL_GT = 39,                        /* GT  */
  YYSYMBOL_GE = 40,                        /* GE  */
  YYSYMBOL_LT = 41,                        /* LT  */
  YYSYMBOL_LE = 42,                        /* LE  */
  YYSYMBOL_UMINUS = 43,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_program = 45,                   /* program  */
  YYSYMBOL_StmList = 46,                   /* StmList  */
  YYSYMBOL_CompSt = 47,                    /* CompSt  */
  YYSYMBOL_Stmt = 48,                      /* Stmt  */
  YYSYMBOL_Var = 49,                       /* Var  */
  YYSYMBOL_VarDecl = 50,                   /* VarDecl  */
  YYSYMBOL_ArrayDecl = 51,                 /* ArrayDecl  */
  YYSYMBOL_ArrayDims = 52,                 /* ArrayDims  */
  YYSYMBOL_Exp = 53                        /* Exp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   257

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  114

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   298


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    54,    54,    70,    73,    82,    90,    95,    99,   104,
     109,   115,   122,   128,   137,   141,   145,   149,   156,   160,
     169,   175,   181,   190,   197,   204,   214,   218,   227,   231,
     235,   240,   244,   250,   256,   262,   268,   274,   279,   284,
     289,   294,   300,   306,   312,   318,   324,   330,   336,   342,
     348,   354,   359
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LC", "RC", "LP", "RP",
  "SEMI", "LB", "RB", "COMMA", "INT", "CHAR", "FLOAT", "ID", "IF", "ELSE",
  "WHILE", "FOR", "BREAK", "CONTINUE", "SCAN", "PRINT", "TYPE_INT",
  "TYPE_FLOAT", "TYPE_CHAR", "ASSIGNOP", "PLUS", "MINUS", "STAR", "DIV",
  "PLUS_ASSIGN", "MINUS_ASSIGN", "STAR_ASSIGN", "DIV_ASSIGN", "INCREMENT",
  "DECREMENT", "EQ", "NE", "GT", "GE", "LT", "LE", "UMINUS", "$accept",
  "program", "StmList", "CompSt", "Stmt", "Var", "VarDecl", "ArrayDecl",
  "ArrayDims", "Exp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-33)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     195,   195,   221,   -33,   -33,   -33,   -33,    -3,    34,    47,
      65,    80,    74,   221,    75,    92,    93,   221,    94,    94,
      48,   -33,   -33,   195,   219,   -33,   -33,   115,   105,    -5,
     221,   221,   195,   -33,   -33,   109,   139,    -4,     7,    43,
     -33,   110,   110,   -33,   -33,   221,   221,   221,   221,   221,
     221,   -33,   -33,   -33,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   -33,   -33,    73,    99,   221,   -33,
     -33,   -33,   106,    53,   -33,    57,   -33,    61,    17,   -21,
     -21,   -21,   -21,   -21,    41,    41,   -33,   -33,     1,     1,
     -17,   -17,   -17,   -17,   195,   195,   155,   111,   -33,   108,
     -33,   -33,   -33,   107,   -33,   195,   -33,   112,   195,   118,
     -33,   -33,   195,   -33
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     3,     0,    28,    30,    29,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     7,     3,    31,    16,    17,     0,     0,     0,
       0,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      51,    37,    38,     1,     4,     0,     0,     0,     0,     0,
       0,    39,    40,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    52,     0,     0,     0,     8,
       9,    20,     0,     0,    21,     0,    22,     0,     0,    32,
      33,    34,    35,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,     0,     0,     0,     0,    23,     0,
      24,    25,    19,    10,    12,     0,    26,     0,     0,     0,
      27,    11,     0,    13
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -33,   -33,     4,   -33,   -32,    56,   -33,   -33,    39,    36
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    73,    27
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      68,    65,    30,    71,    72,    28,    54,    55,    56,    57,
      54,    55,    56,    57,    74,    72,    58,    59,    60,    61,
      62,    63,    54,    55,    56,    57,   102,    44,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    29,    31,
      60,    61,    62,    63,    54,    55,    56,    57,    43,    36,
      76,    72,    32,    40,    58,    59,    60,    61,    62,    63,
      98,    99,   103,   104,   100,    99,    66,    67,   101,    99,
      56,    57,    33,   109,    41,    42,   111,    75,    77,    94,
     113,    78,    79,    80,    81,    82,    83,    34,    35,    37,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      54,    55,    56,    57,    96,    95,    38,    39,     6,    64,
      58,    59,    60,    61,    62,    63,    69,    97,    45,   107,
     106,   110,    53,   108,   112,     0,    54,    55,    56,    57,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,    63,    54,    55,    56,    57,    70,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,    63,     0,     0,
       0,     0,   105,     0,     0,     0,    54,    55,    56,    57,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,    63,    54,    55,    56,    57,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,    63,     1,     0,
       2,     0,     0,     0,     0,     0,     3,     4,     5,     6,
       7,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,     0,     0,    17,     0,     0,     2,    45,     0,     0,
      18,    19,     3,     4,     5,     6,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,     0,     0,     0,    17,
      47,    48,    49,    50,    51,    52,    18,    19
};

static const yytype_int8 yycheck[] =
{
      32,     6,     5,     7,     8,     1,    27,    28,    29,    30,
      27,    28,    29,    30,     7,     8,    37,    38,    39,    40,
      41,    42,    27,    28,    29,    30,     9,    23,    27,    28,
      29,    30,    37,    38,    39,    40,    41,    42,     2,     5,
      39,    40,    41,    42,    27,    28,    29,    30,     0,    13,
       7,     8,     5,    17,    37,    38,    39,    40,    41,    42,
       7,     8,    94,    95,     7,     8,    30,    31,     7,     8,
      29,    30,     7,   105,    18,    19,   108,    38,    39,     6,
     112,    45,    46,    47,    48,    49,    50,     7,    14,    14,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      27,    28,    29,    30,    68,     6,    14,    14,    14,     4,
      37,    38,    39,    40,    41,    42,     7,    11,     8,    11,
       9,     9,     7,    16,     6,    -1,    27,    28,    29,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    27,    28,    29,    30,     7,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,     7,    -1,    -1,    -1,    27,    28,    29,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    27,    28,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,     3,    -1,
       5,    -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    -1,    -1,     5,     8,    -1,    -1,
      35,    36,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    28,
      31,    32,    33,    34,    35,    36,    35,    36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,    11,    12,    13,    14,    15,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    28,    35,    36,
      45,    46,    47,    48,    49,    50,    51,    53,    46,    53,
       5,     5,     5,     7,     7,    14,    53,    14,    14,    14,
      53,    49,    49,     0,    46,     8,    26,    31,    32,    33,
      34,    35,    36,     7,    27,    28,    29,    30,    37,    38,
      39,    40,    41,    42,     4,     6,    53,    53,    48,     7,
       7,     7,     8,    52,     7,    52,     7,    52,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,     6,     6,    53,    11,     7,     8,
       7,     7,     9,    48,    48,     7,     9,    11,    16,    48,
       9,    48,     6,    48
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    49,    49,
      50,    50,    50,    51,    51,    51,    52,    52,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     3,     2,     1,     3,     3,
       5,     7,     5,     8,     2,     2,     1,     1,     1,     4,
       3,     3,     3,     4,     4,     4,     3,     4,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: StmList  */
#line 54 "parser.y"
            {
        (yyval.type_node) = createNode(NODE_PROGRAM);
        (yyval.type_node)->child = (yyvsp[0].type_node);
        displayAST((yyval.type_node), 0);
        checkSemantics((yyval.type_node));
        if (getSemanticErrorCount() == 0) {
            generateTAC((yyval.type_node));        // 语义通过后生成三地址码
            generateMIPSFromTAC();  // 从TAC生成MIPS汇编代码
        } else {
            printf("[跳过代码生成] 语义错误数: %d\n", getSemanticErrorCount());
        }
    }
#line 1615 "parser.tab.c"
    break;

  case 3: /* StmList: %empty  */
#line 70 "parser.y"
    {
        (yyval.type_node) = NULL;
    }
#line 1623 "parser.tab.c"
    break;

  case 4: /* StmList: Stmt StmList  */
#line 74 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_STMLIST);
        (yyval.type_node)->child = (yyvsp[-1].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1633 "parser.tab.c"
    break;

  case 5: /* CompSt: LC StmList RC  */
#line 83 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_COMP_STMT);
        (yyval.type_node)->child = (yyvsp[-1].type_node);
    }
#line 1642 "parser.tab.c"
    break;

  case 6: /* Stmt: Exp SEMI  */
#line 91 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_EXP_STMT);
        (yyval.type_node)->child = (yyvsp[-1].type_node);
    }
#line 1651 "parser.tab.c"
    break;

  case 7: /* Stmt: CompSt  */
#line 96 "parser.y"
    {
        (yyval.type_node) = (yyvsp[0].type_node);
    }
#line 1659 "parser.tab.c"
    break;

  case 8: /* Stmt: SCAN ID SEMI  */
#line 100 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_SCAN_STMT);
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-1].type_id));
    }
#line 1668 "parser.tab.c"
    break;

  case 9: /* Stmt: PRINT Exp SEMI  */
#line 105 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_PRINT_STMT);
        (yyval.type_node)->child = (yyvsp[-1].type_node);
    }
#line 1677 "parser.tab.c"
    break;

  case 10: /* Stmt: IF LP Exp RP Stmt  */
#line 110 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_IF_STMT);
        (yyval.type_node)->child = (yyvsp[-2].type_node);
        (yyval.type_node)->left = (yyvsp[0].type_node);
    }
#line 1687 "parser.tab.c"
    break;

  case 11: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 116 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_IF_ELSE_STMT);
        (yyval.type_node)->child = (yyvsp[-4].type_node);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1698 "parser.tab.c"
    break;

  case 12: /* Stmt: WHILE LP Exp RP Stmt  */
#line 123 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_WHILE_STMT);
        (yyval.type_node)->child = (yyvsp[-2].type_node);
        (yyval.type_node)->left = (yyvsp[0].type_node);
    }
#line 1708 "parser.tab.c"
    break;

  case 13: /* Stmt: FOR LP Stmt Exp SEMI Stmt RP Stmt  */
#line 129 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_FOR_STMT);
        (yyval.type_node)->child = (yyvsp[-5].type_node);  // 初始化语句
        (yyval.type_node)->left = (yyvsp[-4].type_node);   // 条件表达式
        (yyval.type_node)->right = createNode(NODE_STMLIST);
        (yyval.type_node)->right->child = (yyvsp[-2].type_node);  // 更新语句
        (yyval.type_node)->right->right = (yyvsp[0].type_node);  // 循环体
    }
#line 1721 "parser.tab.c"
    break;

  case 14: /* Stmt: BREAK SEMI  */
#line 138 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_BREAK_STMT);
    }
#line 1729 "parser.tab.c"
    break;

  case 15: /* Stmt: CONTINUE SEMI  */
#line 142 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_CONTINUE_STMT);
    }
#line 1737 "parser.tab.c"
    break;

  case 16: /* Stmt: VarDecl  */
#line 146 "parser.y"
    {
        (yyval.type_node) = (yyvsp[0].type_node);
    }
#line 1745 "parser.tab.c"
    break;

  case 17: /* Stmt: ArrayDecl  */
#line 150 "parser.y"
    {
        (yyval.type_node) = (yyvsp[0].type_node);
    }
#line 1753 "parser.tab.c"
    break;

  case 18: /* Var: ID  */
#line 157 "parser.y"
    {
        (yyval.type_node) = createVarNode((yyvsp[0].type_id));
    }
#line 1761 "parser.tab.c"
    break;

  case 19: /* Var: Var LB Exp RB  */
#line 161 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_ARRAY_ACCESS);
        (yyval.type_node)->left = (yyvsp[-3].type_node);
        (yyval.type_node)->right = (yyvsp[-1].type_node);
    }
#line 1771 "parser.tab.c"
    break;

  case 20: /* VarDecl: TYPE_INT ID SEMI  */
#line 170 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_VAR_DECL);
        (yyval.type_node)->decl_type = VAL_INT;
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-1].type_id));
    }
#line 1781 "parser.tab.c"
    break;

  case 21: /* VarDecl: TYPE_FLOAT ID SEMI  */
#line 176 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_VAR_DECL);
        (yyval.type_node)->decl_type = VAL_FLOAT;
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-1].type_id));
    }
#line 1791 "parser.tab.c"
    break;

  case 22: /* VarDecl: TYPE_CHAR ID SEMI  */
#line 182 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_VAR_DECL);
        (yyval.type_node)->decl_type = VAL_CHAR;
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-1].type_id));
    }
#line 1801 "parser.tab.c"
    break;

  case 23: /* ArrayDecl: TYPE_INT ID ArrayDims SEMI  */
#line 191 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_ARRAY_DECL);
        (yyval.type_node)->decl_type = VAL_INT;
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-2].type_id));
        (yyval.type_node)->child = (yyvsp[-1].type_node);  // 多维数组维度信息
    }
#line 1812 "parser.tab.c"
    break;

  case 24: /* ArrayDecl: TYPE_FLOAT ID ArrayDims SEMI  */
#line 198 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_ARRAY_DECL);
        (yyval.type_node)->decl_type = VAL_FLOAT;
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-2].type_id));
        (yyval.type_node)->child = (yyvsp[-1].type_node);  // 多维数组维度信息
    }
#line 1823 "parser.tab.c"
    break;

  case 25: /* ArrayDecl: TYPE_CHAR ID ArrayDims SEMI  */
#line 205 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_ARRAY_DECL);
        (yyval.type_node)->decl_type = VAL_CHAR;
        strcpy((yyval.type_node)->value.var_name, (yyvsp[-2].type_id));
        (yyval.type_node)->child = (yyvsp[-1].type_node);  // 多维数组维度信息
    }
#line 1834 "parser.tab.c"
    break;

  case 26: /* ArrayDims: LB INT RB  */
#line 215 "parser.y"
    {
        (yyval.type_node) = createIntNode((yyvsp[-1].type_int));
    }
#line 1842 "parser.tab.c"
    break;

  case 27: /* ArrayDims: ArrayDims LB INT RB  */
#line 219 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_STMLIST);  // 复用STMLIST节点结构来存储多个维度
        (yyval.type_node)->child = (yyvsp[-3].type_node);
        (yyval.type_node)->right = createIntNode((yyvsp[-1].type_int));
    }
#line 1852 "parser.tab.c"
    break;

  case 28: /* Exp: INT  */
#line 228 "parser.y"
    {
        (yyval.type_node) = createIntNode((yyvsp[0].type_int));
    }
#line 1860 "parser.tab.c"
    break;

  case 29: /* Exp: FLOAT  */
#line 232 "parser.y"
    {
        (yyval.type_node) = createFloatNode((yyvsp[0].type_float));
    }
#line 1868 "parser.tab.c"
    break;

  case 30: /* Exp: CHAR  */
#line 236 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_CHAR_CONST);
        (yyval.type_node)->value.int_val = (yyvsp[0].type_int);
    }
#line 1877 "parser.tab.c"
    break;

  case 31: /* Exp: Var  */
#line 241 "parser.y"
    {
        (yyval.type_node) = (yyvsp[0].type_node);
    }
#line 1885 "parser.tab.c"
    break;

  case 32: /* Exp: Var ASSIGNOP Exp  */
#line 245 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_ASSIGN);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1895 "parser.tab.c"
    break;

  case 33: /* Exp: Var PLUS_ASSIGN Exp  */
#line 251 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_PLUS_ASSIGN);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1905 "parser.tab.c"
    break;

  case 34: /* Exp: Var MINUS_ASSIGN Exp  */
#line 257 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_MINUS_ASSIGN);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1915 "parser.tab.c"
    break;

  case 35: /* Exp: Var STAR_ASSIGN Exp  */
#line 263 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_STAR_ASSIGN);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1925 "parser.tab.c"
    break;

  case 36: /* Exp: Var DIV_ASSIGN Exp  */
#line 269 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_DIV_ASSIGN);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1935 "parser.tab.c"
    break;

  case 37: /* Exp: INCREMENT Var  */
#line 275 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_INCREMENT);
        (yyval.type_node)->left = (yyvsp[0].type_node);
    }
#line 1944 "parser.tab.c"
    break;

  case 38: /* Exp: DECREMENT Var  */
#line 280 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_DECREMENT);
        (yyval.type_node)->left = (yyvsp[0].type_node);
    }
#line 1953 "parser.tab.c"
    break;

  case 39: /* Exp: Var INCREMENT  */
#line 285 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_INCREMENT);
        (yyval.type_node)->left = (yyvsp[-1].type_node);
    }
#line 1962 "parser.tab.c"
    break;

  case 40: /* Exp: Var DECREMENT  */
#line 290 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_DECREMENT);
        (yyval.type_node)->left = (yyvsp[-1].type_node);
    }
#line 1971 "parser.tab.c"
    break;

  case 41: /* Exp: Exp PLUS Exp  */
#line 295 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_PLUS);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1981 "parser.tab.c"
    break;

  case 42: /* Exp: Exp MINUS Exp  */
#line 301 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_MINUS);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 1991 "parser.tab.c"
    break;

  case 43: /* Exp: Exp STAR Exp  */
#line 307 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_STAR);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2001 "parser.tab.c"
    break;

  case 44: /* Exp: Exp DIV Exp  */
#line 313 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_DIV);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2011 "parser.tab.c"
    break;

  case 45: /* Exp: Exp EQ Exp  */
#line 319 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_EQ);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2021 "parser.tab.c"
    break;

  case 46: /* Exp: Exp NE Exp  */
#line 325 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_NE);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2031 "parser.tab.c"
    break;

  case 47: /* Exp: Exp GT Exp  */
#line 331 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_GT);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2041 "parser.tab.c"
    break;

  case 48: /* Exp: Exp GE Exp  */
#line 337 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_GE);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2051 "parser.tab.c"
    break;

  case 49: /* Exp: Exp LT Exp  */
#line 343 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_LT);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2061 "parser.tab.c"
    break;

  case 50: /* Exp: Exp LE Exp  */
#line 349 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_LE);
        (yyval.type_node)->left = (yyvsp[-2].type_node);
        (yyval.type_node)->right = (yyvsp[0].type_node);
    }
#line 2071 "parser.tab.c"
    break;

  case 51: /* Exp: MINUS Exp  */
#line 355 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_UMINUS);
        (yyval.type_node)->left = (yyvsp[0].type_node);
    }
#line 2080 "parser.tab.c"
    break;

  case 52: /* Exp: LP Exp RP  */
#line 360 "parser.y"
    {
        (yyval.type_node) = createNode(NODE_PAREN_EXP);
        (yyval.type_node)->child = (yyvsp[-1].type_node);
    }
#line 2089 "parser.tab.c"
    break;


#line 2093 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 372 "parser.y"


// 将token类型转换为可读字符串
const char* getTokenName(int token) {
	switch(token) {
		case LC: return "LC(左大括号)";
		case RC: return "RC(右大括号)";
		case LP: return "LP(左小括号)";
		case RP: return "RP(右小括号)";
		case LB: return "LB(左中括号)";
		case RB: return "RB(右中括号)";
		case SEMI: return "SEMI(分号)";
		case COMMA: return "COMMA(逗号)";
		case IF: return "IF(关键字)";
		case ELSE: return "ELSE(关键字)";
		case WHILE: return "WHILE(关键字)";
		case FOR: return "FOR(关键字)";
		case BREAK: return "BREAK(关键字)";
		case CONTINUE: return "CONTINUE(关键字)";
		case TYPE_INT: return "TYPE_INT(类型)";
		case TYPE_FLOAT: return "TYPE_FLOAT(类型)";
		case TYPE_CHAR: return "TYPE_CHAR(类型)";
		case SCAN: return "SCAN(关键字)";
		case PRINT: return "PRINT(关键字)";
		case ASSIGNOP: return "ASSIGNOP(赋值)";
		case PLUS_ASSIGN: return "PLUS_ASSIGN(+=)";
		case MINUS_ASSIGN: return "MINUS_ASSIGN(-=)";
		case STAR_ASSIGN: return "STAR_ASSIGN(*=)";
		case DIV_ASSIGN: return "DIV_ASSIGN(/=)";
		case INCREMENT: return "INCREMENT(++)";
		case DECREMENT: return "DECREMENT(--)";
		case PLUS: return "PLUS(+)";
		case MINUS: return "MINUS(-)";
		case STAR: return "STAR(*)";
		case DIV: return "DIV(/)";
		case EQ: return "EQ(==)";
		case NE: return "NE(!=)";
		case GE: return "GE(>=)";
		case LE: return "LE(<=)";
		case GT: return "GT(>)";
		case LT: return "LT(<)";
		case INT: return "INT(整数常量)";
		case FLOAT: return "FLOAT(浮点常量)";
		case CHAR: return "CHAR(字符常量)";
		case ID: return "ID(标识符)";
		default: return "UNKNOWN";
	}
}

// 词法分析：扫描并输出所有token
void performLexicalAnalysis(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) return;
	
	// 使用flex进行词法分析
	yyin = fp;
	yylineno = 1;
	
	printf("===== 词法分析结果 =====\n");
	printf("%-6s %-25s %-20s %s\n", "行号", "Token类型", "Token值", "词素");
	printf("-------------------------------------------------------------------\n");
	
	int token;
	int tokenCount = 0;
	while ((token = yylex()) != 0) {
		tokenCount++;
		printf("%-6d %-25s ", yylineno, getTokenName(token));
		
		// 根据token类型输出值
		switch(token) {
			case INT:
				printf("%-20d %s\n", yylval.type_int, yytext);
				break;
			case FLOAT:
				printf("%-20.2f %s\n", yylval.type_float, yytext);
				break;
			case CHAR:
				printf("%-20c %s\n", (char)yylval.type_int, yytext);
				break;
			case ID:
				printf("%-20s %s\n", yylval.type_id, yytext);
				break;
			default:
				printf("%-20s %s\n", "-", yytext);
				break;
		}
	}
	
	printf("-------------------------------------------------------------------\n");
	printf("共识别 %d 个token\n", tokenCount);
	printf("========================\n\n");
	
	fclose(fp);
}

int main(int argc, char *argv[]){
	// 如果开启源代码显示功能，先输出源代码
	#if SHOW_SOURCE_CODE
	printSourceCode(argv[1]);
	#endif
	
	// 词法分析：输出所有token
	performLexicalAnalysis(argv[1]);
	
	// 语法分析
	yyin=fopen(argv[1],"r");
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
	}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}
	

