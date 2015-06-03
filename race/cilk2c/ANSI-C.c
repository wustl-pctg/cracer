/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "ANSI-C.y"


    /* Copyright (C) 1989,1990 James A. Roskind, All rights reserved.
    This grammar was developed  and  written  by  James  A.  Roskind. 
    Copying  of  this  grammar  description, as a whole, is permitted 
    providing this notice is intact and applicable  in  all  complete 
    copies.   Translations as a whole to other parser generator input 
    languages  (or  grammar  description  languages)   is   permitted 
    provided  that  this  notice is intact and applicable in all such 
    copies,  along  with  a  disclaimer  that  the  contents  are   a 
    translation.   The reproduction of derived text, such as modified 
    versions of this grammar, or the output of parser generators,  is 
    permitted,  provided  the  resulting  work includes the copyright 
    notice "Portions Copyright (c)  1989,  1990  James  A.  Roskind". 
    Derived products, such as compilers, translators, browsers, etc., 
    that  use  this  grammar,  must also provide the notice "Portions 
    Copyright  (c)  1989,  1990  James  A.  Roskind"  in   a   manner 
    appropriate  to  the  utility,  and in keeping with copyright law 
    (e.g.: EITHER displayed when first invoked/executed; OR displayed 
    continuously on display terminal; OR via placement in the  object 
    code  in  form  readable in a printout, with or near the title of 
    the work, or at the end of the file).  No royalties, licenses  or 
    commissions  of  any  kind are required to copy this grammar, its 
    translations, or derivative products, when the copies are made in 
    compliance with this notice. Persons or corporations that do make 
    copies in compliance with this notice may charge  whatever  price 
    is  agreeable  to  a  buyer, for such copies or derivative works. 
    THIS GRAMMAR IS PROVIDED ``AS IS'' AND  WITHOUT  ANY  EXPRESS  OR 
    IMPLIED  WARRANTIES,  INCLUDING,  WITHOUT LIMITATION, THE IMPLIED 
    WARRANTIES  OF  MERCHANTABILITY  AND  FITNESS  FOR  A  PARTICULAR 
    PURPOSE.

    James A. Roskind
    Independent Consultant
    516 Latania Palm Drive
    Indialantic FL, 32903
    (407)729-4348
    jar@ileaf.com
    or ...!uunet!leafusa!jar


    ---end of copyright notice---

 * Copyright (c) 1994 Robert C. Miller
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 * Copyright (c) 2003 Jim Sukha
 * 
 * The changes to this gramamr since James Roskind's original are licensed under the GNU GPL.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

This file is a companion file to a C++ grammar description file.

*/

/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *  
 *************************************************************************/

/* FILENAME: C.Y */

/*  This  is a grammar file for the dpANSI C language.  This file was 
last modified by J. Roskind on 3/7/90. Version 1.00 */




/* ACKNOWLEDGMENT:

Without the effort expended by the ANSI C standardizing committee,  I 
would  have been lost.  Although the ANSI C standard does not include 
a fully disambiguated syntax description, the committee has at  least 
provided most of the disambiguating rules in narratives.

Several  reviewers  have also recently critiqued this grammar, and/or 
assisted in discussions during it's preparation.  These reviewers are 
certainly not responsible for the errors I have committed  here,  but 
they  are responsible for allowing me to provide fewer errors.  These 
colleagues include: Bruce Blodgett, and Mark Langley. */

/* Added by Eric A. Brewer */

#define _Y_TAB_H_  /* prevents redundant inclusion of y.tab.h */
#include "ast.h"
#include "stringParse.h"
 
FILE_IDENTITY(ident_ansi_c_y,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/ANSI-C.y $ $LastChangedBy: bradley $ $Rev: 2836 $ $Date: 2006-04-12 08:36:07 -0400 (Wed, 12 Apr 2006) $");

int yydebug=0;

extern int yylex(void);
PRIVATE void WarnAboutPrecedence(OpType op, Node *node);

PRIVATE void yyerror(const char *msg)
  {
    StringParseError();
    SyntaxError(msg);
  }

/* End of create code (EAB) */


 PRIVATE Bool OmitLookupsFlag = TRUE;

 GLOBAL void SetLookupFlag(Bool dolookup) {
   OmitLookupsFlag = dolookup;
 }

 BisonInputType parseInputType = EntireProgram;

 GLOBAL void SetBisonInputType(BisonInputType t) {
   parseInputType = t;
 }

 GLOBAL BisonInputType GetBisonInputType() {
   return parseInputType;
 }

 GLOBAL inline Bool AcceptWildcards() {
   return (parseInputType != EntireProgram);
 }
 
 GLOBAL Bool OmitLookups() {
   return (OmitLookupsFlag && (parseInputType != EntireProgram));
   /*  return FALSE; */
 }




/* Line 268 of yacc.c  */
#line 222 "ANSI-C.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     AUTO = 258,
     DOUBLE = 259,
     INT = 260,
     STRUCT = 261,
     BREAK = 262,
     ELSE = 263,
     LONG = 264,
     SWITCH = 265,
     CASE = 266,
     ENUM = 267,
     REGISTER = 268,
     TYPEDEF = 269,
     CHAR = 270,
     EXTERN = 271,
     RETURN = 272,
     UNION = 273,
     CONST = 274,
     FLOAT = 275,
     SHORT = 276,
     UNSIGNED = 277,
     CONTINUE = 278,
     FOR = 279,
     SIGNED = 280,
     VOID = 281,
     DEFAULT = 282,
     GOTO = 283,
     SIZEOF = 284,
     VOLATILE = 285,
     DO = 286,
     IF = 287,
     STATIC = 288,
     WHILE = 289,
     UPLUS = 290,
     UMINUS = 291,
     INDIR = 292,
     ADDRESS = 293,
     POSTINC = 294,
     POSTDEC = 295,
     PREINC = 296,
     PREDEC = 297,
     BOGUS = 298,
     IDENTIFIER = 299,
     STRINGliteral = 300,
     FLOATINGconstant = 301,
     INTEGERconstant = 302,
     OCTALconstant = 303,
     HEXconstant = 304,
     WIDECHARconstant = 305,
     CHARACTERconstant = 306,
     COMPLEX = 307,
     REAL = 308,
     IMAG = 309,
     TYPEDEFname = 310,
     ARROW = 311,
     ICR = 312,
     DECR = 313,
     LS = 314,
     RS = 315,
     LE = 316,
     GE = 317,
     EQ = 318,
     NE = 319,
     ANDAND = 320,
     OROR = 321,
     ELLIPSIS = 322,
     MULTassign = 323,
     DIVassign = 324,
     MODassign = 325,
     PLUSassign = 326,
     MINUSassign = 327,
     LSassign = 328,
     RSassign = 329,
     ANDassign = 330,
     ERassign = 331,
     ORassign = 332,
     INLINE = 333,
     ATTRIBUTE = 334,
     EXTENSION = 335,
     RESTRICT = 336,
     ALIGNOF = 337,
     TYPEOF = 338,
     DECLSPEC_EXPORT = 339,
     DECLSPEC_IMPORT = 340,
     BUILTIN_VA_ARG = 341,
     DESIGNATED_INITIALIZER = 342,
     ARRAY_LABELED_INITIALIZER = 343,
     NOOUTPUT = 344,
     CILKONLY = 345,
     CILKSAFE = 346,
     WILDCARD_T = 347,
     WILDCARD_E = 348,
     WILDCARD_D = 349,
     ASM = 350,
     PROCEDURE = 351,
     SPAWN = 352,
     SYNC = 353,
     INLET = 354,
     ABORT = 355,
     XBEGIN = 356,
     XEND = 357,
     SYNCHED = 358,
     SHAREDVAR = 359,
     PRIVATEVAR = 360
   };
#endif
/* Tokens.  */
#define AUTO 258
#define DOUBLE 259
#define INT 260
#define STRUCT 261
#define BREAK 262
#define ELSE 263
#define LONG 264
#define SWITCH 265
#define CASE 266
#define ENUM 267
#define REGISTER 268
#define TYPEDEF 269
#define CHAR 270
#define EXTERN 271
#define RETURN 272
#define UNION 273
#define CONST 274
#define FLOAT 275
#define SHORT 276
#define UNSIGNED 277
#define CONTINUE 278
#define FOR 279
#define SIGNED 280
#define VOID 281
#define DEFAULT 282
#define GOTO 283
#define SIZEOF 284
#define VOLATILE 285
#define DO 286
#define IF 287
#define STATIC 288
#define WHILE 289
#define UPLUS 290
#define UMINUS 291
#define INDIR 292
#define ADDRESS 293
#define POSTINC 294
#define POSTDEC 295
#define PREINC 296
#define PREDEC 297
#define BOGUS 298
#define IDENTIFIER 299
#define STRINGliteral 300
#define FLOATINGconstant 301
#define INTEGERconstant 302
#define OCTALconstant 303
#define HEXconstant 304
#define WIDECHARconstant 305
#define CHARACTERconstant 306
#define COMPLEX 307
#define REAL 308
#define IMAG 309
#define TYPEDEFname 310
#define ARROW 311
#define ICR 312
#define DECR 313
#define LS 314
#define RS 315
#define LE 316
#define GE 317
#define EQ 318
#define NE 319
#define ANDAND 320
#define OROR 321
#define ELLIPSIS 322
#define MULTassign 323
#define DIVassign 324
#define MODassign 325
#define PLUSassign 326
#define MINUSassign 327
#define LSassign 328
#define RSassign 329
#define ANDassign 330
#define ERassign 331
#define ORassign 332
#define INLINE 333
#define ATTRIBUTE 334
#define EXTENSION 335
#define RESTRICT 336
#define ALIGNOF 337
#define TYPEOF 338
#define DECLSPEC_EXPORT 339
#define DECLSPEC_IMPORT 340
#define BUILTIN_VA_ARG 341
#define DESIGNATED_INITIALIZER 342
#define ARRAY_LABELED_INITIALIZER 343
#define NOOUTPUT 344
#define CILKONLY 345
#define CILKSAFE 346
#define WILDCARD_T 347
#define WILDCARD_E 348
#define WILDCARD_D 349
#define ASM 350
#define PROCEDURE 351
#define SPAWN 352
#define SYNC 353
#define INLET 354
#define ABORT 355
#define XBEGIN 356
#define XEND 357
#define SYNCHED 358
#define SHAREDVAR 359
#define PRIVATEVAR 360




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 194 "ANSI-C.y"

    Node      *n;
    List      *L;

  /* tq: type qualifiers */
    struct {
        TypeQual   tq;
	Coord      coord;   /* coordinates where type quals began */ 
    } tq;

  /* tok: token coordinates */
    Coord tok;



