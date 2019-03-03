%option prefix="openmp_"
/*%option outfile="lex.yy.c"*/
%option stack
%option caseless
%x EXPR_STATE
%x CLAUSE_STATE
%x ALLOCATE_STATE
%x DEFAULT_STATE
%x IF_STATE
%x PROC_BIND_STATE
%x REDUCTION_STATE
%x WHEN_STATE

%{

/* DQ (12/10/2016): This is a technique to suppress warnings in generated code that we want to be an error elsewhere in ROSE. 
   See https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html for more detail.
 */
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

/* lex requires me to use extern "C" here */
extern "C" int openmp_wrap() { return 1; }

extern int openmp_lex();
extern void * (*exprParse)(const char*);

#include <stdio.h>
#include <string>
#include <string.h>
#include "ompparser.yy.hpp"
#include <iostream>

/* Moved from Makefile.am to the source file to work with --with-pch 
Liao 12/10/2009 */
#define YY_NO_TOP_STATE
#define YY_NO_POP_STATE

static const char* ompparserinput = NULL;
static std::string CurrentString;
static int ParenLocalCount, ParenGlobalCount, BracketCount;
static char CurrentChar;

/* Liao 6/11/2010,
OpenMP does not preclude the use of clause names as regular variable names.
For example, num_threads could be a clause name or a variable in the variable list.

We introduce a flag to indicate the context: within a variable list like (a,y,y) or outside of it
  We check '(' or ')' to set it to true or false as parsing proceed */
extern bool b_within_variable_list ; /* = false; */

/*conditionally return either a clause token or ID_EXPRESSION, depending on the context.
  We should use it for any OpenMP keyword which could potentially used by users as a variable within a variable list.*/
static int cond_return (int input);
/* pass user specified string to buf, indicate the size using 'result', 
   and shift the current position pointer of user input afterwards 
   to prepare next round of token recognition!!
*/
#define YY_INPUT(buf, result, max_size) { \
                if (*ompparserinput == '\0') result = 0; \
                else { strncpy(buf, ompparserinput, max_size); \
                        buf[max_size] = 0; \
                        result = strlen(buf); \
                        ompparserinput += result; \
                } \
                }

%}

blank           [ ]
newline         [\n]
comment         [\/\/].*

%%
omp             { ; }
parallel        { return PARALLEL; }
metadirective   { return METADIRECTIVE; }
task            { return TASK; }
if              { BEGIN(IF_STATE); return IF; }
simd            { return SIMD; }
num_threads     { return NUM_THREADS; }
default         { yy_push_state(DEFAULT_STATE); return DEFAULT; }
private         { return PRIVATE; }
firstprivate    { return FIRSTPRIVATE; }
shared          { return SHARED; }
none            { return NONE; }
reduction       { BEGIN(REDUCTION_STATE); return REDUCTION; }
copyin          { return COPYIN; }
proc_bind       { BEGIN(PROC_BIND_STATE); return PROC_BIND; }
allocate        { BEGIN(ALLOCATE_STATE); return ALLOCATE; }
close           { return CLOSE; }
spread          { return SPREAD; } /* master should already be recognized */
master          { return MASTER; }
when            { yy_push_state(WHEN_STATE); return WHEN; }
end             { return END; }
score           { return SCORE; }
condition       { return CONDITION; }

"("             { return '('; }
")"             { return ')'; }
":"             { return ':'; }
"}"             { yy_pop_state(); return '}'; }
","             { return ','; }

{comment}       { ; }


{newline}       { /* printf("found a new line\n"); */ /* return (NEWLINE); We ignore NEWLINE since we only care about the pragma string , We relax the syntax check by allowing it as part of line continuation */ }

