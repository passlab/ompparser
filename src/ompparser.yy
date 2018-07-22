/* OpenMP C and C++ Grammar */
/* Author: Markus Schordan, 2003 */
/* Modified by Christian Biesinger 2006 for OpenMP 2.0 */
/* Modified by Chunhua Liao for OpenMP 3.0 and connect to OmpAttribute, 2008 */
/* Updated by Chunhua Liao for OpenMP 4.5,  2017 */

/*
To debug bison conflicts, use the following command line in the build tree

/bin/sh ../../../../sourcetree/config/ylwrap ../../../../sourcetree/src/frontend/Sab.h `echo ompparser.cc | sed -e s/cc$/hh/ -e s/cpp$/hpp/ -e s/cxx$/hxx/ -e s/c++$/h++/ -e s/c$/h/` y.output ompparser.output -- bison -y -d -r state
in the build tree
*/
%name-prefix "omp_"
%defines
%error-verbose

%{
/* DQ (2/10/2014): IF is conflicting with Boost template IF. */
#undef IF

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "OpenMPAttribute.h"
#include <string.h>
#include <cstring>

#ifdef _MSC_VER
  #undef IN
  #undef OUT
  #undef DUPLICATE
#endif

/* Parser - BISON */

/*the scanner function*/
extern int omp_lex(); 

/*A customized initialization function for the scanner, str is the string to be scanned.*/
extern void omp_lexer_init(const char* str);

/* Standalone omppartser */
extern void start_lexer(const char* input);
extern void end_lexer(void);
static void parseParameter (const char*);
static void parseExpression(const char*); 

//! Initialize the parser with the originating SgPragmaDeclaration and its pragma text
// extern void omp_parser_init(SgNode* aNode, const char* str);

/*Treat the entire expression as a string for now
  Implemented in the scanner*/
extern void omp_parse_expr();

static int omp_error(const char*);

// The current AST annotation being built
// static OmpAttribute* ompattribute = NULL;

// The current OpenMP construct or clause type which is being parsed
// It is automatically associated with the current ompattribute
// Used to indicate the OpenMP directive or clause to which a variable list or an expression should get added for the current OpenMP pragma being parsed.
// static omp_construct_enum omptype = e_unknown;

// The context node with the pragma annotation being parsed
//
// We attach the attribute to the pragma declaration directly for now, 
// A few OpenMP directive does not affect the next structure block
// This variable is set by the prefix_parser_init() before prefix_parse() is called.
//Liao
//static SgNode* gNode;

static const char* orig_str; 

// The current expression node being generated 
//static SgExpression* current_exp = NULL;
bool b_within_variable_list  = false;  // a flag to indicate if the program is now processing a list of variables

// We now follow the OpenMP 4.0 standard's C-style array section syntax: [lower-bound:length] or just [length]
// the latest variable symbol being parsed, used to help parsing the array dimensions associated with array symbol
// such as a[0:n][0:m]
//static SgVariableSymbol* array_symbol;
//static SgExpression* lower_exp = NULL;
//static SgExpression* length_exp = NULL;
// check if the parsed a[][] is an array element access a[i][j] or array section a[lower:length][lower:length]
// 
static bool arraySection=true; 


//The directive/clause that are being parsed
static OpenMPDirective* directive = NULL;
static OpenMPClause * clause = NULL;

%}

%locations

/* The %union declaration specifies the entire collection of possible data types for semantic values. these names are used in the %token and %type declarations to pick one of the types for a terminal or nonterminal symbol
corresponding C type is union name defaults to YYSTYPE.
*/

%union {  int itype;
          double ftype;
          const char* stype;
          void* ptype; /* For expressions */
        }

/*Some operators have a suffix 2 to avoid name conflicts with ROSE's existing types, We may want to reuse them if it is proper. 
  experimental BEGIN END are defined by default, we use TARGET_BEGIN TARGET_END instead. 
  Liao*/
