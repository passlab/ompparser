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

#ifndef YY_OMP_OMPPARSER_HH_INCLUDED
# define YY_OMP_OMPPARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int omp_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OMP = 258,
    PARALLEL = 259,
    IF = 260,
    NUM_THREADS = 261,
    ORDERED = 262,
    SCHEDULE = 263,
    STATIC = 264,
    DYNAMIC = 265,
    GUIDED = 266,
    RUNTIME = 267,
    SECTIONS = 268,
    SINGLE = 269,
    NOWAIT = 270,
    SECTION = 271,
    FOR = 272,
    MASTER = 273,
    CRITICAL = 274,
    BARRIER = 275,
    ATOMIC = 276,
    FLUSH = 277,
    TARGET = 278,
    UPDATE = 279,
    DIST_DATA = 280,
    BLOCK = 281,
    DUPLICATE = 282,
    CYCLIC = 283,
    THREADPRIVATE = 284,
    PRIVATE = 285,
    COPYPRIVATE = 286,
    FIRSTPRIVATE = 287,
    LASTPRIVATE = 288,
    SHARED = 289,
    DEFAULT = 290,
    NONE = 291,
    REDUCTION = 292,
    COPYIN = 293,
    IN_REDUCTION = 294,
    TASK = 295,
    TASKWAIT = 296,
    UNTIED = 297,
    COLLAPSE = 298,
    AUTO = 299,
    DECLARE = 300,
    DATA = 301,
    DEVICE = 302,
    MAP = 303,
    ALLOC = 304,
    TO = 305,
    FROM = 306,
    TOFROM = 307,
    PROC_BIND = 308,
    CLOSE = 309,
    SPREAD = 310,
    SIMD = 311,
    SAFELEN = 312,
    ALIGNED = 313,
    LINEAR = 314,
    UNIFORM = 315,
    INBRANCH = 316,
    NOTINBRANCH = 317,
    MPI = 318,
    MPI_ALL = 319,
    MPI_MASTER = 320,
    TARGET_BEGIN = 321,
    TARGET_END = 322,
    LOGAND = 323,
    LOGOR = 324,
    SHLEFT = 325,
    SHRIGHT = 326,
    PLUSPLUS = 327,
    MINUSMINUS = 328,
    PTR_TO = 329,
    LE_OP2 = 330,
    GE_OP2 = 331,
    EQ_OP2 = 332,
    NE_OP2 = 333,
    RIGHT_ASSIGN2 = 334,
    LEFT_ASSIGN2 = 335,
    ADD_ASSIGN2 = 336,
    SUB_ASSIGN2 = 337,
    MUL_ASSIGN2 = 338,
    DIV_ASSIGN2 = 339,
    MOD_ASSIGN2 = 340,
    AND_ASSIGN2 = 341,
    XOR_ASSIGN2 = 342,
    OR_ASSIGN2 = 343,
    DEPEND = 344,
    IN = 345,
    OUT = 346,
    INOUT = 347,
    MERGEABLE = 348,
    LEXICALERROR = 349,
    IDENTIFIER = 350,
    READ = 351,
    WRITE = 352,
    CAPTURE = 353,
    SIMDLEN = 354,
    FINAL = 355,
    PRIORITY = 356,
    ICONSTANT = 357,
    EXPRESSION = 358,
    ID_EXPRESSION = 359,
    RAW_STRING = 360,
    TESTEXPR = 361
  };
#endif
/* Tokens.  */
#define OMP 258
#define PARALLEL 259
#define IF 260
#define NUM_THREADS 261
#define ORDERED 262
#define SCHEDULE 263
#define STATIC 264
#define DYNAMIC 265
#define GUIDED 266
#define RUNTIME 267
#define SECTIONS 268
#define SINGLE 269
#define NOWAIT 270
#define SECTION 271
#define FOR 272
#define MASTER 273
#define CRITICAL 274
#define BARRIER 275
#define ATOMIC 276
#define FLUSH 277
#define TARGET 278
#define UPDATE 279
#define DIST_DATA 280
#define BLOCK 281
#define DUPLICATE 282
#define CYCLIC 283
#define THREADPRIVATE 284
#define PRIVATE 285
#define COPYPRIVATE 286
#define FIRSTPRIVATE 287
#define LASTPRIVATE 288
#define SHARED 289
#define DEFAULT 290
#define NONE 291
#define REDUCTION 292
#define COPYIN 293
#define IN_REDUCTION 294
#define TASK 295
#define TASKWAIT 296
#define UNTIED 297
#define COLLAPSE 298
#define AUTO 299
#define DECLARE 300
#define DATA 301
#define DEVICE 302
#define MAP 303
#define ALLOC 304
#define TO 305
#define FROM 306
#define TOFROM 307
#define PROC_BIND 308
#define CLOSE 309
#define SPREAD 310
#define SIMD 311
#define SAFELEN 312
#define ALIGNED 313
#define LINEAR 314
#define UNIFORM 315
#define INBRANCH 316
#define NOTINBRANCH 317
#define MPI 318
#define MPI_ALL 319
#define MPI_MASTER 320
#define TARGET_BEGIN 321
#define TARGET_END 322
#define LOGAND 323
#define LOGOR 324
#define SHLEFT 325
#define SHRIGHT 326
#define PLUSPLUS 327
#define MINUSMINUS 328
#define PTR_TO 329
#define LE_OP2 330
#define GE_OP2 331
#define EQ_OP2 332
#define NE_OP2 333
#define RIGHT_ASSIGN2 334
#define LEFT_ASSIGN2 335
#define ADD_ASSIGN2 336
#define SUB_ASSIGN2 337
#define MUL_ASSIGN2 338
#define DIV_ASSIGN2 339
#define MOD_ASSIGN2 340
#define AND_ASSIGN2 341
#define XOR_ASSIGN2 342
#define OR_ASSIGN2 343
#define DEPEND 344
#define IN 345
#define OUT 346
#define INOUT 347
#define MERGEABLE 348
#define LEXICALERROR 349
#define IDENTIFIER 350
#define READ 351
#define WRITE 352
#define CAPTURE 353
#define SIMDLEN 354
#define FINAL 355
#define PRIORITY 356
#define ICONSTANT 357
#define EXPRESSION 358
#define ID_EXPRESSION 359
#define RAW_STRING 360

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 107 "../../../../rose_src/src/frontend/SageIII/ompparser.yy" /* yacc.c:1909  */
  int itype;
          double ftype;
          const char* stype;
          void* ptype; /* For expressions */
        

#line 271 "ompparser.hh" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE omp_lval;
extern YYLTYPE omp_lloc;
int omp_parse (void);

#endif /* !YY_OMP_OMPPARSER_HH_INCLUDED  */
