%option prefix="openmp_"
/*%option outfile="lex.yy.c"*/
%option stack
%x EXPR_STATE
%x ALLOCATE_STATE
%x DEFAULT_STATE
%x IF_STATE
%x PROC_BIND_STATE
%x REDUCTION_STATE
%x LASTPRIVATE_STATE
%x LINEAR_STATE
%x SCHEDULE_STATE
%x COLLAPSE_STATE
%x ORDERED_STATE
%x ALIGNED_STATE
%x DIST_SCHEDULE_STATE
%x BIND_STATE
%X ALLOCATOR_STATE
%x WHEN_STATE
%x MATCH_STATE
%x ISA_STATE
%x CONDITION_STATE
%x VENDOR_STATE
%x ARCH_STATE
%x EXTENSION_STATE


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
static std::string current_string;
static int parenthesis_local_count, parenthesis_global_count, bracket_count;
static char current_char;

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
if              { yy_push_state(IF_STATE); return IF; }
simdlen         { return SIMDLEN;}
simd            { return SIMD; }
num_threads     { return NUM_THREADS; }
num_teams       { return NUM_TEAMS; }
thread_limit    { return THREAD_LIMIT; }
default         { yy_push_state(DEFAULT_STATE); return DEFAULT; }
private         { return PRIVATE; }
firstprivate    { return FIRSTPRIVATE; }
shared          { return SHARED; }
none            { return NONE; }
reduction       { yy_push_state(REDUCTION_STATE); return REDUCTION; }
copyin          { return COPYIN; }
proc_bind       { yy_push_state(PROC_BIND_STATE); return PROC_BIND; }
allocate        { yy_push_state(ALLOCATE_STATE); return ALLOCATE; }
close           { return CLOSE; }
spread          { return SPREAD; } /* master should already be recognized */
teams           { return TEAMS; }
master          { return MASTER; } /*YAYING */
for             { return FOR;}
lastprivate     { yy_push_state(LASTPRIVATE_STATE); return LASTPRIVATE;}
linear          { yy_push_state(LINEAR_STATE); return LINEAR;}
schedule        { yy_push_state(SCHEDULE_STATE); return SCHEDULE;}
collapse        { yy_push_state(COLLAPSE_STATE);return COLLAPSE;}
ordered         { yy_push_state(ORDERED_STATE);return ORDERED;}
nowait          { return NOWAIT;}
order           { return ORDER;}
safelen         { return SAFELEN;}
nontemporal     { return NONTEMPORAL;}
aligned         { yy_push_state(ALIGNED_STATE);return ALIGNED;}
declare         { return DECLARE;}
uniform         { return UNIFORM;}
inbranch        { return INBRANCH;}
notinbranch     { return NOTINBRANCH;}
distribute      { return DISTRIBUTE;}
dist_schedule   { yy_push_state(DIST_SCHEDULE_STATE); return DIST_SCHEDULE;}
loop            { return LOOP;}
bind            { yy_push_state(BIND_STATE); return BIND;}
scan            { return SCAN;}
inclusive       { return INCLUSIVE;}
exclusive       { return EXCLUSIVE;}
sections        { return SECTIONS;}
section         { return SECTION;}
single          { return SINGLE;}
copyprivate     { return COPYPRIVATE;}
cancel          { return CANCEL;}
taskgroup       { return TASKGROUP;}
allocator       { yy_push_state(ALLOCATOR_STATE); return ALLOCATOR;}

cancellation    { return CANCELLATION;}
point           { return POINT;}
variant         { return VARIANT; }
when            { yy_push_state(WHEN_STATE); return WHEN; }
match           { yy_push_state(MATCH_STATE); return MATCH; }