%token  OMP PARALLEL IF NUM_THREADS ORDERED SCHEDULE STATIC DYNAMIC GUIDED RUNTIME SECTIONS SINGLE NOWAIT SECTION
        FOR MASTER CRITICAL BARRIER ATOMIC FLUSH TARGET UPDATE DIST_DATA BLOCK DUPLICATE CYCLIC
        THREADPRIVATE PRIVATE COPYPRIVATE FIRSTPRIVATE LASTPRIVATE SHARED DEFAULT NONE REDUCTION COPYIN 
        TASK TASKWAIT UNTIED COLLAPSE AUTO DECLARE DATA DEVICE MAP ALLOC TO FROM TOFROM PROC_BIND CLOSE SPREAD
        SIMD SAFELEN ALIGNED LINEAR UNIFORM INBRANCH NOTINBRANCH MPI MPI_ALL MPI_MASTER TARGET_BEGIN TARGET_END
        '(' ')' ',' ':' '+' '*' '-' '&' '^' '|' LOGAND LOGOR SHLEFT SHRIGHT PLUSPLUS MINUSMINUS PTR_TO '.'
        LE_OP2 GE_OP2 EQ_OP2 NE_OP2 RIGHT_ASSIGN2 LEFT_ASSIGN2 ADD_ASSIGN2
        SUB_ASSIGN2 MUL_ASSIGN2 DIV_ASSIGN2 MOD_ASSIGN2 AND_ASSIGN2 
        XOR_ASSIGN2 OR_ASSIGN2 DEPEND IN OUT INOUT MERGEABLE
        LEXICALERROR IDENTIFIER 
        READ WRITE CAPTURE SIMDLEN FINAL PRIORITY
/*We ignore NEWLINE since we only care about the pragma string , We relax the syntax check by allowing it as part of line continuation */
%token <itype> ICONSTANT   
%token <stype> EXPRESSION ID_EXPRESSION RAW_STRING TESTEXPR 

/* associativity and precedence */
%left '<' '>' '=' "!=" "<=" ">="
%left '+' '-'
%left '*' '/' '%'

/* nonterminals names, types for semantic values, only for nonterminals representing expressions!! not for clauses with expressions.
 */
%type <stype> expression clause_parameter
%type <itype> schedule_kind

/* start point for the parsing */
%start openmp_directive

%%

/* NOTE: We can't use the EXPRESSION lexer token directly. Instead, we have
 * to first call omp_parse_expr, because we parse up to the terminating
 * paren.
 */

openmp_directive : parallel_directive 
                 | for_directive
                 | for_simd_directive
                 | declare_simd_directive
                 | sections_directive
                 | single_directive
                 | parallel_for_directive
                 | parallel_for_simd_directive
                 | parallel_sections_directive
                 | task_directive
                 | master_directive
                 | critical_directive
                 | atomic_directive
                 | ordered_directive
                 | barrier_directive 
                 | taskwait_directive
                 | flush_directive
                 | threadprivate_directive
                 | section_directive
                 | target_directive
                 | target_data_directive
                 | simd_directive
                 ;

parallel_directive : /* #pragma */ OMP PARALLEL {
                       directive = new OpenMPDirective(OMPD_parallel);
                     }
                     parallel_clause_optseq 
                   ;

parallel_clause_optseq : /* empty */
                       | parallel_clause_seq
                       ;

parallel_clause_seq : parallel_clause
                    | parallel_clause_seq parallel_clause
                    | parallel_clause_seq ',' parallel_clause
                    ;

proc_bind_clause : PROC_BIND '(' MASTER ')' { 
                        // ompattribute->addClause(e_proc_bind);
                        // ompattribute->setProcBindPolicy (e_proc_bind_master); 
                      }
                    | PROC_BIND '(' CLOSE ')' {
                        // ompattribute->addClause(e_proc_bind);
                        // ompattribute->setProcBindPolicy (e_proc_bind_close); 
                      }
                    | PROC_BIND '(' SPREAD ')' {
                        // ompattribute->addClause(e_proc_bind);
                        // ompattribute->setProcBindPolicy (e_proc_bind_spread); 
                      }
                    ;

/*  follow the order in the 4.5 specification  */ 
parallel_clause : if_clause
                | num_threads_clause
                | default_clause
                | private_clause
                | firstprivate_clause
                | share_clause
                | copyin_clause
                | reduction_clause
                | proc_bind_clause
                ;

copyin_clause: COPYIN {
                           // ompattribute->addClause(e_copyin);
                           // omptype = e_copyin;
                         } '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list = false;}
                ;


for_directive : /* #pragma */ OMP FOR { 
                  // ompattribute = buildOmpAttribute(e_for,gNode,true); 
                  // omptype = e_for; 
                  // cur_omp_directive=omptype;
                }
                for_clause_optseq
              ;

