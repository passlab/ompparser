%option prefix="omp_"
/*%option outfile="lex.yy.c"*/
%option stack
%x EXPR
%x CLAUSE

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
omp             {ECHO; return cond_return ( OMP); }
parallel        {ECHO; return cond_return ( PARALLEL); }
task            {ECHO; return cond_return ( TASK ); }
taskwait        {ECHO; return cond_return ( TASKWAIT ); }
untied          {ECHO; return cond_return ( UNTIED );}
mergeable       {ECHO; return cond_return ( MERGEABLE );}
if              {ECHO; return ( IF); } /*if is a keyword in C/C++, no change to be a variable*/
num_threads     {ECHO; /*Can be either a clause name or a variable name */ 
                  return cond_return (NUM_THREADS);
                  /*
                  if (b_within_variable_list)
                  {
                    omp_lval.stype = strdup(yytext);
                     return ID_EXPRESSION;
                  }
                  else 
                    return ( NUM_THREADS); 
                    */
                } 
order         	{ECHO; return cond_return ( ORDER  ); }				
ordered         {ECHO; return cond_return ( ORDERED  ); }
schedule        {ECHO; return cond_return ( SCHEDULE ); }
sections        {ECHO; return cond_return  ( SECTIONS ); }
section         {ECHO; return cond_return ( SECTION ); }
single          {ECHO; return cond_return ( SINGLE ); }
nowait          {ECHO; return cond_return ( NOWAIT); }
for             {ECHO; return ( FOR ); } /*keyword in C/C++ */
collapse        {ECHO; return cond_return ( COLLAPSE ); }
master          {ECHO; return cond_return ( MASTER ); }
critical        {ECHO; return cond_return ( CRITICAL ); }
barrier         {ECHO; return cond_return ( BARRIER ); }
atomic          {ECHO; return cond_return ( ATOMIC ); }
flush           {ECHO; return cond_return ( FLUSH ); }
threadprivate   {ECHO; return cond_return ( THREADPRIVATE ); }
private         {ECHO; return cond_return ( PRIVATE ); }
copyprivate     {ECHO; return cond_return ( COPYPRIVATE ); }
firstprivate    {ECHO; return cond_return ( FIRSTPRIVATE ); }
lastprivate     {ECHO; return LASTPRIVATE; }
default         {ECHO; return cond_return ( DEFAULT ); }
shared          {ECHO; return cond_return ( SHARED ); } 
none            {ECHO; return cond_return ( NONE ); } 
reduction       {ECHO; return cond_return ( REDUCTION ); }
copyin          {ECHO; return cond_return ( COPYIN ); }
inbranch        {ECHO; return cond_return ( INBRANCH ); }
notinbranch     {ECHO; return cond_return ( NOTINBRANCH ); }
proc_bind       {ECHO; return cond_return ( PROC_BIND); }
allocate        {ECHO; return ALLOCATE; }
close           {ECHO; return cond_return ( CLOSE ); }
spread          {ECHO; return cond_return ( SPREAD ); } /* master should already be recognized */
depend          {ECHO; return cond_return ( DEPEND ); } 
in              {ECHO; return cond_return ( IN ); } 
out             {ECHO; return cond_return ( OUT ); } 
inout           {ECHO; return cond_return ( INOUT ); } 
read            {ECHO; return cond_return ( READ ); }
write           {ECHO; return cond_return ( WRITE ); }
capture         {ECHO; return cond_return ( CAPTURE ); }
target          {ECHO; return cond_return ( TARGET ); }
mpi             {ECHO; return cond_return ( MPI ); } /*Experimental keywords to support scaling up to MPI processes */
mpi:all         {ECHO; return cond_return ( MPI_ALL ); }
mpi:master      {ECHO; return cond_return ( MPI_MASTER ); }
begin           {ECHO; return cond_return ( TARGET_BEGIN ); }
end             {ECHO; return cond_return ( TARGET_END ); }
declare         {ECHO; return cond_return ( DECLARE ); }
data            {ECHO; return cond_return ( DATA ); }
update          {ECHO; return cond_return ( UPDATE ); }
map             {ECHO; return cond_return ( MAP ); }
device          {ECHO; return cond_return ( DEVICE ); }
alloc           {ECHO; return cond_return ( ALLOC ); }
to              {ECHO; return cond_return ( TO ); /* change the user level keyword to conform to  OpenMP 4.0 */}
from            {ECHO; return cond_return ( FROM ); }
tofrom          {ECHO; return cond_return ( TOFROM ); }
safelen         {ECHO; return cond_return ( SAFELEN ); }
simdlen         {ECHO; return cond_return ( SIMDLEN ); }
aligned         {ECHO; return cond_return ( ALIGNED ); }
linear          {ECHO; return cond_return ( LINEAR ); }
uniform         {ECHO; return cond_return ( UNIFORM ); }
final           {ECHO; return cond_return ( FINAL ); }
priority        {ECHO; return cond_return ( PRIORITY); }
dist_data       {ECHO; return ( DIST_DATA); } /*Extensions for data distribution clause
It is tricky to support mixed variable vs. keyword parsing for dist_data() since it is part of variable list parsing 
We enforce that users won't use variable names colliding with the keywords (no cond_return() is used)
TODO: later we can relax this restriction. Fine-grain control of cond_return with new flags.
*/
block           {ECHO; return ( BLOCK ); }
duplicate       {ECHO; return ( DUPLICATE ); }
cyclic          {ECHO; return ( CYCLIC ); }

