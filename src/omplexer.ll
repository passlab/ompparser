%option prefix="omp_"
/*%option outfile="lex.yy.c"*/
%option stack
%x EXPR_STATE
%x CLAUSE_STATE
%x ALLOCATE_STATE
%x REDUCTION_STATE

%{
#undef ECHO
#define ECHO printf ("[%s]\n", yytext)

/* DQ (12/10/2016): This is a technique to suppress warnings in generated code that we want to be an error elsewhere in ROSE. 
   See https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html for more detail.
 */
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

/* lex requires me to use extern "C" here */
extern "C" int omp_wrap() { return 1; }

extern int omp_lex();

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
omp             { return OMP; }
parallel        { return PARALLEL; }
task            { return TASK; }
if              { return IF; }
simd            { return SIMD; }
num_threads     { return NUM_THREADS; }
default         { return DEFAULT; }
private         { return PRIVATE; }
firstprivate    { return FIRSTPRIVATE; }
shared          { return SHARED; }
none            { return NONE; }
reduction       { BEGIN(REDUCTION_STATE); return REDUCTION; }
copyin          { return COPYIN; }
proc_bind       { return PROC_BIND; }
allocate        { BEGIN(ALLOCATE_STATE); return ALLOCATE; }
close           { return CLOSE; }
spread          { return SPREAD; } /* master should already be recognized */
master          { return MASTER; }

{comment}       {; }


{newline}       { /* printf("found a new line\n"); */ /* return (NEWLINE); We ignore NEWLINE since we only care about the pragma string , We relax the syntax check by allowing it as part of line continuation */ }

<ALLOCATE_STATE>omp_default_mem_alloc       { return DEFAULT_MEM_ALLOC; }
<ALLOCATE_STATE>omp_large_cap_mem_alloc    	{ return LARGE_CAP_MEM_ALLOC; }
<ALLOCATE_STATE>omp_const_mem_alloc       	{ return CONST_MEM_ALLOC; }
<ALLOCATE_STATE>omp_high_bw_mem_alloc       { return HIGH_BW_MEM_ALLOC; }
<ALLOCATE_STATE>omp_low_lat_mem_alloc       { return LOW_LAT_MEM_ALLOC; }
<ALLOCATE_STATE>omp_cgroup_mem_alloc       	{ return CGROUP_MEM_ALLOC; }
<ALLOCATE_STATE>omp_pteam_mem_alloc       	{ return PTEAM_MEM_ALLOC; }
<ALLOCATE_STATE>omp_thread_mem_alloc        { return THREAD_MEM_ALLOC; }

<REDUCTION_STATE>inscan    					{ return MODIFIER_INSCAN;}
<REDUCTION_STATE>task      					{ return MODIFIER_TASK;}
<REDUCTION_STATE>default   					{ return MODIFIER_DEFAULT;}

":"							                { BEGIN(EXPR_STATE); return ':';}
<CLAUSE>.         { BEGIN(EXPR_STATE); CurrentString = yytext[0];}

<EXPR_STATE>.     {   CurrentChar = yytext[0];
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
                                BEGIN(INITIAL);
                                if (CurrentString.size() != 0) {
                                    omp_lval.stype = strdup(CurrentString.c_str());
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
                                omp_lval.stype = strdup(CurrentString.c_str());
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
                                omp_lval.stype = strdup(CurrentString.c_str());
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
.               {/*return (LEXICALERROR);*/ return -1;}

\n|.       		{printf(" unexpected\n");}

%%


/* yy_push_state can't be called outside of this file, provide a wrapper */
extern void omp_parse_expr() {
        yy_push_state(EXPR_STATE);
}

/* entry point invoked by callers to start scanning for a string */
extern void omp_lexer_init(const char* str) {
  ompparserinput = str;
  /* We have omp_ suffix for all flex functions */
  omp_restart(omp_in);
}
/*Conditional return ID_EXPRESSION or input based on the context*/
static int cond_return (int input)
{
  if (b_within_variable_list)
  {
    omp_lval.stype = strdup(yytext);
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