for_clause_optseq: /* emtpy */
              | for_clause_seq
              ;

for_clause_seq: for_clause
              | for_clause_seq for_clause 
              | for_clause_seq ',' for_clause

/*  updated to 4.5 */
for_clause: private_clause
           | firstprivate_clause
           | lastprivate_clause
           | linear_clause
           | reduction_clause
           | schedule_clause
           | collapse_clause
           | ordered_clause
           | nowait_clause  
          ; 

/* use this for the combined for simd directive */
for_or_simd_clause : ordered_clause
           | schedule_clause
           | private_clause
           | firstprivate_clause
           | lastprivate_clause
           | reduction_clause
           | collapse_clause
           | unique_simd_clause
           | nowait_clause  
          ;

schedule_chunk_opt: /* empty */
                | ',' expression { 
                 }
                ; 

ordered_clause: ORDERED {
                      // ompattribute->addClause(e_ordered_clause);
                      // omptype = e_ordered_clause;
                } ordered_parameter_opt
               ;

ordered_parameter_opt: /* empty */
                | '(' expression ')' {
                   }
                 ;

schedule_clause: SCHEDULE '(' schedule_kind {
                      // ompattribute->addClause(e_schedule);
                      // ompattribute->setScheduleKind(static_cast<omp_construct_enum>($3));
                      // omptype = e_schedule; 
                    }
                    schedule_chunk_opt  ')' 
                 ;

collapse_clause: COLLAPSE {
                      // ompattribute->addClause(e_collapse);
                      // omptype = e_collapse;
                    } '(' expression ')' { 
                    }
                  ;
 
schedule_kind : STATIC  { /* $$ = e_schedule_static; */ }
              | DYNAMIC { /* $$ = e_schedule_dynamic; */ }
              | GUIDED  { /* $$ = e_schedule_guided; */ }
              | AUTO    { /* $$ = e_schedule_auto; */ }
              | RUNTIME { /* $$ = e_schedule_runtime; */ }
              ;

sections_directive : /* #pragma */ OMP SECTIONS { 
                       // ompattribute = buildOmpAttribute(e_sections,gNode, true); 
                     } sections_clause_optseq
                   ;

sections_clause_optseq : /* empty */
                       | sections_clause_seq
                       ;

sections_clause_seq : sections_clause
                    | sections_clause_seq sections_clause
                    | sections_clause_seq ',' sections_clause
                    ;

sections_clause : private_clause
                | firstprivate_clause
                | lastprivate_clause
                | reduction_clause
                | nowait_clause
                ;

section_directive : /* #pragma */  OMP SECTION { 
                      // ompattribute = buildOmpAttribute(e_section,gNode,true); 
                    }
                  ;

single_directive : /* #pragma */ OMP SINGLE { 
                     // ompattribute = buildOmpAttribute(e_single,gNode,true); 
                     // omptype = e_single; 
                   } single_clause_optseq
                 ;

single_clause_optseq : /* empty */
                     | single_clause_seq
                     ;

single_clause_seq : single_clause
                  | single_clause_seq single_clause
                  | single_clause_seq ',' single_clause
                  ;
nowait_clause: NOWAIT {
                  // ompattribute->addClause(e_nowait);
                }
              ;

single_clause : unique_single_clause
              | private_clause
              | firstprivate_clause
              | nowait_clause
              ;
unique_single_clause : COPYPRIVATE { 
                         // ompattribute->addClause(e_copyprivate);
                         // omptype = e_copyprivate; 
                       }
                       '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list =false;}

task_directive : /* #pragma */ OMP TASK {
                    //curDirective = addDirective("task"); 
                    printf("direct task");
                 } task_clause_optseq
               ;

task_clause_optseq :  /* empty */ 
                   | task_clause_seq 
                   ; 

task_clause_seq    : task_clause {printf("task here. \n");}
                   | task_clause_seq task_clause
                   | task_clause_seq ',' task_clause
                   ;

task_clause : unique_task_clause
            | default_clause
            | private_clause
            | firstprivate_clause
            | share_clause
            | depend_clause
            | if_clause
            ;