<ALLOCATE_STATE>omp_default_mem_alloc/{blank}*:       { return DEFAULT_MEM_ALLOC; }
<ALLOCATE_STATE>omp_large_cap_mem_alloc/{blank}*:     { return LARGE_CAP_MEM_ALLOC; }
<ALLOCATE_STATE>omp_const_mem_alloc/{blank}*:         { return CONST_MEM_ALLOC; }
<ALLOCATE_STATE>omp_high_bw_mem_alloc/{blank}*:       { return HIGH_BW_MEM_ALLOC; }
<ALLOCATE_STATE>omp_low_lat_mem_alloc/{blank}*:       { return LOW_LAT_MEM_ALLOC; }
<ALLOCATE_STATE>omp_cgroup_mem_alloc/{blank}*:        { return CGROUP_MEM_ALLOC; }
<ALLOCATE_STATE>omp_pteam_mem_alloc/{blank}*:         { return PTEAM_MEM_ALLOC; }
<ALLOCATE_STATE>omp_thread_mem_alloc/{blank}*:        { return THREAD_MEM_ALLOC; }
<ALLOCATE_STATE>"("                         { return '('; }
<ALLOCATE_STATE>":"                         { BEGIN(EXPR_STATE); return ':'; }
<ALLOCATE_STATE>{blank}*                    { ; }
<ALLOCATE_STATE>.                           { BEGIN(EXPR_STATE); CurrentString = yytext[0]; }

<IF_STATE>parallel{blank}*/:          { return PARALLEL; }
<IF_STATE>simd{blank}*/:              { return SIMD; }
<IF_STATE>task{blank}*/:              { return TASK; }
<IF_STATE>"("                         { return '('; }
<IF_STATE>":"                         { BEGIN(EXPR_STATE); return ':'; }
<IF_STATE>{blank}*                    { ; }
<IF_STATE>.                           { BEGIN(EXPR_STATE); CurrentString = yytext[0]; }

<PROC_BIND_STATE>master                     { return MASTER; }
<PROC_BIND_STATE>close                      { return CLOSE; }
<PROC_BIND_STATE>spread                     { return SPREAD; }
<PROC_BIND_STATE>"("                        { return '('; }
<PROC_BIND_STATE>")"                        { BEGIN(INITIAL); return ')'; }
<PROC_BIND_STATE>{blank}*                   { ; }
<PROC_BIND_STATE>.                          { return -1; }

<DEFAULT_STATE>shared                       { return SHARED; }
<DEFAULT_STATE>none                         { return NONE; }
<DEFAULT_STATE>firstprivate                 { return FIRSTPRIVATE; }
<DEFAULT_STATE>private                      { return PRIVATE; }
<DEFAULT_STATE>"("                          { return '('; }
<DEFAULT_STATE>")"                          { yy_pop_state(); return ')'; }
<DEFAULT_STATE>{blank}*                     { ; }
<DEFAULT_STATE>.                            { yy_push_state(INITIAL); unput(yytext[0]); }


<REDUCTION_STATE>inscan/{blank}*,           { return MODIFIER_INSCAN; }
<REDUCTION_STATE>task/{blank}*,				{ return MODIFIER_TASK; }
<REDUCTION_STATE>default/{blank}*,			{ return MODIFIER_DEFAULT; }
<REDUCTION_STATE>"("                        { return '('; }
<REDUCTION_STATE>","                        { return ','; }
<REDUCTION_STATE>":"                        { BEGIN(EXPR_STATE); return ':'; }
<REDUCTION_STATE>"+"                        { return '+'; }
<REDUCTION_STATE>"-"                        { return '-'; }
<REDUCTION_STATE>"*"                        { return '*'; }
<REDUCTION_STATE>"&"                        { return '&'; }
<REDUCTION_STATE>"|"                        { return '|'; }
<REDUCTION_STATE>"^"                        { return '^'; }
<REDUCTION_STATE>"&&"                       { return LOGAND; }
<REDUCTION_STATE>"||"                       { return LOGOR; }
<REDUCTION_STATE>min/{blank}*:              { return MIN; }
<REDUCTION_STATE>max/{blank}*:              { return MAX; }
<REDUCTION_STATE>{blank}*                   { ; }
<REDUCTION_STATE>.                          { BEGIN(EXPR_STATE); CurrentString = yytext[0]; }