/* Line 293 of yacc.c  */
#line 484 "ANSI-C.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 496 "ANSI-C.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  87
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5840

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  184
/* YYNRULES -- Number of rules.  */
#define YYNRULES  519
/* YYNRULES -- Number of states.  */
#define YYNSTATES  899

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   360

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     8,     2,     2,     2,    13,     3,     2,
      15,    16,     4,     5,    22,     6,    17,    14,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    25,    19,
       9,    91,    10,    18,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    23,     2,    24,    12,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    20,    11,    21,     7,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     9,    12,    14,    16,    18,
      20,    24,    30,    37,    39,    41,    43,    45,    50,    54,
      59,    63,    67,    70,    73,    77,    81,    85,    89,    91,
      95,    97,   100,   103,   106,   109,   112,   117,   120,   125,
     132,   135,   138,   140,   142,   144,   146,   148,   150,   152,
     157,   162,   164,   168,   172,   176,   178,   182,   186,   188,
     192,   196,   198,   202,   206,   210,   214,   216,   220,   224,
     226,   230,   232,   236,   238,   242,   244,   248,   250,   254,
     256,   262,   267,   269,   273,   275,   277,   279,   281,   283,
     285,   287,   289,   291,   293,   295,   297,   301,   303,   304,
     306,   309,   312,   315,   318,   319,   324,   325,   331,   332,
     337,   338,   344,   345,   346,   347,   356,   357,   358,   359,
     369,   370,   371,   372,   381,   382,   383,   384,   394,   395,
     396,   397,   407,   408,   415,   416,   423,   427,   428,   429,
     436,   437,   438,   445,   446,   447,   455,   456,   462,   463,
     469,   473,   475,   477,   479,   481,   483,   486,   489,   492,
     495,   498,   502,   505,   508,   511,   514,   517,   522,   525,
     527,   529,   531,   533,   535,   537,   539,   541,   543,   548,
     554,   556,   559,   562,   565,   567,   570,   573,   575,   578,
     581,   583,   586,   588,   591,   593,   595,   597,   599,   601,
     603,   606,   608,   612,   618,   625,   630,   634,   639,   644,
     646,   650,   652,   655,   657,   659,   663,   668,   672,   677,
     679,   681,   683,   686,   690,   694,   699,   703,   707,   711,
     716,   718,   721,   725,   727,   729,   731,   735,   740,   743,
     747,   752,   754,   758,   760,   762,   766,   771,   776,   780,
     785,   787,   791,   793,   795,   797,   800,   802,   805,   807,
     808,   810,   812,   815,   822,   824,   828,   829,   831,   836,
     843,   848,   850,   851,   853,   858,   860,   862,   864,   865,
     868,   870,   874,   878,   883,   888,   893,   899,   906,   908,
     910,   913,   917,   922,   924,   928,   930,   934,   936,   938,
     942,   946,   950,   953,   957,   961,   965,   968,   972,   976,
     979,   983,   988,   992,   995,   999,  1003,  1006,  1010,  1015,
    1019,  1024,  1030,  1033,  1037,  1042,  1044,  1046,  1049,  1053,
    1056,  1059,  1062,  1065,  1069,  1072,  1076,  1079,  1084,  1088,
    1092,  1096,  1097,  1099,  1102,  1108,  1115,  1118,  1121,  1126,
    1127,  1130,  1131,  1133,  1135,  1137,  1139,  1141,  1143,  1145,
    1147,  1149,  1152,  1155,  1159,  1162,  1165,  1168,  1172,  1176,
    1181,  1184,  1188,  1192,  1197,  1199,  1201,  1203,  1205,  1208,
    1212,  1216,  1221,  1223,  1226,  1228,  1231,  1234,  1240,  1248,
    1249,  1256,  1257,  1264,  1265,  1274,  1275,  1286,  1287,  1298,
    1299,  1310,  1311,  1322,  1323,  1330,  1334,  1337,  1340,  1344,
    1348,  1349,  1351,  1353,  1357,  1362,  1364,  1368,  1374,  1382,
    1392,  1404,  1411,  1420,  1431,  1444,  1446,  1449,  1451,  1453,
    1456,  1458,  1459,  1463,  1466,  1467,  1472,  1473,  1478,  1479,
    1483,  1484,  1489,  1490,  1495,  1496,  1501,  1502,  1507,  1508,
    1513,  1514,  1520,  1521,  1527,  1528,  1534,  1535,  1541,  1542,
    1545,  1547,  1549,  1551,  1553,  1555,  1557,  1560,  1562,  1564,
    1566,  1568,  1570,  1572,  1574,  1576,  1578,  1580,  1582,  1584,
    1586,  1588,  1590,  1592,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1608,  1610,  1612,  1614,  1616,  1618,  1620,  1622,  1624,
    1626,  1628,  1633,  1637,  1641,  1644,  1647,  1651,  1655,  1657,
    1658,  1660,  1667,  1676,  1679,  1685,  1688,  1694,  1702,  1704
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     131,     0,    -1,    -1,   283,    -1,   132,   265,    -1,   132,
     264,    -1,    18,    -1,    67,    -1,   301,    -1,   302,    -1,
      15,   152,    16,    -1,    15,   259,   265,   260,    16,    -1,
      15,   259,   264,   265,   260,    16,    -1,   312,    -1,   117,
      -1,   118,    -1,   133,    -1,   134,    23,   152,    24,    -1,
     134,    15,    16,    -1,   134,    15,   135,    16,    -1,   134,
      17,    67,    -1,   134,    79,    67,    -1,   134,    80,    -1,
     134,    81,    -1,   134,    17,    78,    -1,   134,    79,    78,
      -1,   134,    17,   118,    -1,   134,    79,   118,    -1,   150,
      -1,   135,    22,   150,    -1,   134,    -1,    80,   136,    -1,
      81,   136,    -1,   137,   138,    -1,   313,   138,    -1,    52,
     136,    -1,    52,    15,   223,    16,    -1,   106,   136,    -1,
     106,    15,   223,    16,    -1,   110,    15,   150,    22,   223,
      16,    -1,    77,   136,    -1,    76,   136,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
     136,    -1,    15,   223,    16,   236,    -1,    15,   223,    16,
     138,    -1,   138,    -1,   139,     4,   138,    -1,   139,    14,
     138,    -1,   139,    13,   138,    -1,   139,    -1,   140,     5,
     139,    -1,   140,     6,   139,    -1,   140,    -1,   141,    82,
     140,    -1,   141,    83,   140,    -1,   141,    -1,   142,     9,
     141,    -1,   142,    10,   141,    -1,   142,    84,   141,    -1,
     142,    85,   141,    -1,   142,    -1,   143,    86,   142,    -1,
     143,    87,   142,    -1,   143,    -1,   144,     3,   143,    -1,
     144,    -1,   145,    12,   144,    -1,   145,    -1,   146,    11,
     145,    -1,   146,    -1,   147,    88,   146,    -1,   147,    -1,
     148,    89,   147,    -1,   148,    -1,   148,    18,   152,    25,
     149,    -1,   148,    18,    25,   149,    -1,   149,    -1,   138,
     151,   150,    -1,    91,    -1,    92,    -1,    93,    -1,    94,
      -1,    95,    -1,    96,    -1,    97,    -1,    98,    -1,    99,
      -1,   100,    -1,   101,    -1,   150,    -1,   152,    22,   150,
      -1,   149,    -1,    -1,   152,    -1,   160,    19,    -1,   178,
      19,    -1,   190,    19,    -1,   199,    19,    -1,    -1,   187,
     215,   156,   263,    -1,    -1,   188,   225,   215,   157,   263,
      -1,    -1,   195,   215,   158,   263,    -1,    -1,   196,   225,
     215,   159,   263,    -1,    -1,    -1,    -1,   187,   204,   161,
     230,   162,   224,   163,   233,    -1,    -1,    -1,    -1,   188,
     225,   204,   164,   230,   165,   224,   166,   233,    -1,    -1,
      -1,    -1,   195,   204,   167,   230,   168,   224,   169,   233,
      -1,    -1,    -1,    -1,   196,   225,   204,   170,   230,   171,
     224,   172,   233,    -1,    -1,    -1,    -1,   160,    22,   204,
     173,   230,   174,   224,   175,   233,    -1,    -1,   187,     1,
     176,   230,   224,   233,    -1,    -1,   195,     1,   177,   230,
     224,   233,    -1,   160,    22,     1,    -1,    -1,    -1,   192,
     215,   179,   224,   180,   233,    -1,    -1,    -1,   201,   215,
     181,   224,   182,   233,    -1,    -1,    -1,   178,    22,   215,
     183,   224,   184,   233,    -1,    -1,   192,     1,   185,   224,
     233,    -1,    -1,   201,     1,   186,   224,   233,    -1,   178,
      22,     1,    -1,   188,    -1,   190,    -1,   189,    -1,   191,
      -1,   116,    -1,   198,   305,    -1,   192,   306,    -1,   189,
     193,    -1,   189,   306,    -1,   199,   305,    -1,   192,   203,
     224,    -1,   190,   193,    -1,   200,   305,    -1,   192,    78,
      -1,   191,   193,    -1,   305,   224,    -1,   201,   224,   305,
     224,    -1,   192,   193,    -1,   303,    -1,   305,    -1,   152,
      -1,   195,    -1,   196,    -1,   199,    -1,   198,    -1,   200,
      -1,   197,    -1,   107,    15,   194,    16,    -1,   201,   107,
      15,   194,    16,    -1,   306,    -1,   201,   306,    -1,   198,
     303,    -1,   198,   306,    -1,   203,    -1,   201,   203,    -1,
     199,   303,    -1,    78,    -1,   201,    78,    -1,   200,   303,
      -1,   303,    -1,   201,   303,    -1,   304,    -1,   202,   304,
      -1,   242,    -1,   252,    -1,   205,    -1,   208,    -1,   215,
      -1,   219,    -1,   313,   204,    -1,   206,    -1,     4,   224,
     205,    -1,     4,   224,    15,   207,    16,    -1,     4,   224,
     202,    15,   207,    16,    -1,     4,   224,   202,   205,    -1,
      15,   205,    16,    -1,    15,   207,   214,    16,    -1,    15,
     205,    16,   214,    -1,    78,    -1,    15,   207,    16,    -1,
      78,    -1,    78,   214,    -1,   209,    -1,   210,    -1,     4,
     224,   208,    -1,     4,   224,   202,   208,    -1,    15,   209,
      16,    -1,    15,   209,    16,   214,    -1,   212,    -1,   213,
      -1,   214,    -1,     4,   224,    -1,     4,   224,   202,    -1,
       4,   224,   211,    -1,     4,   224,   202,   211,    -1,    15,
     212,    16,    -1,    15,   213,    16,    -1,    15,   214,    16,
      -1,    15,   212,    16,   214,    -1,   241,    -1,    15,    16,
      -1,    15,   238,    16,    -1,   216,    -1,   218,    -1,   217,
      -1,     4,   224,   215,    -1,     4,   224,   202,   215,    -1,
     218,   214,    -1,    15,   216,    16,    -1,    15,   216,    16,
     214,    -1,    67,    -1,    15,   218,    16,    -1,   118,    -1,
     220,    -1,     4,   224,   219,    -1,     4,   224,   202,   219,
      -1,   218,    15,   221,    16,    -1,    15,   219,    16,    -1,
      15,   219,    16,   214,    -1,    67,    -1,   221,    22,    67,
      -1,    67,    -1,    78,    -1,   195,    -1,   195,   211,    -1,
     201,    -1,   201,   211,    -1,   116,    -1,    -1,   225,    -1,
     226,    -1,   225,   226,    -1,   103,    15,    15,   227,    16,
      16,    -1,   228,    -1,   227,    22,   228,    -1,    -1,   232,
      -1,   232,    15,    67,    16,    -1,   232,    15,    67,    22,
     229,    16,    -1,   232,    15,   229,    16,    -1,   135,    -1,
      -1,   231,    -1,   119,    15,   302,    16,    -1,    67,    -1,
      78,    -1,    42,    -1,    -1,    91,   234,    -1,   235,    -1,
      67,    25,   235,    -1,    78,    25,   235,    -1,    17,    67,
      91,   235,    -1,    17,    78,    91,   235,    -1,    23,   150,
      24,   235,    -1,    23,   150,    24,    91,   235,    -1,    23,
     150,    90,   150,    24,   235,    -1,   236,    -1,   150,    -1,
      20,    21,    -1,    20,   237,    21,    -1,    20,   237,    22,
      21,    -1,   234,    -1,   237,    22,   234,    -1,   239,    -1,
     239,    22,    90,    -1,    90,    -1,   240,    -1,   239,    22,
     240,    -1,   240,    91,   234,    -1,   239,    22,     1,    -1,
     224,   187,    -1,   224,   187,   211,    -1,   224,   187,   215,
      -1,   224,   187,   208,    -1,   224,   192,    -1,   224,   192,
     211,    -1,   224,   192,   215,    -1,   224,   195,    -1,   224,
     195,   211,    -1,   224,   195,   215,   224,    -1,   224,   195,
     208,    -1,   224,   201,    -1,   224,   201,   211,    -1,   224,
     201,   215,    -1,    23,    24,    -1,    23,   153,    24,    -1,
     241,    23,   153,    24,    -1,   241,    23,    24,    -1,   243,
      20,   244,    21,    -1,   243,   222,    20,   244,    21,    -1,
     243,   222,    -1,   243,    20,    21,    -1,   243,   222,    20,
      21,    -1,    29,    -1,    41,    -1,   224,   245,    -1,   244,
     224,   245,    -1,   247,    19,    -1,   246,    19,    -1,   313,
     245,    -1,   201,   249,    -1,   246,    22,   249,    -1,   195,
     248,    -1,   196,   225,   248,    -1,   195,   251,    -1,   243,
      20,   244,    21,    -1,   247,    22,   248,    -1,   204,   250,
     224,    -1,   215,   250,   224,    -1,    -1,   251,    -1,    25,
     153,    -1,    35,    20,   253,   255,    21,    -1,    35,   222,
      20,   253,   255,    21,    -1,    35,   222,    -1,   222,   254,
      -1,   253,    22,   222,   254,    -1,    -1,    91,   153,    -1,
      -1,    22,    -1,   257,    -1,   258,    -1,   266,    -1,   267,
      -1,   269,    -1,   277,    -1,   282,    -1,   310,    -1,     1,
      19,    -1,    67,    25,    -1,    34,   153,    25,    -1,    50,
      25,    -1,    78,    25,    -1,   259,   260,    -1,   259,   264,
     260,    -1,   259,   265,   260,    -1,   259,   264,   265,   260,
      -1,   261,   262,    -1,   261,   264,   262,    -1,   261,   265,
     262,    -1,   261,   264,   265,   262,    -1,    20,    -1,    21,
      -1,   125,    -1,   126,    -1,    20,    21,    -1,    20,   264,
      21,    -1,    20,   265,    21,    -1,    20,   264,   265,    21,
      -1,   155,    -1,   264,   155,    -1,   256,    -1,   265,   256,
      -1,   154,    19,    -1,    55,    15,   152,    16,   256,    -1,
      55,    15,   152,    16,   256,    31,   256,    -1,    -1,    33,
     268,    15,   152,    16,   256,    -1,    -1,    57,   270,    15,
     152,    16,   256,    -1,    -1,    54,   271,   256,    57,    15,
     152,    16,    19,    -1,    -1,    47,    15,   154,    19,   154,
      19,   154,    16,   272,   256,    -1,    -1,    47,    15,     1,
      19,   154,    19,   154,    16,   273,   256,    -1,    -1,    47,
      15,   154,    19,   154,    19,     1,    16,   274,   256,    -1,
      -1,    47,    15,   154,    19,     1,    19,   154,    16,   275,
     256,    -1,    -1,    47,    15,     1,    16,   276,   256,    -1,
      51,    67,    19,    -1,    46,    19,    -1,    30,    19,    -1,
      40,   154,    19,    -1,    51,    78,    19,    -1,    -1,   279,
      -1,   280,    -1,   279,    22,   280,    -1,   302,    15,   150,
      16,    -1,   302,    -1,   281,    22,   302,    -1,   119,    15,
     302,    16,    19,    -1,   119,    15,   302,    25,   278,    16,
      19,    -1,   119,    15,   302,    25,   278,    25,   278,    16,
      19,    -1,   119,    15,   302,    25,   278,    25,   278,    25,
     281,    16,    19,    -1,   119,   303,    15,   302,    16,    19,
      -1,   119,   303,    15,   302,    25,   278,    16,    19,    -1,
     119,   303,    15,   302,    25,   278,    25,   278,    16,    19,
      -1,   119,   303,    15,   302,    25,   278,    25,   278,    25,
     281,    16,    19,    -1,   284,    -1,   283,   284,    -1,   155,
      -1,   285,    -1,   313,   284,    -1,    19,    -1,    -1,   215,
     286,   263,    -1,   215,    66,    -1,    -1,   192,   215,   287,
     263,    -1,    -1,   201,   215,   288,   263,    -1,    -1,   219,
     289,   263,    -1,    -1,   187,   219,   290,   263,    -1,    -1,
     195,   219,   291,   263,    -1,    -1,   192,   219,   292,   263,
      -1,    -1,   201,   219,   293,   263,    -1,    -1,   219,   299,
     294,   263,    -1,    -1,   187,   219,   299,   295,   263,    -1,
      -1,   195,   219,   299,   296,   263,    -1,    -1,   192,   219,
     299,   297,   263,    -1,    -1,   201,   219,   299,   298,   263,
      -1,    -1,   300,   264,    -1,    69,    -1,    70,    -1,    71,
      -1,    72,    -1,    74,    -1,    68,    -1,   302,    68,    -1,
      42,    -1,    53,    -1,   102,    -1,   105,    -1,    75,    -1,
     120,    -1,   123,    -1,   128,    -1,   129,    -1,   113,    -1,
     114,    -1,   115,    -1,    42,    -1,    53,    -1,   105,    -1,
     120,    -1,   123,    -1,   114,    -1,   115,    -1,    37,    -1,
      39,    -1,    56,    -1,    26,    -1,    36,    -1,    49,    -1,
      38,    -1,    28,    -1,    43,    -1,    27,    -1,    48,    -1,
      45,    -1,    44,    -1,    32,    -1,   133,    -1,   307,    23,
     152,    24,    -1,   307,    17,    67,    -1,   307,    79,    67,
      -1,   307,    80,    -1,   307,    81,    -1,   307,    17,    78,
      -1,   307,    79,    78,    -1,   307,    -1,    -1,   135,    -1,
     121,   308,    15,   309,    16,    19,    -1,   138,   151,   121,
     308,    15,   309,    16,    19,    -1,   122,    19,    -1,   134,
      15,   311,    16,    19,    -1,   124,    19,    -1,   121,   308,
      15,   309,    16,    -1,   121,   308,    15,   309,    16,    22,
     135,    -1,   127,    -1,   104,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   395,   395,   399,   400,   401,   405,   417,   418,   419,
     420,   425,   430,   435,   436,   442,   451,   452,   454,   456,
     458,   460,   462,   464,   468,   470,   473,   479,   488,   490,
     495,   497,   499,   501,   504,   506,   508,   510,   512,   514,
     516,   518,   523,   524,   525,   526,   527,   528,   532,   533,
     537,   541,   542,   544,   546,   551,   552,   554,   559,   560,
     562,   567,   568,   570,   572,   574,   579,   580,   582,   587,
     588,   593,   594,   602,   603,   610,   611,   616,   617,   624,
     625,   627,   636,   637,   644,   645,   646,   647,   648,   649,
     650,   651,   652,   653,   654,   658,   659,   674,   678,   679,
     721,   723,   725,   727,   734,   733,   741,   740,   748,   747,
     760,   759,   777,   780,   781,   775,   789,   793,   794,   786,
     801,   804,   805,   799,   813,   817,   818,   810,   824,   827,
     828,   823,   841,   840,   852,   851,   862,   869,   872,   868,
     880,   883,   879,   891,   892,   890,   899,   898,   909,   908,
     917,   922,   923,   927,   929,   930,   941,   943,   945,   947,
     954,   956,   958,   965,   967,   970,   977,   982,   988,  1002,
    1003,  1007,  1008,  1013,  1014,  1018,  1020,  1021,  1025,  1036,
    1052,  1053,  1055,  1057,  1063,  1064,  1066,  1073,  1075,  1077,
    1083,  1086,  1092,  1093,  1100,  1101,  1106,  1107,  1108,  1109,
    1114,  1120,  1121,  1124,  1127,  1130,  1138,  1141,  1144,  1151,
    1153,  1160,  1162,  1164,  1173,  1174,  1177,  1184,  1187,  1194,
    1195,  1196,  1201,  1203,  1205,  1208,  1215,  1218,  1221,  1224,
    1231,  1232,  1233,  1239,  1240,  1245,  1246,  1251,  1258,  1260,
    1263,  1270,  1272,  1275,  1286,  1290,  1293,  1300,  1302,  1305,
    1320,  1322,  1328,  1329,  1334,  1336,  1338,  1340,  1342,  1354,
    1355,  1360,  1362,  1370,  1379,  1381,  1387,  1388,  1390,  1392,
    1403,  1408,  1415,  1416,  1419,  1424,  1425,  1426,  1431,  1432,
    1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,  1450,  1451,
    1455,  1458,  1459,  1463,  1465,  1475,  1476,  1479,  1488,  1490,
    1494,  1499,  1505,  1507,  1510,  1513,  1516,  1518,  1520,  1522,
    1524,  1527,  1531,  1534,  1536,  1538,  1544,  1546,  1548,  1550,
    1572,  1576,  1581,  1586,  1592,  1602,  1603,  1608,  1613,  1622,
    1623,  1624,  1630,  1636,  1642,  1644,  1651,  1656,  1664,  1671,
    1688,  1695,  1696,  1701,  1706,  1708,  1710,  1716,  1718,  1724,
    1725,  1729,  1730,  1740,  1741,  1742,  1743,  1744,  1745,  1746,
    1747,  1749,  1754,  1756,  1758,  1762,  1767,  1769,  1773,  1778,
    1784,  1786,  1790,  1795,  1802,  1803,  1806,  1807,  1813,  1815,
    1817,  1822,  1832,  1833,  1838,  1839,  1844,  1851,  1853,  1855,
    1855,  1861,  1860,  1865,  1864,  1869,  1868,  1875,  1874,  1879,
    1878,  1883,  1882,  1886,  1886,  1891,  1893,  1895,  1897,  1901,
    1907,  1908,  1911,  1913,  1917,  1921,  1923,  1927,  1929,  1931,
    1933,  1935,  1937,  1939,  1941,  1952,  1953,  1957,  1967,  1977,
    1979,  1984,  1983,  1992,  2007,  2006,  2016,  2015,  2025,  2024,
    2034,  2033,  2042,  2041,  2050,  2049,  2059,  2058,  2068,  2067,
    2076,  2075,  2083,  2082,  2090,  2089,  2099,  2098,  2112,  2112,
    2129,  2130,  2131,  2132,  2133,  2138,  2139,  2161,  2162,  2163,
    2164,  2165,  2166,  2167,  2168,  2169,  2170,  2171,  2172,  2176,
    2177,  2178,  2179,  2180,  2181,  2182,  2186,  2187,  2188,  2189,
    2190,  2194,  2195,  2196,  2197,  2198,  2200,  2201,  2203,  2204,
    2210,  2211,  2213,  2215,  2217,  2219,  2223,  2225,  2230,  2235,
    2236,  2242,  2246,  2251,  2255,  2264,  2269,  2271,  2276,  2283
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'&'", "'*'", "'+'", "'-'", "'~'", "'!'",
  "'<'", "'>'", "'|'", "'^'", "'%'", "'/'", "'('", "')'", "'.'", "'?'",
  "';'", "'{'", "'}'", "','", "'['", "']'", "':'", "AUTO", "DOUBLE", "INT",
  "STRUCT", "BREAK", "ELSE", "LONG", "SWITCH", "CASE", "ENUM", "REGISTER",
  "TYPEDEF", "CHAR", "EXTERN", "RETURN", "UNION", "CONST", "FLOAT",
  "SHORT", "UNSIGNED", "CONTINUE", "FOR", "SIGNED", "VOID", "DEFAULT",
  "GOTO", "SIZEOF", "VOLATILE", "DO", "IF", "STATIC", "WHILE", "UPLUS",
  "UMINUS", "INDIR", "ADDRESS", "POSTINC", "POSTDEC", "PREINC", "PREDEC",
  "BOGUS", "IDENTIFIER", "STRINGliteral", "FLOATINGconstant",
  "INTEGERconstant", "OCTALconstant", "HEXconstant", "WIDECHARconstant",
  "CHARACTERconstant", "COMPLEX", "REAL", "IMAG", "TYPEDEFname", "ARROW",
  "ICR", "DECR", "LS", "RS", "LE", "GE", "EQ", "NE", "ANDAND", "OROR",
  "ELLIPSIS", "'='", "MULTassign", "DIVassign", "MODassign", "PLUSassign",
  "MINUSassign", "LSassign", "RSassign", "ANDassign", "ERassign",
  "ORassign", "INLINE", "ATTRIBUTE", "EXTENSION", "RESTRICT", "ALIGNOF",
  "TYPEOF", "DECLSPEC_EXPORT", "DECLSPEC_IMPORT", "BUILTIN_VA_ARG",
  "DESIGNATED_INITIALIZER", "ARRAY_LABELED_INITIALIZER", "NOOUTPUT",
  "CILKONLY", "CILKSAFE", "WILDCARD_T", "WILDCARD_E", "WILDCARD_D", "ASM",
  "PROCEDURE", "SPAWN", "SYNC", "INLET", "ABORT", "XBEGIN", "XEND",
  "SYNCHED", "SHAREDVAR", "PRIVATEVAR", "$accept", "prog.start",
  "string.start.symbol", "primary.expression", "postfix.expression",
  "argument.expression.list", "unary.expression", "unary.operator",
  "cast.expression", "multiplicative.expression", "additive.expression",
  "shift.expression", "relational.expression", "equality.expression",
  "AND.expression", "exclusive.OR.expression", "inclusive.OR.expression",
  "logical.AND.expression", "logical.OR.expression",
  "conditional.expression", "assignment.expression", "assignment.operator",
  "expression", "constant.expression", "expression.opt", "declaration",
  "$@1", "$@2", "$@3", "$@4", "declaring.list", "$@5", "$@6", "$@7", "$@8",
  "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "@17",
  "$@18", "$@19", "$@20", "$@21", "default.declaring.list", "$@22", "$@23",
  "$@24", "$@25", "@26", "$@27", "$@28", "$@29", "declaration.specifier",
  "declaration.specifier.nosue", "basic.declaration.specifier",
  "sue.declaration.specifier", "typedef.declaration.specifier",
  "declaration.qualifier.list", "declaration.qualifier",
  "typeof.expression", "type.specifier", "type.specifier.nosue",
  "typeof.type.specifier", "basic.type.specifier", "sue.type.specifier",
  "typedef.type.specifier", "type.qualifier.list",
  "pointer.type.qualifier.list", "elaborated.type.name", "declarator",
  "paren.typedef.declarator", "paren.postfix.typedef.declarator",
  "simple.paren.typedef.declarator", "parameter.typedef.declarator",
  "clean.typedef.declarator", "clean.postfix.typedef.declarator",
  "abstract.declarator", "unary.abstract.declarator",
  "postfix.abstract.declarator", "postfixing.abstract.declarator",
  "identifier.declarator", "unary.identifier.declarator",
  "postfix.identifier.declarator", "paren.identifier.declarator",
  "old.function.declarator", "postfix.old.function.declarator",
  "identifier.list", "identifier.or.typedef.name", "type.name",
  "attributes.opt", "attributes", "attribute", "attribute.list", "attrib",
  "nonnull.expression.list", "asm.opt", "asm", "any.word",
  "initializer.opt", "initializer", "basic.initializer",
  "bracketed.initializer.list", "initializer.list", "parameter.type.list",
  "parameter.list", "parameter.declaration", "array.abstract.declarator",
  "struct.or.union.specifier", "struct.or.union",
  "struct.declaration.list", "struct.declaration",
  "struct.default.declaring.list", "struct.declaring.list",
  "struct.declarator", "struct.identifier.declarator",
  "bit.field.size.opt", "bit.field.size", "enum.specifier",
  "enumerator.list", "enumerator.value.opt", "comma.opt", "statement",
  "labeled.statement", "compound.statement", "lblock", "rblock", "xlblock",
  "xrblock", "compound.statement.no.new.scope", "declaration.list",
  "statement.list", "expression.statement", "selection.statement", "$@30",
  "iteration.statement", "$@31", "$@32", "$@33", "$@34", "$@35", "$@36",
  "$@37", "jump.statement", "asm.argument.list.opt", "asm.argument.list",
  "asm.argument", "asm.clobbered.list", "gcc.statement",
  "translation.unit", "external.definition", "function.definition", "$@38",
  "$@39", "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47",
  "$@48", "$@49", "$@50", "old.function.declaration.list", "$@51",
  "constant", "string.literal.list", "type.qualifier",
  "pointer.type.qualifier", "storage.class", "basic.type.name",
  "spawn.postfix.expression", "spawn.procedure", "spawn.args",
  "cilk.statement", "inlet.call.argument.expression.list",
  "cilk.expression", "gcc.extension", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    38,    42,    43,    45,   126,    33,    60,
      62,   124,    94,    37,    47,    40,    41,    46,    63,    59,
     123,   125,    44,    91,    93,    58,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    61,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   130,   131,   131,   131,   131,   132,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   135,   135,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   137,   137,   137,   137,   137,   137,   138,   138,
     138,   139,   139,   139,   139,   140,   140,   140,   141,   141,
     141,   142,   142,   142,   142,   142,   143,   143,   143,   144,
     144,   145,   145,   146,   146,   147,   147,   148,   148,   149,
     149,   149,   150,   150,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   152,   152,   153,   154,   154,
     155,   155,   155,   155,   156,   155,   157,   155,   158,   155,
     159,   155,   161,   162,   163,   160,   164,   165,   166,   160,
     167,   168,   169,   160,   170,   171,   172,   160,   173,   174,
     175,   160,   176,   160,   177,   160,   160,   179,   180,   178,
     181,   182,   178,   183,   184,   178,   185,   178,   186,   178,
     178,   187,   187,   188,   188,   188,   189,   189,   189,   189,
     190,   190,   190,   191,   191,   191,   192,   192,   192,   193,
     193,   194,   194,   195,   195,   196,   196,   196,   197,   197,
     198,   198,   198,   198,   199,   199,   199,   200,   200,   200,
     201,   201,   202,   202,   203,   203,   204,   204,   204,   204,
     204,   205,   205,   205,   205,   205,   206,   206,   206,   207,
     207,   208,   208,   208,   209,   209,   209,   210,   210,   211,
     211,   211,   212,   212,   212,   212,   213,   213,   213,   213,
     214,   214,   214,   215,   215,   216,   216,   216,   217,   217,
     217,   218,   218,   218,   219,   219,   219,   220,   220,   220,
     221,   221,   222,   222,   223,   223,   223,   223,   223,   224,
     224,   225,   225,   226,   227,   227,   228,   228,   228,   228,
     228,   229,   230,   230,   231,   232,   232,   232,   233,   233,
     234,   234,   234,   234,   234,   234,   234,   234,   235,   235,
     236,   236,   236,   237,   237,   238,   238,   238,   239,   239,
     239,   239,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   241,   241,   241,   241,
     242,   242,   242,   242,   242,   243,   243,   244,   244,   245,
     245,   245,   246,   246,   247,   247,   247,   247,   247,   248,
     249,   250,   250,   251,   252,   252,   252,   253,   253,   254,
     254,   255,   255,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   257,   257,   257,   257,   258,   258,   258,   258,
     258,   258,   258,   258,   259,   260,   261,   262,   263,   263,
     263,   263,   264,   264,   265,   265,   266,   267,   267,   268,
     267,   270,   269,   271,   269,   272,   269,   273,   269,   274,
     269,   275,   269,   276,   269,   277,   277,   277,   277,   277,
     278,   278,   279,   279,   280,   281,   281,   282,   282,   282,
     282,   282,   282,   282,   282,   283,   283,   284,   284,   284,
     284,   286,   285,   285,   287,   285,   288,   285,   289,   285,
     290,   285,   291,   285,   292,   285,   293,   285,   294,   285,
     295,   285,   296,   285,   297,   285,   298,   285,   300,   299,
     301,   301,   301,   301,   301,   302,   302,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   304,
     304,   304,   304,   304,   304,   304,   305,   305,   305,   305,
     305,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     307,   307,   307,   307,   307,   307,   307,   307,   308,   309,
     309,   310,   310,   310,   310,   310,   311,   311,   312,   313
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     2,     1,     1,     1,     1,
       3,     5,     6,     1,     1,     1,     1,     4,     3,     4,
       3,     3,     2,     2,     3,     3,     3,     3,     1,     3,
       1,     2,     2,     2,     2,     2,     4,     2,     4,     6,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       5,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     0,     1,
       2,     2,     2,     2,     0,     4,     0,     5,     0,     4,
       0,     5,     0,     0,     0,     8,     0,     0,     0,     9,
       0,     0,     0,     8,     0,     0,     0,     9,     0,     0,
       0,     9,     0,     6,     0,     6,     3,     0,     0,     6,
       0,     0,     6,     0,     0,     7,     0,     5,     0,     5,
       3,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     3,     2,     2,     2,     2,     2,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     5,
       1,     2,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     5,     6,     4,     3,     4,     4,     1,
       3,     1,     2,     1,     1,     3,     4,     3,     4,     1,
       1,     1,     2,     3,     3,     4,     3,     3,     3,     4,
       1,     2,     3,     1,     1,     1,     3,     4,     2,     3,
       4,     1,     3,     1,     1,     3,     4,     4,     3,     4,
       1,     3,     1,     1,     1,     2,     1,     2,     1,     0,
       1,     1,     2,     6,     1,     3,     0,     1,     4,     6,
       4,     1,     0,     1,     4,     1,     1,     1,     0,     2,
       1,     3,     3,     4,     4,     4,     5,     6,     1,     1,
       2,     3,     4,     1,     3,     1,     3,     1,     1,     3,
       3,     3,     2,     3,     3,     3,     2,     3,     3,     2,
       3,     4,     3,     2,     3,     3,     2,     3,     4,     3,
       4,     5,     2,     3,     4,     1,     1,     2,     3,     2,
       2,     2,     2,     3,     2,     3,     2,     4,     3,     3,
       3,     0,     1,     2,     5,     6,     2,     2,     4,     0,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     2,     2,     2,     3,     3,     4,
       2,     3,     3,     4,     1,     1,     1,     1,     2,     3,
       3,     4,     1,     2,     1,     2,     2,     5,     7,     0,
       6,     0,     6,     0,     8,     0,    10,     0,    10,     0,
      10,     0,    10,     0,     6,     3,     2,     2,     3,     3,
       0,     1,     1,     3,     4,     1,     3,     5,     7,     9,
      11,     6,     8,    10,    12,     1,     2,     1,     1,     2,
       1,     0,     3,     2,     0,     4,     0,     4,     0,     3,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     2,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     2,     2,     3,     3,     1,     0,
       1,     6,     8,     2,     5,     2,     5,     7,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,   259,     0,     6,   430,   489,   495,   493,   325,   499,
       0,   490,   486,   492,   487,   326,   467,   494,   498,   497,
     496,   491,   468,   488,   241,   471,   187,   469,   519,   470,
       0,   476,   477,   478,   155,   243,   472,   473,   474,   475,
       0,     0,   427,     0,     0,     0,   151,   153,   152,   154,
       0,     0,   173,   177,   175,   174,   176,     0,   184,   431,
     233,   235,   234,   458,   244,   194,     0,   195,     3,   425,
     428,   190,   259,   180,     0,     0,     0,   260,   261,     0,
       0,     0,     0,   252,   253,   346,     0,     1,     0,    42,
      43,    44,    45,    46,    47,     0,   374,     0,   389,     0,
      98,     0,     0,     0,     0,     0,   393,     0,   391,     7,
     465,   460,   461,   462,   463,   464,     0,     0,   187,     0,
       0,     0,     0,    14,    15,     0,     0,     0,     0,   376,
     518,    16,    30,    48,     0,    51,    55,    58,    61,    66,
      69,    71,    73,    75,    77,    79,    82,    95,    99,     0,
     382,     0,     0,     0,     0,   384,   353,   354,     0,     0,
       5,     0,   355,   356,   357,   358,   359,     8,     9,   360,
      13,     0,   100,     0,   101,     0,   132,   259,     0,   211,
     112,   196,   201,   197,   213,   214,   198,   458,     0,     0,
     158,   169,   170,   159,   102,   162,   165,   146,   164,   168,
     259,   137,   458,   157,   134,   120,   198,   458,     0,   182,
     156,   183,   103,   186,   160,   189,   163,   148,   188,     0,
     185,   140,   458,     0,   191,   181,   433,     0,   259,     0,
     238,   230,     0,   448,     0,   259,   322,   426,   166,   429,
       0,   479,   480,   481,   484,   485,   482,   483,     0,   236,
     245,   192,   262,   239,   242,   248,   349,   351,     0,     7,
      30,    51,   171,     0,   172,   173,   175,   174,   176,     0,
     361,   258,     0,   254,   256,     0,     0,   407,     0,    51,
      97,     0,     0,   406,     0,   364,     0,     0,     0,    35,
       0,     0,     0,   362,     0,    41,    40,   365,    31,    32,
       0,    37,     0,     0,     0,   500,   508,     0,   513,   515,
       0,     0,     0,     0,    22,    23,    33,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   386,
     199,   259,     0,   137,   234,   140,   375,   366,     0,     0,
     377,   370,     0,     0,   383,     0,   385,   466,    34,   136,
     128,   198,   150,   143,   272,     0,     0,   209,     0,     0,
       0,   259,   212,   272,     0,     0,   450,   200,   116,   198,
     259,   161,   259,     0,     0,   454,   272,   272,     0,     0,
     452,   124,   198,   259,     0,   259,     0,     0,   456,   259,
       0,   432,   231,   250,   297,     0,     0,     0,   295,   298,
     316,     0,     0,   439,     0,   459,   323,     0,   259,   259,
     266,   237,   246,   193,   240,   249,     0,   347,   352,     0,
     351,     0,     0,   178,    10,   259,   259,   255,   219,   220,
     221,   257,     0,     0,     0,     0,   363,   408,     0,     0,
     405,   409,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   509,    18,     0,     0,    28,
       0,    20,    24,    26,     0,    21,    25,    27,     0,    83,
      52,    54,    53,    56,    57,    59,    60,    62,    63,    64,
      65,    67,    68,    70,    72,    74,    76,     0,     0,    78,
      96,     0,     0,   367,     0,   368,   371,     0,   372,   272,
     259,     0,   259,   273,     0,     0,   202,   215,     0,   206,
       0,   217,   113,   105,   441,     0,   272,     0,   278,   138,
     435,   445,     0,   259,   121,   109,   443,     0,   272,     0,
     278,     0,   141,   437,   447,     0,   167,   378,     0,     0,
     247,     0,   302,   151,   152,   306,   309,   174,   259,   232,
       0,     0,   317,   319,     0,   449,     0,   173,     0,     0,
     327,     0,     0,     0,   320,     0,   324,   259,   277,   275,
     276,     0,   264,   267,   350,   349,   344,     0,   222,     0,
       0,     0,     0,    50,    49,     0,     0,     0,   403,    98,
       0,    36,     0,     0,     0,    38,     0,     0,   410,     0,
     502,   506,     0,   503,   507,   510,     0,     0,    19,     0,
       0,    17,     0,    81,     0,     0,   369,   373,   129,   144,
       0,   278,     0,     0,   205,   216,   210,   208,   207,   218,
     259,   451,   117,   107,     0,   147,   278,   455,   278,   259,
     453,   125,   111,   149,   179,   278,   457,   379,     0,   380,
     251,   259,   259,   305,   303,   304,   259,   259,   307,   308,
     312,   310,   259,   314,   315,   301,   296,   299,     0,     0,
       7,     0,   289,   300,   280,   288,   318,     0,   341,   334,
     336,     0,   341,   332,   259,   330,     0,   329,     0,   331,
     328,   321,     0,   266,     0,   348,   345,   223,   224,   226,
     227,   228,   290,   293,     0,     0,    11,     0,     0,     0,
       0,     0,     0,   387,     0,     0,   417,     0,   411,   412,
       0,     0,   410,   501,     0,   509,    29,   514,   509,    80,
     259,   278,     0,   133,   203,     0,   114,   259,   279,   139,
     135,   122,   259,   142,   381,   222,   222,   311,     0,     0,
       0,     0,     0,   343,   259,   342,   335,   259,   259,   333,
     338,   263,   265,     7,   271,     0,   225,   229,   291,     0,
      12,   390,   404,    98,    98,     0,     0,     0,   392,    39,
       0,   410,     0,     0,   421,     0,   511,     0,     0,   130,
     145,   274,   204,   278,   118,   278,   126,   223,   223,     0,
       0,     0,     0,   281,   282,   339,   340,   320,   268,     0,
     270,   292,   294,     0,     0,     0,     0,     0,   388,   418,
       0,   413,     0,     0,   410,   516,     0,   278,   115,   278,
     123,   278,   283,   284,     0,   285,     0,     0,   397,   401,
     399,   395,   394,     0,     0,   414,   422,     0,     0,   512,
     131,   119,   127,   286,     0,   269,     0,     0,     0,     0,
     419,     0,   415,     0,     0,   517,   287,   398,   402,   400,
     396,     0,     0,   423,     0,   420,   416,     0,   424
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    40,    41,   131,   260,   625,   133,   134,   261,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   328,   148,   281,   149,   150,   384,   537,   398,   549,
      43,   383,   650,   813,   536,   757,   849,   397,   659,   815,
     548,   762,   851,   519,   750,   847,   374,   396,    44,   392,
     656,   405,   665,   520,   751,   390,   403,   151,    46,    47,
      48,    49,   152,   199,   263,   153,    52,    53,    54,    55,
      56,   154,   248,    58,   698,   181,   182,   379,   183,   184,
     185,   718,   448,   449,   450,   249,    60,    61,    62,   350,
      64,   415,   236,   275,   416,    77,    78,   591,   592,   785,
     522,   523,   593,   655,   693,   694,   695,   724,   417,   418,
     419,   231,    65,    66,   428,   580,   581,   582,   699,   703,
     774,   775,    67,   257,   437,   439,   155,   156,   157,   158,
     357,   159,   361,   411,   160,   161,   162,   163,   278,   164,
     292,   290,   879,   876,   878,   877,   728,   165,   737,   738,
     739,   881,   166,    68,    69,    70,   227,   393,   406,   232,
     385,   399,   394,   407,   424,   535,   547,   542,   555,   233,
     234,   167,   168,    71,   251,    72,    73,   306,   307,   626,
     169,   480,   170,   171
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -718
static const yytype_int16 yypact[] =
{
    3469,   -26,   267,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
     241,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
      72,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
     114,  1629,  -718,   243,   437,   588,   -26,  5477,  3879,  2741,
    1972,   811,   -26,  -718,  5477,  5378,  2741,  1758,  -718,    59,
    -718,  -718,   217,   120,  -718,  -718,   444,  -718,  3575,  -718,
    -718,  -718,   -26,  -718,  3575,   157,  1922,   -26,  -718,   186,
     424,   222,     4,  -718,  -718,   228,  2502,  -718,   288,  -718,
    -718,  -718,  -718,  -718,  -718,  2375,  -718,   301,  -718,  5015,
    5015,   305,   271,   341,   203,  5094,  -718,   355,  -718,   372,
    -718,  -718,  -718,  -718,  -718,  -718,  5173,  5173,   385,  5173,
    5173,  5252,   359,  -718,  -718,  1837,   947,   396,   404,  -718,
    -718,  -718,   702,  -718,  5015,  1119,   195,   162,   423,   188,
     518,   452,   429,   458,   393,    92,  -718,  -718,   488,   501,
    -718,   588,  2078,   811,  1865,  -718,  -718,  -718,  1080,  1233,
    5433,  2267,  -718,  -718,  -718,  -718,  -718,  -718,   485,  -718,
    -718,  5015,  -718,   926,  -718,   413,  -718,   -26,    61,   310,
    -718,  -718,  -718,  -718,  -718,  -718,   540,   480,  1704,   929,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
     -26,   548,   564,  -718,  -718,  -718,   566,   729,   929,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,   576,
    -718,   586,   595,   598,  -718,  -718,  -718,   597,    56,  4461,
    -718,   608,   597,  -718,  5433,    28,   613,  -718,  -718,  -718,
     623,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  1922,  -718,
    -718,  -718,  -718,   310,  -718,   310,   562,   638,     4,  -718,
    1147,  1119,   488,   649,  -718,  -718,  5711,  1730,  1730,  5692,
    -718,  -718,   289,   364,  4002,   654,  1629,  -718,   667,  -718,
    -718,   659,   669,  -718,  3214,  -718,   688,   711,  2375,  -718,
    3133,  5015,   717,  -718,  4540,  -718,  -718,  -718,  -718,  -718,
    2375,  -718,  5015,   674,   730,  -718,   234,   739,  -718,  -718,
    4303,   136,  5015,   431,  -718,  -718,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  4619,  5015,
    5015,  5015,  5015,  5015,  5015,  5015,  5015,  5015,  5015,  5015,
    5015,  5015,  5015,  5015,  5015,  5015,  4699,  5015,  5015,  -718,
    -718,   -26,   403,  -718,   310,  -718,  -718,  -718,  1080,  2631,
    -718,  -718,  1233,  2756,  -718,   385,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,   639,   957,    61,  -718,   743,   310,
     748,    19,  -718,   639,   597,   597,  -718,  -718,  -718,   746,
     -26,  -718,   -26,   597,   597,  -718,   639,   639,   597,   597,
    -718,  -718,   753,   -26,  2502,   -26,   597,   597,  -718,   -26,
    1371,  -718,  -718,  -718,  -718,   307,  5433,   760,   762,   695,
    -718,   764,  4778,  -718,   597,  5433,  -718,  5566,    35,    53,
      77,  -718,  -718,  -718,  -718,  -718,  5015,  -718,     4,   771,
     638,  4857,  5015,  -718,  -718,   -26,   791,  -718,  -718,  -718,
    -718,  -718,  4936,  1629,  2631,  5015,  -718,  -718,   539,   774,
    -718,  -718,   783,   745,   377,  5015,   787,   789,   212,   674,
     274,  5015,   286,  -718,  -718,  5015,  -718,   947,   416,  -718,
     793,  -718,  -718,  -718,   263,  -718,  -718,  -718,   947,  -718,
    -718,  -718,  -718,   195,   195,   162,   162,   423,   423,   423,
     423,   188,   188,   518,   452,   429,   458,  5015,   652,   393,
    -718,  2142,   434,  -718,  2631,  -718,  -718,  2756,  -718,   639,
     -26,   803,   -26,  -718,    61,  2121,  -718,  -718,   438,   310,
     805,   310,  -718,  -718,  -718,   597,   639,   597,   731,  -718,
    -718,  -718,   597,   -26,  -718,  -718,  -718,   597,   639,   597,
     731,   807,  -718,  -718,  -718,   597,  -718,  -718,  1500,  2883,
    -718,   757,   701,  -718,  2741,  3679,   701,  2741,  3794,  -718,
    2176,  4224,  -718,  -718,   804,  -718,   927,   -26,  3898,   484,
    -718,   689,   718,  5566,  -718,  5566,  -718,    79,  -718,  -718,
    -718,   492,  -718,   819,  -718,   562,  -718,   820,    55,   824,
     826,   827,  4066,  -718,  -718,  2631,   835,   512,  -718,  5015,
    3295,  -718,   842,  3133,   513,  -718,  5594,   840,   674,   409,
    -718,  -718,   334,  -718,  -718,   841,   848,   850,  -718,  5015,
     852,  -718,   853,  -718,  5015,  2142,  -718,  -718,  -718,  -718,
     674,   731,   625,    61,  -718,  -718,  -718,  -718,  -718,  -718,
     -26,  -718,  -718,  -718,  4224,  -718,   731,  -718,   731,   -26,
    -718,  -718,  -718,  -718,  -718,   731,  -718,  -718,  3008,  -718,
    -718,   -26,   960,  -718,  -718,  -718,   -26,  1365,  -718,  -718,
    -718,  -718,   -26,  -718,  -718,  -718,  -718,  -718,   349,  5015,
     844,   854,  -718,  -718,  -718,  -718,  -718,  5015,   855,  -718,
    -718,   929,   855,  -718,    28,  -718,   403,  -718,  1704,  -718,
    -718,  -718,   851,    77,  5331,  -718,  -718,    55,  -718,   310,
    -718,  -718,  -718,  -718,   622,   857,  -718,  3133,  3133,   856,
     858,   863,  5015,   843,  3133,   869,  -718,    82,   864,  -718,
      45,   877,   674,  -718,   880,  5015,  -718,  -718,  5015,  -718,
     -26,   731,   108,  -718,  -718,   657,  -718,   -26,  -718,  -718,
    -718,  -718,   -26,  -718,  -718,   872,  1815,  -718,   810,   817,
      57,  4936,  4936,  -718,   -26,  -718,  -718,   -26,    80,  -718,
    -718,  -718,  -718,   532,   841,   893,  -718,  -718,  -718,  4145,
    -718,  -718,  -718,  5015,  5015,  3376,   554,  3133,  -718,  -718,
     891,   674,   674,  5015,  -718,   129,  -718,   895,   896,  -718,
    -718,  -718,  -718,   731,  -718,   731,  -718,   872,  1815,  4936,
    4936,  4382,  5015,  -718,  -718,  -718,  -718,   722,  -718,  5015,
    -718,  -718,  -718,   897,   900,   902,   903,   905,  -718,  -718,
     190,  -718,   912,   913,   674,   899,   924,   731,  -718,   731,
    -718,   731,  -718,  -718,  4936,  -718,   925,   940,  -718,  -718,
    -718,  -718,  -718,   941,   674,  -718,  -718,   208,  5015,  -718,
    -718,  -718,  -718,  -718,  4936,  -718,  3133,  3133,  3133,  3133,
    -718,   600,   485,   944,   674,   841,  -718,  -718,  -718,  -718,
    -718,   952,   674,  -718,   603,  -718,   485,   954,  -718
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -718,  -718,  -718,  -112,   -32,  -305,   849,  -718,   -41,   390,
     418,   433,   450,   617,   631,   634,   635,   632,  -718,   -77,
     237,   720,    48,  -218,   -94,   332,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,    17,   568,  -718,
     572,  -718,    20,   -20,   585,    47,   287,  -718,   -47,   351,
      67,   242,  -362,   339,   749,  -153,  -718,  -366,  -345,  -147,
    -718,  -222,  -427,  -393,   133,    12,    -1,  -718,    32,    16,
    -718,  -718,    -3,  -267,    39,   -44,   -73,  -718,   278,   168,
     149,  -718,  -718,  -389,  -559,  -717,   546,  -718,  -718,  -718,
     430,  -718,  -718,  -412,  -426,   -97,  -718,  -718,  -496,   295,
     300,   427,  -718,   754,   411,   571,  -116,  -718,  -718,   -69,
    -322,  -718,  -321,   499,  -135,    -8,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -696,  -718,
     211,   138,  -718,  -718,   -24,  -718,  -718,  -718,  -718,  -718,
    -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,  -718,   733,
    -718,  -718,  -235,   679,  -216,   806,   209,  -718,  -413,     2,
    -718,  -718,  -718,    38
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -447
static const yytype_int16 yytable[] =
{
     135,    79,   189,   587,   252,   478,   282,    85,   208,   132,
     528,   421,    59,   525,   305,   579,    63,    45,    81,   599,
      50,   462,   280,   358,   362,   378,   276,   190,   195,   196,
     527,   380,   433,   466,    80,   412,   513,   515,    74,   266,
      76,   516,   518,   723,   237,   366,   805,    51,   266,   426,
     239,   447,   451,   600,   823,   824,   584,   186,   279,   445,
     803,   187,   201,   206,   627,   177,   202,   207,   468,   221,
     446,    83,   412,   222,   586,   632,   376,    75,   229,   256,
      59,   821,    84,   188,    63,    45,    59,    86,    50,   188,
      63,    45,   250,   316,    50,   758,   223,   241,   800,   425,
     711,   827,   852,   853,   855,   840,    74,   801,   242,   414,
     346,   238,    74,   367,    87,    51,   252,   135,   135,   588,
     135,    51,    75,   413,   811,   226,   132,   132,    24,   132,
     368,    75,   606,   264,   262,   252,   478,   873,    75,   377,
    -438,   453,   273,   272,   589,   843,   414,   822,   867,   635,
     359,   363,   280,   268,   844,   590,    75,   886,   642,    75,
     243,   663,   268,   186,   353,   206,   355,   332,   333,   244,
     245,   579,   240,   579,   463,   246,   367,    79,   247,    35,
     645,   347,    75,    75,   354,   371,   354,   373,   279,   188,
     459,   188,   636,   223,    81,   230,   637,   336,   337,   329,
     371,   389,   253,   481,   574,   776,   863,   354,   330,   331,
      80,   188,   780,   230,   482,   864,   375,   673,   594,   276,
     402,   680,   526,   378,   883,   276,   188,   188,   617,   380,
     832,   276,   228,   884,   619,   135,   717,   618,   255,   391,
     229,   266,    57,   366,   132,   599,   188,   366,   258,   135,
     599,   470,   753,   266,   483,   256,   193,   471,   132,   203,
     431,    82,   172,   211,   432,   173,   225,   759,   454,   760,
     286,     1,   338,   339,   427,   558,   763,   755,   778,   600,
     367,   287,     2,   725,   600,   348,   284,   631,   490,   491,
     492,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   444,   279,   270,    83,   433,
      57,   348,   382,   472,   473,   474,    57,   135,   135,    84,
     277,   135,   135,   560,   283,   381,   132,   132,   269,   561,
     132,   132,    42,   229,    24,   273,   272,   274,   366,   464,
     674,   620,   272,   678,   681,   280,   683,   273,   272,   735,
     514,    79,   621,   623,   517,   268,   348,   266,   743,   280,
     484,   203,   810,   225,   624,   305,   285,   268,   445,   135,
     291,   378,   644,   265,   302,    79,   305,   380,   132,   446,
     266,   279,   265,   740,   512,    35,   434,   229,   435,   200,
     511,   250,    81,   613,   508,   279,   220,   293,   366,   348,
      42,   366,   559,   817,   818,   752,    42,   351,    80,   784,
     297,   603,   135,   135,   372,   308,   768,   351,   352,   433,
     527,   132,   132,   309,   848,   741,   850,   769,   352,   538,
     633,   539,   628,   562,   742,   595,   565,   267,   629,   228,
     254,   343,   550,   366,   552,   605,   267,   229,   556,   381,
     254,   264,   262,   381,   646,   342,   174,   229,   870,   175,
     871,   229,   872,   566,   235,   583,   279,   585,   427,   344,
      24,   268,   645,   135,   576,   211,   135,   367,   225,   773,
      24,   345,   132,   225,   598,   132,   709,   230,   710,   366,
     378,   200,   364,   220,   268,   786,   380,   733,   485,  -199,
    -440,   433,  -199,   607,   704,   334,   335,   740,   712,   486,
     348,    83,   530,   614,   713,   729,   731,   135,   135,   622,
     349,    35,    84,    79,   784,   380,   132,   132,   727,   734,
     274,    35,   532,   701,   348,   348,   266,   431,   266,   467,
      81,   432,   274,   354,   195,   543,   544,   479,   828,   487,
     668,    83,   366,   367,   829,   608,    80,   749,   609,   639,
    -104,   641,    84,   885,   135,   489,   740,   740,  -434,   266,
     837,  -199,   135,   132,   675,   265,   348,   679,   682,   601,
     684,   132,   658,  -199,  -444,   510,  -108,   265,   371,   176,
     702,   404,   177,   279,   354,   786,   786,   354,   354,  -199,
     354,   433,   433,   178,   340,   341,  -436,   223,   220,   740,
     354,   791,   792,   220,   188,  -446,   891,   410,   798,   897,
     280,   583,   892,   583,     5,   892,   585,   135,   252,   882,
     576,   422,   576,   429,    11,    12,   132,    14,   430,   267,
     381,   754,    79,   788,   789,   230,   269,   431,   229,   882,
     268,   267,   268,   436,    23,    24,   279,   896,   568,    81,
     438,   530,   647,   273,   649,   443,   179,   354,   638,   578,
     452,    79,   381,   812,   348,    80,    79,   634,   479,   489,
     229,   838,   455,   268,   456,   652,   135,   135,   457,   756,
     364,   265,    28,   135,   364,   132,   132,   661,   761,   833,
     834,   836,   132,   265,   512,   671,    35,   460,   705,   512,
     765,   706,   479,   371,   577,   766,   672,   310,   702,   311,
     371,   767,   493,   494,   229,   312,   191,   191,   191,   191,
     461,   423,   465,   209,   213,   215,   224,   707,   354,   188,
     708,  -337,   110,   427,  -337,   469,   188,   807,  -199,  -442,
     808,  -199,   495,   496,   475,   267,   135,   364,   521,   529,
     887,   888,   889,   890,   531,   132,  -106,   567,    24,   497,
     498,   499,   500,  -110,   203,   530,   569,   225,   267,   179,
     796,   313,   314,   315,   570,   364,   571,   225,   572,   809,
     501,   502,   596,   610,   180,   445,   814,   354,   354,   611,
     205,   816,   612,   615,   304,   601,   446,   412,   692,   630,
     601,   616,   204,   825,   229,   177,   826,   585,   640,    35,
    -199,   648,   654,   664,   670,   578,   178,   578,   696,   431,
     431,   191,  -199,   224,   714,   135,   135,   135,   135,   692,
     719,   716,   720,   721,   132,   132,   132,   132,  -199,   354,
     354,   726,   787,   192,   192,   192,   192,   732,   274,   736,
     210,   214,   216,   629,   744,   745,   746,   781,   748,   771,
     577,   747,   577,   790,   797,   793,   671,   794,    24,   772,
     697,   414,   795,   533,   534,   799,   802,   672,   530,   179,
     364,   692,   540,   541,    75,   229,   804,   545,   546,   806,
     180,   819,   205,   265,   200,   553,   554,   220,   820,   830,
     839,   845,   846,   858,   241,    28,   859,   220,   860,   861,
     386,   868,   370,   575,   862,   242,   770,   369,   865,    35,
     177,   177,   866,   177,   267,   395,   267,   387,   388,    24,
     400,   178,   178,   869,   178,   209,   213,   215,   224,   874,
     179,   479,   697,   224,   289,   408,   875,   401,   192,   503,
     880,   177,   294,   893,   671,   295,   296,   267,   298,   299,
     301,   895,   524,   898,   504,   672,   412,   243,   505,   509,
     506,   442,   479,   229,   563,   479,   244,   245,   564,   551,
      35,   782,   246,    24,    24,   247,    24,   857,   604,   241,
     687,   779,   777,   700,   179,   179,   715,   179,   692,   692,
     242,   597,   440,   841,   259,   110,   111,   112,   113,   114,
       0,   115,   894,     0,    24,     0,   692,    24,     0,   409,
      28,    28,    75,    28,   651,   179,   653,     0,     0,     0,
     842,   657,     0,     0,    35,    35,   660,    35,   662,     0,
     414,     0,     0,     0,   666,     0,   692,   692,   692,   856,
       0,     0,   243,    75,   123,   124,   479,     0,     0,     0,
       0,   244,   245,     0,   130,    35,     0,   246,    35,     0,
     247,    88,     0,    89,    90,    91,    92,    93,    94,     0,
       0,   692,     0,     0,     0,    95,     0,     0,     0,   -98,
      96,   356,     0,     0,     0,   479,     5,     6,     7,     8,
      97,   692,     9,    98,    99,    10,    11,    12,    13,    14,
     100,    15,    16,    17,    18,    19,   101,   102,    20,    21,
     103,   104,   105,    22,   106,   107,    23,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,   110,   111,
     112,   113,   114,     0,   115,    25,   116,   117,   118,     0,
     119,   120,   441,     0,   311,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,    28,    29,   121,    30,     0,     0,
     122,     0,     0,    31,    32,    33,    34,   123,   124,   125,
      36,   126,   127,    37,   128,   129,     0,   130,    38,    39,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,     0,     0,     0,     0,     0,   313,   314,   315,     0,
       0,     0,     0,     0,    88,     0,    89,    90,    91,    92,
      93,    94,     0,   191,   191,     0,   213,   224,    95,     0,
       0,     0,   -98,    96,     0,     0,     0,   224,     0,     5,
       6,     7,     8,    97,     0,     9,    98,    99,    10,    11,
      12,    13,    14,   100,    15,    16,    17,    18,    19,   101,
     102,    20,    21,   103,   104,   105,    22,   106,   107,    23,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   110,   111,   112,   113,   114,     0,   115,    25,   116,
     117,   118,     0,   119,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    27,     0,    28,    29,   121,
      30,     0,     0,   122,     0,     0,    31,    32,    33,    34,
     123,   124,   125,    36,   126,   127,    37,   128,   129,   360,
     130,    38,    39,     0,     0,     0,     0,     0,     0,   676,
     192,   192,    88,   214,    89,    90,    91,    92,    93,    94,
     677,   412,     0,     0,     0,     0,    95,     0,   229,     0,
     -98,    96,   557,     0,     0,     0,     0,     5,     6,     7,
       8,    97,     0,     9,    98,    99,    10,    11,    12,    13,
      14,   100,    15,    16,    17,    18,    19,   101,   102,    20,
      21,   103,   104,   105,    22,   106,   107,    23,   108,     0,
       0,     0,    24,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   114,     0,   115,    25,   116,   117,   118,
       0,   119,   120,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,     0,
       0,     0,     0,    27,     0,    28,    29,   121,    30,     0,
       0,   122,     0,    35,    31,    32,    33,    34,   123,   124,
     125,    36,   126,   127,    37,   128,   129,     0,   130,    38,
      39,    88,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,   -98,
      96,   667,     0,     0,     0,     0,     5,     6,     7,     8,
      97,     0,     9,    98,    99,    10,    11,    12,    13,    14,
     100,    15,    16,    17,    18,    19,   101,   102,    20,    21,
     103,   104,   105,    22,   106,   107,    23,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,   110,   111,
     112,   113,   114,     0,   115,    25,   116,   117,   118,     0,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,    28,    29,   121,    30,     0,     0,
     122,     0,     0,    31,    32,    33,    34,   123,   124,   125,
      36,   126,   127,    37,   128,   129,     0,   130,    38,    39,
      88,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,   -98,    96,
       0,     0,     0,     0,     0,     5,     6,     7,     8,    97,
       0,     9,    98,    99,    10,    11,    12,    13,    14,   100,
      15,    16,    17,    18,    19,   101,   102,    20,    21,   103,
     104,   105,    22,   106,   107,    23,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   109,   110,   111,   112,
     113,   114,     0,   115,    25,   116,   117,   118,   177,   119,
     120,     0,     0,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,    28,    29,   121,    30,     0,     0,   122,
       0,     0,    31,    32,    33,    34,   123,   124,   125,    36,
     126,   127,    37,   128,   129,     0,   130,    38,    39,   217,
       0,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    16,     2,     0,     0,     0,     0,     0,     0,
       0,     0,   179,    22,  -259,     6,     7,     8,     0,     0,
       9,     0,     0,    10,  -259,  -259,    13,  -259,     0,    15,
      16,    17,    18,    19,     0,    25,    20,    21,    28,     0,
       0,    22,     0,     0,  -259,     0,     0,     0,     0,   676,
       0,     0,    35,     0,     0,    24,     0,     0,     0,     0,
     677,     0,    27,    25,     0,    29,   218,     0,   229,     0,
       0,     0,     0,    31,    32,    33,     0,     0,     0,     0,
      36,     0,   303,    37,     0,     0,     0,   241,    38,    39,
      27,    75,     0,    29,     0,   219,   217,     0,   242,   351,
       0,    31,    32,    33,     0,     0,    35,     0,    36,    16,
     352,    37,    24,     0,     0,     0,    38,    39,     0,     0,
      22,  -259,     6,     7,     8,     0,     0,     9,     0,     0,
      10,  -259,  -259,    13,  -259,     0,    15,    16,    17,    18,
      19,     0,    25,    20,    21,     0,     0,     0,    22,     0,
     243,  -259,     0,     0,     0,     0,     1,     0,     0,   244,
     245,     0,    24,    35,     0,   246,     0,     2,   247,    27,
      25,     0,    29,   218,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     0,     0,     0,     0,    36,     0,     0,
      37,     0,     0,     0,   241,    38,    39,    27,    75,     0,
      29,     0,   219,   197,     0,   242,     1,     0,    31,    32,
      33,     0,     0,    35,     0,    36,     0,     2,    37,    24,
       0,     0,     0,    38,    39,     0,     0,     0,     5,     6,
       7,     8,     0,     0,     9,     0,     0,    10,    11,    12,
      13,    14,     0,    15,    16,    17,    18,    19,     0,     0,
      20,    21,     0,     0,     0,    22,     0,   243,    23,     0,
       0,     0,     0,     0,     0,     0,   244,   245,     0,    24,
      35,     0,   246,     0,     0,   247,     0,    25,     0,     0,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    29,     0,   197,
       0,     0,   351,     0,     0,    31,    32,    33,     0,     0,
      35,     0,    36,   352,     0,    37,     0,     0,     0,     0,
      38,    39,     0,     0,     5,     6,     7,     8,     0,     0,
       9,     0,     0,    10,    11,    12,    13,    14,     0,    15,
      16,    17,    18,    19,     0,   177,    20,    21,     0,     0,
       0,    22,     0,     0,    23,     0,   643,     0,     0,     0,
       0,     0,     0,     0,     0,    24,   351,     0,     0,     0,
       0,     0,     0,    25,     0,     0,   198,   352,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   685,     0,     0,
      27,     0,     0,    29,   241,     0,     0,     0,    24,     0,
       0,    31,    32,    33,     0,   242,    35,     0,    36,   179,
       0,    37,  -259,  -259,  -259,  -259,    38,    39,  -259,    24,
       0,  -259,  -259,  -259,  -259,  -259,     0,  -259,  -259,  -259,
    -259,  -259,     0,     0,  -259,  -259,   243,     0,     0,  -259,
       0,     0,  -259,     0,     0,   244,   245,     0,     0,    35,
       0,   246,     0,     0,   247,     0,     0,   243,     0,     0,
       0,  -259,     0,     0,  -259,     0,   244,   245,     0,     0,
      35,     0,   246,     0,     0,   247,   686,    -4,    88,     0,
      89,    90,    91,    92,    93,    94,     0,     0,  -259,    75,
       0,  -259,    95,  -259,     0,     0,   -98,    96,     0,  -259,
    -259,  -259,  -259,     0,     0,     0,  -259,    97,     0,  -259,
      98,    99,     0,     0,  -259,  -259,     0,   100,     0,     0,
       0,     0,     0,   101,   102,     0,     0,   103,   104,   105,
       0,   106,   107,     0,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,   110,   111,   112,   113,   114,
       0,   115,     0,   116,   117,   365,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   121,     0,     0,     0,   122,    89,    90,
      91,    92,    93,    94,   123,   124,   125,     0,   126,   127,
      95,   128,   129,     0,   130,    96,     0,     0,     0,     0,
       0,     0,     6,     7,     8,     0,     0,     9,     0,     0,
      10,     0,     0,    13,     0,     0,    15,    16,    17,    18,
      19,     0,     0,    20,    21,     0,     0,   105,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   110,   111,   112,   113,   114,     0,   115,
      25,   116,   117,    26,     0,   119,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,    28,
      29,   121,    30,     0,     0,   122,     0,     0,    31,    32,
      33,   271,   123,   124,     0,    36,     0,     0,    37,     0,
       0,     0,   130,    38,    39,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       7,     8,     0,     0,     9,     0,     0,    10,     0,     0,
      13,     0,     0,    15,    16,    17,    18,    19,     0,     0,
      20,    21,     0,     0,   105,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     110,   111,   112,   113,   114,     0,   115,    25,   116,   117,
      26,     0,   119,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,    28,    29,   121,    30,
       0,     0,   122,     0,     0,    31,    32,    33,     0,   123,
     124,     0,    36,     0,     0,    37,     0,     0,     0,   130,
      38,    39,    88,     0,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
     -98,    96,   356,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,   101,   102,     0,
       0,   103,   104,   105,     0,   106,   107,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   114,     0,   115,     0,   116,   117,   365,
       0,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
     125,     0,   126,   127,     0,   128,   129,    88,   130,    89,
      90,    91,    92,    93,    94,     0,     0,     5,     0,     0,
       0,    95,     0,     0,     0,   -98,    96,    11,    12,     0,
      14,     0,     0,    16,     0,     0,    97,     0,     0,    98,
      99,     0,     0,     0,    22,     0,   100,    23,     0,     0,
       0,     0,   101,   102,     0,     0,   103,   104,   105,     0,
     106,   107,     0,   108,     0,     0,    25,     0,     0,     0,
       0,     0,     0,   109,   110,   111,   112,   113,   114,     0,
     115,     0,   116,   117,   365,     0,   119,   120,     0,     0,
       0,     0,     0,    27,     0,     0,    29,     0,     0,     0,
       0,     0,     0,     0,    31,    32,    33,     0,     0,     0,
      28,    36,   121,     0,    37,     0,   122,     0,     0,    38,
      39,     0,     0,   123,   124,   125,     0,   126,   127,     0,
     128,   129,   360,   130,    88,     0,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,   -98,    96,   669,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,   101,
     102,     0,     0,   103,   104,   105,     0,   106,   107,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,   365,     0,   119,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,   125,     0,   126,   127,     0,   128,   129,    88,
     130,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,   -98,    96,   764,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,     0,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,   101,   102,     0,     0,   103,   104,
     105,     0,   106,   107,     0,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   109,   110,   111,   112,   113,
     114,     0,   115,     0,   116,   117,   365,     0,   119,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   121,     0,     0,     0,   122,     0,
       0,     0,     0,     0,     0,   123,   124,   125,     0,   126,
     127,     0,   128,   129,    88,   130,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,   -98,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,   101,
     102,     0,     0,   103,   104,   105,     0,   106,   107,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,   365,     0,   119,   120,   458,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,   -98,     0,     0,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,   125,     0,   126,   127,     0,   128,   129,     0,
     130,     0,     0,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,     0,     0,   119,   120,   730,     0,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,   -98,     0,     0,     0,    28,     0,
     121,     0,     0,     0,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   110,   111,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   119,   120,   835,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,     0,
       0,    95,   -98,     0,     0,     0,     0,     0,     0,    28,
       0,   121,     0,     0,     0,   122,     0,     0,     0,     0,
       0,     0,   123,   124,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   110,   111,   112,   113,   114,     0,
     115,     0,   116,   117,     0,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     0,     0,
      28,     0,   121,     0,     2,     0,   122,     3,     4,     0,
       0,     0,     0,   123,   124,     5,     6,     7,     8,     0,
       0,     9,     0,   130,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,     0,     0,    20,    21,     0,
       0,     0,    22,     0,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,    28,    29,     0,    30,     0,     0,     1,
       0,     0,    31,    32,    33,    34,     0,    35,     0,    36,
       2,     0,    37,     0,     4,     0,     0,    38,    39,     0,
       0,     5,     6,     7,     8,     0,     0,     9,     0,     0,
      10,    11,    12,    13,    14,     0,    15,    16,    17,    18,
      19,     0,     0,    20,    21,     0,     0,     0,    22,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,    26,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,    28,
      29,     0,    30,   676,     0,     0,     0,     0,    31,    32,
      33,    34,     0,    35,   677,    36,     0,     0,    37,     0,
       0,     0,   229,    38,    39,     5,     6,     7,     8,     0,
       0,     9,     0,     0,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,     0,     0,    20,    21,     0,
       0,     0,    22,     0,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,   198,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,    29,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,    35,   676,    36,
       0,     0,    37,     0,     0,     0,     0,    38,    39,   677,
    -313,     0,     0,     0,     0,     0,  -313,   229,     0,     0,
       0,     6,     7,     8,     0,     0,     9,     0,     0,    10,
       0,     0,    13,     0,     0,    15,    16,    17,    18,    19,
       0,     0,    20,    21,     0,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,   218,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,    75,   194,    29,
       0,   219,   351,     0,     0,     5,     0,    31,    32,    33,
       0,     0,    35,   352,    36,    11,    12,    37,    14,     0,
       0,    16,    38,    39,     0,     6,     7,     8,     0,     0,
       9,     0,    22,    10,     0,    23,    13,     0,     0,    15,
      16,    17,    18,    19,     0,     0,    20,    21,     0,     0,
       0,    22,     0,     0,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,   218,     0,     0,     0,
       0,    27,     0,     0,    29,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,    36,
      27,     0,    37,    29,     0,   219,   445,    38,    39,     0,
       0,    31,    32,    33,     0,     0,    35,   446,    36,     0,
       0,    37,     0,     0,     0,   229,    38,    39,     0,     6,
       7,     8,     0,     0,     9,     0,     0,    10,     0,     0,
      13,     0,     0,    15,    16,    17,    18,    19,     0,     0,
      20,    21,     0,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,    25,     0,     0,
     218,    95,     0,   688,     0,     0,   602,   722,     0,   689,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    29,     0,   219,
       0,     0,     0,     0,     0,    31,    32,    33,   105,     0,
       0,     0,    36,     0,     0,    37,     0,     0,     0,     0,
      38,    39,     0,   690,   110,   111,   112,   113,   114,     0,
     115,     0,   116,   117,   691,     0,   119,   120,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,     0,     0,
      95,     0,   688,     0,     0,   602,   831,     0,   689,     0,
      28,     0,   121,     0,     0,     0,   122,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,     0,     0,
       0,     0,     0,   130,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   690,   110,   111,   112,   113,   114,     0,   115,
       0,   116,   117,   691,     0,   119,   120,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,     0,     0,    95,
       0,   688,     0,     0,   602,     0,     0,   689,     0,    28,
       0,   121,     0,     0,     0,   122,     0,     0,     0,     0,
       0,     0,   123,   124,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   690,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,   691,     0,   119,   120,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,    95,   476,
       0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     121,     0,     0,     0,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,     0,     0,   119,   120,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,   602,     0,     0,     0,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,   477,     0,     0,     0,     0,     0,
     130,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     110,   111,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   119,   120,    89,    90,    91,    92,    93,    94,
       0,     0,     0,   854,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,   420,    28,     0,   121,     0,
       0,     0,   122,     0,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   110,
     111,   112,   113,   114,     0,   115,     0,   116,   117,     0,
       0,   119,   120,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
       0,     0,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   110,   111,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     119,   120,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   121,     0,     0,     0,
     122,     0,     0,     0,     0,     0,     0,   123,   124,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   110,   111,   112,
     113,   114,     0,   115,     0,   116,   117,     0,     0,   119,
     120,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    28,   507,   121,     0,     0,     0,   122,
       0,     0,     0,     0,     0,     0,   123,   124,     0,     0,
     488,     0,     0,     0,     0,     0,   130,     0,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   110,   111,   112,
     113,   114,     0,   115,     0,   116,   117,     0,     0,   119,
     120,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,   573,    28,     0,   121,     0,     0,     0,   122,
       0,     0,     0,     0,     0,     0,   123,   124,     0,     0,
       0,     0,     0,     0,     0,     0,   130,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   110,   111,   112,   113,
     114,     0,   115,     0,   116,   117,     0,     0,   119,   120,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
       0,     0,    95,   476,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   121,     0,     0,     0,   122,     0,
       0,     0,     0,     0,     0,   123,   124,     0,     0,     0,
       0,     0,     0,     0,     0,   130,     0,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,   110,   111,   112,   113,   114,
       0,   115,     0,   116,   117,     0,     0,   119,   120,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,   602,     0,     0,     0,
       0,    28,     0,   121,     0,     0,     0,   122,     0,     0,
       0,     0,     0,     0,   123,   124,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   110,   111,   112,   113,   114,     0,
     115,     0,   116,   117,     0,     0,   119,   120,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   121,     0,     0,     0,   122,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,     0,     0,
       0,     0,     0,   130,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   110,   111,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   119,   120,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,     0,     0,   288,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   121,     0,     0,     0,   122,     0,     0,     0,     0,
       0,     0,   123,   124,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,     0,     0,   119,   120,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,   294,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     121,     0,     0,     0,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,     0,     0,   119,   120,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     110,   111,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   119,   120,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   121,     0,
       0,     0,   122,     0,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,   783,   110,
     111,   112,   113,   114,     5,   115,     0,   116,   117,     0,
       0,   119,   120,     0,    11,    12,     0,    14,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,    23,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
       0,     0,     0,    25,     0,     0,     0,     0,   130,     5,
       6,     7,     8,     0,     0,     9,     0,     0,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,     0,
      27,    20,    21,    29,     0,     0,    22,     0,     0,    23,
       0,    31,    32,    33,     0,     0,     0,     0,    36,     0,
       0,    37,     0,     5,     6,     7,    38,    39,    25,     9,
       0,    26,     0,    11,    12,    13,    14,     0,     0,    16,
      17,    18,    19,     0,     0,    20,    21,     0,     0,     0,
      22,     0,     0,    23,     0,    27,     0,     0,    29,     0,
      30,     0,     0,     0,     0,     0,    31,    32,    33,    34,
       0,     0,    25,    36,     0,     0,    37,     0,     0,     0,
       0,    38,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     6,     7,     8,     0,    36,     9,     0,
      37,    10,     0,     0,    13,    38,    39,    15,    16,    17,
      18,    19,     0,     0,    20,    21,     0,     0,     0,    22,
       0,     6,     7,     8,     0,     0,     9,     0,     0,    10,
       0,     0,    13,     0,     0,    15,    16,    17,    18,    19,
       0,    25,    20,    21,    26,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    27,    25,
      28,    29,    26,    30,     0,     0,     0,     0,     0,    31,
      32,    33,     0,     0,     0,     0,    36,     0,     0,    37,
       0,     0,     0,     0,    38,    39,    27,     0,     0,    29,
       0,    30,     0,     0,     0,     0,     0,    31,    32,    33,
     271,     0,     0,     0,    36,     0,     0,    37,     0,     6,
       7,     8,    38,    39,     9,     0,     0,    10,     0,     0,
      13,     0,     0,    15,    16,    17,    18,    19,     6,     7,
      20,    21,     0,     9,     0,    22,     0,     0,     0,    13,
       0,     0,     0,    16,    17,    18,    19,     0,     0,    20,
      21,     0,     0,     0,    22,     0,     0,    25,     0,     0,
     218,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    29,     0,   219,
       0,     0,     0,     0,     0,    31,    32,    33,     0,     0,
       0,     0,    36,    27,     0,    37,    29,     0,     0,     0,
      38,    39,     0,     0,    31,    32,    33,     0,     0,     0,
       0,    36,     0,     0,    37,     0,     0,     0,     0,    38,
      39
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-718))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      41,     2,    46,   429,    77,   310,   100,    10,    52,    41,
     376,   229,     0,   375,   126,   427,     0,     0,     2,   446,
       0,   288,    99,   158,   159,   178,    95,    47,    48,    49,
     375,   178,   248,   300,     2,    16,   358,   359,     0,    86,
       1,   362,   363,   602,    68,   161,   742,     0,    95,    21,
      74,   273,   274,   446,   771,   772,    21,    45,    99,     4,
      15,    45,    50,    51,   477,     4,    50,    51,   303,    57,
      15,    67,    16,    57,    21,   488,    15,   103,    23,    82,
      68,    24,    78,    45,    68,    68,    74,    15,    68,    51,
      74,    74,    76,   134,    74,   654,    57,    42,    16,   234,
      21,    21,   819,   820,   821,   801,    68,    25,    53,    90,
      18,    72,    74,    68,     0,    68,   189,   158,   159,    42,
     161,    74,   103,    67,    16,    66,   158,   159,    67,   161,
     171,   103,   454,    86,    86,   208,   441,   854,   103,    78,
      20,   276,    95,    95,    67,    16,    90,    90,   844,   511,
     158,   159,   229,    86,    25,    78,   103,   874,   524,   103,
     105,   550,    95,   151,   152,   153,   154,     5,     6,   114,
     115,   583,    15,   585,   290,   120,    68,   178,   123,   118,
     525,    89,   103,   103,   152,   173,   154,   175,   229,   151,
     284,   153,   514,   154,   178,    62,   517,     9,    10,     4,
     188,   189,    16,    67,   422,   701,    16,   175,    13,    14,
     178,   173,   708,    80,    78,    25,   177,   562,   436,   288,
     208,   566,   375,   376,    16,   294,   188,   189,    16,   376,
     789,   300,    15,    25,   469,   276,   598,    25,    16,   200,
      23,   288,     0,   359,   276,   672,   208,   363,    20,   290,
     677,    17,   641,   300,   118,   258,    47,    23,   290,    50,
     248,    20,    19,    54,   248,    22,    57,   656,   276,   658,
      67,     4,    84,    85,   235,   410,   665,   643,   704,   672,
      68,    78,    15,   605,   677,    22,    15,    24,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,    16,   347,    19,    67,   525,
      68,    22,   179,    79,    80,    81,    74,   358,   359,    78,
      19,   362,   363,    16,    19,    15,   358,   359,    86,    22,
     362,   363,     0,    23,    67,   288,   288,    95,   454,   291,
     562,    67,   294,   565,   566,   422,   568,   300,   300,   616,
     358,   352,    78,    67,   362,   288,    22,   404,    24,   436,
     312,   152,   751,   154,    78,   477,    25,   300,     4,   410,
      15,   524,   525,    86,    15,   376,   488,   524,   410,    15,
     427,   422,    95,   618,   352,   118,   253,    23,   255,    50,
     351,   375,   376,    16,   346,   436,    57,    25,   514,    22,
      68,   517,   410,   765,   766,   640,    74,     4,   376,   714,
      25,   452,   453,   454,     1,    19,    67,     4,    15,   635,
     765,   453,   454,    19,   813,    16,   815,    78,    15,   390,
     507,   392,    16,   416,    25,   438,   416,    86,    22,    15,
      16,    12,   403,   559,   405,   453,    95,    23,   409,    15,
      16,   404,   404,    15,    16,     3,    19,    23,   847,    22,
     849,    23,   851,   416,    20,   427,   507,   428,   429,    11,
      67,   404,   817,   514,   427,   266,   517,    68,   269,   697,
      67,    88,   514,   274,   445,   517,   583,   354,   585,   605,
     643,   152,   160,   154,   427,   717,   643,   613,    67,    19,
      20,   717,    22,   455,    20,    82,    83,   742,    16,    78,
      22,    67,   379,   465,    22,   609,   610,   558,   559,   471,
      19,   118,    78,   524,   829,   672,   558,   559,    16,    16,
     288,   118,   383,   577,    22,    22,   583,   525,   585,   302,
     524,   525,   300,   511,   564,   396,   397,   310,    16,   118,
     558,    67,   668,    68,    22,    16,   524,   634,    19,   520,
      20,   522,    78,   868,   605,   328,   801,   802,    20,   616,
      16,    91,   613,   605,   562,   288,    22,   565,   566,   446,
     568,   613,   543,   103,    20,   348,    20,   300,   576,     1,
     578,    15,     4,   634,   562,   817,   818,   565,   566,   119,
     568,   817,   818,    15,    86,    87,    20,   568,   269,   844,
     578,   727,   728,   274,   576,    20,    16,    20,   734,    16,
     697,   583,    22,   585,    26,    22,   587,   668,   701,   864,
     583,    23,   585,    20,    36,    37,   668,    39,    15,   288,
      15,    16,   643,    21,    22,   512,   404,   635,    23,   884,
     583,   300,   585,    91,    56,    67,   697,   892,   416,   643,
      22,   528,   529,   616,   531,    16,    78,   635,   519,   427,
      16,   672,    15,    16,    22,   643,   677,    25,   441,   442,
      23,   797,    15,   616,    25,   536,   727,   728,    19,   650,
     358,   404,   104,   734,   362,   727,   728,   548,   659,   793,
     794,   795,   734,   416,   672,     4,   118,    19,    19,   677,
     671,    22,   475,   701,   427,   676,    15,    15,   706,    17,
     708,   682,   332,   333,    23,    23,    47,    48,    49,    50,
      19,   232,    15,    54,    55,    56,    57,    19,   706,   701,
      22,    19,    68,   704,    22,    15,   708,   745,    19,    20,
     748,    22,   334,   335,    15,   404,   797,   425,   119,    16,
     876,   877,   878,   879,    16,   797,    20,   416,    67,   336,
     337,   338,   339,    20,   565,   642,    16,   568,   427,    78,
     732,    79,    80,    81,    22,   453,    91,   578,    24,   750,
     340,   341,    21,    19,    45,     4,   757,   765,   766,    16,
      51,   762,    57,    16,   125,   672,    15,    16,   571,    16,
     677,    22,     1,   774,    23,     4,   777,   778,    15,   118,
      91,    16,    91,    16,    67,   583,    15,   585,    24,   817,
     818,   152,   103,   154,    15,   876,   877,   878,   879,   602,
      16,    21,    16,    16,   876,   877,   878,   879,   119,   817,
     818,    16,   719,    47,    48,    49,    50,    15,   616,    19,
      54,    55,    56,    22,    16,    15,   629,    16,    15,    25,
     583,    19,   585,    16,    31,    19,     4,    19,    67,    25,
      25,    90,    19,   384,   385,    16,    22,    15,   755,    78,
     558,   654,   393,   394,   103,    23,    19,   398,   399,    19,
     151,    91,   153,   616,   565,   406,   407,   568,    91,    16,
      19,    16,    16,    16,    42,   104,    16,   578,    16,    16,
     187,    22,   173,   424,    19,    53,   689,     1,    16,   118,
       4,     4,    19,     4,   583,   202,   585,   188,   189,    67,
     207,    15,    15,    19,    15,   266,   267,   268,   269,    24,
      78,   714,    25,   274,   105,   222,    16,   208,   152,   342,
      19,     4,    15,    19,     4,   116,   117,   616,   119,   120,
     121,    19,    15,    19,   343,    15,    16,   105,   344,   347,
     345,   261,   745,    23,   416,   748,   114,   115,   416,   404,
     118,   713,   120,    67,    67,   123,    67,   829,   452,    42,
     570,   706,   702,   576,    78,    78,   595,    78,   771,   772,
      53,   440,   258,   802,    67,    68,    69,    70,    71,    72,
      -1,    74,   884,    -1,    67,    -1,   789,    67,    -1,   223,
     104,   104,   103,   104,   535,    78,   537,    -1,    -1,    -1,
     803,   542,    -1,    -1,   118,   118,   547,   118,   549,    -1,
      90,    -1,    -1,    -1,   555,    -1,   819,   820,   821,   822,
      -1,    -1,   105,   103,   117,   118,   829,    -1,    -1,    -1,
      -1,   114,   115,    -1,   127,   118,    -1,   120,   118,    -1,
     123,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,   854,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,   868,    26,    27,    28,    29,
      30,   874,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    -1,
      80,    81,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,   107,    -1,    -1,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   564,   565,    -1,   567,   568,    15,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,   578,    -1,    26,
      27,    28,    29,    30,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
     107,    -1,    -1,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,     4,
     564,   565,     1,   567,     3,     4,     5,     6,     7,     8,
      15,    16,    -1,    -1,    -1,    -1,    15,    -1,    23,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      -1,    80,    81,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,   107,    -1,
      -1,   110,    -1,   118,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,   107,    -1,    -1,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,     4,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,   107,    -1,    -1,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,     1,
      -1,    -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    42,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    53,    26,    27,    28,    29,    -1,    -1,
      32,    -1,    -1,    35,    36,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    -1,    75,    48,    49,   104,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,     4,
      -1,    -1,   118,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      15,    -1,   102,    75,    -1,   105,    78,    -1,    23,    -1,
      -1,    -1,    -1,   113,   114,   115,    -1,    -1,    -1,    -1,
     120,    -1,    15,   123,    -1,    -1,    -1,    42,   128,   129,
     102,   103,    -1,   105,    -1,   107,     1,    -1,    53,     4,
      -1,   113,   114,   115,    -1,    -1,   118,    -1,   120,    42,
      15,   123,    67,    -1,    -1,    -1,   128,   129,    -1,    -1,
      53,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    36,    37,    38,    39,    -1,    41,    42,    43,    44,
      45,    -1,    75,    48,    49,    -1,    -1,    -1,    53,    -1,
     105,    56,    -1,    -1,    -1,    -1,     4,    -1,    -1,   114,
     115,    -1,    67,   118,    -1,   120,    -1,    15,   123,   102,
      75,    -1,   105,    78,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    42,   128,   129,   102,   103,    -1,
     105,    -1,   107,     1,    -1,    53,     4,    -1,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    15,   123,    67,
      -1,    -1,    -1,   128,   129,    -1,    -1,    -1,    26,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    49,    -1,    -1,    -1,    53,    -1,   105,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,    67,
     118,    -1,   120,    -1,    -1,   123,    -1,    75,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,     1,
      -1,    -1,     4,    -1,    -1,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    15,    -1,   123,    -1,    -1,    -1,    -1,
     128,   129,    -1,    -1,    26,    27,    28,    29,    -1,    -1,
      32,    -1,    -1,    35,    36,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    -1,     4,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    56,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    78,    15,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,     1,    -1,    -1,
     102,    -1,    -1,   105,    42,    -1,    -1,    -1,    67,    -1,
      -1,   113,   114,   115,    -1,    53,   118,    -1,   120,    78,
      -1,   123,    26,    27,    28,    29,   128,   129,    32,    67,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    -1,    -1,    48,    49,   105,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,   105,    -1,    -1,
      -1,    75,    -1,    -1,    78,    -1,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    90,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,   102,   103,
      -1,   105,    15,   107,    -1,    -1,    19,    20,    -1,   113,
     114,   115,   116,    -1,    -1,    -1,   120,    30,    -1,   123,
      33,    34,    -1,    -1,   128,   129,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    47,    -1,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    78,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,     3,     4,
       5,     6,     7,     8,   117,   118,   119,    -1,   121,   122,
      15,   124,   125,    -1,   127,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    -1,    -1,    38,    -1,    -1,    41,    42,    43,    44,
      45,    -1,    -1,    48,    49,    -1,    -1,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,   107,    -1,    -1,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,   127,   128,   129,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    49,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,   107,
      -1,    -1,   110,    -1,    -1,   113,   114,   115,    -1,   117,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,   127,
     128,   129,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    78,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,    -1,   121,   122,    -1,   124,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    26,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    19,    20,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    30,    -1,    -1,    33,
      34,    -1,    -1,    -1,    53,    -1,    40,    56,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    57,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,    -1,    -1,    -1,
     104,   120,   106,    -1,   123,    -1,   110,    -1,    -1,   128,
     129,    -1,    -1,   117,   118,   119,    -1,   121,   122,    -1,
     124,   125,   126,   127,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,   119,    -1,   121,   122,    -1,   124,   125,     1,
     127,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    76,    77,    78,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,    -1,   121,
     122,    -1,   124,   125,     1,   127,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    19,    -1,    -1,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,   119,    -1,   121,   122,    -1,   124,   125,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    -1,    -1,    80,    81,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    19,    -1,    -1,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    -1,    -1,    80,    81,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,    -1,    15,    -1,   110,    18,    19,    -1,
      -1,    -1,    -1,   117,   118,    26,    27,    28,    29,    -1,
      -1,    32,    -1,   127,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,    -1,   107,    -1,    -1,     4,
      -1,    -1,   113,   114,   115,   116,    -1,   118,    -1,   120,
      15,    -1,   123,    -1,    19,    -1,    -1,   128,   129,    -1,
      -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    36,    37,    38,    39,    -1,    41,    42,    43,    44,
      45,    -1,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,    -1,   107,     4,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,    -1,   118,    15,   120,    -1,    -1,   123,    -1,
      -1,    -1,    23,   128,   129,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    -1,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,    -1,    -1,   118,     4,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,   128,   129,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    38,    -1,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    19,   105,
      -1,   107,     4,    -1,    -1,    26,    -1,   113,   114,   115,
      -1,    -1,   118,    15,   120,    36,    37,   123,    39,    -1,
      -1,    42,   128,   129,    -1,    27,    28,    29,    -1,    -1,
      32,    -1,    53,    35,    -1,    56,    38,    -1,    -1,    41,
      42,    43,    44,    45,    -1,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    78,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
     102,    -1,   123,   105,    -1,   107,     4,   128,   129,    -1,
      -1,   113,   114,   115,    -1,    -1,   118,    15,   120,    -1,
      -1,   123,    -1,    -1,    -1,    23,   128,   129,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    75,    -1,    -1,
      78,    15,    -1,    17,    -1,    -1,    20,    21,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,    52,    -1,
      -1,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
     128,   129,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    17,    -1,    -1,    20,    21,    -1,    23,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    -1,    80,    81,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    17,    -1,    -1,    20,    -1,    -1,    23,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    -1,    -1,    80,    81,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      -1,    -1,    80,    81,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    91,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    -1,
      -1,    80,    81,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    76,    77,    -1,    -1,
      80,    81,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    -1,    -1,    80,
      81,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    25,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    -1,    -1,    80,
      81,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,   104,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    76,    77,    -1,    -1,    80,    81,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    -1,    -1,    80,    81,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    -1,    -1,    80,    81,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    -1,    -1,    80,    81,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    -1,    -1,    80,    81,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    -1,    -1,    80,    81,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      -1,    -1,    80,    81,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    67,    68,
      69,    70,    71,    72,    26,    74,    -1,    76,    77,    -1,
      -1,    80,    81,    -1,    36,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    56,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,   127,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    -1,    41,    42,    43,    44,    45,    -1,
     102,    48,    49,   105,    -1,    -1,    53,    -1,    -1,    56,
      -1,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,   123,    -1,    26,    27,    28,   128,   129,    75,    32,
      -1,    78,    -1,    36,    37,    38,    39,    -1,    -1,    42,
      43,    44,    45,    -1,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,   102,    -1,    -1,   105,    -1,
     107,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
      -1,    -1,    75,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,    27,    28,    29,    -1,   120,    32,    -1,
     123,    35,    -1,    -1,    38,   128,   129,    41,    42,    43,
      44,    45,    -1,    -1,    48,    49,    -1,    -1,    -1,    53,
      -1,    27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    38,    -1,    -1,    41,    42,    43,    44,    45,
      -1,    75,    48,    49,    78,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    75,
     104,   105,    78,   107,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,   128,   129,   102,    -1,    -1,   105,
      -1,   107,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,    -1,    -1,    -1,   120,    -1,    -1,   123,    -1,    27,
      28,    29,   128,   129,    32,    -1,    -1,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    27,    28,
      48,    49,    -1,    32,    -1,    53,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    43,    44,    45,    -1,    -1,    48,
      49,    -1,    -1,    -1,    53,    -1,    -1,    75,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,   102,    -1,   123,   105,    -1,    -1,    -1,
     128,   129,    -1,    -1,   113,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,   128,
     129
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     4,    15,    18,    19,    26,    27,    28,    29,    32,
      35,    36,    37,    38,    39,    41,    42,    43,    44,    45,
      48,    49,    53,    56,    67,    75,    78,   102,   104,   105,
     107,   113,   114,   115,   116,   118,   120,   123,   128,   129,
     131,   132,   155,   160,   178,   187,   188,   189,   190,   191,
     192,   195,   196,   197,   198,   199,   200,   201,   203,   215,
     216,   217,   218,   219,   220,   242,   243,   252,   283,   284,
     285,   303,   305,   306,   313,   103,   224,   225,   226,   216,
     218,   219,    20,    67,    78,   222,    15,     0,     1,     3,
       4,     5,     6,     7,     8,    15,    20,    30,    33,    34,
      40,    46,    47,    50,    51,    52,    54,    55,    57,    67,
      68,    69,    70,    71,    72,    74,    76,    77,    78,    80,
      81,   106,   110,   117,   118,   119,   121,   122,   124,   125,
     127,   133,   134,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   152,   154,
     155,   187,   192,   195,   201,   256,   257,   258,   259,   261,
     264,   265,   266,   267,   269,   277,   282,   301,   302,   310,
     312,   313,    19,    22,    19,    22,     1,     4,    15,    78,
     204,   205,   206,   208,   209,   210,   215,   219,   313,   225,
     193,   303,   305,   306,    19,   193,   193,     1,    78,   193,
     203,   215,   219,   306,     1,   204,   215,   219,   225,   303,
     305,   306,    19,   303,   305,   303,   305,     1,    78,   107,
     203,   215,   219,   224,   303,   306,    66,   286,    15,    23,
     214,   241,   289,   299,   300,    20,   222,   284,   224,   284,
      15,    42,    53,   105,   114,   115,   120,   123,   202,   215,
     219,   304,   226,    16,    16,    16,   222,   253,    20,    67,
     134,   138,   152,   194,   195,   196,   198,   199,   200,   201,
      19,   116,   152,   195,   201,   223,   259,    19,   268,   138,
     149,   153,   154,    19,    15,    25,    67,    78,    15,   136,
     271,    15,   270,    25,    15,   136,   136,    25,   136,   136,
      15,   136,    15,    15,   303,   133,   307,   308,    19,    19,
      15,    17,    23,    79,    80,    81,   138,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   151,     4,
      13,    14,     5,     6,    82,    83,     9,    10,    84,    85,
      86,    87,     3,    12,    11,    88,    18,    89,    22,    19,
     219,     4,    15,   215,   218,   215,    21,   260,   264,   265,
     126,   262,   264,   265,   155,    78,   256,    68,   138,     1,
     204,   215,     1,   215,   176,   224,    15,    78,   205,   207,
     209,    15,   214,   161,   156,   290,   299,   204,   204,   215,
     185,   224,   179,   287,   292,   299,   177,   167,   158,   291,
     299,   204,   215,   186,    15,   181,   288,   293,   299,   305,
      20,   263,    16,    67,    90,   221,   224,   238,   239,   240,
      24,   153,    23,   263,   294,   264,    21,   224,   244,    20,
      15,   215,   219,   304,   214,   214,    91,   254,    22,   255,
     253,    15,   151,    16,    16,     4,    15,   211,   212,   213,
     214,   211,    16,   264,   265,    15,    25,    19,     1,   154,
      19,    19,   223,   256,   152,    15,   223,   150,   302,    15,
      17,    23,    79,    80,    81,    15,    16,   121,   135,   150,
     311,    67,    78,   118,   152,    67,    78,   118,   121,   150,
     138,   138,   138,   139,   139,   140,   140,   141,   141,   141,
     141,   142,   142,   143,   144,   145,   146,    25,   152,   147,
     150,   224,   218,   260,   265,   260,   262,   265,   262,   173,
     183,   119,   230,   231,    15,   202,   205,   208,   207,    16,
     214,    16,   230,   263,   263,   295,   164,   157,   224,   224,
     263,   263,   297,   230,   230,   263,   263,   296,   170,   159,
     224,   194,   224,   263,   263,   298,   224,    21,   264,   265,
      16,    22,   187,   188,   190,   192,   195,   199,   201,    16,
      22,    91,    24,    24,   153,   263,   195,   196,   201,   243,
     245,   246,   247,   313,    21,   224,    21,   244,    42,    67,
      78,   227,   228,   232,   153,   222,    21,   255,   224,   212,
     213,   214,    20,   138,   236,   265,   260,   152,    16,    19,
      19,    16,    57,    16,   152,    16,    22,    16,    25,   302,
      67,    78,   152,    67,    78,   135,   309,   308,    16,    22,
      16,    24,   308,   149,    25,   202,   260,   262,   230,   224,
      15,   224,   207,    15,   205,   208,    16,   214,    16,   214,
     162,   263,   230,   263,    91,   233,   180,   263,   224,   168,
     263,   230,   263,   233,    16,   182,   263,    21,   265,    21,
      67,     4,    15,   208,   211,   215,     4,    15,   211,   215,
     208,   211,   215,   211,   215,     1,    90,   240,    17,    23,
      67,    78,   150,   234,   235,   236,    24,    25,   204,   248,
     251,   225,   215,   249,    20,    19,    22,    19,    22,   245,
     245,    21,    16,    22,    15,   254,    21,   202,   211,    16,
      16,    16,    21,   234,   237,   260,    16,    16,   276,   154,
       1,   154,    15,   256,    16,   223,    19,   278,   279,   280,
     302,    16,    25,    24,    16,    15,   150,    19,    15,   149,
     174,   184,   302,   233,    16,   207,   224,   165,   234,   233,
     233,   224,   171,   233,    21,   224,   224,   224,    67,    78,
     150,    25,    25,   153,   250,   251,   248,   250,   244,   249,
     248,    16,   228,    67,   135,   229,   211,   214,    21,    22,
      16,   256,   256,    19,    19,    19,   152,    31,   256,    16,
      16,    25,    22,    15,    19,   278,    19,   309,   309,   224,
     233,    16,    16,   163,   224,   169,   224,   202,   202,    91,
      91,    24,    90,   235,   235,   224,   224,    21,    16,    22,
      16,    21,   234,   154,   154,     1,   154,    16,   256,    19,
     278,   280,   150,    16,    25,    16,    16,   175,   233,   166,
     233,   172,   235,   235,    91,   235,   150,   229,    16,    16,
      16,    16,    19,    16,    25,    16,    19,   278,    22,    19,
     233,   233,   233,   235,    24,    16,   273,   275,   274,   272,
      19,   281,   302,    16,    25,   135,   235,   256,   256,   256,
     256,    16,    22,    19,   281,    19,   302,    16,    19
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
      yychar = YYLEX;
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

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