unique_task_clause : FINAL { 
                       // ompattribute->addClause(e_final);
                       // omptype = e_final; 
                     } '(' expression ')' { 
                     }
                   | PRIORITY { 
                       // ompattribute->addClause(e_priority);
                       // omptype = e_priority; 
                     } '(' expression ')' { 
                     }
                   | UNTIED {
                       // ompattribute->addClause(e_untied);
                     }
                   | MERGEABLE {
                       // ompattribute->addClause(e_mergeable);
                     }
                   ;
                   
depend_clause : DEPEND { 
                          // ompattribute->addClause(e_depend);
                        } '(' dependence_type ':' {b_within_variable_list = true; /* array_symbol=NULL; */ } expression ')'
                        {
                          // assert ((ompattribute->getVariableList(omptype)).size()>0); /* I believe that depend() must have variables */
                          b_within_variable_list = false;
                        }
                      ;

dependence_type : IN {
                       // ompattribute->setDependenceType(e_depend_in); 
                       // omptype = e_depend_in; /*variables are stored for each operator*/
                     }
                   | OUT {
                       // ompattribute->setDependenceType(e_depend_out);  
                       // omptype = e_depend_out;
                     }
                   | INOUT {
                       // ompattribute->setDependenceType(e_depend_inout); 
                       // omptype = e_depend_inout;
                      }
                   ;


parallel_for_directive : /* #pragma */ OMP PARALLEL FOR { 
                            //curDirective = addDirective("parallel_for");
                            directive = new OpenMPDirective(OMPD_parallel_for);
                         } parallel_for_clauseoptseq
                       ;

parallel_for_clauseoptseq : /* empty */
                          | parallel_for_clause_seq
                          ;

/* tracking: change the order */
parallel_for_clause_seq : parallel_for_clause
                        | parallel_for_clause parallel_for_clause_seq
                        | parallel_for_clause ',' parallel_for_clause_seq
                        ;
/*
clause can be any of the clauses accepted by the parallel or for directives, except the
nowait clause, updated for 4.5.
*/
parallel_for_clause : if_clause
                    | num_threads_clause
                    | default_clause
                    | private_clause
                    | firstprivate_clause
                    | share_clause
                    | copyin_clause
                    | reduction_clause
                    | proc_bind_clause
                    | lastprivate_clause
                    | linear_clause
                    | schedule_clause 
                    | collapse_clause
                    | ordered_clause
                   ;

parallel_for_simd_directive : /* #pragma */ OMP PARALLEL FOR SIMD { 
                           // ompattribute = buildOmpAttribute(e_parallel_for_simd, gNode, true); 
                           // omptype= e_parallel_for_simd;
                           // cur_omp_directive = omptype;
                         } parallel_for_simd_clauseoptseq
                       ;

parallel_for_simd_clauseoptseq : /* empty */
                          | parallel_for_simd_clause_seq

parallel_for_simd_clause_seq : parallel_for_simd_clause
                        | parallel_for_simd_clause_seq parallel_for_simd_clause
                        | parallel_for_simd_clause_seq ',' parallel_for_simd_clause
                          
parallel_for_simd_clause: copyin_clause
                    | ordered_clause
                    | schedule_clause
                    | unique_simd_clause
                    | default_clause
                    | private_clause
                    | firstprivate_clause
                    | lastprivate_clause
                    | reduction_clause
                    | collapse_clause
                    | share_clause
                    | if_clause
                    | num_threads_clause
                    | proc_bind_clause
                   ; 
 
parallel_sections_directive : /* #pragma */ OMP PARALLEL SECTIONS { 
                                // ompattribute =buildOmpAttribute(e_parallel_sections,gNode, true); 
                                // omptype = e_parallel_sections; 
                                // cur_omp_directive = omptype;
                              } parallel_sections_clause_optseq
                            ;

parallel_sections_clause_optseq : /* empty */
                                | parallel_sections_clause_seq
                                ;

parallel_sections_clause_seq : parallel_sections_clause
                             | parallel_sections_clause_seq parallel_sections_clause
                             | parallel_sections_clause_seq ',' parallel_sections_clause
                             ;

parallel_sections_clause : copyin_clause
                         | default_clause
                         | private_clause
                         | firstprivate_clause
                         | lastprivate_clause
                         | share_clause
                         | reduction_clause
                         | if_clause
                         | num_threads_clause
                         | proc_bind_clause
                         ;

master_directive : /* #pragma */ OMP MASTER { 
                     // ompattribute = buildOmpAttribute(e_master, gNode, true);
                     // cur_omp_directive = e_master; 
}
                 ;