BLOCK           {ECHO; return ( BLOCK ); }
DUPLICATE       {ECHO; return ( DUPLICATE ); }
CYCLIC          {ECHO; return ( CYCLIC ); }

"("             {ECHO; BEGIN(CLAUSE); return '('; }
")"             {ECHO; return ')'; }
","             {ECHO; return ','; }

{comment}       {ECHO; ; }


{newline}       { /* printf("found a new line\n"); */ /* return (NEWLINE); We ignore NEWLINE since we only care about the pragma string , We relax the syntax check by allowing it as part of line continuation */ }

<CLAUSE>monotonic   				{ECHO; BEGIN(INITIAL); printf("TOKEN monotonic found. \n"); 	return MONOTONIC; }
<CLAUSE>nonmonotonic				{ECHO; BEGIN(INITIAL); printf("TOKEN nonmonotonic found. \n"); 	return NONMONOTONIC; }
<CLAUSE>simd    					{ECHO; BEGIN(INITIAL); printf("TOKEN simd found. \n"); 			return SIMD; }
<CLAUSE>static    					{ECHO; BEGIN(INITIAL); printf("TOKEN static found. \n"); 		return STATIC; }
<CLAUSE>dynamic    					{ECHO; BEGIN(INITIAL); printf("TOKEN dynamic found. \n"); 		return DYNAMIC; }
<CLAUSE>guided    					{ECHO; BEGIN(INITIAL); printf("TOKEN guided found. \n"); 		return GUIDED; }
<CLAUSE>auto    					{ECHO; BEGIN(INITIAL); printf("TOKEN auto found. \n"); 			return AUTO; }
<CLAUSE>runtime    					{ECHO; BEGIN(INITIAL); printf("TOKEN runtime found. \n"); 		return RUNTIME; }
<CLAUSE>shared    					{ECHO; BEGIN(INITIAL); printf("TOKEN shared found. \n"); 		return ATTR_SHARED; }
<CLAUSE>none      					{ECHO; BEGIN(INITIAL); printf("TOKEN none found. \n"); 			return ATTR_NONE; }
<CLAUSE>master    					{ECHO; BEGIN(INITIAL); printf("TOKEN master found. \n"); 		return ATTR_MASTER; }
<CLAUSE>close     					{ECHO; BEGIN(INITIAL); printf("TOKEN close found. \n"); 		return ATTR_CLOSE; }
<CLAUSE>spread    					{ECHO; BEGIN(INITIAL); printf("TOKEN spread found. \n"); 		return ATTR_SPREAD; }
<CLAUSE>concurrent  				{ECHO; BEGIN(INITIAL); printf("TOKEN concurrent found. \n"); 	return ATTR_CONCURRENT; }
<CLAUSE>parallel  					{ECHO; printf("TOKEN parallel found. \n"); 						return ATTR_PARALLEL; }
<CLAUSE>omp_default_mem_alloc       {ECHO; printf("TOKEN omp_default_mem_alloc found. \n"); 		return DEFAULT_MEM_ALLOC; }
<CLAUSE>omp_large_cap_mem_alloc    	{ECHO; printf("TOKEN omp_large_cap_mem_alloc found. \n"); 		return LARGE_CAP_MEM_ALLOC; }
<CLAUSE>omp_const_mem_alloc       	{ECHO; printf("TOKEN omp_const_mem_alloc found. \n"); 			return CONST_MEM_ALLOC; }
<CLAUSE>omp_high_bw_mem_alloc       {ECHO; printf("TOKEN omp_high_bw_mem_alloc found. \n"); 		return HIGH_BW_MEM_ALLOC; }
<CLAUSE>omp_low_lat_mem_alloc       {ECHO; printf("TOKEN omp_low_lat_mem_alloc found. \n"); 		return LOW_LAT_MEM_ALLOC; }
<CLAUSE>omp_cgroup_mem_alloc       	{ECHO; printf("TOKEN omp_cgroup_mem_alloc found. \n"); 			return CGROUP_MEM_ALLOC; }
<CLAUSE>omp_pteam_mem_alloc       	{ECHO; printf("TOKEN omp_pteam_mem_alloc found. \n"); 			return PTEAM_MEM_ALLOC; }
<CLAUSE>omp_thread_mem_alloc       	{ECHO; printf("TOKEN omp_thread_mem_alloc found. \n"); 			return THREAD_MEM_ALLOC; }
<CLAUSE>inscan    					{ECHO; printf("TOKEN inscan found. \n"); 						return MODIFIER_INSCAN; }
<CLAUSE>task      					{ECHO; printf("TOKEN task found. \n"); 							return MODIFIER_TASK; }
<CLAUSE>default   					{ECHO; printf("TOKEN default found. \n"); 						return MODIFIER_DEFAULT; }
<CLAUSE>"+"							{ECHO; printf("TOKEN + found. \n"); 							return '+'; }
<CLAUSE>"-"       					{ECHO; printf("TOKEN - found. \n"); 							return '-'; }
<CLAUSE>"*"       					{ECHO; printf("TOKEN * found. \n"); 							return '*'; }
<CLAUSE>"&"       					{ECHO; printf("TOKEN & found. \n"); 							return '&'; }
<CLAUSE>"|"							{ECHO; printf("TOKEN | found. \n"); 							return '|'; }
<CLAUSE>"^"       					{ECHO; printf("TOKEN ^ found. \n"); 							return '^'; }
<CLAUSE>"&&"						{ECHO; printf("TOKEN && found. \n"); 							return LOGAND; }
<CLAUSE>"||"						{ECHO; printf("TOKEN || found. \n"); 							return LOGOR; }
<CLAUSE>"max"						{ECHO; printf("TOKEN max found. \n"); 							return MAX; }
<CLAUSE>"min"						{ECHO; printf("TOKEN min found. \n"); 							return MIN; }
<CLAUSE>"false"						{ECHO; printf("TOKEN 0 found. \n"); 							return FALSE; }
<CLAUSE>"true"						{ECHO; printf("TOKEN 1 found. \n"); 							return TRUE; }
<CLAUSE>","							{ECHO; return ','; }
<CLAUSE>{blank}						{ECHO; ; }
<CLAUSE>":"							{ECHO; BEGIN(EXPR); return ':';}
<CLAUSE>.							{ECHO; BEGIN(EXPR); CurrentString = yytext[0];}

<EXPR>.     {   CurrentChar = yytext[0];
                ParenLocalCount = 0;
                ParenGlobalCount = 1;
                BracketCount = 0;
                for (;;) {
                    switch (CurrentChar) {
                        case EOF :
                            return LEXICALERROR;

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

expr            {ECHO; return (EXPRESSION); }
identifier      {ECHO; return (IDENTIFIER); /*not in use for now*/ }

{blank}*        ;
.               {ECHO; return (LEXICALERROR);}

\n|.       		{ECHO; printf(" unexpected\n");}

%%


/* yy_push_state can't be called outside of this file, provide a wrapper */
extern void omp_parse_expr() {
        yy_push_state(EXPR);
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






/**
 * @file
 * Lexer for OpenMP-pragmas.
 */