end             { return END; }
score           { return SCORE; }
condition       { yy_push_state(CONDITION_STATE); return CONDITION; }
kind            { return KIND; }
host            { return HOST; }
nohost          { return NOHOST; }
any             { return ANY; }
cpu             { return CPU; }
gpu             { return GPU; }
fpga            { return FPGA; }
isa             { yy_push_state(ISA_STATE); return ISA; }
arch            { yy_push_state(ARCH_STATE); return ARCH; }
vendor          { yy_push_state(VENDOR_STATE); return VENDOR; }
extension       { yy_push_state(EXTENSION_STATE); return EXTENSION; }



"("             { return '('; }
")"             { return ')'; }
":"             { return ':'; }
"}"             { yy_pop_state(); return '}'; }
","             { return ','; }
"\\"            { ; }

{comment}       {; }


{newline}       { /* printf("found a new line\n"); */ /* return (NEWLINE); We ignore NEWLINE since we only care about the pragma string , We relax the syntax check by allowing it as part of line continuation */ }

<ALLOCATE_STATE>omp_default_mem_alloc/{blank}*:       { return DEFAULT_MEM_ALLOC; }
<ALLOCATE_STATE>omp_large_cap_mem_alloc/{blank}*:     { return LARGE_CAP_MEM_ALLOC; }
<ALLOCATE_STATE>omp_const_mem_alloc/{blank}*:         { return CONST_MEM_ALLOC; }
<ALLOCATE_STATE>omp_high_bw_mem_alloc/{blank}*:       { return HIGH_BW_MEM_ALLOC; }
<ALLOCATE_STATE>omp_low_lat_mem_alloc/{blank}*:       { return LOW_LAT_MEM_ALLOC; }
<ALLOCATE_STATE>omp_cgroup_mem_alloc/{blank}*:        { return CGROUP_MEM_ALLOC; }
<ALLOCATE_STATE>omp_pteam_mem_alloc/{blank}*:         { return PTEAM_MEM_ALLOC; }
<ALLOCATE_STATE>omp_thread_mem_alloc/{blank}*:        { return THREAD_MEM_ALLOC; }
<ALLOCATE_STATE>"("                                   { return '('; }
<ALLOCATE_STATE>")"                                   { yy_pop_state(); return ')'; }
<ALLOCATE_STATE>":"                                   { yy_push_state(EXPR_STATE); return ':'; }
<ALLOCATE_STATE>{blank}*                              { ; }
<ALLOCATE_STATE>.                                     { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<IF_STATE>parallel{blank}*/:                { return PARALLEL; }
<IF_STATE>simd{blank}*/:                    { return SIMD; }
<IF_STATE>task{blank}*/:                    { return TASK; }
<IF_STATE>cancel{blank}*/:                  { return CANCEL; }
<IF_STATE>"("                               { return '('; }
<IF_STATE>")"                               { yy_pop_state(); return ')'; }
<IF_STATE>":"                               { yy_push_state(EXPR_STATE); return ':'; }
<IF_STATE>{blank}*                          { ; }
<IF_STATE>.                                 { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<PROC_BIND_STATE>master                     { return MASTER; }
<PROC_BIND_STATE>close                      { return CLOSE; }
<PROC_BIND_STATE>spread                     { return SPREAD; }
<PROC_BIND_STATE>"("                        { return '('; }
<PROC_BIND_STATE>")"                        { yy_pop_state(); return ')'; }
<PROC_BIND_STATE>{blank}*                   { ; }
<PROC_BIND_STATE>.                          { return -1; }

<DEFAULT_STATE>shared                       { return SHARED; }
<DEFAULT_STATE>none                         { return NONE; }
<DEFAULT_STATE>firstprivate                 { return FIRSTPRIVATE; }
<DEFAULT_STATE>private                      { return PRIVATE; }
<DEFAULT_STATE>"("                          { return '('; }
<DEFAULT_STATE>")"                          { yy_pop_state(); return ')'; }
<DEFAULT_STATE>{blank}*                     { ; }
<DEFAULT_STATE>.                            { yy_push_state(INITIAL); unput(yytext[0]); } /* So far, only for default in metadirective meaning that a new directive is coming up. */

<REDUCTION_STATE>inscan/{blank}*,           { return MODIFIER_INSCAN; }
<REDUCTION_STATE>task/{blank}*,             { return MODIFIER_TASK; }
<REDUCTION_STATE>default/{blank}*,          { return MODIFIER_DEFAULT; }
<REDUCTION_STATE>"("                        { return '('; }
<REDUCTION_STATE>")"                        { yy_pop_state(); return ')'; }
<REDUCTION_STATE>","                        { return ','; }
<REDUCTION_STATE>":"                        { yy_push_state(EXPR_STATE); return ':'; }
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
<REDUCTION_STATE>.                          { yy_push_state(EXPR_STATE); current_string = yytext[0]; }


<LASTPRIVATE_STATE>conditional/{blank}*:    { return MODIFIER_CONDITIONAL;}
<LASTPRIVATE_STATE>"("                      { return '('; }
<LASTPRIVATE_STATE>")"                      { yy_pop_state(); return ')'; }
<LASTPRIVATE_STATE>":"                      { yy_push_state(EXPR_STATE); return ':';}
<LASTPRIVATE_STATE>{blank}*                 { ; }
<LASTPRIVATE_STATE>.                        { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<LINEAR_STATE>"("                           { return '('; }
<LINEAR_STATE>")"                           { yy_pop_state(); return ')'; }
<LINEAR_STATE>val/{blank}*"("               { return MODOFIER_VAL; }
<LINEAR_STATE>ref/{blank}*                  { return MODOFIER_REF; }
<LINEAR_STATE>uval/{blank}*                 { return MODOFIER_UVAL; }
<LINEAR_STATE>":"                           { yy_push_state(EXPR_STATE); return ':';}
<LINEAR_STATE>{blank}*                      { ; }
<LINEAR_STATE>.                             { yy_push_state(EXPR_STATE); current_string = yytext[0]; }


<SCHEDULE_STATE>monotonic                   { return MODIFIER_MONOTONIC;}
<SCHEDULE_STATE>nomonotonic                 { return MODIFIER_NOMONOTONIC;}
<SCHEDULE_STATE>simd                        { return MODIFIER_SIMD;}
<SCHEDULE_STATE>static                      { return STATIC;}
<SCHEDULE_STATE>dynamic                     { return DYNAMIC;}
<SCHEDULE_STATE>guided                      { return GUIDED;}
<SCHEDULE_STATE>auto                        { return AUTO;}
<SCHEDULE_STATE>runtime                     { return RUNTIME;}
<SCHEDULE_STATE>","                         { return ','; }
<SCHEDULE_STATE>":"                         { return ':';}
<SCHEDULE_STATE>"("                         { return '('; }
<SCHEDULE_STATE>")"                         { yy_pop_state(); return ')'; }
<SCHEDULE_STATE>{blank}*                    { ; }
<SCHEDULE_STATE>.                           { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<COLLAPSE_STATE>"("                         { return '('; }
<COLLAPSE_STATE>")"                         { yy_pop_state(); return ')'; }
<COLLAPSE_STATE>{blank}*                    { ; }
<COLLAPSE_STATE>.                           { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ORDERED_STATE>"("                          { return '('; }
<ORDERED_STATE>")"                          { yy_pop_state(); return ')'; }
<ORDERED_STATE>{blank}*                     { ; }
<ORDERED_STATE>.                            { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ALIGNED_STATE>"("                          { return '('; }
<ALIGNED_STATE>":"                          { yy_push_state(EXPR_STATE); return ':';}
<ALIGNED_STATE>")"                          { yy_pop_state(); return ')'; }
<ALIGNED_STATE>{blank}*                     { ; }
<ALIGNED_STATE>.                            { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<DIST_SCHEDULE_STATE>static/{blank}*        {return STATIC;}
<DIST_SCHEDULE_STATE>"("                    { return '('; }
<DIST_SCHEDULE_STATE>","                    { return ','; }
<DIST_SCHEDULE_STATE>")"                    { yy_pop_state(); return ')'; }
<DIST_SCHEDULE_STATE>{blank}*               { ; }
<DIST_SCHEDULE_STATE>.                      { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<BIND_STATE>teams                           { return TEAMS; }
<BIND_STATE>parallel                        { return PARALLEL; }
<BIND_STATE>thread                          { return THREAD; }
<BIND_STATE>"("                             { return '('; }
<BIND_STATE>")"                             { yy_pop_state(); return ')'; }
<BIND_STATE>{blank}*                        { ; }
<BIND_STATE>.                               { return -1; }

<ALLOCATOR_STATE>omp_default_mem_aloc       { return DEFAULT_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_large_cap_mem_lloc     { return LARGE_CAP_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_const_mem_allo         { return CONST_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_high_bw_mem_aloc       { return HIGH_BW_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_low_lat_mem_aloc       { return LOW_LAT_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_cgroup_mem_allc        { return CGROUP_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_pteam_mem_allo         { return PTEAM_MEM_ALLOC; }
<ALLOCATOR_STATE>omp_thread_mem_allc        { return THREAD_MEM_ALLOC; }
<ALLOCATOR_STATE>{blank}*                   { ; }
<ALLOCATOR_STATE>"("                        { return '('; }
<ALLOCATOR_STATE>")"                        { yy_pop_state(); return ')'; }
<ALLOCATOR_STATE>.                          { return -1; }

":"                                         { yy_push_state(EXPR_STATE); return ':'; }

<WHEN_STATE>"("                             { return '('; }
<WHEN_STATE>":"                             { yy_push_state(INITIAL); return ':'; }
<WHEN_STATE>")"                             { yy_pop_state(); return ')'; }
<WHEN_STATE>"="                             { return '='; }
<WHEN_STATE>"{"                             { yy_push_state(INITIAL); return '{'; } /* now parsrsing enters to pass a full construct, directive, condition, etc */
<WHEN_STATE>"}"                             { return '}'; }
<WHEN_STATE>","                             { ; }
<WHEN_STATE>user                            { return USER; }
<WHEN_STATE>construct                       { return CONSTRUCT; }
<WHEN_STATE>device                          { return DEVICE; }
<WHEN_STATE>implementation                  { return IMPLEMENTATION; }
<WHEN_STATE>{blank}*                        { ; }
<WHEN_STATE>.                               { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<MATCH_STATE>"("                            { return '('; }
<MATCH_STATE>":"                            { yy_push_state(INITIAL); return ':'; }
<MATCH_STATE>")"                            { yy_pop_state(); return ')'; }
<MATCH_STATE>"="                            { return '='; }
<MATCH_STATE>"{"                            { yy_push_state(INITIAL); return '{'; } /* now parsrsing enters to pass a full construct, directive, condition, etc */
<MATCH_STATE>"}"                            { return '}'; }
<MATCH_STATE>user                           { return USER; }
<MATCH_STATE>construct                      { return CONSTRUCT; }
<MATCH_STATE>device                         { return DEVICE; }
<MATCH_STATE>implementation                 { return IMPLEMENTATION; }
<MATCH_STATE>{blank}*                       { ; }
<MATCH_STATE>.                              { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ISA_STATE>"("                              { return '('; }
<ISA_STATE>")"                              { yy_pop_state(); return ')'; }
<ISA_STATE>{blank}*                         { ; }
<ISA_STATE>.                                { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ARCH_STATE>"("                             { return '('; }
<ARCH_STATE>")"                             { yy_pop_state(); return ')'; }
<ARCH_STATE>{blank}*                        { ; }
<ARCH_STATE>.                               { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<CONDITION_STATE>"("                        { return '('; }
<CONDITION_STATE>")"                        { yy_pop_state(); return ')'; }
<CONDITION_STATE>{blank}*                   { ; }
<CONDITION_STATE>.                          { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<VENDOR_STATE>"("                           { return '('; }
<VENDOR_STATE>")"                           { yy_pop_state(); return ')'; }
<VENDOR_STATE>{blank}*                      { ; }
<VENDOR_STATE>amd/{blank}*\)                { return AMD; }
<VENDOR_STATE>arm/{blank}*\)                { return ARM; }
<VENDOR_STATE>bas/{blank}*\)                { return BSC; }
<VENDOR_STATE>cray/{blank}*\)               { return CRAY; }
<VENDOR_STATE>fujitsu/{blank}*\)            { return FUJITSU; }
<VENDOR_STATE>gnu/{blank}*\)                { return GNU; }
<VENDOR_STATE>ibm/{blank}*\)                { return IBM; }
<VENDOR_STATE>intel/{blank}*\)              { return INTEL; }
<VENDOR_STATE>llvm/{blank}*\)               { return LLVM; }
<VENDOR_STATE>pgi/{blank}*\)                { return PGI; }
<VENDOR_STATE>ti/{blank}*\)                 { return TI; }
<VENDOR_STATE>unknown/{blank}*\)            { return UNKNOWN; }
<VENDOR_STATE>.                             { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<EXTENSION_STATE>"("                        { return '('; }
<EXTENSION_STATE>")"                        { yy_pop_state(); return ')'; }
<EXTENSION_STATE>{blank}*                   { ; }
<EXTENSION_STATE>.                          { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<EXPR_STATE>.   { current_char = yytext[0];
                parenthesis_local_count = 0;
                parenthesis_global_count = 1;
                bracket_count = 0;
                for (;;) {
                    switch (current_char) {
                        case EOF :
                            /*return LEXICALERROR*/;
                            return -1;

                        case '(' :
                            parenthesis_local_count++;
                            parenthesis_global_count++;
                            current_string.append(1, current_char);
                            break;

                        case ')' :
                            parenthesis_local_count--;
                            parenthesis_global_count--;
                            if (parenthesis_global_count == 0) {
                                yy_pop_state();
                                if (current_string.size() != 0) {
                                    openmp_lval.stype = strdup(current_string.c_str());
                                    current_string = "";
                                    unput(')');
                                    return EXPR_STRING;
                                }
                                else {
                                    break;
                                };
                            }
                            else {
                                current_string.append(1, current_char);
                            };
                            break;

                        case ',' :
                            if (current_string == "") {
                                return ',';
                            }
                            else if (parenthesis_local_count == 0) {
                                openmp_lval.stype = strdup(current_string.c_str());
                                current_string = "";
                                unput(',');
                                return EXPR_STRING;
                            }
                            else {
                                current_string.append(1, current_char);
                            };
                            break;

                        case '[' :
                            bracket_count++;
                            current_string.append(1, current_char);
                            break;

                        case ']' :
                            bracket_count--;
                            current_string.append(1, current_char);
                            break;

                        case ':' :
                            if (current_string == "") {
                                return ':';
                            }
                            else if (bracket_count == 0) {
                                yy_pop_state();
                                openmp_lval.stype = strdup(current_string.c_str());
                                current_string = "";
                                unput(':');
                                return EXPR_STRING;
                            }
                            else {
                                current_string.append(1, current_char);
                            }
                            break;

                        default :
                            if (current_char != ' ' || parenthesis_local_count != 0) {
                                current_string.append(1, current_char);
                            }
                    }
                    current_char = yyinput();
                }
            }

expr            {return (EXPRESSION); }

{blank}*        ;
.               { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

\n|.            {printf(" unexpected\n");}

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


/* Standalone ompparser */
void start_lexer(const char* input) {
    yy_scan_string(input);
}

void end_lexer(void) {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

