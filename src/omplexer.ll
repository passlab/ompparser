/*
 * Copyright (c) 2018-2020, High Performance Computing Architecture and System
 * research laboratory at University of North Carolina at Charlotte (HPCAS@UNCC)
 * and Lawrence Livermore National Security, LLC.
 *
 * SPDX-License-Identifier: (BSD-3-Clause)
 */

%option prefix="openmp_"
%option stack
%option noyy_top_state

%x AFFINITY_EXPR_STATE
%x AFFINITY_ITERATOR_STATE
%x AFFINITY_STATE
%x ALIGNED_STATE
%x ALLOCATE_STATE
%x ALLOCATOR_STATE
%x ALLOC_EXPR_STATE
%x ARCH_STATE
%x ATOMIC_DEFAULT_MEM_ORDER_STATE
%x BIND_STATE
%x COLLAPSE_STATE
%x CONDITION_STATE
%x COPYIN_STATE
%x COPYPRIVATE_STATE
%x DEFAULTMAP_STATE
%x DEFAULT_STATE
%x DEPEND_EXPR_STATE
%x DEPEND_ITERATOR_STATE
%x DEPEND_STATE
%x DEVICE_STATE
%x DEVICE_TYPE_STATE
%x DIST_SCHEDULE_STATE
%x EXPR_STATE
%x EXTENSION_STATE
%x FINAL_STATE
%x FIRSTPRIVATE_STATE
%x FROM_MAPPER_STATE
%x FROM_STATE
%x ID_EXPR_STATE
%x IF_STATE
%x IMPLEMENTATION_STATE
%x INITIALIZER_STATE
%x IN_REDUCTION_STATE
%x ISA_STATE
%x LASTPRIVATE_STATE
%x LINEAR_STATE
%x MAPPER_STATE
%x MAP_MAPPER_STATE
%x MAP_STATE
%x MATCH_STATE
%x NONTEMPORAL_STATE
%x NUM_TEAMS_STATE
%x NUM_THREADS_STATE
%x ORDERED_STATE
%x ORDER_STATE
%x PRIVATE_STATE
%x PROC_BIND_STATE
%x REDUCTION_STATE
%x SAFELEN_STATE
%x SCHEDULE_STATE
%x SCORE_STATE
%x SHARED_STATE
%x SIMDLEN_STATE
%x SIMD_STATE
%x TASK_REDUCTION_STATE
%x THREADPRIVATE_STATE
%x TO_MAPPER_STATE
%x TO_STATE
%x TYPE_STR_STATE
%x UPDATE_STATE
%x USES_ALLOCATORS_STATE
%x VENDOR_STATE
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
#include "ompparser.hh"
#include <iostream>

/* Moved from Makefile.am to the source file to work with --with-pch 
Liao 12/10/2009 */
#define YY_NO_TOP_STATE
#define YY_NO_POP_STATE

static const char* ompparserinput = NULL;
static std::string current_string;
static int parenthesis_local_count=0, parenthesis_global_count = 1, bracket_count;
static int brace_count = 0;
static char current_char;

/* Liao 6/11/2010,
OpenMP does not preclude the use of clause names as regular variable names.
For example, num_threads could be a clause name or a variable in the variable list.

We introduce a flag to indicate the context: within a variable list like (a,y,y) or outside of it
  We check '(' or ')' to set it to true or false as parsing proceed */
extern bool b_within_variable_list ; /* = false; */

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