/* Line 1806 of yacc.c  */
#line 395 "ANSI-C.y"
    {
                       if (ANSIOnly) SyntaxError("ANSI C forbids an empty source file");
                       Program = GrabPragmas(NULL);
                     }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 399 "ANSI-C.y"
    { Program = JoinLists((yyvsp[(1) - (1)].L), GrabPragmas(0)); parseInputType = EntireProgram; }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 400 "ANSI-C.y"
    { StmtListOutput = (yyvsp[(2) - (2)].L); parseInputType = StatementListString; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 401 "ANSI-C.y"
    {DeclListOutput = (yyvsp[(2) - (2)].L); parseInputType = DeclListString; }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 405 "ANSI-C.y"
    {}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 417 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 420 "ANSI-C.y"
    { (yyvsp[(2) - (3)].n)->coord = (yyvsp[(1) - (3)].tok);   /* We used to set the coord only for comma expressions.  We really need to do it everywhere. */ 
                                  (yyvsp[(2) - (3)].n)->parenthesized = TRUE;
                                  (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 426 "ANSI-C.y"
    { if (ANSIOnly)
	         SyntaxError("statement expressions not allowed with -ansi switch");
               (yyval.n) = MakeBlockCoord(NULL, NULL, GrabPragmas((yyvsp[(3) - (5)].L)), (yyvsp[(1) - (5)].tok), (yyvsp[(4) - (5)].tok));
	    }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 431 "ANSI-C.y"
    { if (ANSIOnly)
	         SyntaxError("statement expressions not allowed with -ansi switch");
              (yyval.n) = MakeBlockCoord(NULL, (yyvsp[(3) - (6)].L), GrabPragmas((yyvsp[(4) - (6)].L)), (yyvsp[(1) - (6)].tok), (yyvsp[(5) - (6)].tok));
	    }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 436 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			 SyntaxError("Wildcard tokens not allowed in program. \n");
		       }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 442 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			 SyntaxError("Wildcard tokens not allowed in program. \n");
		       }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 453 "ANSI-C.y"
    { (yyval.n) = ExtendArray((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok)); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 455 "ANSI-C.y"
    { (yyval.n) = MakeCallCoord((yyvsp[(1) - (3)].n), NULL, (yyvsp[(2) - (3)].tok)); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 457 "ANSI-C.y"
    { (yyval.n) = MakeCallCoord((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].L), (yyvsp[(2) - (4)].tok)); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 459 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 461 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 463 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTINC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 465 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTDEC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 469 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 471 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 474 "ANSI-C.y"
    { if (!AcceptWildcards()) {
	         SyntaxError("Wildcard tokens not allowed in program. \n");
               }
	       (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok));
	     }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 480 "ANSI-C.y"
    { if (!AcceptWildcards()) {
	         SyntaxError("Wildcard tokens not allowed in program. \n");
               }
	       (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok));
	    }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 489 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 491 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 496 "ANSI-C.y"
    { (yyval.n) = OmitLookups() ? (yyvsp[(1) - (1)].n) : LookupPostfixExpression((yyvsp[(1) - (1)].n));}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 498 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(PREINC, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 500 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(PREDEC, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 502 "ANSI-C.y"
    { (yyvsp[(1) - (2)].n)->u.unary.expr = (yyvsp[(2) - (2)].n);
              (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 505 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 507 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(SIZEOF, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 509 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(SIZEOF, (yyvsp[(3) - (4)].n), (yyvsp[(1) - (4)].tok)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 511 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(ALIGNOF, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 513 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(ALIGNOF, (yyvsp[(3) - (4)].n), (yyvsp[(1) - (4)].tok)); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 515 "ANSI-C.y"
    { (yyval.n) = MakeBuiltinVaArgCoord((yyvsp[(3) - (6)].n), (yyvsp[(5) - (6)].n), (yyvsp[(1) - (6)].tok)); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 517 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(IMAG, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 519 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(REAL, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 523 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('&', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 524 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('*', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 525 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('+', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 526 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('-', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 527 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('~', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 528 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('!', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 533 "ANSI-C.y"
    { if (ANSIOnly)
 	                                                   SyntaxError("constructors not allowed with -ansi switch");
                                                         (yyval.n) = MakeConstructorCoord((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(1) - (4)].tok)); 
                                                       }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 537 "ANSI-C.y"
    { (yyval.n) = MakeCastCoord((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(1) - (4)].tok)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 543 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('*', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 545 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('/', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 547 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('%', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 553 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('+', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 555 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('-', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 561 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(LS, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 563 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(RS, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 569 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('<', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 571 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('>', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 573 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(LE, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 575 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(GE, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 581 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(EQ, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 583 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(NE, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 589 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('&', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 595 "ANSI-C.y"
    { 
              WarnAboutPrecedence('^', (yyvsp[(1) - (3)].n));
              WarnAboutPrecedence('^', (yyvsp[(3) - (3)].n));
	      (yyval.n) = MakeBinopCoord('^', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 604 "ANSI-C.y"
    { WarnAboutPrecedence('|', (yyvsp[(1) - (3)].n));
              WarnAboutPrecedence('|', (yyvsp[(3) - (3)].n));
              (yyval.n) = MakeBinopCoord('|', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 612 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ANDAND, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 618 "ANSI-C.y"
    { WarnAboutPrecedence(OROR, (yyvsp[(1) - (3)].n));
              WarnAboutPrecedence(OROR, (yyvsp[(3) - (3)].n));
              (yyval.n) = MakeBinopCoord(OROR, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 626 "ANSI-C.y"
    { (yyval.n) = MakeTernaryCoord((yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n), (yyvsp[(2) - (5)].tok), (yyvsp[(4) - (5)].tok)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 628 "ANSI-C.y"
    {
	      if (ANSIOnly)
	        SyntaxError("ISO C forbids omitting the middle term of a ?: expression");
	      (yyval.n) = MakeTernaryCoord((yyvsp[(1) - (4)].n), NULL, (yyvsp[(4) - (4)].n), (yyvsp[(2) - (4)].tok), (yyvsp[(3) - (4)].tok));
	    }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 638 "ANSI-C.y"
    { (yyvsp[(2) - (3)].n)->u.binop.left = (yyvsp[(1) - (3)].n);
              (yyvsp[(2) - (3)].n)->u.binop.right = (yyvsp[(3) - (3)].n);
              (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 644 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('=', NULL, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 645 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(MULTassign, NULL, NULL, (yyvsp[(1) - (1)].tok));  }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 646 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(DIVassign, NULL, NULL, (yyvsp[(1) - (1)].tok));   }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 647 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(MODassign, NULL, NULL, (yyvsp[(1) - (1)].tok));   }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 648 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(PLUSassign, NULL, NULL, (yyvsp[(1) - (1)].tok));  }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 649 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(MINUSassign, NULL, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 650 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(LSassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 651 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(RSassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 652 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ANDassign, NULL, NULL, (yyvsp[(1) - (1)].tok));   }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 653 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ERassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 654 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ORassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 660 "ANSI-C.y"
    {  
              if ((yyvsp[(1) - (3)].n)->typ == Comma) 
                {
		  AppendItem((yyvsp[(1) - (3)].n)->u.comma.exprs, (yyvsp[(3) - (3)].n));
		  (yyval.n) = (yyvsp[(1) - (3)].n);
		}
              else
                {
		    (yyval.n) = MakeCommaCoord(AppendItem(MakeNewList((yyvsp[(1) - (3)].n)), (yyvsp[(3) - (3)].n)), (yyvsp[(1) - (3)].n)->coord);
		}
	    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 674 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 678 "ANSI-C.y"
    { (yyval.n) = (Node *) NULL; }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 679 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 722 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (2)].L); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 724 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (2)].L); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 726 "ANSI-C.y"
    { (yyval.L) = MakeNewList(ForceNewSU((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok))); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 728 "ANSI-C.y"
    { (yyval.L) = MakeNewList(ForceNewSU((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok))); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 734 "ANSI-C.y"
    {
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
	    }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 739 "ANSI-C.y"
    { (yyval.L) = MakeNewList(SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n))); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 741 "ANSI-C.y"
    {    
		 SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
		 (yyvsp[(3) - (3)].n) = DefineProc(FALSE, SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl));
	    }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 746 "ANSI-C.y"
    { (yyval.L) = MakeNewList(SetProcBody((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n))); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 748 "ANSI-C.y"
    {
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
	      (yyvsp[(2) - (2)].n)->pragmas_before = GrabPragmas(0);
	    }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 754 "ANSI-C.y"
    {
		(yyval.L) = AppendItem((yyvsp[(2) - (4)].n)->pragmas_before,
				SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)));
		(yyvsp[(2) - (4)].n)->pragmas_before = 0;
	    }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 760 "ANSI-C.y"
    {
	      SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
	      (yyvsp[(3) - (3)].n) = DefineProc(FALSE, SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl));
	      (yyvsp[(3) - (3)].n)->pragmas_before = GrabPragmas(0);
	    }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 766 "ANSI-C.y"
    {
		(yyval.L) = AppendItem((yyvsp[(3) - (5)].n)->pragmas_before,
				SetProcBody((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)));
		(yyvsp[(3) - (5)].n)->pragmas_before = 0;
	    }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 777 "ANSI-C.y"
    { 
	      SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
	    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 780 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 781 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].tq).tq); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 783 "ANSI-C.y"
    { 
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (8)].n), (yyvsp[(8) - (8)].n))); 
            }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 789 "ANSI-C.y"
    { 
	      SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
	      SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
	    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 793 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 794 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].tq).tq); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 796 "ANSI-C.y"
    { 
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(3) - (9)].n), (yyvsp[(9) - (9)].n))); 
            }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 801 "ANSI-C.y"
    { 
              SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
            }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 804 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 805 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].tq).tq); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 807 "ANSI-C.y"
    { 
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (8)].n), (yyvsp[(8) - (8)].n))); 
	    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 813 "ANSI-C.y"
    { 
		 SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
		 SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
            }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 817 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 818 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].tq).tq); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 820 "ANSI-C.y"
    { 
		 (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(3) - (9)].n), (yyvsp[(9) - (9)].n))); 
	    }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 824 "ANSI-C.y"
    { 
	      (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), Redecl);
	    }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 827 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 828 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].tq).tq); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 830 "ANSI-C.y"
    { 
              SetDeclInit((yyvsp[(3) - (9)].n), (yyvsp[(9) - (9)].n)); 
	      if (yydebug) {
		  //printf("%s:%d ", __FILE__, __LINE__); DPL($7); 
		  printf("%s:%d ", __FILE__, __LINE__); DPN((yyvsp[(3) - (9)].n));
	      }
            }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 841 "ANSI-C.y"
    { 
              SyntaxError("declaration without a variable"); 
            }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 847 "ANSI-C.y"
    { 
              (yyval.L) = NULL; /* empty list */ 
            }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 852 "ANSI-C.y"
    { 
              SyntaxError("declaration without a variable"); 
            }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 858 "ANSI-C.y"
    { 
              (yyval.L) = NULL; /* empty list */ 
            }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 869 "ANSI-C.y"
    { 
              SetDeclType((yyvsp[(2) - (2)].n), MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord), NoRedecl);
            }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 872 "ANSI-C.y"
    {
              SetDeclAttribs((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].tq).tq);
          }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 876 "ANSI-C.y"
    { 
		(yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].n))); 
            }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 880 "ANSI-C.y"
    { 
              SetDeclType((yyvsp[(2) - (2)].n), MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord), NoRedecl);
            }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 883 "ANSI-C.y"
    {
	    SetDeclAttribs((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].tq).tq);
	  }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 887 "ANSI-C.y"
    { 
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].n))); 
	    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 891 "ANSI-C.y"
    { (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), NoRedecl); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 892 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].tq).tq); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 894 "ANSI-C.y"
    { SetDeclInit((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].n)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 899 "ANSI-C.y"
    { 
              SyntaxError("declaration without a variable"); 
	    }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 904 "ANSI-C.y"
    { 
              (yyval.L) = NULL; /* empty list */ 
	    }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 909 "ANSI-C.y"
    { 
              SyntaxError("declaration without a variable"); 
	    }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 914 "ANSI-C.y"
    { 
              (yyval.L) = NULL; /* empty list */ 
            }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 928 "ANSI-C.y"
    { (yyval.n) = FinishPrimType((yyvsp[(1) - (1)].n)); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 930 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			  SyntaxError("Wildcard tokens not allowed in program. \n");
	               }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 942 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 944 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, & (yyvsp[(1) - (2)].tq).coord); }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 946 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 948 "ANSI-C.y"
    { (yyval.n) = MergePrimTypes((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 955 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 957 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (3)].n), tq_union((yyvsp[(1) - (3)].tq).tq, (yyvsp[(3) - (3)].tq).tq), & (yyvsp[(1) - (3)].tq).coord); }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 959 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 966 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 968 "ANSI-C.y"
    { (yyval.n) = ConvertIdToTdef((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, GetTypedefType((yyvsp[(2) - (2)].n)));
              (yyval.n)->coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 971 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 978 "ANSI-C.y"
    {
		 (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
		 (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord; 
	    }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 983 "ANSI-C.y"
    {
		 (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (4)].tq).tq, (yyvsp[(2) - (4)].tq).tq, (yyvsp[(2) - (4)].tq).coord);
		 (yyval.tq).tq = MergeTypeQuals((yyval.tq).tq, (yyvsp[(3) - (4)].tq).tq, (yyvsp[(3) - (4)].tq).coord);
		 (yyval.tq).coord = (yyvsp[(1) - (4)].tq).coord; 
	    }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 989 "ANSI-C.y"
    {
              (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
              (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord; 
	    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 1019 "ANSI-C.y"
    { (yyval.n) = FinishPrimType((yyvsp[(1) - (1)].n)); }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1031 "ANSI-C.y"
    {
		(yyval.n) = NodeCopy(NodeDataType(SemCheckNode((yyvsp[(3) - (4)].n))), Subtree); /* copy the node so we can remove type qualifiers without hurting the code */	      
		SetCoords((yyval.n), (yyvsp[(1) - (4)].tok), Subtree);
		NodeUpdateTq((yyval.n), tq_remove_everything); /* Take off all the type qualifiers */
	    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1042 "ANSI-C.y"
    {
		/* we have to copy the node because we don't want the type qualifiers to modify the original node. */
		Node *thetype=SetCoords(NodeCopy(NodeDataType(SemCheckNode((yyvsp[(4) - (5)].n))), Subtree), (yyvsp[(2) - (5)].tok), Subtree);
		NodeUpdateTq(thetype, tq_remove_everything); /* Take off all the type qualifiers */
		(yyval.n) = TypeQualifyNode(thetype, (yyvsp[(1) - (5)].tq).tq, & (yyvsp[(1) - (5)].tq).coord); /* Add on the new type qualifiers */
	    }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1054 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, & (yyvsp[(1) - (2)].tq).coord);  }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 1056 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1058 "ANSI-C.y"
    { (yyval.n) = MergePrimTypes((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1065 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, & (yyvsp[(1) - (2)].tq).coord); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1067 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1074 "ANSI-C.y"
    { (yyval.n) = ConvertIdToTdef((yyvsp[(1) - (1)].n), EMPTY_TQ, GetTypedefType((yyvsp[(1) - (1)].n))); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1076 "ANSI-C.y"
    { (yyval.n) = ConvertIdToTdef((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, GetTypedefType((yyvsp[(2) - (2)].n))); (yyval.n)->coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1078 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1084 "ANSI-C.y"
    { /*$$.tq = tq_union($1.tq, $2.tq);*/
	      (yyval.tq).coord = (yyvsp[(1) - (1)].tq).coord; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1087 "ANSI-C.y"
    { (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
              (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1094 "ANSI-C.y"
    { (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
              (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1110 "ANSI-C.y"
    {
	      Warning(2, "function prototype parameters must have types");
              (yyval.n) = AddDefaultParameterTypes((yyvsp[(1) - (1)].n));
            }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1114 "ANSI-C.y"
    {  (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1122 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)), Redecl);
               }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1125 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (5)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (5)].tok)), Redecl); 
               }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1128 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(5) - (6)].n), MakePtrCoord(   (yyvsp[(3) - (6)].tq).tq,    NULL, (yyvsp[(1) - (6)].tok)), Redecl);
               }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1131 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (4)].n), MakePtrCoord(   (yyvsp[(3) - (4)].tq).tq,    NULL, (yyvsp[(1) - (4)].tok)), Redecl); 
               }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1139 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);  
              }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1142 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(3) - (4)].n)); 
               }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1145 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); 
               }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1152 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL); }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1154 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);  
               }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1161 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL); }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1163 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (2)].n), EMPTY_TQ, (yyvsp[(2) - (2)].n), NULL, NULL);   }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1175 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)), Redecl); 
               }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1178 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (4)].n), MakePtrCoord((yyvsp[(3) - (4)].tq).tq, NULL, (yyvsp[(1) - (4)].tok)), Redecl); 
               }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1185 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1188 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); 
               }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1202 "ANSI-C.y"
    { (yyval.n) = MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (2)].tok)); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1204 "ANSI-C.y"
    { (yyval.n) = MakePtrCoord((yyvsp[(3) - (3)].tq).tq, NULL, (yyvsp[(1) - (3)].tok)); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1206 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok))); 
               }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1209 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(4) - (4)].n), MakePtrCoord((yyvsp[(3) - (4)].tq).tq, NULL, (yyvsp[(1) - (4)].tok))); 
               }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1216 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1219 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1222 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1225 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); 
               }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1231 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n);                   }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1232 "ANSI-C.y"
    { (yyval.n) = MakeFdclCoord(EMPTY_TQ, NULL, NULL, (yyvsp[(1) - (2)].tok)); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1234 "ANSI-C.y"
    { (yyval.n) = DemoteProcArgs(MakeFdclCoord(EMPTY_TQ, (yyvsp[(2) - (3)].L), NULL, (yyvsp[(1) - (3)].tok))); }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1247 "ANSI-C.y"
    { //printf("%s:%d ", __FILE__, __LINE__); DPL($2);
              SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
              (yyval.n) = ModifyDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok))); 
              }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1252 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(4) - (4)].n), MakePtrCoord(tq_union((yyvsp[(2) - (4)].tq).tq, (yyvsp[(3) - (4)].tq).tq),    NULL, (yyvsp[(1) - (4)].tok))); 
               }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1259 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1261 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1264 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); 
               }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1271 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1273 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1276 "ANSI-C.y"
    { if (!AcceptWildcards())
	      {
	        SyntaxError("Wildcard tokens not allowed in program. \n");
              }
	      (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL);
	    }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1287 "ANSI-C.y"
    { 
              (yyval.n) = (yyvsp[(1) - (1)].n); 
            }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1291 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)), SU); 
               }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1294 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (4)].n), MakePtrCoord((yyvsp[(3) - (4)].tq).tq, NULL, (yyvsp[(1) - (4)].tok)), SU); 
               }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1301 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(1) - (4)].n), MakeFdclCoord(EMPTY_TQ, (yyvsp[(3) - (4)].L), NULL, (yyvsp[(2) - (4)].tok))); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1303 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); 
               }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1306 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); 
               }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1321 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1323 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1335 "ANSI-C.y"
    { (yyval.n) = FinishType((yyvsp[(1) - (1)].n)); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1337 "ANSI-C.y"
    { (yyval.n) = FinishType(SetBaseType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n))); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1339 "ANSI-C.y"
    { (yyval.n) = MakeDefaultPrimType((yyvsp[(1) - (1)].tq).tq, (yyvsp[(1) - (1)].tq).coord); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1341 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(2) - (2)].n), MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1342 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			  SyntaxError("Wildcard tokens not allowed in program. \n");
	               }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1354 "ANSI-C.y"
    { (yyval.tq).tq = EMPTY_TQ; (yyval.tq).coord = UnknownCoord; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1356 "ANSI-C.y"
    { (yyval.tq) = (yyvsp[(1) - (1)].tq); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1361 "ANSI-C.y"
    { (yyval.tq) = (yyvsp[(1) - (1)].tq); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1363 "ANSI-C.y"
    {
		  (yyval.tq).tq = tq_union ((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq);
		  (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord;
	      }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1371 "ANSI-C.y"
    { if (ANSIOnly)
	            SyntaxError("__attribute__ not allowed with -ansi switch");
                  (yyval.tq).tq = tq_attribute((yyvsp[(4) - (6)].L)); (yyval.tq).coord=UnknownCoord;
		}
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1380 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1382 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1387 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1389 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (1)].n), NULL); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1391 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (4)].n), MakeNewList((yyvsp[(3) - (4)].n))); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1393 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (6)].n), ConsItem((yyvsp[(3) - (6)].n), (yyvsp[(5) - (6)].L))); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1404 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].L)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1409 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (1)].L); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1415 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1420 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (4)].n), NULL, NULL, NULL, (yyvsp[(1) - (4)].tok)); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1426 "ANSI-C.y"
    { (yyval.n) = MakeIdCoord(UniqueString("const"), (yyvsp[(1) - (1)].tok)); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1431 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1432 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1438 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(':', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1439 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(':', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1440 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(DESIGNATED_INITIALIZER, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(3) - (4)].tok)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1441 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(DESIGNATED_INITIALIZER, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(3) - (4)].tok)); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1442 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARRAY_LABELED_INITIALIZER, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(3) - (4)].tok)); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1443 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARRAY_LABELED_INITIALIZER, (yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n), (yyvsp[(3) - (5)].tok)); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1444 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARRAY_LABELED_INITIALIZER,
														   MakeBinopCoord(ELLIPSIS, (yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].n), (yyvsp[(3) - (6)].tok)),
														   (yyvsp[(6) - (6)].n), (yyvsp[(5) - (6)].tok)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1451 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1455 "ANSI-C.y"
    { if (ANSIOnly) SyntaxError("ISO C forbits empty initializer braces"); /* See Bug 83 */
	                                   (yyval.n) = MakeInitializerCoord(NULL, (yyvsp[(1) - (2)].tok));
                                         }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1458 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); (yyval.n)->coord = (yyvsp[(1) - (3)].tok); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1459 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (4)].n); (yyval.n)->coord = (yyvsp[(1) - (4)].tok); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1464 "ANSI-C.y"
    { (yyval.n) = MakeInitializerCoord(MakeNewList((yyvsp[(1) - (1)].n)), (yyvsp[(1) - (1)].n)->coord); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1466 "ANSI-C.y"
    { 
              assert((yyvsp[(1) - (3)].n)->typ == Initializer);
              AppendItem((yyvsp[(1) - (3)].n)->u.initializer.exprs, (yyvsp[(3) - (3)].n));
              (yyval.n) = (yyvsp[(1) - (3)].n); 
            }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1476 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), EllipsisNode); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1480 "ANSI-C.y"
    { Node *n = MakePrimCoord(EMPTY_TQ, Void, (yyvsp[(1) - (1)].tok));
	      SyntaxErrorCoord(n->coord, "First argument cannot be `...'");
              (yyval.L) = MakeNewList(n);
            }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1489 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1491 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1495 "ANSI-C.y"
    { 
	      SyntaxErrorCoord((yyvsp[(1) - (3)].n)->coord, "formals cannot have initializers");
              (yyval.L) = MakeNewList((yyvsp[(1) - (3)].n)); 
            }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1500 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (3)].L); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1506 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1508 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n)); 
            }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1511 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n), Formal); 
            }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1514 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n), Formal); 
            }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1517 "ANSI-C.y"
    { (yyval.n) = MakeDefaultPrimType((yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1519 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), MakeDefaultPrimType((yyvsp[(2) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).coord)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1521 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakeDefaultPrimType((yyvsp[(2) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).coord), Formal); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1523 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1525 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n)); 
            }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1528 "ANSI-C.y"
    {
	      (yyval.n) = SetDeclType((yyvsp[(3) - (4)].n), TypeQualifyNode((yyvsp[(2) - (4)].n), tq_union((yyvsp[(1) - (4)].tq).tq, (yyvsp[(4) - (4)].tq).tq), NULL), Formal);
	    }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1532 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n), Formal); 
            }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1535 "ANSI-C.y"
    { (yyval.n) = MakeDefaultPrimType((yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1537 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), MakeDefaultPrimType((yyvsp[(2) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).coord)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1539 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakeDefaultPrimType(tq_union((yyvsp[(1) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).tq), (yyvsp[(2) - (3)].tq).coord), Formal); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1545 "ANSI-C.y"
    { (yyval.n) = MakeAdclCoord(EMPTY_TQ, NULL, NULL, (yyvsp[(1) - (2)].tok)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1547 "ANSI-C.y"
    { (yyval.n) = MakeAdclCoord(EMPTY_TQ, NULL, (yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1549 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(1) - (4)].n), MakeAdclCoord(EMPTY_TQ, NULL, (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok))); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1551 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(1) - (3)].n), MakeAdclCoord(EMPTY_TQ, NULL, NULL, (yyvsp[(2) - (3)].tok))); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1573 "ANSI-C.y"
    { 
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (4)].n), NULL, (yyvsp[(3) - (4)].L), (yyvsp[(2) - (4)].tok), (yyvsp[(4) - (4)].tok), NULL);
            }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1578 "ANSI-C.y"
    { 
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (5)].n), (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].L), (yyvsp[(3) - (5)].tok), (yyvsp[(5) - (5)].tok), NULL);
	    }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1582 "ANSI-C.y"
    { 
              (yyval.n) = SetSUdclName((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n)->coord);
	    }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1587 "ANSI-C.y"
    { 
              if (ANSIOnly)
                 Warning(1, "empty structure declaration");
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (3)].n), NULL, NULL, (yyvsp[(2) - (3)].tok), (yyvsp[(3) - (3)].tok), NULL);
	    }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1593 "ANSI-C.y"
    { 
              if (ANSIOnly)
                 Warning(1, "empty structure declaration");
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (4)].n), (yyvsp[(2) - (4)].n), NULL, (yyvsp[(3) - (4)].tok), (yyvsp[(4) - (4)].tok), NULL); 
	    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1602 "ANSI-C.y"
    { (yyval.n) = MakeSdclCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1603 "ANSI-C.y"
    { (yyval.n) = MakeUdclCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1609 "ANSI-C.y"
    { 
              (yyval.L) = (yyvsp[(2) - (2)].L); 
              NodeUpdateTq2(FirstItem((yyval.L)), tq_union, (yyvsp[(1) - (2)].tq).tq); 
            }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1614 "ANSI-C.y"
    { 
	      NodeUpdateTq2(FirstItem((yyvsp[(3) - (3)].L)), tq_union, (yyvsp[(2) - (3)].tq).tq);
              (yyval.L) = JoinLists((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].L));
	    }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1624 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(2) - (2)].L); NodeUpdateTq((Node*)(FirstItem((yyval.L))), tq_add_extension); ((Node*)(FirstItem((yyval.L))))->coord = (yyvsp[(1) - (2)].tok); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1631 "ANSI-C.y"
    { 
	      (yyval.L) = MakeNewList(SetDeclType((yyvsp[(2) - (2)].n),
					    MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
					    SU)); 
	    }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1637 "ANSI-C.y"
    { (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), SU); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1643 "ANSI-C.y"
    { (yyval.L) = MakeNewList(SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), SU)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1645 "ANSI-C.y"
    {
	       SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
	       (yyval.L) = MakeNewList(SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), SU));
	  }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1652 "ANSI-C.y"
    {
	    Node *d = MakeDecl(NULL, TQ_SU_DECL, (yyvsp[(1) - (2)].n), NULL, (yyvsp[(2) - (2)].n));
	    (yyval.L) = MakeNewList(d);
	  }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1657 "ANSI-C.y"
    {
		 /* gcc extension: unnamed struct/union fields */
		 Node *a = SetSUdclFields((yyvsp[(1) - (4)].n), NULL, (yyvsp[(3) - (4)].L), (yyvsp[(2) - (4)].tok), (yyvsp[(4) - (4)].tok), NULL);
		 Node *d = MakeDeclCoord(NULL, EMPTY_TQ, a, NULL, NULL, a->coord);
		 NodeSetDeclLocation(d, TQ_SU_DECL);
		 (yyval.L) = MakeNewList(d);
	    }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1665 "ANSI-C.y"
    { (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), SU); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1672 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].tq).tq);
              (yyval.n) = SetDeclBitSize((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].n)); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1689 "ANSI-C.y"
    { (yyval.n) = SetDeclBitSize((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].n));
              SetDeclAttribs((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].tq).tq); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1695 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1701 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1707 "ANSI-C.y"
    { (yyval.n) = BuildEnum(NULL, (yyvsp[(3) - (5)].L), (yyvsp[(1) - (5)].tok), (yyvsp[(2) - (5)].tok), (yyvsp[(5) - (5)].tok)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1709 "ANSI-C.y"
    { (yyval.n) = BuildEnum((yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].L), (yyvsp[(1) - (6)].tok), (yyvsp[(3) - (6)].tok), (yyvsp[(6) - (6)].tok));   }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1711 "ANSI-C.y"
    { (yyval.n) = BuildEnum((yyvsp[(2) - (2)].n), NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].n)->coord, (yyvsp[(2) - (2)].n)->coord); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1717 "ANSI-C.y"
    { (yyval.L) = MakeNewList(BuildEnumConst((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n))); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1719 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (4)].L), BuildEnumConst((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n))); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1724 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1725 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n);   }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1729 "ANSI-C.y"
    { }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1730 "ANSI-C.y"
    { }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1750 "ANSI-C.y"
    {  (yyval.n) = NULL; }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1755 "ANSI-C.y"
    { (yyval.n) = BuildLabel((yyvsp[(1) - (2)].n)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1757 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeCaseCoord((yyvsp[(2) - (3)].n), NULL, (yyvsp[(1) - (3)].tok))); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1759 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeDefaultCoord(NULL, (yyvsp[(1) - (2)].tok))); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1763 "ANSI-C.y"
    { (yyval.n) = BuildLabel((yyvsp[(1) - (2)].n)); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1768 "ANSI-C.y"
    { (yyval.n) = MakeBlockCoord(PrimVoid, NULL, NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].tok)); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1770 "ANSI-C.y"
    {
		(yyval.n) = MakeBlockCoord(PrimVoid, GrabPragmas((yyvsp[(2) - (3)].L)), NULL, (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1774 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(2) - (3)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, NULL, GrabPragmas((yyvsp[(2) - (3)].L)), (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1779 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(3) - (4)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, (yyvsp[(2) - (4)].L), GrabPragmas((yyvsp[(3) - (4)].L)), (yyvsp[(1) - (4)].tok), (yyvsp[(4) - (4)].tok));
	    }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1785 "ANSI-C.y"
    { (yyval.n) = MakeXBlockCoord(PrimVoid, NULL, NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].tok)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1787 "ANSI-C.y"
    {
		(yyval.n) = MakeXBlockCoord(PrimVoid, GrabPragmas((yyvsp[(2) - (3)].L)), NULL, (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1791 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(2) - (3)].L));
		(yyval.n) = MakeXBlockCoord(PrimVoid, NULL, GrabPragmas((yyvsp[(2) - (3)].L)), (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1796 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(3) - (4)].L));
		(yyval.n) = MakeXBlockCoord(PrimVoid, (yyvsp[(2) - (4)].L), GrabPragmas((yyvsp[(3) - (4)].L)), (yyvsp[(1) - (4)].tok), (yyvsp[(4) - (4)].tok));
	    }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1802 "ANSI-C.y"
    { EnterScope(); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1803 "ANSI-C.y"
    { ExitScope(); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1806 "ANSI-C.y"
    { EnterScope(); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1807 "ANSI-C.y"
    { ExitScope();  }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1814 "ANSI-C.y"
    { (yyval.n) = MakeBlockCoord(PrimVoid, NULL, NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].tok)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1816 "ANSI-C.y"
    { (yyval.n) = MakeBlockCoord(PrimVoid, GrabPragmas((yyvsp[(2) - (3)].L)), NULL, (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1818 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(2) - (3)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, NULL, GrabPragmas((yyvsp[(2) - (3)].L)), (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1823 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(3) - (4)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, (yyvsp[(2) - (4)].L), GrabPragmas((yyvsp[(3) - (4)].L)), (yyvsp[(1) - (4)].tok), (yyvsp[(4) - (4)].tok));
	    }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1832 "ANSI-C.y"
    { (yyval.L) = GrabPragmas((yyvsp[(1) - (1)].L)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1833 "ANSI-C.y"
    { (yyval.L) = JoinLists(GrabPragmas((yyvsp[(1) - (2)].L)),
                                                         (yyvsp[(2) - (2)].L)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1838 "ANSI-C.y"
    { (yyval.L) = GrabPragmas(MakeNewList((yyvsp[(1) - (1)].n))); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1839 "ANSI-C.y"
    { (yyval.L) = AppendItem(GrabPragmas((yyvsp[(1) - (2)].L)), 
                                                        (yyvsp[(2) - (2)].n)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1844 "ANSI-C.y"
    {
            (yyval.n)=(yyvsp[(1) - (2)].n);
	    if ((yyval.n) && (yyval.n)->typ==Block) (yyval.n)->parenthesized=TRUE; /* Expression statements get their parenthesized bits set, so we can remember that it is an expression statement, rather than just a compound statement */
          }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1852 "ANSI-C.y"
    { (yyval.n) = MakeIfCoord((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n), (yyvsp[(1) - (5)].tok)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1854 "ANSI-C.y"
    { (yyval.n) = MakeIfElseCoord((yyvsp[(3) - (7)].n), (yyvsp[(5) - (7)].n), (yyvsp[(7) - (7)].n), (yyvsp[(1) - (7)].tok), (yyvsp[(6) - (7)].tok)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1855 "ANSI-C.y"
    { PushContainer(Switch); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1856 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeSwitchCoord((yyvsp[(4) - (6)].n), (yyvsp[(6) - (6)].n), NULL, (yyvsp[(1) - (6)].tok))); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1861 "ANSI-C.y"
    { PushContainer(While);}
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1863 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeWhileCoord((yyvsp[(4) - (6)].n), (yyvsp[(6) - (6)].n), (yyvsp[(1) - (6)].tok))); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1865 "ANSI-C.y"
    { PushContainer(Do);}
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1867 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeDoCoord((yyvsp[(3) - (8)].n), (yyvsp[(6) - (8)].n), (yyvsp[(1) - (8)].tok), (yyvsp[(4) - (8)].tok))); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1869 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1871 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord((yyvsp[(3) - (10)].n), (yyvsp[(5) - (10)].n), (yyvsp[(7) - (10)].n), (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1875 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1877 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord(NULL, (yyvsp[(5) - (10)].n), (yyvsp[(7) - (10)].n), (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1879 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1881 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord((yyvsp[(3) - (10)].n), (yyvsp[(5) - (10)].n), NULL, (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1883 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1885 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord((yyvsp[(3) - (10)].n), NULL, (yyvsp[(7) - (10)].n), (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1886 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1887 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord(NULL, SintZero, NULL, (yyvsp[(6) - (6)].n), (yyvsp[(1) - (6)].tok))); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1892 "ANSI-C.y"
    { (yyval.n) = ResolveGoto((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1894 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeContinueCoord(NULL, (yyvsp[(1) - (2)].tok))); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1896 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeBreakCoord(NULL, (yyvsp[(1) - (2)].tok))); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1898 "ANSI-C.y"
    { (yyval.n) = AddReturn(MakeReturnCoord((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok))); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1902 "ANSI-C.y"
    { (yyval.n) = ResolveGoto((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1907 "ANSI-C.y"
    { (yyval.L) = NULL; }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1912 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1914 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1918 "ANSI-C.y"
    { (yyval.n) = MakeAsmArgCoord((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1922 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1924 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1928 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (5)].n), NULL, NULL, NULL, (yyvsp[(1) - (5)].tok)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1930 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (7)].n), (yyvsp[(5) - (7)].L), NULL, NULL, (yyvsp[(1) - (7)].tok)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1932 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (9)].n), (yyvsp[(5) - (9)].L), (yyvsp[(7) - (9)].L), NULL, (yyvsp[(1) - (9)].tok)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1934 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (11)].n), (yyvsp[(5) - (11)].L), (yyvsp[(7) - (11)].L), (yyvsp[(9) - (11)].L), (yyvsp[(1) - (11)].tok)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1936 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (6)].tq).tq, (yyvsp[(4) - (6)].n), NULL, NULL, NULL, (yyvsp[(1) - (6)].tok)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1938 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (8)].tq).tq, (yyvsp[(4) - (8)].n), (yyvsp[(6) - (8)].L), NULL, NULL, (yyvsp[(1) - (8)].tok)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1940 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (10)].tq).tq, (yyvsp[(4) - (10)].n), (yyvsp[(6) - (10)].L), (yyvsp[(8) - (10)].L), NULL, (yyvsp[(1) - (10)].tok)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1942 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (12)].tq).tq, (yyvsp[(4) - (12)].n), (yyvsp[(6) - (12)].L), (yyvsp[(8) - (12)].L), (yyvsp[(10) - (12)].L), (yyvsp[(1) - (12)].tok)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1952 "ANSI-C.y"
    { (yyval.L) = GrabPragmas((yyvsp[(1) - (1)].L)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1953 "ANSI-C.y"
    { (yyval.L) = JoinLists((yyvsp[(1) - (2)].L), GrabPragmas((yyvsp[(2) - (2)].L))); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1958 "ANSI-C.y"
    {
              if (yydebug)
                {
                  printf("external.definition # declaration\n");
                  fPrintNode(stdout, FirstItem((yyvsp[(1) - (1)].L)), 0); 
                  printf("\n\n\n");
		}
              (yyval.L) = (yyvsp[(1) - (1)].L);
            }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1968 "ANSI-C.y"
    { 
              if (yydebug)
                {
                  printf("external.definition # function.definition\n");
                  fPrintNode(stdout, (yyvsp[(1) - (1)].n), 0); 
                  printf("\n\n\n");
                }
              (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n));
            }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1978 "ANSI-C.y"
    {  (yyval.L) = (yyvsp[(2) - (2)].L); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1979 "ANSI-C.y"
    { (yyval.L) = (List *) NULL; }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1984 "ANSI-C.y"
    { 
              (yyvsp[(1) - (1)].n) = DefineProc(FALSE, 
                              SetDeclType((yyvsp[(1) - (1)].n),
					  MakeDefaultPrimType(EMPTY_TQ, (yyvsp[(1) - (1)].n)->coord),
					  Redecl));
            }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1991 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 2007 "ANSI-C.y"
    { 
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n),
				       MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
				       Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
            }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 2014 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 2016 "ANSI-C.y"
    { 
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n),
				       MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
				       Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
            }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 2023 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 2025 "ANSI-C.y"
    { 
              (yyvsp[(1) - (1)].n) = DefineProc(TRUE, 
                              SetDeclType((yyvsp[(1) - (1)].n),
					  MakeDefaultPrimType(EMPTY_TQ, (yyvsp[(1) - (1)].n)->coord),
					  Redecl));
            }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 2032 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 2034 "ANSI-C.y"
    {  Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);  

               AddParameterTypes(decl, NULL);
               (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 2040 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 2042 "ANSI-C.y"
    { Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);

              AddParameterTypes(decl, NULL);
              (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 2048 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 2050 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (2)].n), type, Redecl);

              AddParameterTypes(decl, NULL);
              (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 2057 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 2059 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (2)].n), type, Redecl);

              AddParameterTypes(decl, NULL);
              (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 2066 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 2068 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType(EMPTY_TQ, (yyvsp[(1) - (2)].n)->coord),
                   *decl = SetDeclType((yyvsp[(1) - (2)].n), type, Redecl);
              AddParameterTypes(decl, (yyvsp[(2) - (2)].L));
              (yyvsp[(1) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 2074 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 2076 "ANSI-C.y"
    { Node *decl = SetDeclType((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 2081 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 2083 "ANSI-C.y"
    { Node *decl = SetDeclType((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 2088 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 2090 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (3)].tq).tq, (yyvsp[(1) - (3)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (3)].n), type, Redecl);

              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 2097 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 2099 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (3)].tq).tq, (yyvsp[(1) - (3)].tq).coord), 
                   *decl = SetDeclType((yyvsp[(2) - (3)].n), type, Redecl);
				       

              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 2107 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 2112 "ANSI-C.y"
    { OldStyleFunctionDefinition = TRUE; }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 2114 "ANSI-C.y"
    { OldStyleFunctionDefinition = FALSE; 
               (yyval.L) = (yyvsp[(2) - (2)].L); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 2129 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 2130 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 2131 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 2132 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 2133 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 2138 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 2140 "ANSI-C.y"
    { const char *first_text  = (yyvsp[(1) - (2)].n)->u.Const.text;
              const char *second_text = (yyvsp[(2) - (2)].n)->u.Const.text;
              int   length = strlen(first_text) + strlen(second_text) + 1;
              char *buffer = HeapNewArray(char, length);
              char *new_text, *new_val;
	
              /* since text (which includes quotes and escape codes)
		 is always longer than value, it's safe to use buffer
		 to concat both */
              strcpy(buffer, NodeConstantStringValue((yyvsp[(1) - (2)].n)));
	      strcat(buffer, NodeConstantStringValue((yyvsp[(2) - (2)].n)));
              new_val = UniqueString(buffer);

              strcpy(buffer, first_text);
	      strcat(buffer, second_text);
              new_text = buffer;
              (yyval.n) = MakeStringTextCoord(new_text, new_val, (yyvsp[(1) - (2)].n)->coord);
	     }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 2161 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CONST;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 2162 "ANSI-C.y"
    { (yyval.tq).tq = TQ_VOLATILE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 2163 "ANSI-C.y"
    { (yyval.tq).tq = TQ_INLINE;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 2164 "ANSI-C.y"
    { (yyval.tq).tq = TQ_RESTRICT; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 2165 "ANSI-C.y"
    { (yyval.tq).tq = TQ_COMPLEX;  (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 2166 "ANSI-C.y"
    { (yyval.tq).tq = TQ_PROCEDURE;(yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 2167 "ANSI-C.y"
    { (yyval.tq).tq = TQ_INLET;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 2168 "ANSI-C.y"
    { (yyval.tq).tq = TQ_SHARED;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 2169 "ANSI-C.y"
    { (yyval.tq).tq = TQ_PRIVATE;  (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 2170 "ANSI-C.y"
    { (yyval.tq).tq = TQ_NOOUTPUT; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 2171 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKONLY; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 2172 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKSAFE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 2176 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CONST;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 2177 "ANSI-C.y"
    { (yyval.tq).tq = TQ_VOLATILE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 2178 "ANSI-C.y"
    { (yyval.tq).tq = TQ_RESTRICT; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 2179 "ANSI-C.y"
    { (yyval.tq).tq = TQ_PROCEDURE;(yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 2180 "ANSI-C.y"
    { (yyval.tq).tq = TQ_INLET;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 2181 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKONLY; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 485:

/* Line 1806 of yacc.c  */
#line 2182 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKSAFE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 486:

/* Line 1806 of yacc.c  */
#line 2186 "ANSI-C.y"
    { (yyval.tq).tq = TQ_TYPEDEF;  (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 487:

/* Line 1806 of yacc.c  */
#line 2187 "ANSI-C.y"
    { (yyval.tq).tq = TQ_EXTERN;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 488:

/* Line 1806 of yacc.c  */
#line 2188 "ANSI-C.y"
    { (yyval.tq).tq = TQ_STATIC;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 489:

/* Line 1806 of yacc.c  */
#line 2189 "ANSI-C.y"
    { (yyval.tq).tq = TQ_AUTO;     (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 490:

/* Line 1806 of yacc.c  */
#line 2190 "ANSI-C.y"
    { (yyval.tq).tq = TQ_REGISTER; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 491:

/* Line 1806 of yacc.c  */
#line 2194 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Void, (yyvsp[(1) - (1)].tok));    }
    break;

  case 492:

/* Line 1806 of yacc.c  */
#line 2195 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Char, (yyvsp[(1) - (1)].tok));     }
    break;

  case 493:

/* Line 1806 of yacc.c  */
#line 2196 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Int_ParseOnly, (yyvsp[(1) - (1)].tok));     }
    break;

  case 494:

/* Line 1806 of yacc.c  */
#line 2197 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Float, (yyvsp[(1) - (1)].tok));   }
    break;

  case 495:

/* Line 1806 of yacc.c  */
#line 2198 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Double, (yyvsp[(1) - (1)].tok));  }
    break;

  case 496:

/* Line 1806 of yacc.c  */
#line 2200 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Signed, (yyvsp[(1) - (1)].tok));  }
    break;

  case 497:

/* Line 1806 of yacc.c  */
#line 2201 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Unsigned, (yyvsp[(1) - (1)].tok));}
    break;

  case 498:

/* Line 1806 of yacc.c  */
#line 2203 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Short, (yyvsp[(1) - (1)].tok));   }
    break;

  case 499:

/* Line 1806 of yacc.c  */
#line 2204 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Long, (yyvsp[(1) - (1)].tok));    }
    break;

  case 501:

/* Line 1806 of yacc.c  */
#line 2212 "ANSI-C.y"
    { (yyval.n) = ExtendArray((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok)); }
    break;

  case 502:

/* Line 1806 of yacc.c  */
#line 2214 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 503:

/* Line 1806 of yacc.c  */
#line 2216 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 504:

/* Line 1806 of yacc.c  */
#line 2218 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTINC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 505:

/* Line 1806 of yacc.c  */
#line 2220 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTDEC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 506:

/* Line 1806 of yacc.c  */
#line 2224 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 507:

/* Line 1806 of yacc.c  */
#line 2226 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 508:

/* Line 1806 of yacc.c  */
#line 2231 "ANSI-C.y"
    { (yyval.n) = OmitLookups() ? (yyvsp[(1) - (1)].n) : LookupPostfixExpression((yyvsp[(1) - (1)].n));}
    break;

  case 509:

/* Line 1806 of yacc.c  */
#line 2235 "ANSI-C.y"
    { (yyval.L) = (List *) NULL; }
    break;

  case 510:

/* Line 1806 of yacc.c  */
#line 2236 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (1)].L); }
    break;

  case 511:

/* Line 1806 of yacc.c  */
#line 2243 "ANSI-C.y"
    { (yyval.n) = MakeSpawnCoord(NULL, '=', (yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].L), FALSE, (yyvsp[(1) - (6)].tok)); }
    break;

  case 512:

/* Line 1806 of yacc.c  */
#line 2248 "ANSI-C.y"
    { (yyval.n) = MakeSpawnCoord((yyvsp[(1) - (8)].n), (yyvsp[(2) - (8)].n)->u.binop.op, (yyvsp[(4) - (8)].n), (yyvsp[(6) - (8)].L), FALSE, (yyvsp[(3) - (8)].tok)); }
    break;

  case 513:

/* Line 1806 of yacc.c  */
#line 2252 "ANSI-C.y"
    { (yyval.n) = MakeSyncCoord((yyvsp[(1) - (2)].tok)); }
    break;

  case 514:

/* Line 1806 of yacc.c  */
#line 2256 "ANSI-C.y"
    { (yyval.n) = MakeInletCallCoord((OmitLookups() ? (yyvsp[(1) - (5)].n) : LookupPostfixExpression((yyvsp[(1) - (5)].n))), (yyvsp[(3) - (5)].L), (yyvsp[(2) - (5)].tok)); }
    break;

  case 515:

/* Line 1806 of yacc.c  */
#line 2265 "ANSI-C.y"
    { (yyval.n) = MakeAbortCoord((yyvsp[(1) - (2)].tok)); }
    break;

  case 516:

/* Line 1806 of yacc.c  */
#line 2270 "ANSI-C.y"
    { (yyval.L) = MakeNewList(MakeSpawnCoord(NULL, '=', (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].L), TRUE, (yyvsp[(1) - (5)].tok))); }
    break;

  case 517:

/* Line 1806 of yacc.c  */
#line 2272 "ANSI-C.y"
    { (yyval.L) = ConsItem(MakeSpawnCoord(NULL, '=', (yyvsp[(2) - (7)].n), (yyvsp[(4) - (7)].L), TRUE, (yyvsp[(1) - (7)].tok)), (yyvsp[(7) - (7)].L)); }
    break;

  case 518:

/* Line 1806 of yacc.c  */
#line 2277 "ANSI-C.y"
    { (yyval.n) = MakeSynchedCoord((yyvsp[(1) - (1)].tok)); }
    break;



/* Line 1806 of yacc.c  */
#line 7162 "ANSI-C.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 2286 "ANSI-C.y"

/* ----end of grammar----*/


PRIVATE void WarnAboutPrecedence(OpType op, Node *node)
{
  if (node->typ == Binop && !node->parenthesized) {
    OpType subop = node->u.binop.op;

    if (op == OROR && subop == ANDAND)
      WarningCoord(4, node->coord, "suggest parentheses around && in operand of ||");
    else if ((op == '|' || op == '^') && 
	     (subop == '+' || subop == '-' || subop == '&' || subop == '^') &&
	     op != subop)
      WarningCoord(4, node->coord, "suggest parentheses around arithmetic in operand of %c", op);
  }
}