critical_directive : /* #pragma */ OMP CRITICAL {
                       // ompattribute = buildOmpAttribute(e_critical, gNode, true); 
                       // cur_omp_directive = e_critical;
                     } region_phraseopt
                   ;

region_phraseopt : /* empty */
                 | region_phrase
                 ;

/* This used to use IDENTIFIER, but our lexer does not ever return that:
 * Things that'd match it are, instead, ID_EXPRESSION. So use that here.
 * named critical section
 */
region_phrase : '(' ID_EXPRESSION ')' { 
                  // ompattribute->setCriticalName((const char*)$2);
                }
              ;

barrier_directive : /* #pragma */ OMP BARRIER { 
                      // ompattribute = buildOmpAttribute(e_barrier,gNode, true); 
                      // cur_omp_directive = e_barrier;
}
                  ;

taskwait_directive : /* #pragma */ OMP TASKWAIT { 
                       // ompattribute = buildOmpAttribute(e_taskwait, gNode, true);  
                       // cur_omp_directive = e_taskwait;
                       }
                   ;

atomic_directive : /* #pragma */ OMP ATOMIC { 
                     // ompattribute = buildOmpAttribute(e_atomic,gNode, true); 
                     // cur_omp_directive = e_atomic;
                     } atomic_clauseopt
                 ;

atomic_clauseopt : /* empty */
                 | atomic_clause
                 ;

atomic_clause : READ { // ompattribute->addClause(e_atomic_clause);
                       // ompattribute->setAtomicAtomicity(e_atomic_read);
                      }
               | WRITE{ // ompattribute->addClause(e_atomic_clause);
                       // ompattribute->setAtomicAtomicity(e_atomic_write);
                  }

               | UPDATE { // ompattribute->addClause(e_atomic_clause);
                       // ompattribute->setAtomicAtomicity(e_atomic_update);
                  }
               | CAPTURE { // ompattribute->addClause(e_atomic_clause);
                       // ompattribute->setAtomicAtomicity(e_atomic_capture);
                  }
                ;
flush_directive : /* #pragma */ OMP FLUSH {
                    // ompattribute = buildOmpAttribute(e_flush,gNode, true);
                    // omptype = e_flush; 
                    // cur_omp_directive = omptype;
                  } flush_varsopt
                ;

flush_varsopt : /* empty */
              | flush_vars
              ;

flush_vars : '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list = false;}
           ;

ordered_directive : /* #pragma */ OMP ORDERED { 
                      // ompattribute = buildOmpAttribute(e_ordered_directive,gNode, true); 
                      // cur_omp_directive = e_ordered_directive;
                    }
                  ;

threadprivate_directive : /* #pragma */ OMP THREADPRIVATE {
                            // ompattribute = buildOmpAttribute(e_threadprivate,gNode, true); 
                            // omptype = e_threadprivate; 
                            // cur_omp_directive = omptype;
                          } '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list = false;}
                        ;

default_clause : DEFAULT { 
                        //curClause = addClause("default", curDirective);
                      } clause_parameter
                    ;

                   
private_clause : PRIVATE {
                            clause = new OpenMPClause(OMPC_private);
                            directive->addClause(clause);
                            } clause_parameter {
                                parseParameter(strdup($3));
                            }
                          ;

firstprivate_clause : FIRSTPRIVATE { 
                                 // ompattribute->addClause(e_firstprivate); 
                                 // omptype = e_firstprivate;
                               } '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list = false;}
                             ;

lastprivate_clause : LASTPRIVATE { 
                                  // ompattribute->addClause(e_lastprivate); 
                                  // omptype = e_lastprivate;
                                } '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list = false;}
                              ;

share_clause : SHARED {
                        //curClause = addClause("shared", curDirective);
// ompattribute->addClause(e_shared); omptype = e_shared; 
                      } clause_parameter
                    ;

reduction_clause : REDUCTION { 
                        //curClause = addClause("reduction", curDirective);
                        } clause_parameter
                      ;

clause_parameter : RAW_STRING {char* res = strdup($1); std::cout << res << "\n"; $$ = res;}
                   | TESTEXPR {
                        //char* res = strdup($1);
                        //std::cout << res << "\n";
                        /*
                        std::vector<char*>* nodes = parseParameter(res);
                        if (nodes != NULL) {
                            std::vector<char*>::iterator it;
                            for (it = nodes->begin(); it != nodes->end(); it++) {
                                clause->addLangExpr(*it);
                            }
                        }
                        */
                        //$$ = res;
                        $$ = $1;
                    }
                        ;