[!c*]$omp       { ; }
#pragma         { ; }
omp             { ; }
parallel        { return PARALLEL; }
metadirective   { return METADIRECTIVE; }
task            { return TASK; }
if              { yy_push_state(IF_STATE); return IF; }
simdlen         { yy_push_state(SIMDLEN_STATE); return SIMDLEN; }
simd/{blank}*\( { yy_push_state(SIMD_STATE); return SIMD; }
simd            { return SIMD; }
num_threads     { yy_push_state(NUM_THREADS_STATE); return NUM_THREADS; }
num_teams       { yy_push_state(NUM_TEAMS_STATE); return NUM_TEAMS; }
thread_limit    { return THREAD_LIMIT; }
default         { yy_push_state(DEFAULT_STATE); return DEFAULT; }
private         { yy_push_state(PRIVATE_STATE); return PRIVATE; }
firstprivate    { yy_push_state(FIRSTPRIVATE_STATE); return FIRSTPRIVATE; }
shared          { yy_push_state(SHARED_STATE); return SHARED; }
none            { return NONE; }
reduction       { yy_push_state(REDUCTION_STATE); return REDUCTION; }
copyin          { yy_push_state(COPYIN_STATE); return COPYIN; }
proc_bind       { yy_push_state(PROC_BIND_STATE); return PROC_BIND; }
allocate        { yy_push_state(ALLOCATE_STATE); return ALLOCATE; }
close           { return CLOSE; }
spread          { return SPREAD; } /* master should already be recognized */
teams           { return TEAMS; }
master          { return MASTER; } /*YAYING */
for             { return FOR; }
do              { return DO; }
lastprivate     { yy_push_state(LASTPRIVATE_STATE); return LASTPRIVATE; }
linear          { yy_push_state(LINEAR_STATE); return LINEAR; }
schedule        { yy_push_state(SCHEDULE_STATE); return SCHEDULE; }
collapse        { yy_push_state(COLLAPSE_STATE); return COLLAPSE; }
ordered/{blank}*\( { yy_push_state(ORDERED_STATE); return ORDERED; }
ordered         { return ORDERED; }
partial         { return PARTIAL; }
nowait          { return NOWAIT; }
full            { return FULL; }
order           { yy_push_state(ORDER_STATE); return ORDER; }
safelen         { yy_push_state(SAFELEN_STATE); return SAFELEN; }
nontemporal     { yy_push_state(NONTEMPORAL_STATE); return NONTEMPORAL; }
aligned         { yy_push_state(ALIGNED_STATE); return ALIGNED; }
align           { return ALIGN; }
declare         { return DECLARE; }
uniform         { return UNIFORM; }
inbranch        { return INBRANCH; }
notinbranch     { return NOTINBRANCH; }
distribute      { return DISTRIBUTE; }
dist_schedule   { yy_push_state(DIST_SCHEDULE_STATE); return DIST_SCHEDULE; }
loop            { return LOOP; }
bind            { yy_push_state(BIND_STATE); return BIND; }
scan            { return SCAN; }
inclusive       { return INCLUSIVE; }
exclusive       { return EXCLUSIVE; }
sections        { return SECTIONS; }
section         { return SECTION; }
single          { return SINGLE; }
copyprivate     { yy_push_state(COPYPRIVATE_STATE); return COPYPRIVATE; }
cancel          { return CANCEL; }
workshare       { return WORKSHARE; }
taskgroup       { return TASKGROUP; }
allocator       { yy_push_state(ALLOCATOR_STATE); return ALLOCATOR; }
threadprivate/{blank}*\( { yy_push_state(THREADPRIVATE_STATE); return THREADPRIVATE; }
threadprivate   { return THREADPRIVATE; }
cancellation    { return CANCELLATION; }
point           { return POINT; }
variant         { return VARIANT; }
when            { yy_push_state(WHEN_STATE); return WHEN; }
match           { yy_push_state(MATCH_STATE); return MATCH; }
initializer     { yy_push_state(INITIALIZER_STATE); return INITIALIZER; }
mapper          { yy_push_state(MAPPER_STATE); return MAPPER; }
unroll          { return UNROLL;}
tile            { return TILE;}

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

final           { yy_push_state(FINAL_STATE); return FINAL; }
untied          { return UNTIED; }
mergeable       { return MERGEABLE; }
in_reduction    { yy_push_state(IN_REDUCTION_STATE); return IN_REDUCTION; }
depend          { yy_push_state(DEPEND_STATE); return DEPEND; }
priority        { return PRIORITY; }
affinity        { yy_push_state(AFFINITY_STATE); return AFFINITY; }
detach          { return DETACH; }

taskloop        { return TASKLOOP; }
taskyield       { return TASKYIELD; }
grainsize       { return GRAINSIZE; }
num_tasks       { return NUM_TASKS; }
nogroup         { return NOGROUP; }

requires                  { return REQUIRES; } 
reverse_offload           { return REVERSE_OFFLOAD; }
unified_address           { return UNIFIED_ADDRESS; }
unified_shared_memory     { return UNIFIED_SHARED_MEMORY; }
atomic_default_mem_order  { yy_push_state(ATOMIC_DEFAULT_MEM_ORDER_STATE); return ATOMIC_DEFAULT_MEM_ORDER; } 
dynamic_allocators        { return DYNAMIC_ALLOCATORS; }
seq_cst                   { return SEQ_CST; }
acq_rel                   { return ACQ_REL; }
relaxed                   { return RELAXED; }
target                    { return TARGET; }
data                      { return DATA; }
device                    { yy_push_state(DEVICE_STATE); return DEVICE; }
use_device_ptr            { return USE_DEVICE_PTR; }
use_device_addr           { return USE_DEVICE_ADDR; }  
enter                     { return ENTER; }
exit                      { return EXIT; }
is_device_ptr             { return IS_DEVICE_PTR; }
has_device_addr           { return HAS_DEVICE_ADDR; }
defaultmap                { yy_push_state(DEFAULTMAP_STATE); return DEFAULTMAP; }
update                    { yy_push_state(UPDATE_STATE); return UPDATE; }

to                        { yy_push_state(TO_STATE); return TO; }
from                      { yy_push_state(FROM_STATE); return FROM; }
uses_allocators           { yy_push_state(USES_ALLOCATORS_STATE); return USES_ALLOCATORS; }
link                      { return LINK; }
device_type               { yy_push_state(DEVICE_TYPE_STATE); return DEVICE_TYPE; }
map                       { yy_push_state(MAP_STATE); return MAP; }
ext_                      { parenthesis_global_count = 0; yy_push_state(EXPR_STATE); return EXT_; }
barrier                   { return BARRIER; }
taskwait                  { return TASKWAIT; }
task_reduction            { yy_push_state(TASK_REDUCTION_STATE); return TASK_REDUCTION; }
flush                     { return FLUSH; }
release                   { return RELEASE; }
acquire                   { return ACQUIRE; }
atomic                    { return ATOMIC; }
read                      { return READ; }
write                     { return WRITE; }
capture                   { return CAPTURE; }
hint                      { return HINT; }
critical                  { return CRITICAL; }
depobj                    { return DEPOBJ; }
destroy                   { return DESTROY; }
threads                   { return THREADS; }
sizes                     { return SIZES; }


"("             { return '('; }
")"             { return ')'; }
":"             { return ':'; }
"}"             { yy_pop_state(); return '}'; }
","             { return ','; }
"\\"            { ; }
"<="            { return LESSOREQUAL; }
">="            { return MOREOREQUAL; }
"!="            { return NOTEQUAL; }

{comment}       { ; }


{newline}       { ; }

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
<ALLOCATE_STATE>":"                                   { return ':'; }
<ALLOCATE_STATE>{blank}*                              { ; }
<ALLOCATE_STATE>.                                     { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<IF_STATE>parallel{blank}*/:                { return PARALLEL; }
<IF_STATE>simd{blank}*/:                    { return SIMD; }
<IF_STATE>task{blank}*/:                    { return TASK; }
<IF_STATE>taskloop{blank}*/:                { return TASKLOOP; }
<IF_STATE>cancel{blank}*/:                  { return CANCEL; }
<IF_STATE>target/{blank}*[{data}{enter}{exit}{update}{:}] { return TARGET; }
<IF_STATE>data/{blank}*:                    { return DATA; }
<IF_STATE>enter/{blank}*data                { return ENTER; }
<IF_STATE>exit/{blank}*data                 { return EXIT; }
<IF_STATE>update/{blank}*:                  { return UPDATE; }
<IF_STATE>"("                               { return '('; }
<IF_STATE>")"                               { yy_pop_state(); return ')'; }
<IF_STATE>":"                               { return ':'; }
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

<ORDER_STATE>concurrent                     { return CONCURRENT; }
<ORDER_STATE>"("                            { return '('; }
<ORDER_STATE>")"                            { yy_pop_state(); return ')'; }
<ORDER_STATE>{blank}*                       { ; }
<ORDER_STATE>.                              { yy_push_state(INITIAL); }

<REDUCTION_STATE>inscan/{blank}*,           { return MODIFIER_INSCAN; }
<REDUCTION_STATE>task/{blank}*,             { return MODIFIER_TASK; }
<REDUCTION_STATE>default/{blank}*,          { return MODIFIER_DEFAULT; }
<REDUCTION_STATE>"("                        { return '('; }
<REDUCTION_STATE>")"                        { yy_pop_state(); return ')'; }
<REDUCTION_STATE>","                        { return ','; }
<REDUCTION_STATE>":"                        { return ':'; }
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

<SIMD_STATE>"("                             { yy_push_state(EXPR_STATE); return '('; }
<SIMD_STATE>")"                             { yy_pop_state(); return ')'; }
<SIMD_STATE>{blank}*                        { ; }

<THREADPRIVATE_STATE>"("                    { yy_push_state(EXPR_STATE); return '('; }
<THREADPRIVATE_STATE>")"                    { yy_pop_state(); return ')'; }
<THREADPRIVATE_STATE>{blank}*               { ; }

<PRIVATE_STATE>"("                          { return '('; }
<PRIVATE_STATE>")"                          { yy_pop_state(); return ')'; }
<PRIVATE_STATE>{blank}*                     { ; }
<PRIVATE_STATE>.                            { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<FIRSTPRIVATE_STATE>"("                     { return '('; }
<FIRSTPRIVATE_STATE>")"                     { yy_pop_state(); return ')'; }
<FIRSTPRIVATE_STATE>{blank}*                { ; }
<FIRSTPRIVATE_STATE>.                       { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<SHARED_STATE>"("                           { return '('; }
<SHARED_STATE>")"                           { yy_pop_state(); return ')'; }
<SHARED_STATE>{blank}*                      { ; }
<SHARED_STATE>.                             { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<COPYPRIVATE_STATE>"("                      { return '('; }
<COPYPRIVATE_STATE>")"                      { yy_pop_state(); return ')'; }
<COPYPRIVATE_STATE>{blank}*                 { ; }
<COPYPRIVATE_STATE>.                        { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<COPYIN_STATE>"("                           { return '('; }
<COPYIN_STATE>")"                           { yy_pop_state(); return ')'; }
<COPYIN_STATE>{blank}*                      { ; }
<COPYIN_STATE>.                             { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<LASTPRIVATE_STATE>conditional/{blank}*:    { return MODIFIER_CONDITIONAL; }
<LASTPRIVATE_STATE>"("                      { return '('; }
<LASTPRIVATE_STATE>")"                      { yy_pop_state(); return ')'; }
<LASTPRIVATE_STATE>":"                      { return ':'; }
<LASTPRIVATE_STATE>{blank}*                 { ; }
<LASTPRIVATE_STATE>.                        { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<LINEAR_STATE>"("                           { return '('; }
<LINEAR_STATE>")"                           { yy_pop_state(); return ')'; }
<LINEAR_STATE>val/{blank}*"("               { return MODOFIER_VAL; }
<LINEAR_STATE>ref/{blank}*                  { return MODOFIER_REF; }
<LINEAR_STATE>uval/{blank}*                 { return MODOFIER_UVAL; }
<LINEAR_STATE>":"                           { return ':'; }
<LINEAR_STATE>{blank}*                      { ; }
<LINEAR_STATE>.                             { yy_push_state(EXPR_STATE); current_string = yytext[0]; }


<SCHEDULE_STATE>monotonic                   { return MODIFIER_MONOTONIC; }
<SCHEDULE_STATE>nonmonotonic                { return MODIFIER_NONMONOTONIC; }
<SCHEDULE_STATE>simd                        { return MODIFIER_SIMD; }
<SCHEDULE_STATE>static                      { return STATIC; }
<SCHEDULE_STATE>dynamic                     { return DYNAMIC; }
<SCHEDULE_STATE>guided                      { return GUIDED; }
<SCHEDULE_STATE>auto                        { return AUTO; }
<SCHEDULE_STATE>runtime                     { return RUNTIME; }
<SCHEDULE_STATE>","                         { return ','; }
<SCHEDULE_STATE>":"                         { return ':'; }
<SCHEDULE_STATE>"("                         { return '('; }
<SCHEDULE_STATE>")"                         { yy_pop_state(); return ')'; }
<SCHEDULE_STATE>{blank}*                    { ; }
<SCHEDULE_STATE>.                           { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<COLLAPSE_STATE>"("                         { return '('; }
<COLLAPSE_STATE>")"                         { yy_pop_state(); return ')'; }
<COLLAPSE_STATE>{blank}*                    { ; }
<COLLAPSE_STATE>.                           { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ORDERED_STATE>"("                          { yy_push_state(EXPR_STATE); return '('; }
<ORDERED_STATE>")"                          { yy_pop_state(); return ')'; }
<ORDERED_STATE>{blank}*                     { ; }

<SIMDLEN_STATE>"("                          { return '('; }
<SIMDLEN_STATE>")"                          { yy_pop_state(); return ')'; }
<SIMDLEN_STATE>{blank}*                     { ; }
<SIMDLEN_STATE>.                            { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<SAFELEN_STATE>"("                          { return '('; }
<SAFELEN_STATE>")"                          { yy_pop_state(); return ')'; }
<SAFELEN_STATE>{blank}*                     { ; }
<SAFELEN_STATE>.                            { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<NONTEMPORAL_STATE>"("                      { return '('; }
<NONTEMPORAL_STATE>")"                      { yy_pop_state(); return ')'; }
<NONTEMPORAL_STATE>{blank}*                 { ; }
<NONTEMPORAL_STATE>.                        { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<NUM_TEAMS_STATE>"("                        { return '('; }
<NUM_TEAMS_STATE>")"                        { yy_pop_state(); return ')'; }
<NUM_TEAMS_STATE>{blank}*                   { ; }
<NUM_TEAMS_STATE>.                          { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<NUM_THREADS_STATE>"("                      { return '('; }
<NUM_THREADS_STATE>")"                      { yy_pop_state(); return ')'; }
<NUM_THREADS_STATE>{blank}*                 { ; }
<NUM_THREADS_STATE>.                        { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ALIGNED_STATE>"("                          { return '('; }
<ALIGNED_STATE>":"                          { return ':'; }
<ALIGNED_STATE>")"                          { yy_pop_state(); return ')'; }
<ALIGNED_STATE>{blank}*                     { ; }
<ALIGNED_STATE>.                            { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<DIST_SCHEDULE_STATE>static/{blank}*        {return STATIC; }
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
<ALLOCATOR_STATE>.                          { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<INITIALIZER_STATE>omp_priv                 { return OMP_PRIV; }
<INITIALIZER_STATE>"="                      { return '='; }
<INITIALIZER_STATE>{blank}*                 { ; }
<INITIALIZER_STATE>"("                      { return '('; }
<INITIALIZER_STATE>")"                      { yy_pop_state(); return ')'; }
<INITIALIZER_STATE>.                        { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<MAPPER_STATE>default                       { return IDENTIFIER_DEFAULT; }
<MAPPER_STATE>"::"                          { return DOUBLE_COLON; }
<MAPPER_STATE>":"                           { yy_push_state(ID_EXPR_STATE); return ':'; }
<MAPPER_STATE>{blank}*                      { ; }
<MAPPER_STATE>"("                           { return '('; }
<MAPPER_STATE>")"                           { yy_pop_state(); return ')'; }
<MAPPER_STATE>.                             { yy_push_state(ID_EXPR_STATE); current_string = yytext[0]; }

<TYPE_STR_STATE>.                           { current_char = yytext[0];
                                            switch (current_char) {
                                                case '(': {
                                                    parenthesis_local_count++;
                                                    parenthesis_global_count++;
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                case ')': {
                                                    parenthesis_local_count--;
                                                    parenthesis_global_count--;
                                                    if (parenthesis_global_count == 0) {
                                                        yy_pop_state();
                                                        if (current_string.size() != 0) {
                                                            openmp_lval.stype = strdup(current_string.c_str());
                                                            current_string.clear();
                                                            unput(')');
                                                            parenthesis_local_count = 0;
                                                            parenthesis_global_count = 1;
                                                            bracket_count = 0;
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
                                                }
                                                case ' ': {
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                default: {
                                                    if (current_char != ' ' || parenthesis_local_count != 0) {
                                                        current_string.append(1, current_char);
                                                    }
                                                }
                                            }
                                        }

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
<WHEN_STATE>implementation                  { yy_push_state(IMPLEMENTATION_STATE); return IMPLEMENTATION; }
<WHEN_STATE>{blank}*                        { ; }
<WHEN_STATE>.                               { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<IMPLEMENTATION_STATE>"("                            { return '('; }
<IMPLEMENTATION_STATE>","                            { return ','; }
<IMPLEMENTATION_STATE>")"                            { return ')'; }
<IMPLEMENTATION_STATE>"="                            { return '='; }
<IMPLEMENTATION_STATE>"{"                            { brace_count++; return '{'; }
<IMPLEMENTATION_STATE>"}"                            { yy_pop_state(); return '}'; }
<IMPLEMENTATION_STATE>vendor/{blank}*\(              { yy_push_state(VENDOR_STATE); return VENDOR; }
<IMPLEMENTATION_STATE>extension/{blank}*\(           { yy_push_state(EXTENSION_STATE); return EXTENSION; }
<IMPLEMENTATION_STATE>{blank}*                       { ; }
<IMPLEMENTATION_STATE>.                              { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<MATCH_STATE>"("                            { return '('; }
<MATCH_STATE>":"                            { yy_push_state(INITIAL); return ':'; }
<MATCH_STATE>")"                            { yy_pop_state(); return ')'; }
<MATCH_STATE>"="                            { return '='; }
<MATCH_STATE>"{"                            { yy_push_state(INITIAL); return '{'; } /* now parsing enters to pass a full construct, directive, condition, etc */
<MATCH_STATE>"}"                            { return '}'; }
<MATCH_STATE>user                           { return USER; }
<MATCH_STATE>construct                      { return CONSTRUCT; }
<MATCH_STATE>device                         { return DEVICE; }
<MATCH_STATE>implementation                 { yy_push_state(IMPLEMENTATION_STATE); return IMPLEMENTATION; }
<MATCH_STATE>{blank}*                       { ; }
<MATCH_STATE>.                              { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ISA_STATE>"("/score{blank}*\(              { return '('; }
<ISA_STATE>"("                              { parenthesis_global_count = 1; return '('; }
<ISA_STATE>")"                              { yy_pop_state(); return ')'; }
<ISA_STATE>{blank}*                         { ; }
<ISA_STATE>score/{blank}*\(                 { yy_push_state(SCORE_STATE); return SCORE; }
<ISA_STATE>.                                { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ARCH_STATE>"("/score{blank}*\(             { return '('; }
<ARCH_STATE>"("                             { parenthesis_global_count = 1; return '('; }
<ARCH_STATE>")"                             { yy_pop_state(); return ')'; }
<ARCH_STATE>{blank}*                        { ; }
<ARCH_STATE>score/{blank}*\(                { yy_push_state(SCORE_STATE); return SCORE; }
<ARCH_STATE>.                               { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<SCORE_STATE>"("{blank}*                    { yy_push_state(EXPR_STATE); parenthesis_global_count = 1; return '('; }
<SCORE_STATE>")"                            { return ')'; }
<SCORE_STATE>":"                            { yy_pop_state(); parenthesis_global_count = 1; return ':'; }
<SCORE_STATE>{blank}*                       { ; }

<CONDITION_STATE>"("/score{blank}*\(        { return '('; }
<CONDITION_STATE>"("                        { parenthesis_global_count = 1; return '('; }
<CONDITION_STATE>")"                        { yy_pop_state(); return ')'; }
<CONDITION_STATE>{blank}*                   { ; }
<CONDITION_STATE>score/{blank}*\(           { yy_push_state(SCORE_STATE); return SCORE; }
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
<VENDOR_STATE>score/{blank}*\(              { yy_push_state(SCORE_STATE); return SCORE; }

<EXTENSION_STATE>"("                        { return '('; }
<EXTENSION_STATE>")"                        { yy_pop_state(); return ')'; }
<EXTENSION_STATE>{blank}*                   { ; }
<EXTENSION_STATE>.                          { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<IN_REDUCTION_STATE>"("                     { return '('; }
<IN_REDUCTION_STATE>")"                     { yy_pop_state(); return ')'; }
<IN_REDUCTION_STATE>","                     { return ','; }
<IN_REDUCTION_STATE>":"                     { return ':'; }
<IN_REDUCTION_STATE>"+"                     { return '+'; }
<IN_REDUCTION_STATE>"-"                     { return '-'; }
<IN_REDUCTION_STATE>"*"                     { return '*'; }
<IN_REDUCTION_STATE>"&"                     { return '&'; }
<IN_REDUCTION_STATE>"|"                     { return '|'; }
<IN_REDUCTION_STATE>"^"                     { return '^'; }
<IN_REDUCTION_STATE>"&&"                    { return LOGAND; }
<IN_REDUCTION_STATE>"||"                    { return LOGOR; }
<IN_REDUCTION_STATE>min/{blank}*:           { return MIN; }
<IN_REDUCTION_STATE>max/{blank}*:           { return MAX; }
<IN_REDUCTION_STATE>{blank}*                { ; }
<IN_REDUCTION_STATE>.                       { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<DEPEND_STATE>"("                           { return '('; }
<DEPEND_STATE>")"                           { yy_pop_state(); return ')'; }
<DEPEND_STATE>","                           { return ','; }
<DEPEND_STATE>"="                           { return '='; }
<DEPEND_STATE>":"                           { yy_push_state(EXPR_STATE); return ':'; }
<DEPEND_STATE>iterator/{blank}*"("          { current_string.clear(); yy_push_state(DEPEND_ITERATOR_STATE);return MODIFIER_ITERATOR; }

<DEPEND_STATE>in                            { return IN; }
<DEPEND_STATE>out                           { return OUT; }
<DEPEND_STATE>inout                         { return INOUT; }
<DEPEND_STATE>mutexinoutset                 { return MUTEXINOUTSET; }
<DEPEND_STATE>depobj                        { return DEPOBJ; }
<DEPEND_STATE>source                        { return SOURCE; }
<DEPEND_STATE>sink                          { return SINK; }
<DEPEND_STATE>{blank}*                      { ; }
<DEPEND_STATE>.                             { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<DEPEND_ITERATOR_STATE>"("                  { return '('; }
<DEPEND_ITERATOR_STATE>"="                  { return '='; }
<DEPEND_ITERATOR_STATE>","                  { return ','; }
<DEPEND_ITERATOR_STATE>":"                  { return ':'; }
<DEPEND_ITERATOR_STATE>")"                  { yy_pop_state(); return ')'; }
<DEPEND_ITERATOR_STATE>{blank}*             { ; }
<DEPEND_ITERATOR_STATE>.                    { yy_push_state(DEPEND_EXPR_STATE); unput(yytext[0]); }

<DEPEND_EXPR_STATE>"("{blank}*              { return '('; }
<DEPEND_EXPR_STATE>{blank}*")"              { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(')'); return EXPR_STRING; }
<DEPEND_EXPR_STATE>","                      { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(','); return EXPR_STRING; } 
<DEPEND_EXPR_STATE>{blank}                  { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); return EXPR_STRING; }
<DEPEND_EXPR_STATE>"="                      { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput('='); return EXPR_STRING; }   
<DEPEND_EXPR_STATE>":"                      { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(':'); return EXPR_STRING; }
<DEPEND_EXPR_STATE>.                        { current_string += yytext[0]; }

<AFFINITY_STATE>"("                         { return '('; }
<AFFINITY_STATE>")"                         { yy_pop_state(); return ')'; }
<AFFINITY_STATE>","                         { return ','; }
<AFFINITY_STATE>":"                         { return ':'; }
<AFFINITY_STATE>iterator/{blank}*"("        { current_string.clear(); yy_push_state(AFFINITY_ITERATOR_STATE);return MODIFIER_ITERATOR; }
<AFFINITY_STATE>{blank}*                    { ; }
<AFFINITY_STATE>.                           { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<AFFINITY_EXPR_STATE>"("{blank}*            { return '('; }
<AFFINITY_EXPR_STATE>{blank}*")"            { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(')'); return EXPR_STRING; }
<AFFINITY_EXPR_STATE>{blank}                { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); return EXPR_STRING; }
<AFFINITY_EXPR_STATE>","                    { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(','); return EXPR_STRING; }
<AFFINITY_EXPR_STATE>"="                    { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput('='); return EXPR_STRING; } 
<AFFINITY_EXPR_STATE>":"                    { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(':'); return EXPR_STRING; }
<AFFINITY_EXPR_STATE>.                      { current_string += yytext[0]; }

<AFFINITY_ITERATOR_STATE>"("                { return '('; }
<AFFINITY_ITERATOR_STATE>"="                { return '='; }
<AFFINITY_ITERATOR_STATE>":"                { return ':'; }
<AFFINITY_ITERATOR_STATE>","                { return ','; }
<AFFINITY_ITERATOR_STATE>")"                { yy_pop_state(); return ')'; }
<AFFINITY_ITERATOR_STATE>{blank}*           { ; }
<AFFINITY_ITERATOR_STATE>.                  { yy_push_state(AFFINITY_EXPR_STATE); unput(yytext[0]); }

<FINAL_STATE>"("                            { return '('; }
<FINAL_STATE>")"                            { yy_pop_state(); return ')'; }
<FINAL_STATE>{blank}*                       { ; }
<FINAL_STATE>.                              { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<ATOMIC_DEFAULT_MEM_ORDER_STATE>seq_cst     { return SEQ_CST; }
<ATOMIC_DEFAULT_MEM_ORDER_STATE>acq_rel     { return ACQ_REL; }
<ATOMIC_DEFAULT_MEM_ORDER_STATE>relaxed     { return RELAXED; }
<ATOMIC_DEFAULT_MEM_ORDER_STATE>"("         { return '('; }
<ATOMIC_DEFAULT_MEM_ORDER_STATE>")"         { yy_pop_state(); return ')'; }
<ATOMIC_DEFAULT_MEM_ORDER_STATE>{blank}*    { ; }

<DEVICE_STATE>ancestor/{blank}*:            { return ANCESTOR; }
<DEVICE_STATE>device_num/{blank}*:          { return DEVICE_NUM; }
<DEVICE_STATE>"("                           { return '('; }
<DEVICE_STATE>")"                           { yy_pop_state(); return ')'; }
<DEVICE_STATE>":"                           { return ':'; }
<DEVICE_STATE>{blank}*                      { ; }
<DEVICE_STATE>.                             { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<DEFAULTMAP_STATE>alloc/{blank}*            { return BEHAVIOR_ALLOC; }
<DEFAULTMAP_STATE>to/{blank}*               { return BEHAVIOR_TO; }
<DEFAULTMAP_STATE>from/{blank}*             { return BEHAVIOR_FROM; }
<DEFAULTMAP_STATE>tofrom/{blank}*           { return BEHAVIOR_TOFROM; }
<DEFAULTMAP_STATE>firstprivate/{blank}*     { return BEHAVIOR_FIRSTPRIVATE; }
<DEFAULTMAP_STATE>none/{blank}*             { return BEHAVIOR_NONE; }
<DEFAULTMAP_STATE>default/{blank}*          { return BEHAVIOR_DEFAULT; }
<DEFAULTMAP_STATE>scalar/{blank}*           { return CATEGORY_SCALAR; }
<DEFAULTMAP_STATE>aggregate/{blank}*        { return CATEGORY_AGGREGATE; }
<DEFAULTMAP_STATE>pointer/{blank}*          { return CATEGORY_POINTER; }
<DEFAULTMAP_STATE>allocatable/{blank}*      { return CATEGORY_ALLOCATABLE; }
<DEFAULTMAP_STATE>"("                       { return '('; }
<DEFAULTMAP_STATE>")"                       { yy_pop_state(); return ')'; }
<DEFAULTMAP_STATE>":"                       { return ':'; }
<DEFAULTMAP_STATE>{blank}*                  { ; }

<TO_STATE>"("                               { return '('; }
<TO_STATE>")"                               { yy_pop_state(); return ')'; }
<TO_STATE>","                               { return ','; }
<TO_STATE>":"                               { return ':'; }
<TO_STATE>mapper/{blank}*"("                { current_string.clear(); yy_push_state(TO_MAPPER_STATE);return TO_MAPPER; }
<TO_STATE>{blank}*                          { ; }
<TO_STATE>.                                 { yy_push_state(EXPR_STATE); unput(yytext[0]); }


<TO_MAPPER_STATE>"("                        { return '('; }
<TO_MAPPER_STATE>")"                        { yy_pop_state(); return ')'; }
<TO_MAPPER_STATE>.                          { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<FROM_STATE>"("                             { return '('; }
<FROM_STATE>")"                             { yy_pop_state(); return ')'; }
<FROM_STATE>","                             { return ','; }
<FROM_STATE>":"                             { return ':'; }
<FROM_STATE>mapper/{blank}*"("              { current_string.clear(); yy_push_state(FROM_MAPPER_STATE);return FROM_MAPPER; }
<FROM_STATE>{blank}*                        { ; }
<FROM_STATE>.                               { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<FROM_MAPPER_STATE>"("                      { return '('; }
<FROM_MAPPER_STATE>")"                      { yy_pop_state(); return ')'; }
<FROM_MAPPER_STATE>.                        { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<USES_ALLOCATORS_STATE>"("                                     { return '('; }
<USES_ALLOCATORS_STATE>","                                     { return ','; }
<USES_ALLOCATORS_STATE>")"/{blank}*")"                         { yy_pop_state(); return ')'; }
<USES_ALLOCATORS_STATE>")"                                     { return ')'; }
<USES_ALLOCATORS_STATE>omp_default_mem_alloc/{blank}*"("       { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return DEFAULT_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_large_cap_mem_alloc/{blank}*"("     { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return LARGE_CAP_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_const_mem_alloc/{blank}*"("         { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return CONST_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_high_bw_mem_alloc/{blank}*"("       { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return HIGH_BW_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_low_lat_mem_alloc/{blank}*"("       { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return LOW_LAT_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_cgroup_mem_alloc/{blank}*"("        { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return CGROUP_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_pteam_mem_alloc/{blank}*"("         { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return PTEAM_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>omp_thread_mem_alloc/{blank}*"("        { current_string.clear(); yy_push_state(ALLOC_EXPR_STATE);return THREAD_MEM_ALLOC; }
<USES_ALLOCATORS_STATE>{blank}*                                { ; }
<USES_ALLOCATORS_STATE>.                                       { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<ALLOC_EXPR_STATE>"("                        { return '('; }
<ALLOC_EXPR_STATE>")"                        { yy_pop_state(); openmp_lval.stype = strdup(current_string.c_str()); current_string.clear(); unput(')'); return EXPR_STRING; }
<ALLOC_EXPR_STATE>.                          { current_string += yytext[0]; }


<DEVICE_TYPE_STATE>host                      { return HOST; }
<DEVICE_TYPE_STATE>nohost                    { return NOHOST; }
<DEVICE_TYPE_STATE>any                       { return ANY; }
<DEVICE_TYPE_STATE>"("                       { return '('; }
<DEVICE_TYPE_STATE>")"                       { yy_pop_state(); return ')'; }
<DEVICE_TYPE_STATE>{blank}*                  { ; }
<DEVICE_TYPE_STATE>.                         { yy_push_state(INITIAL); unput(yytext[0]); } 

<MAP_STATE>always/{blank}*,                  { return MAP_MODIFIER_ALWAYS; }
<MAP_STATE>close/{blank}*,                   { return MAP_MODIFIER_CLOSE; }
<MAP_STATE>mapper/{blank}*"("                { current_string.clear(); yy_push_state(MAP_MAPPER_STATE);return MAP_MODIFIER_MAPPER; }
<MAP_STATE>"("                               { return '('; }
<MAP_STATE>")"                               { yy_pop_state(); return ')'; }
<MAP_STATE>","                               { return ','; }
<MAP_STATE>":"                               { return ':'; }
<MAP_STATE>to/{blank}*:                      { return MAP_TYPE_TO; }
<MAP_STATE>from/{blank}*:                    { return MAP_TYPE_FROM; }
<MAP_STATE>tofrom/{blank}*:                  { return MAP_TYPE_TOFROM; }
<MAP_STATE>alloc/{blank}*:                   { return MAP_TYPE_ALLOC; }
<MAP_STATE>release/{blank}*:                 { return MAP_TYPE_RELEASE; }
<MAP_STATE>delete                            { return MAP_TYPE_DELETE; }
<MAP_STATE>{blank}*                          { ; }
<MAP_STATE>.                                 { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<MAP_MAPPER_STATE>"("                        { return '('; }
<MAP_MAPPER_STATE>")"                        { yy_pop_state(); return ')'; }
<MAP_MAPPER_STATE>.                          { yy_push_state(EXPR_STATE); unput(yytext[0]); }

<TASK_REDUCTION_STATE>"("                     { return '('; }
<TASK_REDUCTION_STATE>")"                     { yy_pop_state(); return ')'; }
<TASK_REDUCTION_STATE>","                     { return ','; }
<TASK_REDUCTION_STATE>":"                     { return ':'; }
<TASK_REDUCTION_STATE>"+"                     { return '+'; }
<TASK_REDUCTION_STATE>"-"                     { return '-'; }
<TASK_REDUCTION_STATE>"*"                     { return '*'; }
<TASK_REDUCTION_STATE>"&"                     { return '&'; }
<TASK_REDUCTION_STATE>"|"                     { return '|'; }
<TASK_REDUCTION_STATE>"^"                     { return '^'; }
<TASK_REDUCTION_STATE>"&&"                    { return LOGAND; }
<TASK_REDUCTION_STATE>"||"                    { return LOGOR; }
<TASK_REDUCTION_STATE>min/{blank}*:           { return MIN; }
<TASK_REDUCTION_STATE>max/{blank}*:           { return MAX; }
<TASK_REDUCTION_STATE>{blank}*                { ; }
<TASK_REDUCTION_STATE>.                       { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

<UPDATE_STATE>"("                             { return '('; }
<UPDATE_STATE>")"                             { yy_pop_state(); return ')'; }
<UPDATE_STATE>source                          { return SOURCE; }
<UPDATE_STATE>in                              { return IN; }
<UPDATE_STATE>out                             { return OUT; }
<UPDATE_STATE>inout                           { return INOUT; }
<UPDATE_STATE>mutexinoutset                   { return MUTEXINOUTSET; }
<UPDATE_STATE>depobj                          { return DEPOBJ; }
<UPDATE_STATE>sink                            { return SINK; }
<UPDATE_STATE>{blank}*                        { ; }
<UPDATE_STATE>.                               { yy_pop_state(); unput(yytext[0]); }

<EXPR_STATE>.                           { current_char = yytext[0];
                                            switch (current_char) {
                                                case '\n': {
                                                    break;
                                                }
                                                case '(': {
                                                    parenthesis_local_count++;
                                                    parenthesis_global_count++;
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                case ')': {
                                                    parenthesis_local_count--;
                                                    parenthesis_global_count--;
                                                    if (parenthesis_global_count == 0) {
                                                        yy_pop_state();
                                                        if (current_string.size() != 0) {
                                                            openmp_lval.stype = strdup(current_string.c_str());
                                                            current_string.clear();
                                                            unput(')');
                                                            parenthesis_local_count = 0;
                                                            parenthesis_global_count = 1;
                                                            bracket_count = 0;
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
                                                }
                                                case ',': {
                                                    if (current_string.size() == 0) {
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return ',';
                                                    }
                                                    else if (parenthesis_local_count == 0) {
                                                        openmp_lval.stype = strdup(current_string.c_str());
                                                        current_string.clear();
                                                        unput(',');
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return EXPR_STRING;
                                                    }
                                                    else {
                                                        current_string.append(1, current_char);
                                                    };
                                                    break;
                                                }
                                                case '[': {
                                                    bracket_count++;
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                case ']': {
                                                    bracket_count--;
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                case '{': {
                                                    brace_count++;
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                case '}': {
                                                    brace_count--;
                                                    if (brace_count == 0) {
                                                        yy_pop_state();
                                                        if (current_string.size() != 0) {
                                                            openmp_lval.stype = strdup(current_string.c_str());
                                                            current_string.clear();
                                                            unput('}');
                                                            return EXPR_STRING;
                                                        }
                                                        else {
                                                            unput('}');
                                                            break;
                                                        };
                                                    }
                                                    else {
                                                        current_string.append(1, current_char);
                                                    };
                                                    break;
                                                }
                                                case ':': {
                                                    if (current_string.size() == 0) {
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return ':';
                                                    }
                                                    else if (bracket_count == 0) {
                                                        yy_pop_state();
                                                        openmp_lval.stype = strdup(current_string.c_str());
                                                        current_string.clear();
                                                        unput(':');
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return EXPR_STRING;
                                                    }
                                                    else {
                                                        current_string.append(1, current_char);
                                                    }
                                                    break;
                                                }
                                                case ' ': {
                                                    if (parenthesis_global_count == 0) {
                                                        yy_pop_state();
                                                        openmp_lval.stype = strdup(current_string.c_str());
                                                        current_string.clear();
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return EXPR_STRING;
                                                    };
                                                    break;
                                                }
                                                default: {
                                                    if (current_char != ' ' || parenthesis_local_count != 0) {
                                                        current_string.append(1, current_char);
                                                    }
                                                }
                                            }
                                        }
<ID_EXPR_STATE>.                           { current_char = yytext[0];
                                            switch (current_char) {
                                                case '(': {
                                                    parenthesis_local_count++;
                                                    parenthesis_global_count++;
                                                    current_string.append(1, current_char);
                                                    break;
                                                }
                                                case ')': {
                                                    parenthesis_local_count--;
                                                    parenthesis_global_count--;
                                                    if (parenthesis_global_count == 0) {
                                                        yy_pop_state();
                                                        if (current_string.size() != 0) {
                                                            openmp_lval.stype = strdup(current_string.c_str());
                                                            current_string.clear();
                                                            unput(')');
                                                            parenthesis_local_count = 0;
                                                            parenthesis_global_count = 1;
                                                            bracket_count = 0;
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
                                                }
                                                case ':': {
                                                    if (current_string.size() == 0) {
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return ':';
                                                    }
                                                    else if (bracket_count == 0) {
                                                        yy_pop_state();
                                                        openmp_lval.stype = strdup(current_string.c_str());
                                                        current_string.clear();
                                                        unput(':');
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return EXPR_STRING;
                                                    }
                                                    else {
                                                        current_string.append(1, current_char);
                                                    }
                                                    break;
                                                }
                                                case ' ': {
                                                    if (parenthesis_global_count == 0) {
                                                        yy_pop_state();
                                                        openmp_lval.stype = strdup(current_string.c_str());
                                                        current_string.clear();
                                                        parenthesis_local_count = 0;
                                                        parenthesis_global_count = 1;
                                                        bracket_count = 0;
                                                        return EXPR_STRING;
                                                    }
                                                    else {
                                                        current_string.append(1, current_char);
                                                    }
                                                    break;
                                                }
                                                default: {
                                                    if (current_char != ' ' || parenthesis_local_count != 0) {
                                                        current_string.append(1, current_char);
                                                    }
                                                }
                                            }
                                        }

<<EOF>>         { if (current_string.size() != 0) {
                      openmp_lval.stype = strdup(current_string.c_str());
                      current_string.clear();
                      parenthesis_local_count = 0;
                      parenthesis_global_count = 1;
                      bracket_count = 0;
                      return EXPR_STRING;
                  }
                  else {
                      return 0;
                  };
                }

expr            {return (EXPRESSION); }

{blank}*        ;
.               { yy_push_state(EXPR_STATE); current_string = yytext[0]; }

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
    // If the lexer exited due to some error, the condition stack could be nonempty.
    // In this case, it has to be reset to the initial state manually, where yy_start_stack_ptr == 0.
    while (yy_start_stack_ptr > 0) {
        yy_pop_state();
    };
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