<WHEN_STATE>"("                             { return '('; }
<WHEN_STATE>":"                             { yy_pop_state(); return ':'; }
<WHEN_STATE>")"                             { yy_pop_state(); return ')'; }
<WHEN_STATE>"="                             { return '='; }
<WHEN_STATE>"{"                             { yy_push_state(INITIAL); return '{'; }
<WHEN_STATE>"}"                             { return '}'; }
<WHEN_STATE>user                            { return USER; }
<WHEN_STATE>construct                       { return CONSTRUCT; }
<WHEN_STATE>device                          { return DEVICE; }
<WHEN_STATE>implementation                  { return IMPLEMENTATION; }
<WHEN_STATE>{blank}*                        { ; }
<WHEN_STATE>.                               { yy_push_state(EXPR_STATE); CurrentString = yytext[0]; }


<CLAUSE_STATE>. { BEGIN(EXPR_STATE); CurrentString = yytext[0]; }

<EXPR_STATE>.   { CurrentChar = yytext[0];
                ParenLocalCount = 0;
                ParenGlobalCount = 1;
                BracketCount = 0;
                for (;;) {
                    switch (CurrentChar) {
                        case EOF :
                            /*return LEXICALERROR*/;
                            return -1;

                        case '(' :
                            ParenLocalCount++;
                            ParenGlobalCount++;
                            CurrentString.append(1, CurrentChar);
                            break;

                        case ')' :
                            ParenLocalCount--;
                            ParenGlobalCount--;
                            if (ParenGlobalCount == 0) {
                                yy_pop_state();
                                if (CurrentString.size() != 0) {
                                    openmp_lval.stype = strdup(CurrentString.c_str());
                                    CurrentString = "";
                                    unput(')');
                                    return EXPR_STRING;
                                }
                                else {
                                    break;
                                };
                            }
                            else {
                                CurrentString.append(1, CurrentChar);
                            };
                            break;

                        case ',' :
                            if (CurrentString == "") {
                                return ',';
                            }
                            else if (ParenLocalCount == 0) {
                                openmp_lval.stype = strdup(CurrentString.c_str());
                                CurrentString = "";
                                unput(',');
                                return EXPR_STRING;
                            }
                            else {
                                CurrentString.append(1, CurrentChar);
                            };
                            break;

                        case '[' :
                            BracketCount++;
                            CurrentString.append(1, CurrentChar);
                            break;

                        case ']' :
                            BracketCount--;
                            CurrentString.append(1, CurrentChar);
                            break;

                        case ':' :
                            if (CurrentString == "") {
                                return ':';
                            }
                            else if (BracketCount == 0) {
                                yy_pop_state();
                                openmp_lval.stype = strdup(CurrentString.c_str());
                                CurrentString = "";
								unput(':');
                                return EXPR_STRING;
                            }
                            else {
                                CurrentString.append(1, CurrentChar);
                            }
                            break;

                        default :
                            if (CurrentChar != ' ' || ParenLocalCount != 0) {
                                CurrentString.append(1, CurrentChar);
                            }
                    }
                    CurrentChar = yyinput();
                }
            }

expr            {return (EXPRESSION); }

{blank}*        ;
.               { yy_push_state(EXPR_STATE); CurrentString = yytext[0]; }

\n|.       		{printf(" unexpected\n");}

%%


/* yy_push_state can't be called outside of this file, provide a wrapper */
extern void openmp_parse_expr() {
        yy_push_state(EXPR_STATE);
}

/* entry point invoked by callers to start scanning for a string */
extern void openmp_lexer_init(const char* str) {
  ompparserinput = str;
  /* We have openmp_ suffix for all flex functions */
  openmp_restart(openmp_in);
}
/*Conditional return ID_EXPRESSION or input based on the context*/
static int cond_return (int input)
{
  if (b_within_variable_list)
  {
    openmp_lval.stype = strdup(yytext);
    return ID_EXPRESSION;
  }
  else
    return ( input); 
}


/* Standalone ompparser */
void start_lexer(const char* input) {
    yy_scan_string(input);
}

void end_lexer(void) {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