target_data_directive: /* pragma */ OMP TARGET DATA {
                       // ompattribute = buildOmpAttribute(e_target_data, gNode,true);
                       // omptype = e_target_data;
                     }
                      target_data_clause_seq
                    ;

target_data_clause_seq : target_data_clause
                    | target_data_clause_seq target_data_clause
                    | target_data_clause_seq ',' target_data_clause
                    ;

target_data_clause : device_clause 
                | map_clause
                | if_clause
                ;

target_directive: /* #pragma */ OMP TARGET {
                       // ompattribute = buildOmpAttribute(e_target,gNode,true);
                       // omptype = e_target;
                       // cur_omp_directive = omptype;
                     }
                     target_clause_optseq 
                   ;

target_clause_optseq : /* empty */
                       | target_clause_seq
                       ;

target_clause_seq : target_clause
                    | target_clause_seq target_clause
                    | target_clause_seq ',' target_clause
                    ;

target_clause : device_clause 
                | map_clause
                | if_clause
                | num_threads_clause
                | begin_clause
                | end_clause
                ;
/*
device_clause : DEVICE {
                           ompattribute->addClause(e_device);
                           omptype = e_device;
                         } '(' expression ')' {
                           addExpression("");
                         }
                ;
*/

/* Experimental extensions to support multiple devices and MPI */
device_clause : DEVICE {
                           // ompattribute->addClause(e_device);
                           // omptype = e_device;
                         } '(' expression_or_star_or_mpi 
                ;
expression_or_star_or_mpi: 
                  MPI ')' { // special mpi device for supporting MPI code generation
                            // current_exp= SageBuilder::buildStringVal("mpi");
                          }
                  | MPI_ALL ')' { // special mpi device for supporting MPI code generation
                            // current_exp= SageBuilder::buildStringVal("mpi:all");
                          }
                  | MPI_MASTER ')' { // special mpi device for supporting MPI code generation
                            // current_exp= SageBuilder::buildStringVal("mpi:master");
                          }
                  | expression ')' { //normal expression
                          }
                  | '*' ')' { // our extension device (*) 
                            // current_exp= SageBuilder::buildCharVal('*'); 
                             }; 


begin_clause: TARGET_BEGIN {
                           // ompattribute->addClause(e_begin);
                           // omptype = e_begin;
                    }
                    ;

end_clause: TARGET_END {
                           // ompattribute->addClause(e_end);
                           // omptype = e_end;
                    }
                    ;

                    
if_clause: IF {
                           // ompattribute->addClause(e_if);
                           // omptype = e_if;
             } '(' expression ')' {
             }
             ;

num_threads_clause: NUM_THREADS {
                            clause = new OpenMPClause(OMPC_num_threads);
                            directive->addClause(clause);
                         } clause_parameter {
                            parseExpression(strdup($3));
                         }
                      ;
map_clause: MAP {
                          // ompattribute->addClause(e_map);
                           // omptype = e_map; // use as a flag to see if it will be reset later
                     } '(' map_clause_optseq 
                     { 
                       b_within_variable_list = true;
                       // if (omptype == e_map) // map data directions are not explicitly specified
                       {
                          // ompattribute->setMapVariant(e_map_tofrom);  omptype = e_map_tofrom;  
                       }
                     } 
                     map_variable_list ')' { b_within_variable_list =false;} 

map_clause_optseq: /* empty, default to be tofrom*/ { 
                 // ompattribute->setMapVariant(e_map_tofrom);  omptype = e_map_tofrom; /*No effect here???*/ 
}
                    | ALLOC ':' { 
// ompattribute->setMapVariant(e_map_alloc);  omptype = e_map_alloc; 
} 
                    | TO     ':' { 
// ompattribute->setMapVariant(e_map_to); omptype = e_map_to; 
} 
                    | FROM    ':' { 
// ompattribute->setMapVariant(e_map_from); omptype = e_map_from; 
} 
                    | TOFROM  ':' { 
// ompattribute->setMapVariant(e_map_tofrom); omptype = e_map_tofrom; 
} 
                    ;

for_simd_directive : /* #pragma */ OMP FOR SIMD { 
                  // ompattribute = buildOmpAttribute(e_for_simd, gNode,true); 
                  // cur_omp_directive = e_for_simd;
                }
                for_or_simd_clause_optseq
              ;


for_or_simd_clause_optseq:  /* empty*/
                      | for_or_simd_clause_seq
                      ;

simd_directive: /* # pragma */ OMP SIMD
                  { 
                    // ompattribute = buildOmpAttribute(e_simd,gNode,true); 
                    // omptype = e_simd; 
                    // cur_omp_directive = omptype;
                    }
                   simd_clause_optseq
                ;

simd_clause_optseq: /*empty*/
             | simd_clause_seq 
            ;

simd_clause_seq: simd_clause
               |  simd_clause_seq simd_clause 
               |  simd_clause_seq ',' simd_clause 
              ;

/* updated to 4.5 */
simd_clause: safelen_clause
           | simdlen_clause
           | linear_clause
           | aligned_clause
           | private_clause
           | lastprivate_clause
           | reduction_clause
           | collapse_clause
            ;


for_or_simd_clause_seq
                : for_or_simd_clause
                | for_or_simd_clause_seq for_or_simd_clause
                | for_or_simd_clause_seq ',' for_or_simd_clause
                ;

safelen_clause :  SAFELEN {
                        // ompattribute->addClause(e_safelen);
                        // omptype = e_safelen;
                      } '(' expression ')' {
                 }
                ; 

unique_simd_clause: safelen_clause
                | simdlen_clause
                | aligned_clause
                | linear_clause
                ;

simdlen_clause: SIMDLEN {
                          // ompattribute->addClause(e_simdlen);
                          // omptype = e_simdlen;
                          } '(' expression ')' {
                      } 
                  ;

declare_simd_directive: OMP DECLARE SIMD {
                        // ompattribute = buildOmpAttribute(e_declare_simd, gNode,true);
                        // cur_omp_directive = e_declare_simd;
                     }
                     declare_simd_clause_optseq
                     ;

declare_simd_clause_optseq : /* empty*/
                        | declare_simd_clause_seq
                        ;

declare_simd_clause_seq
                : declare_simd_clause
                | declare_simd_clause_seq declare_simd_clause
                | declare_simd_clause_seq ',' declare_simd_clause
                ; 

declare_simd_clause     : simdlen_clause
                | linear_clause
                | aligned_clause
                | uniform_clause
                | INBRANCH { 
// ompattribute->addClause(e_inbranch); omptype = e_inbranch; /*TODO: this is temporary, to be moved to declare simd */
}
                | NOTINBRANCH { 
// ompattribute->addClause(e_notinbranch); omptype = e_notinbranch; /*TODO: this is temporary, to be moved to declare simd */ 
}
              ;

uniform_clause : UNIFORM { 
                         // ompattribute->addClause(e_uniform);
                         // omptype = e_uniform; 
                       }
                       '(' {b_within_variable_list = true;} variable_list ')' {b_within_variable_list =false;}
                ;

aligned_clause : ALIGNED { 
                         // ompattribute->addClause(e_aligned);
                         // omptype = e_aligned; 
                       }
                       '(' {b_within_variable_list = true;} variable_list {b_within_variable_list =false;} aligned_clause_optseq ')'
               ;
aligned_clause_optseq: /* empty */
                        | aligned_clause_alignment
                        ;

aligned_clause_alignment: ':' expression { } 


linear_clause :  LINEAR { 
                         // ompattribute->addClause(e_linear);
                         // omptype = e_linear; 
                        }
                       '(' {b_within_variable_list = true;} variable_list {b_within_variable_list =false;}  linear_clause_step_optseq ')'
                ;

linear_clause_step_optseq: /* empty */
                        | linear_clause_step
                        ;

linear_clause_step: ':' expression { }

expression : RAW_STRING

/*  in C
variable-list : identifier
              | variable-list , identifier 
*/

/* in C++ (we use the C++ version) */ 
variable_list : ID_EXPRESSION {  }
              | variable_list ',' ID_EXPRESSION {  }
              ;

/* map (array[lower:length][lower:length])  , not array references, but array section notations */
map_variable_list : id_expression_opt_dimension
              | map_variable_list ',' id_expression_opt_dimension
              ;
/* mapped variables may have optional dimension information */
id_expression_opt_dimension: ID_EXPRESSION {  } dimension_field_optseq
                           ;

/* Parse optional dimension information associated with map(a[0:n][0:m]) Liao 1/22/2013 */
dimension_field_optseq: /* empty */
                      | dimension_field_seq
                      ;
/* sequence of dimension fields */
dimension_field_seq : dimension_field
                    | dimension_field_seq dimension_field
                    ;

dimension_field: '[' expression { /* lower_exp = current_exp; */} 
                 ':' expression { /* length_exp = current_exp; */
                      // assert (array_symbol != NULL);
                      // SgType* t = array_symbol->get_type();
                      // bool isPointer= (isSgPointerType(t) != NULL );
                      // bool isArray= (isSgArrayType(t) != NULL);
                      // if (!isPointer && ! isArray )
                      {
                        std::cerr<<"Error. ompparser.yy expects a pointer or array type."<<std::endl;
                        // std::cerr<<"while seeing "<<t->class_name()<<std::endl;
                      }
                      // ompattribute->array_dimensions[array_symbol].push_back( std::make_pair (lower_exp, length_exp));
                      } 
                  ']'
               ;
/* commenting out experimental stuff
Optional data distribution clause: dist_data(dim1_policy, dim2_policy, dim3_policy)
mixed keyword or variable parsing is tricky TODO 
one or more dimensions, each has a policy
reset current_exp to avoid leaving stale values
Optional (exp) for some policy                   
id_expression_opt_dimension: ID_EXPRESSION { if (!addVar((const char*)$1)) YYABORT; } dimension_field_optseq id_expression_opt_dist_data
                           ;
id_expression_opt_dist_data: empty 
                           | DIST_DATA '(' dist_policy_seq ')'
                           ;
dist_policy_seq: dist_policy_per_dim
               | dist_policy_seq ',' dist_policy_per_dim
               ;
dist_policy_per_dim: DUPLICATE  { ompattribute->appendDistDataPolicy(array_symbol, e_duplicate, NULL); }
                   | BLOCK dist_size_opt { ompattribute->appendDistDataPolicy(array_symbol, e_block, current_exp );  current_exp = NULL;}
                   | CYCLIC dist_size_opt { ompattribute->appendDistDataPolicy(array_symbol, e_cyclic, current_exp ); current_exp = NULL;}
                   ;
dist_size_opt: empty {current_exp = NULL;}
             | '(' expression ')'
             ;
*/

%%
int yyerror(const char *s) {
    // SgLocatedNode* lnode = isSgLocatedNode(gNode);
    // assert (lnode);
    // printf("Error when parsing pragma:\n\t %s \n\t associated with node at line %d\n", orig_str, lnode->get_file_info()->get_line()); 
    printf(" %s!\n", s);
    assert(0);
    return 0; // we want to the program to stop on error
}

/* OmpAttribute* getParsedDirective() {
    return ompattribute;
}
*/

// Standalone ompparser
OpenMPDirective* parseOpenMP(const char* input) {
    
    printf("Start parsing...\n");
    
    start_lexer(input);
    int res = yyparse();
    end_lexer();
    
    return directive;
}

static void parseParameter (const char* input) {
    
    // later create a new function to handle special case.
    /*
    if (strcmp(input, "shared") == 0) {
        addClause("shared", curClause);
        return NULL;
    }
    else if (strcmp(input, "none") == 0) {
        addClause("none", curClause);
        return NULL;
    }
    */

    printf("Start splitting raw strings...\n");

    std::string CurrentString(input);
    std::cout << CurrentString << "\n";
    std::string* clip = new std::string("");
    int counter = 0;
    for (int i = 0; i < CurrentString.size(); i++) {
        if (CurrentString[i] == '(') {
            clip->append(1, CurrentString[i]);
            counter++;
        }
        else if (CurrentString[i] == ')') {
            clip->append(1, CurrentString[i]);
            counter--;
        }
        else if (CurrentString[i] == ',' && counter == 0) {
            clause->addLangExpr((const char*)clip->c_str());
            clip = new std::string("");
        }
        else {
            if (CurrentString[i] != ' ' || counter != 0) {
                clip->append(1, CurrentString[i]);
            }
        }
    };
    if (clip->size() != 0) {
        clause->addLangExpr((const char*)clip->c_str());
    };
}

static void parseExpression(const char* input) {

    clause->addLangExpr(input);
}



