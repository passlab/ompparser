/*
 * Copyright (c) 2018-2020, High Performance Computing Architecture and System
 * research laboratory at University of North Carolina at Charlotte (HPCAS@UNCC)
 * and Lawrence Livermore National Security, LLC.
 *
 * SPDX-License-Identifier: (BSD-3-Clause)
 */

/* OpenMP C/C++/Fortran Grammar */

%define api.prefix {openmp_}
%defines
%define parse.error verbose

%{
/* DQ (2/10/2014): IF is conflicting with Boost template IF. */
#undef IF

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "OpenMPIR.h"
#include <string.h>
#include <regex>
#include <assert.h>

/*the scanner function*/
extern int openmp_lex(); 

/*A customized initialization function for the scanner, str is the string to be scanned.*/
extern void openmp_lexer_init(const char* str);

/* Standalone ompparser */
extern void start_lexer(const char* input);
extern void end_lexer(void);

//The directive/clause that are being parsed
static OpenMPDirective* current_directive = NULL;
static OpenMPClause* current_clause = NULL;
static OpenMPDirective* current_parent_directive = NULL;
static OpenMPClause* current_parent_clause = NULL;
static int firstParameter = 0;
static int secondParameter = 0;
static int thirdParameter = 0;
static OpenMPUsesAllocatorsClauseAllocator usesAllocator;
static const char* firstStringParameter = "";
static const char* secondStringParameter = "";
static std::vector<const char*>* iterator_definition = new std::vector<const char*>();
static std::vector<const char*>* depend_iterator_definition = new std::vector<const char*>();
static std::vector<std::vector<const char*>* >* depend_iterators_definition_class;
//std::vector<std::vector<const char*>* >* depend_iterators_definition_class;
static const char* trait_score = "";
/* Treat the entire expression as a string for now */
extern void openmp_parse_expr();
static int openmp_error(const char*);
void * (*exprParse)(const char*) = NULL;

bool b_within_variable_list  = false;  // a flag to indicate if the program is now processing a list of variables

/* used for language setting and detecting*/
OpenMPBaseLang user_set_lang = Lang_unknown;
OpenMPBaseLang auto_lang;
void setLang(OpenMPBaseLang _lang) { user_set_lang = _lang; };

%}

%locations

/* The %union declaration specifies the entire collection of possible data types for semantic values.
these names are used in the %token and %type declarations to pick one of the types for a terminal or nonterminal symbol
corresponding C type is union name defaults to YYSTYPE.
*/

%union {  int itype;
          double ftype;
          const char* stype;
          void* ptype; /* For expressions or variables */
        }


%token  OMP PARALLEL FOR DO DECLARE DISTRIBUTE LOOP SCAN SECTIONS SECTION SINGLE CANCEL TASKGROUP CANCELLATION POINT THREAD VARIANT THREADPRIVATE METADIRECTIVE MAPPER
        IF NUM_THREADS DEFAULT PRIVATE FIRSTPRIVATE SHARED COPYIN REDUCTION PROC_BIND ALLOCATE SIMD TASK LASTPRIVATE WHEN MATCH PARTIAL FULL
        LINEAR SCHEDULE COLLAPSE NOWAIT ORDER ORDERED MODIFIER_CONDITIONAL MODIFIER_MONOTONIC MODIFIER_NONMONOTONIC STATIC DYNAMIC GUIDED AUTO RUNTIME MODOFIER_VAL MODOFIER_REF MODOFIER_UVAL MODIFIER_SIMD
        SAFELEN SIMDLEN ALIGNED ALIGN NONTEMPORAL UNIFORM INBRANCH NOTINBRANCH DIST_SCHEDULE BIND INCLUSIVE EXCLUSIVE COPYPRIVATE ALLOCATOR INITIALIZER OMP_PRIV IDENTIFIER_DEFAULT WORKSHARE/*YAYING*/
        NONE MASTER CLOSE SPREAD MODIFIER_INSCAN MODIFIER_TASK MODIFIER_DEFAULT 
        PLUS MINUS STAR BITAND BITOR BITXOR LOGAND LOGOR EQV NEQV MAX MIN
        DEFAULT_MEM_ALLOC LARGE_CAP_MEM_ALLOC CONST_MEM_ALLOC HIGH_BW_MEM_ALLOC LOW_LAT_MEM_ALLOC CGROUP_MEM_ALLOC
        PTEAM_MEM_ALLOC THREAD_MEM_ALLOC
        TEAMS
        NUM_TEAMS THREAD_LIMIT DOUBLE_COLON
        END USER CONSTRUCT DEVICE IMPLEMENTATION CONDITION SCORE VENDOR
        KIND HOST NOHOST ANY CPU GPU FPGA ISA ARCH EXTENSION
        AMD ARM BSC CRAY FUJITSU GNU IBM INTEL LLVM PGI TI UNKNOWN
        FINAL UNTIED MERGEABLE IN_REDUCTION DEPEND PRIORITY AFFINITY DETACH MODIFIER_ITERATOR DEPOBJ FINAL_CLAUSE IN INOUT MUTEXINOUTSET OUT
        TASKLOOP GRAINSIZE NUM_TASKS NOGROUP TASKYIELD REQUIRES REVERSE_OFFLOAD UNIFIED_ADDRESS UNIFIED_SHARED_MEMORY ATOMIC_DEFAULT_MEM_ORDER DYNAMIC_ALLOCATORS SEQ_CST ACQ_REL RELAXED UNROLL TILE
        USE_DEVICE_PTR USE_DEVICE_ADDR TARGET DATA ENTER EXIT ANCESTOR DEVICE_NUM IS_DEVICE_PTR HAS_DEVICE_ADDR SIZES
        DEFAULTMAP BEHAVIOR_ALLOC BEHAVIOR_TO BEHAVIOR_FROM BEHAVIOR_TOFROM BEHAVIOR_FIRSTPRIVATE BEHAVIOR_NONE BEHAVIOR_DEFAULT CATEGORY_SCALAR CATEGORY_AGGREGATE CATEGORY_POINTER CATEGORY_ALLOCATABLE UPDATE TO FROM TO_MAPPER FROM_MAPPER USES_ALLOCATORS
 LINK DEVICE_TYPE MAP MAP_MODIFIER_ALWAYS MAP_MODIFIER_CLOSE MAP_MODIFIER_MAPPER MAP_TYPE_TO MAP_TYPE_FROM MAP_TYPE_TOFROM MAP_TYPE_ALLOC MAP_TYPE_RELEASE MAP_TYPE_DELETE EXT_ BARRIER TASKWAIT FLUSH RELEASE ACQUIRE ATOMIC READ WRITE CAPTURE HINT CRITICAL SOURCE SINK DESTROY THREADS
        CONCURRENT
        LESSOREQUAL MOREOREQUAL NOTEQUAL
%token <itype> ICONSTANT
%token <stype> EXPRESSION ID_EXPRESSION EXPR_STRING VAR_STRING TASK_REDUCTION
/* associativity and precedence */
%left '<' '>' '='
%left '+' '-'
%left '*' '/' '%'

%type <stype> expression

/* start point for the parsing */
%start openmp_directive

%%

/* lang-dependent expression is only used in clause, at this point, the current_clause object should already be created. */
expression : EXPR_STRING { current_clause->addLangExpr($1); /*void * astnode = exprParse)($1);*/ }
variable :   EXPR_STRING { current_clause->addLangExpr($1); } /* we use expression for variable so far */

/*expr_list : expression
        | expr_list ',' expression
        ;
*/
var_list : variable
        | var_list ',' variable
        ;

openmp_directive : parallel_directive
                 | metadirective_directive
                 | declare_variant_directive
                 | for_directive
                 | do_directive
                 | simd_directive
                 | teams_directive
                 | for_simd_directive
                 | do_simd_directive
                 | parallel_for_simd_directive
                 | parallel_do_simd_directive
                 | declare_simd_directive
                 | declare_simd_fortran_directive
                 | distribute_directive
                 | distribute_simd_directive
                 | distribute_parallel_for_directive
                 | distribute_parallel_do_directive
                 | distribute_parallel_for_simd_directive
                 | distribute_parallel_do_simd_directive
                 | parallel_for_directive
                 | parallel_do_directive
                 | parallel_loop_directive
                 | parallel_sections_directive
                 | parallel_workshare_directive
                 | parallel_master_directive
                 | master_taskloop_directive
                 | master_taskloop_simd_directive
                 | parallel_master_taskloop_directive
                 | parallel_master_taskloop_simd_directive
                 | loop_directive
                 | scan_directive
                 | sections_directive
                 | section_directive
                 | single_directive
                 | workshare_directive
                 | cancel_directive
//                 | cancel_fortran_directive
                 | cancellation_point_directive
//                 | cancellation_point_fortran_directive
                 | allocate_directive
                 | task_directive
                 | taskloop_directive
                 | taskloop_simd_directive
                 | taskyield_directive
                 | requires_directive
                 | target_data_directive
                 | target_enter_data_directive
                 | target_exit_data_directive
                 | target_directive
                 | target_update_directive
                 | declare_target_directive
                 | end_declare_target_directive
                 | master_directive
                 | threadprivate_directive
                 | declare_reduction_directive
                 | declare_mapper_directive
                 | end_directive
                 | barrier_directive
                 | taskwait_directive
                 | taskgroup_directive
                 | flush_directive
                 | atomic_directive
                 | critical_directive
                 | depobj_directive
                 | ordered_directive
                 | teams_distribute_directive
                 | teams_distribute_simd_directive
                 | teams_distribute_parallel_for_directive
                 | teams_distribute_parallel_for_simd_directive
                 | teams_loop_directive
                 | target_parallel_directive
                 | target_parallel_for_directive
                 | target_parallel_for_simd_directive
                 | target_parallel_loop_directive
                 | target_simd_directive
                 | target_teams_directive
                 | target_teams_distribute_directive
                 | target_teams_distribute_simd_directive
                 | target_teams_loop_directive
                 | target_teams_distribute_parallel_for_directive
                 | target_teams_distribute_parallel_for_simd_directive
                 | teams_distribute_parallel_do_directive
                 | teams_distribute_parallel_do_simd_directive
                 | target_parallel_do_directive
                 | target_parallel_do_simd_directive
                 | target_teams_distribute_parallel_do_directive
                 | target_teams_distribute_parallel_do_simd_directive
                 | unroll_directive
                 | tile_directive
                 ;

variant_directive : parallel_directive
                  | metadirective_directive
                  | declare_variant_directive
                  | for_directive
                  | simd_directive
                  | teams_directive
                  | for_simd_directive
                  | declare_simd_directive
                  | distribute_directive
                  | distribute_simd_directive
                  | distribute_parallel_for_directive
                  | distribute_parallel_for_simd_directive
                  | loop_directive
                  | scan_directive
                  | sections_directive
                  | section_directive
                  | single_directive
                  | cancel_directive
                  | cancellation_point_directive
                  | allocate_directive
                  ;

fortran_paired_directive : parallel_directive
                         | do_paired_directive
                         | metadirective_directive
                         | master_directive
                         | teams_directive
                         | section_directive
                         | sections_paired_directive
                         | simd_directive
                         | do_simd_paired_directive
                         | distribute_directive
                         | distribute_simd_directive
                         | distribute_parallel_do_directive
                         | distribute_parallel_do_simd_directive
                         | parallel_do_directive
                         | parallel_loop_directive
                         | parallel_workshare_directive
                         | parallel_do_simd_directive
                         | parallel_master_directive
                         | master_taskloop_directive
                         | master_taskloop_simd_directive
                         | parallel_master_taskloop_directive
                         | parallel_master_taskloop_simd_directive
                         | loop_directive
                         | single_paired_directive
                         | workshare_paired_directive
                         | task_directive
                         | taskloop_directive
                         | taskloop_simd_directive
                         | target_directive
                         | target_data_directive
                         | critical_directive
                         | taskgroup_directive
                         | atomic_directive
                         | ordered_directive
                         | teams_distribute_directive
                         | teams_distribute_simd_directive
                         | teams_distribute_parallel_do_directive
                         | teams_distribute_parallel_do_simd_directive
                         | teams_loop_directive
                         | target_parallel_directive
                         | target_parallel_do_directive
                         | target_parallel_do_simd_directive
                         | target_parallel_loop_directive
                         | target_simd_directive
                         | target_teams_directive
                         | target_teams_distribute_directive
                         | target_teams_distribute_simd_directive
                         | target_teams_loop_directive
                         | target_teams_distribute_parallel_do_directive
                         | target_teams_distribute_parallel_do_simd_directive
                         | unroll_directive
                         | tile_directive
                         ;

end_directive : END { current_directive = new OpenMPEndDirective();
                current_parent_directive = current_directive;
                current_parent_clause = current_clause;
              } end_clause_seq {
                ((OpenMPEndDirective*)current_parent_directive)->setPairedDirective(current_directive);
                current_directive = current_parent_directive;
                current_clause = current_parent_clause;
                current_parent_directive = NULL;
                current_parent_clause = NULL;
              }
              ;

end_clause_seq : fortran_paired_directive
               ;

metadirective_directive : METADIRECTIVE { current_directive = new OpenMPDirective(OMPD_metadirective); }
                          metadirective_clause_optseq
                        ;

metadirective_clause_optseq : /* empty */
                            | metadirective_clause_seq
                            ;

metadirective_clause_seq : metadirective_clause
                         | metadirective_clause_seq metadirective_clause
                         | metadirective_clause_seq ',' metadirective_clause
                         ;

metadirective_clause : when_clause
                     | default_variant_clause
                     ;

when_clause : WHEN { current_clause = current_directive->addOpenMPClause(OMPC_when); }
                '(' context_selector_specification ':' {
                current_parent_directive = current_directive;
                current_parent_clause = current_clause;
                } when_variant_directive {
                current_directive = current_parent_directive;
                current_clause = current_parent_clause;
                current_parent_directive = NULL;
                current_parent_clause = NULL;
                } ')'
            ;

when_variant_directive : variant_directive {((OpenMPWhenClause*)current_parent_clause)->setVariantDirective(current_directive); }
                | { ; }
                ;

context_selector_specification : trait_set_selector
                | context_selector_specification trait_set_selector
                | context_selector_specification ',' trait_set_selector
                ;

trait_set_selector : trait_set_selector_name { } '=' '{' trait_selector_list {
                        if (current_parent_clause) {
                            current_directive = current_parent_directive;
                            current_clause = current_parent_clause;
                            current_parent_directive = NULL;
                            current_parent_clause = NULL;
                        };
                     } '}'
                   ;

trait_set_selector_name : USER { }
                | CONSTRUCT { current_parent_directive = current_directive;
                    current_parent_clause = current_clause; }
                | DEVICE { }
                | IMPLEMENTATION { }
                ;

trait_selector_list : trait_selector { trait_score = ""; }
                | trait_selector_list trait_selector { trait_score = ""; }
                | trait_selector_list ',' trait_selector { trait_score = ""; }
                ;

trait_selector : condition_selector
                | construct_selector {
                    ((OpenMPVariantClause*)current_parent_clause)->addConstructDirective(trait_score, current_directive);
                    std::cout << "A construct directive has been added to WHEN clause.\n"; 
                }
                | device_selector
                | implementation_selector
                ;

condition_selector : CONDITION '(' trait_score EXPR_STRING { ((OpenMPVariantClause*)current_clause)->setUserCondition(trait_score, $4); } ')'
                ;

device_selector : context_kind
                | context_isa
                | context_arch
                ;

context_kind : KIND '(' trait_score context_kind_name ')'
             ;

context_kind_name : HOST { ((OpenMPVariantClause*)current_clause)->setContextKind(trait_score, OMPC_CONTEXT_KIND_host); }
                  | NOHOST { ((OpenMPVariantClause*)current_clause)->setContextKind(trait_score, OMPC_CONTEXT_KIND_nohost); }
                  | ANY { ((OpenMPVariantClause*)current_clause)->setContextKind(trait_score, OMPC_CONTEXT_KIND_any); }
                  | CPU { ((OpenMPVariantClause*)current_clause)->setContextKind(trait_score, OMPC_CONTEXT_KIND_cpu); }
                  | GPU { ((OpenMPVariantClause*)current_clause)->setContextKind(trait_score, OMPC_CONTEXT_KIND_gpu); }
                  | FPGA { ((OpenMPVariantClause*)current_clause)->setContextKind(trait_score, OMPC_CONTEXT_KIND_fpga); }
                  ;

context_isa : ISA '(' trait_score EXPR_STRING { ((OpenMPVariantClause*)current_clause)->setIsaExpression(trait_score, $4); } ')'
            ;

context_arch : ARCH '(' trait_score EXPR_STRING { ((OpenMPVariantClause*)current_clause)->setArchExpression(trait_score, $4); } ')'
             ;

implementation_selector : VENDOR '(' trait_score context_vendor_name ')'
                        | EXTENSION '(' trait_score EXPR_STRING { ((OpenMPVariantClause*)current_clause)->setExtensionExpression(trait_score, $4); } ')'
                        | EXPR_STRING { ((OpenMPVariantClause*)current_clause)->setImplementationExpression(trait_score, $1); }
                        | EXPR_STRING '(' trait_score ')' { ((OpenMPVariantClause*)current_clause)->setImplementationExpression(trait_score, $1); }
                        ;

context_vendor_name : AMD { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_amd); }
                    | ARM { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_arm); }
                    | BSC { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_bsc); }
                    | CRAY { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_cray); }
                    | FUJITSU { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_fujitsu); }
                    | GNU { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_gnu); }
                    | IBM { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_ibm); }
                    | INTEL { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_intel); }
                    | LLVM { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_llvm); }
                    | PGI { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_pgi); }
                    | TI { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_ti); }
                    | UNKNOWN { ((OpenMPVariantClause*)current_clause)->setImplementationKind(trait_score, OMPC_CONTEXT_VENDOR_unknown); }
                    ;

construct_selector : parallel_selector
                   ;

parallel_selector : PARALLEL { current_directive = new OpenMPDirective(OMPD_parallel); }
                | PARALLEL '(' { current_directive = new OpenMPDirective(OMPD_parallel); } parallel_selector_parameter ')'
                ;

parallel_selector_parameter : trait_score parallel_clause_optseq
                            ;

trait_score : /* empty */
            | SCORE '(' EXPR_STRING { trait_score = $3; } ')' ':'
            ;

declare_variant_directive : DECLARE VARIANT {
                        current_directive = new OpenMPDeclareVariantDirective();
                     } variant_func_id
                     declare_variant_clause_optseq
                   ;

variant_func_id : '(' EXPR_STRING { ((OpenMPDeclareVariantDirective*)current_directive)->setVariantFuncID($2); } ')'
                ;

declare_variant_clause_optseq : /* empty */
                       | declare_variant_clause_seq
                       ;

declare_variant_clause_seq : declare_variant_clause
                    | declare_variant_clause_seq declare_variant_clause
                    | declare_variant_clause_seq ',' declare_variant_clause
                    ;

declare_variant_clause : match_clause
                ;

match_clause : MATCH { current_clause = current_directive->addOpenMPClause(OMPC_match); }
                '(' context_selector_specification ')' { }
             ;


parallel_directive : PARALLEL {
                        current_directive = new OpenMPDirective(OMPD_parallel);
                     }
                     parallel_clause_optseq
                   ;
/*xinyao*/
task_directive : TASK {
                        current_directive = new OpenMPDirective(OMPD_task);
                      }
                      task_clause_optseq
               ;
taskloop_directive : TASKLOOP {
                        current_directive = new OpenMPDirective(OMPD_taskloop);
                              }
                     taskloop_clause_optseq
                   ;
taskloop_simd_directive : TASKLOOP SIMD {
                        current_directive = new OpenMPDirective(OMPD_taskloop_simd);
                                         }
                     taskloop_simd_clause_optseq 
                        ;
taskyield_directive : TASKYIELD {
                        current_directive = new OpenMPDirective(OMPD_taskyield);
                     }
                    ;
requires_directive : REQUIRES {
                        current_directive = new OpenMPRequiresDirective();
                     }
                     requires_clause_optseq
                   ;
target_data_directive :  TARGET DATA {
                        current_directive = new OpenMPDirective(OMPD_target_data);
                     }
                     target_data_clause_optseq 
                      ;
target_enter_data_directive :  TARGET ENTER DATA {
                        current_directive = new OpenMPDirective(OMPD_target_enter_data);
                     }
                     target_enter_data_clause_optseq 
                            ;
target_exit_data_directive :  TARGET EXIT DATA {
                        current_directive = new OpenMPDirective(OMPD_target_exit_data);
                     }
                     target_exit_data_clause_optseq 
                   ;
target_directive :  TARGET {
                        current_directive = new OpenMPDirective(OMPD_target);
                     }
                     target_clause_optseq 
                   ;
target_update_directive :  TARGET UPDATE{
                        current_directive = new OpenMPDirective(OMPD_target_update);
                     }
                     target_update_clause_optseq 
                   ;
declare_target_directive : DECLARE TARGET {
                        current_directive = new OpenMPDeclareTargetDirective ();
                     }
                     declare_target_clause_optseq 
                   ;
flush_directive : FLUSH {
                        current_directive = new OpenMPFlushDirective ();
                        }
                     flush_clause_optseq 
                ;

end_declare_target_directive : END DECLARE TARGET {
                        current_directive = new OpenMPDirective(OMPD_end_declare_target);
                                  }
                             ;
master_directive : MASTER {
                        current_directive = new OpenMPDirective(OMPD_master);
                     }
                   ;
barrier_directive : BARRIER {
                        current_directive = new OpenMPDirective(OMPD_barrier);
                             }
                  ;
taskwait_directive : TASKWAIT {
                        current_directive = new OpenMPDirective(OMPD_taskwait);
                     }
                      taskwait_clause_optseq
                   ;
unroll_directive : UNROLL {
                        current_directive = new OpenMPDirective(OMPD_unroll);
                     }
                      unroll_clause_optseq
                   ;
tile_directive : TILE {
                        current_directive = new OpenMPDirective(OMPD_tile);
                     }
                      tile_clause_optseq
                   ;
taskgroup_directive : TASKGROUP {
                        current_directive = new OpenMPDirective(OMPD_taskgroup);
                     }
                      taskgroup_clause_optseq
                    ;
critical_directive : CRITICAL {
                        current_directive = new OpenMPCriticalDirective();
                     }
                      critical_clause_optseq
                   ;
depobj_directive : DEPOBJ {
                        current_directive = new OpenMPDepobjDirective ();
                     }
                     depobj_clause_optseq 
                 ;
ordered_directive : ORDERED {
                        current_directive = new OpenMPOrderedDirective ();
                     }
                     ordered_clause_optseq 
                  ;
critical_clause_optseq : /*empty*/
                       | '(' critical_name')'
                       | '(' critical_name')' hint_clause
                       | '(' critical_name')' ',' hint_clause
                       ;
depobj_clause_optseq : '(' depobj ')' depobj_clause
                     ;
depobj : EXPR_STRING { ((OpenMPDepobjDirective*)current_directive)->addDepobj($1); } 
       ;

depobj_clause : depend_depobj_clause
              | destroy_clause
              | depobj_update_clause
              ;
destroy_clause : DESTROY{current_clause = current_directive->addOpenMPClause(OMPC_destroy); }
               ;

depobj_update_clause : UPDATE '(' update_dependence_type ')'
                     ;
update_dependence_type : SOURCE { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update, OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_source); }
                       | IN { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update, OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_in); }
                       | OUT { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update, OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_out); }
                       | INOUT { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update, OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_inout); }
                       | MUTEXINOUTSET { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update,OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_mutexinoutset); }
                       | DEPOBJ { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update, OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_depobj); }
                       | SINK { current_clause = current_directive->addOpenMPClause(OMPC_depobj_update, OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_sink); }
                       ;

critical_name : EXPR_STRING { ((OpenMPCriticalDirective*)current_directive)->setCriticalName($1); }
              ;
task_clause_optseq : /* empty */
                   | task_clause_seq
                   ;
taskloop_clause_optseq : /* empty */
                       | taskloop_clause_seq
                       ;
taskloop_simd_clause_optseq : /* empty */
                            | taskloop_simd_clause_seq
                            ;
requires_clause_optseq : requires_clause_seq
                       ;
target_data_clause_optseq :target_data_clause_seq
                          ;
target_enter_data_clause_optseq :/* empty */
                                |target_enter_data_clause_seq
                                ;
target_exit_data_clause_optseq :/* empty */
                               |target_exit_data_clause_seq
                               ;
target_clause_optseq :/* empty */
                     |target_clause_seq
                     ;
target_update_clause_optseq :target_update_clause_seq
                            ;
declare_target_clause_optseq : /* empty */
                             | '(' declare_target_extended_list ')'
                             | declare_target_seq
                             ;

extended_variable : EXPR_STRING { ((OpenMPDeclareTargetDirective*)current_directive)->addExtendedList($1); }
                  ;
declare_target_extended_list : extended_variable
                             | declare_target_extended_list ',' extended_variable
                             ;
flush_clause_optseq : /* empty */
                    | '(' flush_list ')'
                    | flush_clause_seq
                    ;
flush_list : flush_variable
           | flush_list ',' flush_variable
           ;
flush_variable : EXPR_STRING { ((OpenMPFlushDirective*)current_directive)->addFlushList($1); }
               ;
flush_clause_seq : flush_memory_order_clause
                 | flush_memory_order_clause '(' flush_list ')'
                 ;
flush_memory_order_clause : acq_rel_clause
                          | release_clause
                          | acquire_clause
                          ;

atomic_directive : ATOMIC {
                        current_directive = new OpenMPAtomicDirective ();
                    }
                    atomic_clause_optseq 
                 ;
atomic_clause_optseq : memory_order_clause_seq
                     | memory_order_clause_seq atomic_clause_seq
                     | hint_clause ',' memory_order_clause ',' atomic_clause_seq
                     | memory_order_clause ',' hint_clause ',' atomic_clause_seq
                     | memory_order_clause ','atomic_clause_seq
                     | hint_clause ',' memory_order_clause atomic_clause_seq
                     | memory_order_clause ',' hint_clause atomic_clause_seq
                     | hint_clause ','atomic_clause_seq
                     ;

atomic_clause_seq : atomic_clause memory_order_clause_seq_after
                  | atomic_clause ',' memory_order_clause_seq_after
                  ;

memory_order_clause_seq : 
                        | memory_order_clause hint_clause
                        | hint_clause memory_order_clause
                        | memory_order_clause
                        | hint_clause
                        ;
memory_order_clause_seq_after : 
                              | memory_order_clause hint_clause
                              | hint_clause memory_order_clause
                              | memory_order_clause ',' hint_clause
                              | hint_clause ',' memory_order_clause
                              | memory_order_clause
                              | hint_clause
                              ;
atomic_clause : read_clause
              | write_clause
              | update_clause
              | capture_clause
              ;

memory_order_clause : seq_cst_clause
                    | acq_rel_clause
                    | release_clause
                    | acquire_clause
                    | relaxed_clause
                    ; 

hint_clause : HINT{ current_clause = current_directive->addOpenMPClause(OMPC_hint);
                     } '(' expression ')' 
            ;
read_clause : READ { current_clause = current_directive->addOpenMPClause(OMPC_read);
                   } 
            ;
write_clause : WRITE { current_clause = current_directive->addOpenMPClause(OMPC_write);
                     } 
             ;
update_clause : UPDATE { current_clause = current_directive->addOpenMPClause(OMPC_update);
                       } 
              ;
capture_clause : CAPTURE { current_clause = current_directive->addOpenMPClause(OMPC_capture);
                         } 
               ;

seq_cst_clause : SEQ_CST { current_clause = current_directive->addOpenMPClause(OMPC_seq_cst); }
               ;
acq_rel_clause : ACQ_REL { current_clause = current_directive->addOpenMPClause(OMPC_acq_rel); }
               ;
release_clause : RELEASE { current_clause = current_directive->addOpenMPClause(OMPC_release); }
               ;
acquire_clause : ACQUIRE { current_clause = current_directive->addOpenMPClause(OMPC_acquire); }
               ;
relaxed_clause : RELAXED { current_clause = current_directive->addOpenMPClause(OMPC_relaxed); }
               ;

taskwait_clause_optseq : /* empty */
                       | taskwait_clause_seq
                       ;
unroll_clause_optseq : /* empty */
                     | unroll_clause_seq
                     ;
tile_clause_optseq : tile_clause_seq
                   ;
taskgroup_clause_optseq : /* empty */
                        | taskgroup_clause_seq
                        ;

task_clause_seq : task_clause
                | task_clause_seq task_clause
                | task_clause_seq ',' task_clause
                ;
taskloop_clause_seq : taskloop_clause
                    | taskloop_clause_seq taskloop_clause
                    | taskloop_clause_seq ',' taskloop_clause
                    ;
taskloop_simd_clause_seq : taskloop_simd_clause
                         | taskloop_simd_clause_seq taskloop_simd_clause
                         | taskloop_simd_clause_seq ',' taskloop_simd_clause
                         ;
requires_clause_seq : requires_clause
                    | requires_clause_seq requires_clause
                    | requires_clause_seq ',' requires_clause
                    ;

target_data_clause_seq : target_data_clause
                       | target_data_clause_seq target_data_clause
                       | target_data_clause_seq ',' target_data_clause
                       ;
target_enter_data_clause_seq : target_enter_data_clause
                             | target_enter_data_clause_seq target_enter_data_clause
                             | target_enter_data_clause_seq ',' target_enter_data_clause
                             ;
target_exit_data_clause_seq : target_exit_data_clause
                            | target_exit_data_clause_seq target_exit_data_clause
                            | target_exit_data_clause_seq ',' target_exit_data_clause
                            ;
target_clause_seq : target_clause
                  | target_clause_seq target_clause
                  | target_clause_seq ',' target_clause
                  ;
target_update_clause_seq : target_update_clause
                         | target_update_clause_seq target_update_clause
                         | target_update_clause_seq ',' target_update_clause
                         ;
declare_target_seq : declare_target_clause
                   | declare_target_seq declare_target_clause
                   | declare_target_seq ',' declare_target_clause
                   ;
taskwait_clause_seq : taskwait_clause
                    | taskwait_clause_seq taskwait_clause
                    | taskwait_clause_seq ',' taskwait_clause
                    ;
unroll_clause_seq : unroll_clause
                  ;
tile_clause_seq : tile_clause
                ;
taskgroup_clause_seq : taskgroup_clause
                     | taskgroup_clause_seq taskgroup_clause
                     | taskgroup_clause_seq ',' taskgroup_clause
                     ;

task_clause : if_task_clause
            | final_clause
            | untied_clause
            | default_clause
            | mergeable_clause
            | private_clause
            | firstprivate_clause
            | shared_clause
            | in_reduction_clause
            | depend_with_modifier_clause
            | priority_clause
            | allocate_clause
            | affinity_clause
            | detach_clause
            ;
taskloop_clause : if_taskloop_clause
                | shared_clause
                | private_clause
                | firstprivate_clause
                | lastprivate_clause
                | reduction_default_only_clause
                | in_reduction_clause
                | default_clause
                | grainsize_clause
                | num_tasks_clause
                | collapse_clause
                | final_clause
                | priority_clause
                | untied_clause
                | mergeable_clause
                | nogroup_clause
                | allocate_clause
                ;
taskloop_simd_clause : if_taskloop_simd_clause
                     | shared_clause
                     | private_clause
                     | firstprivate_clause
                     | lastprivate_clause
                     | reduction_clause 
                     | in_reduction_clause
                     | default_clause
                     | grainsize_clause
                     | num_tasks_clause
                     | collapse_clause
                     | final_clause
                     | priority_clause
                     | untied_clause
                     | mergeable_clause
                     | nogroup_clause
                     | allocate_clause               
                     | safelen_clause
                     | simdlen_clause
                     | linear_clause
                     | aligned_clause
                     | nontemporal_clause
                     | order_clause 
                     ;
requires_clause : reverse_offload_clause
                | unified_address_clause
                | unified_shared_memory_clause   
                | atomic_default_mem_order_clause 
                | dynamic_allocators_clause
                | ext_implementation_defined_requirement_clause       
                ;
target_data_clause : if_target_data_clause
                   | device_clause
                   | map_clause
                   | use_device_ptr_clause
                   | use_device_addr_clause
                   ;
target_enter_data_clause: if_target_enter_data_clause
                        | device_clause
                        | map_clause
                        | depend_with_modifier_clause
                        | nowait_clause
                        ;
target_exit_data_clause: if_target_exit_data_clause
                       | device_clause
                       | map_clause
                       | depend_with_modifier_clause
                       | nowait_clause
                       ;
target_clause: if_target_clause
             | device_clause
             | thread_limit_clause
             | private_clause
             | firstprivate_clause
             | in_reduction_clause
             | map_clause
             | is_device_ptr_clause
             | has_device_addr_clause
             | defaultmap_clause
             | nowait_clause
             | allocate_clause
             | depend_with_modifier_clause
             | uses_allocators_clause
             ;
target_update_clause: motion_clause
                    | target_update_other_clause
                    ;
motion_clause: to_clause
             | from_clause
             ;
target_update_other_clause: if_target_update_clause
                          | device_without_modifier_clause
                          | depend_with_modifier_clause
                          | nowait_clause
                          ;
declare_target_clause : to_clause
                      | link_clause
                      | device_type_clause
                      ;
taskwait_clause : depend_with_modifier_clause
                ;
unroll_clause : full_clause
              | partial_clause
              ;
tile_clause : sizes_clause
            ;
taskgroup_clause : task_reduction_clause
                 | allocate_clause
                 ;
final_clause: FINAL {
                            current_clause = current_directive->addOpenMPClause(OMPC_final);
                         } '(' expression ')'
            ;
untied_clause: UNTIED {
                            current_clause = current_directive->addOpenMPClause(OMPC_untied);
                         } 
             ;
mergeable_clause: MERGEABLE {
                            current_clause = current_directive->addOpenMPClause(OMPC_mergeable);
                         } 
                ;
in_reduction_clause : IN_REDUCTION '(' in_reduction_identifier ':' var_list ')' { }
                    ;
in_reduction_identifier : in_reduction_enum_identifier
                        | EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_user, $1); }
                        ;

in_reduction_enum_identifier :  '+'{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_plus); }
                             | '-'{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_minus); }
                             | '*'{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_mul); }
                             | '&'{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_bitand); }
                             | '|'{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_bitor); }
                             | '^'{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_bitxor); }
                             | LOGAND{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_logand); }
                             | LOGOR{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_logor); }
                             | MAX{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_max); }
                             | MIN{ current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_min); }
                             ;

depend_with_modifier_clause : DEPEND { firstParameter = OMPC_DEPEND_MODIFIER_unspecified; } '(' depend_parameter ':' var_list ')' { ((OpenMPDependClause*)current_clause)->mergeDepend(current_directive, current_clause); }
                            ;

depend_parameter : dependence_type
                 | depend_modifier ',' dependence_type { ((OpenMPDependClause*)current_clause)->setDependIteratorsDefinitionClass(depend_iterators_definition_class); depend_iterators_definition_class->clear(); }
                 ;
dependence_type : depend_enum_type 
                ;
depend_modifier : MODIFIER_ITERATOR { depend_iterators_definition_class = new std::vector<std::vector<const char *> *>(); firstParameter = OMPC_DEPEND_MODIFIER_iterator; } '('depend_iterators_definition ')'
                ;
depend_iterators_definition : depend_iterator_specifier
                            | depend_iterators_definition ',' depend_iterator_specifier
                            ;
depend_iterator_specifier : EXPR_STRING EXPR_STRING { depend_iterator_definition->push_back($1); depend_iterator_definition->push_back($2); } '=' depend_range_specification
                          | EXPR_STRING { depend_iterator_definition->push_back(""); depend_iterator_definition->push_back($1); } '=' depend_range_specification
                          ;
depend_range_specification : EXPR_STRING { depend_iterator_definition->push_back($1); } ':' EXPR_STRING { depend_iterator_definition->push_back($4); } depend_range_step { depend_iterator_definition = new std::vector<const char*>(); }
                           ;
depend_range_step : /*empty*/ { depend_iterator_definition->push_back(""); depend_iterators_definition_class->push_back(depend_iterator_definition); }
                  | ':' EXPR_STRING { depend_iterator_definition->push_back($2);depend_iterators_definition_class->push_back(depend_iterator_definition); }
                  ;
depend_enum_type : IN { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_in); }
                 | OUT { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_out); }
                 | INOUT { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_inout); }
                 | MUTEXINOUTSET { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_mutexinoutset); }
                 | DEPOBJ { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_depobj); }
                 ;

depend_depobj_clause : DEPEND { firstParameter = OMPC_DEPEND_MODIFIER_unspecified; }'(' dependence_depobj_parameter ')' {
}
                     ;
dependence_depobj_parameter : dependence_depobj_type ':' expression
                            ;
dependence_depobj_type : IN             { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_in, depend_iterators_definition_class); }
                       | OUT            { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_out, depend_iterators_definition_class); }
                       | INOUT          { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_inout, depend_iterators_definition_class); }
                       | MUTEXINOUTSET  { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_mutexinoutset); }
                       ;
depend_ordered_clause : DEPEND { firstParameter = OMPC_DEPEND_MODIFIER_unspecified; }'(' dependence_ordered_parameter ')' {
}
                      ;
dependence_ordered_parameter : dependence_ordered_type
                             ;
dependence_ordered_type :  SOURCE { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_source, depend_iterators_definition_class); }
                        | SINK { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_sink, depend_iterators_definition_class); } ':' var_list
                        ;

priority_clause: PRIORITY {
                            current_clause = current_directive->addOpenMPClause(OMPC_priority);
                         } '(' expression ')'
               ;

affinity_clause: AFFINITY '(' affinity_parameter ')' ;

affinity_parameter : EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_affinity, OMPC_AFFINITY_MODIFIER_unspecified); current_clause->addLangExpr($1); }
                   | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_affinity, OMPC_AFFINITY_MODIFIER_unspecified); current_clause->addLangExpr($1); } var_list
                   | affinity_modifier ':' var_list
                   ;

affinity_modifier : MODIFIER_ITERATOR { current_clause = current_directive->addOpenMPClause(OMPC_affinity, OMPC_AFFINITY_MODIFIER_iterator); 
                              }'('iterators_definition')'{}
                  ;
iterators_definition : iterator_specifier
                     | iterators_definition ',' iterator_specifier
                     ;
iterator_specifier : EXPR_STRING EXPR_STRING { iterator_definition->push_back($1); iterator_definition->push_back($2); } '=' range_specification
                   | EXPR_STRING{iterator_definition->push_back(""); iterator_definition->push_back($1); }  '=' range_specification
                   ;
range_specification : EXPR_STRING { iterator_definition->push_back($1); } ':' EXPR_STRING { iterator_definition->push_back($4); } range_step { iterator_definition = new std::vector<const char*>(); }

                    ;
range_step : /*empty*/ { iterator_definition->push_back(""); ((OpenMPAffinityClause*)current_clause)->addIteratorsDefinitionClass(iterator_definition); }
           | ':' EXPR_STRING { iterator_definition->push_back($2); ((OpenMPAffinityClause*)current_clause)->addIteratorsDefinitionClass(iterator_definition); }
           ;

detach_clause: DETACH {
                            current_clause = current_directive->addOpenMPClause(OMPC_detach);
                         } '(' expression ')'
             ;
grainsize_clause: GRAINSIZE {
                            current_clause = current_directive->addOpenMPClause(OMPC_grainsize);
                         } '(' expression ')'
                ;
num_tasks_clause: NUM_TASKS {
                            current_clause = current_directive->addOpenMPClause(OMPC_num_tasks);
                         } '(' expression ')'
                ;
nogroup_clause: NOGROUP {
                            current_clause = current_directive->addOpenMPClause(OMPC_nogroup);
                         } 
              ;
reverse_offload_clause: REVERSE_OFFLOAD {
                            current_clause = current_directive->addOpenMPClause(OMPC_reverse_offload);
                         } 
                      ;
unified_address_clause: UNIFIED_ADDRESS {
                            current_clause = current_directive->addOpenMPClause(OMPC_unified_address);
                         } 
                      ;
unified_shared_memory_clause: UNIFIED_SHARED_MEMORY {
                            current_clause = current_directive->addOpenMPClause(OMPC_unified_shared_memory);
                         } 
                      ;
atomic_default_mem_order_clause : ATOMIC_DEFAULT_MEM_ORDER '(' atomic_default_mem_order_parameter ')' { } ;

atomic_default_mem_order_parameter : SEQ_CST { current_clause = current_directive->addOpenMPClause(OMPC_atomic_default_mem_order, OMPC_ATOMIC_DEFAULT_MEM_ORDER_seq_cst); }
                                   | ACQ_REL { current_clause = current_directive->addOpenMPClause(OMPC_atomic_default_mem_order, OMPC_ATOMIC_DEFAULT_MEM_ORDER_acq_rel); }
                                   | RELAXED { current_clause = current_directive->addOpenMPClause(OMPC_atomic_default_mem_order, OMPC_ATOMIC_DEFAULT_MEM_ORDER_relaxed); }
                                   ;
dynamic_allocators_clause: DYNAMIC_ALLOCATORS {
                            current_clause = current_directive->addOpenMPClause(OMPC_dynamic_allocators);
                         } 
                         ;
ext_implementation_defined_requirement_clause: EXT_ EXPR_STRING {
                                               current_clause = current_directive->addOpenMPClause(OMPC_ext_implementation_defined_requirement);
                                               ((OpenMPExtImplementationDefinedRequirementClause*)current_clause)->setImplementationDefinedRequirement($2);
                                               ((OpenMPExtImplementationDefinedRequirementClause*)current_clause)->mergeExtImplementationDefinedRequirement(current_directive, current_clause);
                                             }
                                             ;
device_clause : DEVICE '(' device_parameter ')' ;

device_parameter : EXPR_STRING  { current_clause = current_directive->addOpenMPClause(OMPC_device, OMPC_DEVICE_MODIFIER_unspecified); current_clause->addLangExpr($1); }
                 | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_device); current_clause->addLangExpr($1); } var_list
                 | device_modifier_parameter ':' var_list
                 ;

device_modifier_parameter : ANCESTOR { current_clause = current_directive->addOpenMPClause(OMPC_device, OMPC_DEVICE_MODIFIER_ancestor); }
                          | DEVICE_NUM { current_clause = current_directive->addOpenMPClause(OMPC_device, OMPC_DEVICE_MODIFIER_device_num); }
                          ;
                          
device_without_modifier_clause : DEVICE '(' device_without_modifier_parameter ')' ;

device_without_modifier_parameter : EXPR_STRING  { current_clause = current_directive->addOpenMPClause(OMPC_device, OMPC_DEVICE_MODIFIER_unspecified); current_clause->addLangExpr($1); }
                                  | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_device); current_clause->addLangExpr($1); } var_list
                                  ;

use_device_ptr_clause : USE_DEVICE_PTR {
                current_clause = current_directive->addOpenMPClause(OMPC_use_device_ptr);
} '(' var_list ')'
                      ;
            
sizes_clause : SIZES {
                current_clause = current_directive->addOpenMPClause(OMPC_sizes);
} '(' var_list ')'
                      ;

use_device_addr_clause : USE_DEVICE_ADDR {
                current_clause = current_directive->addOpenMPClause(OMPC_use_device_addr);
} '(' var_list ')'
                       ;
is_device_ptr_clause : IS_DEVICE_PTR {
                current_clause = current_directive->addOpenMPClause(OMPC_is_device_ptr);
} '(' var_list ')' {
}
                     ;
                     
has_device_addr_clause : HAS_DEVICE_ADDR {
                current_clause = current_directive->addOpenMPClause(OMPC_has_device_addr);
} '(' var_list ')' {
}
                     ;
defaultmap_clause : DEFAULTMAP{ firstParameter = OMPC_DEFAULTMAP_BEHAVIOR_unspecified; } '('  defaultmap_parameter ')'
                  ;
defaultmap_parameter : defaultmap_behavior { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap, firstParameter,OMPC_DEFAULTMAP_CATEGORY_unspecified); } 
                     | defaultmap_behavior ':' defaultmap_category
                     ;

defaultmap_behavior : BEHAVIOR_ALLOC { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_alloc; }
                    | BEHAVIOR_TO { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_to; }
                    | BEHAVIOR_FROM { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_from; }
                    | BEHAVIOR_TOFROM {firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_tofrom; }
                    | BEHAVIOR_FIRSTPRIVATE { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_firstprivate; }
                    | BEHAVIOR_NONE { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_none; }
                    | BEHAVIOR_DEFAULT { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_default; }
                    ;
defaultmap_category : CATEGORY_SCALAR { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap, firstParameter,OMPC_DEFAULTMAP_CATEGORY_scalar); }
                    | CATEGORY_AGGREGATE { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap, firstParameter,OMPC_DEFAULTMAP_CATEGORY_aggregate); }
                    | CATEGORY_POINTER { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap,firstParameter,OMPC_DEFAULTMAP_CATEGORY_pointer); }
                    | CATEGORY_ALLOCATABLE { if (user_set_lang == Lang_Fortran || auto_lang == Lang_Fortran) {current_clause = current_directive->addOpenMPClause(OMPC_defaultmap,firstParameter,OMPC_DEFAULTMAP_CATEGORY_allocatable);} else { yyerror("Defaultmap clause does not support allocatable in C/C++."); YYABORT;} }
                    ;
uses_allocators_clause : USES_ALLOCATORS  { current_clause = current_directive->addOpenMPClause(OMPC_uses_allocators); firstParameter = OMPC_USESALLOCATORS_ALLOCATOR_unspecified; firstStringParameter = ""; secondStringParameter = ""; } '(' uses_allocators_parameter ')' ;
uses_allocators_parameter : allocators_list
                          | allocators_list ',' uses_allocators_parameter
                          ;

allocators_list : allocators_list_parameter_enum { firstStringParameter = NULL; ((OpenMPUsesAllocatorsClause*)current_clause)->addUsesAllocatorsAllocatorSequence(usesAllocator, firstStringParameter, secondStringParameter); }
                | allocators_list_parameter_enum '(' EXPR_STRING ')' { firstStringParameter = $3; ((OpenMPUsesAllocatorsClause*)current_clause)->addUsesAllocatorsAllocatorSequence(usesAllocator, firstStringParameter, secondStringParameter); }
                | allocators_list_parameter_user { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_user; firstStringParameter = NULL; ((OpenMPUsesAllocatorsClause*)current_clause)->addUsesAllocatorsAllocatorSequence(usesAllocator, firstStringParameter, secondStringParameter); }
                | allocators_list_parameter_user '(' EXPR_STRING ')' { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_user; firstStringParameter = $3; ((OpenMPUsesAllocatorsClause*)current_clause)->addUsesAllocatorsAllocatorSequence(usesAllocator, firstStringParameter, secondStringParameter); }
                ;

allocators_list_parameter_enum : DEFAULT_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_default; }
                               | LARGE_CAP_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_large_cap; }
                               | CONST_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_cons_mem; }
                               | HIGH_BW_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_high_bw; }
                               | LOW_LAT_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_low_lat;}
                               | CGROUP_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_cgroup;  }
                               | PTEAM_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_pteam;  }
                               | THREAD_MEM_ALLOC { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_thread; }
                               ;
allocators_list_parameter_user : EXPR_STRING { usesAllocator = OMPC_USESALLOCATORS_ALLOCATOR_unspecified; secondStringParameter = $1; }
                               ;
to_clause: TO '(' to_parameter ')' ;
to_parameter : EXPR_STRING  { current_clause = current_directive->addOpenMPClause(OMPC_to, OMPC_TO_unspecified); current_clause->addLangExpr($1);  }
             | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_to, OMPC_TO_unspecified); current_clause->addLangExpr($1); } var_list
             | to_mapper ':' var_list
             ;
to_mapper : TO_MAPPER { current_clause = current_directive->addOpenMPClause(OMPC_to, OMPC_TO_mapper);
                              }'('EXPR_STRING')' { ((OpenMPToClause*)current_clause)->setMapperIdentifier($4); }
          ;

from_clause: FROM '(' from_parameter ')' ;
from_parameter : EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_from, OMPC_FROM_unspecified); current_clause->addLangExpr($1);  }
               | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_from, OMPC_FROM_unspecified); current_clause->addLangExpr($1); } var_list
               | from_mapper ':' var_list
               ;
from_mapper : FROM_MAPPER { current_clause = current_directive->addOpenMPClause(OMPC_from, OMPC_FROM_mapper); 
                              }'('EXPR_STRING')' { ((OpenMPFromClause*)current_clause)->setMapperIdentifier($4); }
            ;
link_clause : LINK {
                current_clause = current_directive->addOpenMPClause(OMPC_link);
} '(' var_list ')' {
}
  ;
device_type_clause : DEVICE_TYPE '(' device_type_parameter ')' { } ;

device_type_parameter : HOST { current_clause = current_directive->addOpenMPClause(OMPC_device_type, OMPC_DEVICE_TYPE_host); }
                    | NOHOST { current_clause = current_directive->addOpenMPClause(OMPC_device_type, OMPC_DEVICE_TYPE_nohost); }
                    | ANY { current_clause = current_directive->addOpenMPClause(OMPC_device_type, OMPC_DEVICE_TYPE_any); }
                    ;

map_clause : MAP { firstParameter = OMPC_MAP_MODIFIER_unspecified; secondParameter = OMPC_MAP_MODIFIER_unspecified; thirdParameter = OMPC_MAP_MODIFIER_unspecified; }'(' map_parameter')';

map_parameter : EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_unspecified, firstStringParameter); current_clause->addLangExpr($1); }
              | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_unspecified, firstStringParameter); current_clause->addLangExpr($1); } var_list
              | map_modifier_type ':' var_list
              ;
map_modifier_type : map_type
                  | map_modifier1 map_type
                  | map_modifier1 ',' map_type
                  | map_modifier1 ',' map_modifier_parameter1
                  | map_modifier1 map_modifier_parameter1                  
                  ;
map_modifier_parameter1 : map_modifier2 map_type
                        | map_modifier2 ',' map_type
                        | map_modifier2 map_modifier_parameter2
                        | map_modifier2 ',' map_modifier_parameter2
                        ;
map_modifier_parameter2 : map_modifier3 map_type
                        | map_modifier3 ',' map_type
                        ; 

map_modifier1 : MAP_MODIFIER_ALWAYS { firstParameter = OMPC_MAP_MODIFIER_always; }
              | MAP_MODIFIER_CLOSE  { firstParameter = OMPC_MAP_MODIFIER_close; }
              | map_modifier_mapper { firstParameter = OMPC_MAP_MODIFIER_mapper; }
              ;
map_modifier2 : MAP_MODIFIER_ALWAYS { if (firstParameter == OMPC_MAP_MODIFIER_always) { yyerror("ALWAYS modifier can appear in the map clause only once\n"); YYABORT; } else { secondParameter = OMPC_MAP_MODIFIER_always; }}
              | MAP_MODIFIER_CLOSE  { if (firstParameter == OMPC_MAP_MODIFIER_close) { yyerror("CLOSE modifier can appear in the map clause only once\n"); YYABORT;} else { secondParameter = OMPC_MAP_MODIFIER_close; }}
              | map_modifier_mapper { if (firstParameter == OMPC_MAP_MODIFIER_mapper) { yyerror("MAPPER modifier can appear in the map clause only once\n"); YYABORT; } else { secondParameter = OMPC_MAP_MODIFIER_mapper; }}
              ;
map_modifier3 : MAP_MODIFIER_ALWAYS { if (firstParameter == OMPC_MAP_MODIFIER_always || secondParameter==OMPC_MAP_MODIFIER_always) { yyerror("ALWAYS modifier can appear in the map clause only once\n"); YYABORT; } else { thirdParameter = OMPC_MAP_MODIFIER_always; }}
              | MAP_MODIFIER_CLOSE  { if (firstParameter == OMPC_MAP_MODIFIER_close || secondParameter==OMPC_MAP_MODIFIER_close) { yyerror("CLOSE modifier can appear in the map clause only once\n"); YYABORT; } else { thirdParameter = OMPC_MAP_MODIFIER_close; }}
              | map_modifier_mapper { if (firstParameter == OMPC_MAP_MODIFIER_mapper || secondParameter==OMPC_MAP_MODIFIER_mapper) { yyerror("MAPPER modifier can appear in the map clause only once\n"); YYABORT; } else { thirdParameter = OMPC_MAP_MODIFIER_mapper; }}
              ;
map_type : MAP_TYPE_TO { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_to, firstStringParameter); }
         | MAP_TYPE_FROM { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter, thirdParameter, OMPC_MAP_TYPE_from, firstStringParameter); }
         | MAP_TYPE_TOFROM { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter, thirdParameter, OMPC_MAP_TYPE_tofrom, firstStringParameter); }
         | MAP_TYPE_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter, thirdParameter, OMPC_MAP_TYPE_alloc, firstStringParameter); }
         | MAP_TYPE_RELEASE { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter, thirdParameter, OMPC_MAP_TYPE_release, firstStringParameter); }
         | MAP_TYPE_DELETE { current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter, thirdParameter, OMPC_MAP_TYPE_delete, firstStringParameter); }
         ;
map_modifier_mapper : MAP_MODIFIER_MAPPER '('EXPR_STRING')' { firstStringParameter = $3; }
                    ;

task_reduction_clause : TASK_REDUCTION '(' task_reduction_identifier ':' var_list ')' {
                      }
                      ;
task_reduction_identifier : task_reduction_enum_identifier
                          | EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_user, $1); }
                          ;

task_reduction_enum_identifier : '+' { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_plus); }
                               | '-' { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_minus); }
                               | '*' { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_mul); }
                               | '&' { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_bitand); }
                               | '|' { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_bitor); }
                               | '^' { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_bitxor); }
                               | LOGAND { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_logand); }
                               | LOGOR { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_logor); }
                               | MAX { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_max); }
                               | MIN { current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_min); }
                               ;
ordered_clause_optseq : /* empty */
                      | ordered_clause_threads_simd_seq
                      | ordered_clause_depend_seq
                      ;
ordered_clause_threads_simd_seq : ordered_clause_threads_simd
                                | ordered_clause_threads_simd_seq ordered_clause_threads_simd
                                | ordered_clause_threads_simd_seq ',' ordered_clause_threads_simd
                                ;
ordered_clause_depend_seq : ordered_clause_depend
                          | ordered_clause_depend_seq ordered_clause_depend
                          | ordered_clause_depend_seq ',' ordered_clause_depend
                          ;
ordered_clause_depend : depend_ordered_clause
                      ;
ordered_clause_threads_simd : threads_clause
                            | simd_ordered_clause
                            ;
threads_clause : THREADS {
                            current_clause = current_directive->addOpenMPClause(OMPC_threads);
                         } 
               ;
simd_ordered_clause : SIMD {
                            current_clause = current_directive->addOpenMPClause(OMPC_simd);
                         } 
                    ;
teams_distribute_directive : TEAMS DISTRIBUTE {
                        current_directive = new OpenMPDirective(OMPD_teams_distribute);
                                         }
                     teams_distribute_clause_optseq 
                           ;
teams_distribute_clause_optseq : /* empty */
                               | teams_distribute_clause_seq
                               ;
teams_distribute_clause_seq : teams_distribute_clause
                            | teams_distribute_clause_seq teams_distribute_clause
                            | teams_distribute_clause_seq ',' teams_distribute_clause
                            ;
teams_distribute_clause : num_teams_clause
                        | thread_limit_clause
                        | default_clause
                        | private_clause
                        | firstprivate_clause
                        | shared_clause
                        | reduction_default_only_clause
                        | allocate_clause              
                        | lastprivate_distribute_clause
                        | collapse_clause
                        | dist_schedule_clause
                        ;
teams_distribute_simd_directive :  TEAMS DISTRIBUTE SIMD {
                        current_directive = new OpenMPDirective(OMPD_teams_distribute_simd);
                                         }
                     teams_distribute_simd_clause_optseq 
                                ;
teams_distribute_simd_clause_optseq : /* empty */
                                    | teams_distribute_simd_clause_seq
                                    ;
teams_distribute_simd_clause_seq : teams_distribute_simd_clause
                                 | teams_distribute_simd_clause_seq teams_distribute_simd_clause
                                 | teams_distribute_simd_clause_seq ',' teams_distribute_simd_clause
                                 ;
teams_distribute_simd_clause : num_teams_clause
                             | thread_limit_clause
                             | default_clause
                             | private_clause
                             | firstprivate_clause
                             | shared_clause
                             | reduction_clause
                             | allocate_clause
                             | lastprivate_clause
                             | collapse_clause
                             | dist_schedule_clause
                             | if_simd_clause
                             | safelen_clause
                             | simdlen_clause
                             | linear_clause
                             | aligned_clause
                             | nontemporal_clause
                             | order_clause
                             ;
teams_distribute_parallel_for_directive :  TEAMS DISTRIBUTE PARALLEL FOR {
                     current_directive = new OpenMPDirective(OMPD_teams_distribute_parallel_for);
                                         }
                     teams_distribute_parallel_for_clause_optseq 
                                        ;
teams_distribute_parallel_for_clause_optseq : /* empty */
                                            | teams_distribute_parallel_for_clause_seq
                                            ;
teams_distribute_parallel_for_clause_seq : teams_distribute_parallel_for_clause
                                         | teams_distribute_parallel_for_clause_seq teams_distribute_parallel_for_clause
                                         | teams_distribute_parallel_for_clause_seq ',' teams_distribute_parallel_for_clause
                                         ;
teams_distribute_parallel_for_clause : num_teams_clause
                                     | thread_limit_clause
                                     | default_clause
                                     | private_clause
                                     | firstprivate_clause
                                     | shared_clause
                                     | reduction_clause
                                     | allocate_clause
                                     | if_parallel_clause
                                     | num_threads_clause                   
                                     | copyin_clause                            
                                     | proc_bind_clause                      
                                     | lastprivate_clause 
                                     | linear_clause
                                     | schedule_clause
                                     | collapse_clause
                                     | ordered_clause
                                     | nowait_clause
                                     | order_clause 
                                     | dist_schedule_clause                                   
                                     ;
teams_distribute_parallel_do_directive :  TEAMS DISTRIBUTE PARALLEL DO {
                     current_directive = new OpenMPDirective(OMPD_teams_distribute_parallel_do);
                                        }
                     teams_distribute_parallel_do_clause_optseq 
                                       ;
teams_distribute_parallel_do_clause_optseq : /* empty */
                                           | teams_distribute_parallel_do_clause_seq
                                           ;
teams_distribute_parallel_do_clause_seq : teams_distribute_parallel_do_clause
                                        | teams_distribute_parallel_do_clause_seq teams_distribute_parallel_do_clause
                                        | teams_distribute_parallel_do_clause_seq ',' teams_distribute_parallel_do_clause
                                        ;
teams_distribute_parallel_do_clause : num_teams_clause
                                    | thread_limit_clause
                                    | default_clause
                                    | private_clause
                                    | firstprivate_clause
                                    | shared_clause
                                    | reduction_clause
                                    | allocate_clause
                                    | if_parallel_clause
                                    | num_threads_clause                   
                                    | copyin_clause                            
                                    | proc_bind_clause                      
                                    | lastprivate_clause 
                                    | linear_clause
                                    | schedule_clause
                                    | collapse_clause
                                    | ordered_clause
                                    | nowait_clause
                                    | order_clause 
                                    | dist_schedule_clause                                   
                                     ;
teams_distribute_parallel_for_simd_directive : TEAMS DISTRIBUTE PARALLEL FOR SIMD {
                        current_directive = new OpenMPDirective(OMPD_teams_distribute_parallel_for_simd);
                                         }
                       teams_distribute_parallel_for_simd_clause_optseq 
                                             ;
teams_distribute_parallel_for_simd_clause_optseq : /* empty */
                                                 | teams_distribute_parallel_for_simd_clause_seq
                                                 ;
teams_distribute_parallel_for_simd_clause_seq : teams_distribute_parallel_for_simd_clause
                                              | teams_distribute_parallel_for_simd_clause_seq teams_distribute_parallel_for_simd_clause
                                              | teams_distribute_parallel_for_simd_clause_seq ',' teams_distribute_parallel_for_simd_clause
                                              ;
teams_distribute_parallel_for_simd_clause : num_teams_clause
                                          | thread_limit_clause
                                          | default_clause
                                          | private_clause
                                          | firstprivate_clause
                                          | shared_clause
                                          | reduction_clause
                                          | allocate_clause
                                          | if_parallel_simd_clause
                                          | num_threads_clause
                                          | copyin_clause                               
                                          | proc_bind_clause                                  
                                          | lastprivate_clause 
                                          | linear_clause
                                          | schedule_clause
                                          | collapse_clause
                                          | ordered_clause
                                          | nowait_clause
                                          | order_clause 
                                          | dist_schedule_clause
                                          | safelen_clause
                                          | simdlen_clause
                                          | aligned_clause
                                          | nontemporal_clause
                                          ;
teams_distribute_parallel_do_simd_directive : TEAMS DISTRIBUTE PARALLEL DO SIMD {
                        current_directive = new OpenMPDirective(OMPD_teams_distribute_parallel_do_simd);
                                         }
                       teams_distribute_parallel_do_simd_clause_optseq 
                                            ;
teams_distribute_parallel_do_simd_clause_optseq : /* empty */
                                                | teams_distribute_parallel_do_simd_clause_seq
                                                ;
teams_distribute_parallel_do_simd_clause_seq : teams_distribute_parallel_do_simd_clause
                                             | teams_distribute_parallel_do_simd_clause_seq teams_distribute_parallel_do_simd_clause
                                             | teams_distribute_parallel_do_simd_clause_seq ',' teams_distribute_parallel_do_simd_clause
                                             ;
teams_distribute_parallel_do_simd_clause : num_teams_clause
                                         | thread_limit_clause
                                         | default_clause
                                         | private_clause
                                         | firstprivate_clause
                                         | shared_clause
                                         | reduction_clause
                                         | allocate_clause
                                         | if_parallel_simd_clause
                                         | num_threads_clause
                                         | copyin_clause                               
                                         | proc_bind_clause                                  
                                         | lastprivate_clause 
                                         | linear_clause
                                         | schedule_clause
                                         | collapse_clause
                                         | ordered_clause
                                         | nowait_clause
                                         | order_clause 
                                         | dist_schedule_clause
                                         | safelen_clause
                                         | simdlen_clause
                                         | aligned_clause
                                         | nontemporal_clause
                                         ;
teams_loop_directive : TEAMS LOOP{
                        current_directive = new OpenMPDirective(OMPD_teams_loop);
                                         }
                     teams_loop_clause_optseq 
                     ;
teams_loop_clause_optseq : /* empty */
                         | teams_loop_clause_seq
                         ;
teams_loop_clause_seq : teams_loop_clause
                      | teams_loop_clause_seq teams_loop_clause
                      | teams_loop_clause_seq ',' teams_loop_clause
                      ;
teams_loop_clause : num_teams_clause
                  | thread_limit_clause
                  | default_clause
                  | private_clause
                  | firstprivate_clause
                  | shared_clause
                  | reduction_default_only_clause
                  | allocate_clause
                  | bind_clause
                  | collapse_clause
                  | order_clause
                  | lastprivate_clause
                  ;
target_parallel_directive : TARGET PARALLEL{
                        current_directive = new OpenMPDirective(OMPD_target_parallel);
                                         }
                     target_parallel_clause_optseq 
                          ;
target_parallel_clause_optseq : /* empty */
                              | target_parallel_clause_seq
                              ;
target_parallel_clause_seq : target_parallel_clause
                           | target_parallel_clause_seq target_parallel_clause
                           | target_parallel_clause_seq ',' target_parallel_clause
                           ;
target_parallel_clause : if_target_parallel_clause
                       | device_clause
                       | private_clause
                       | firstprivate_clause
                       | in_reduction_clause
                       | map_clause
                       | is_device_ptr_clause
                       | defaultmap_clause
                       | nowait_clause
                       | allocate_clause
                       | depend_with_modifier_clause
                       | uses_allocators_clause
                       | num_threads_clause
                       | default_clause
                       | shared_clause
                       | copyin_clause
                       | reduction_clause
                       | proc_bind_clause
                       ;
target_parallel_for_directive : TARGET PARALLEL FOR{
                        current_directive = new OpenMPDirective(OMPD_target_parallel_for);
                                         }
                     target_parallel_for_clause_optseq 
                              ;
target_parallel_for_clause_optseq : /* empty */
                                  | target_parallel_for_clause_seq
                                  ;
target_parallel_for_clause_seq : target_parallel_for_clause
                               | target_parallel_for_clause_seq target_parallel_for_clause
                               | target_parallel_for_clause_seq ',' target_parallel_for_clause
                               ;
target_parallel_for_clause : if_target_parallel_clause
                           | device_clause
                           | private_clause
                           | firstprivate_clause
                           | in_reduction_clause
                           | map_clause
                           | is_device_ptr_clause
                           | defaultmap_clause
                           | nowait_clause
                           | allocate_clause
                           | depend_with_modifier_clause
                           | uses_allocators_clause
                           | num_threads_clause
                           | default_clause                          
                           | shared_clause
                           | copyin_clause
                           | reduction_clause
                           | proc_bind_clause
                           | lastprivate_clause 
                           | linear_clause
                           | schedule_clause
                           | collapse_clause
                           | ordered_clause
                           | order_clause 
                           | dist_schedule_clause
                           ;
target_parallel_do_directive : TARGET PARALLEL DO{
                        current_directive = new OpenMPDirective(OMPD_target_parallel_do);
                                         }
                     target_parallel_do_clause_optseq 
                             ;
target_parallel_do_clause_optseq : /* empty */
                                 | target_parallel_do_clause_seq
                                 ;
target_parallel_do_clause_seq : target_parallel_do_clause
                              | target_parallel_do_clause_seq target_parallel_do_clause
                              | target_parallel_do_clause_seq ',' target_parallel_do_clause
                              ;
target_parallel_do_clause : if_target_parallel_clause
                          | device_clause
                          | private_clause
                          | firstprivate_clause
                          | in_reduction_clause
                          | map_clause
                          | is_device_ptr_clause
                          | defaultmap_clause
                          | nowait_clause
                          | allocate_clause
                          | depend_with_modifier_clause
                          | uses_allocators_clause
                          | num_threads_clause
                          | default_clause                          
                          | shared_clause
                          | copyin_clause
                          | reduction_clause
                          | proc_bind_clause
                          | lastprivate_clause 
                          | linear_clause
                          | schedule_clause
                          | collapse_clause
                          | ordered_clause
                          | order_clause 
                          | dist_schedule_clause
                          ;
target_parallel_for_simd_directive : TARGET PARALLEL FOR SIMD{
                        current_directive = new OpenMPDirective(OMPD_target_parallel_for_simd);
                                         }
                     target_parallel_for_simd_clause_optseq 
                                   ;
target_parallel_for_simd_clause_optseq : /* empty */
                                       | target_parallel_for_simd_clause_seq
                                       ;
target_parallel_for_simd_clause_seq : target_parallel_for_simd_clause
                                    | target_parallel_for_simd_clause_seq target_parallel_for_simd_clause
                                    | target_parallel_for_simd_clause_seq ',' target_parallel_for_simd_clause
                                    ;
target_parallel_for_simd_clause : if_target_parallel_simd_clause
                                | device_clause
                                | private_clause
                                | firstprivate_clause
                                | in_reduction_clause
                                | map_clause
                                | is_device_ptr_clause
                                | defaultmap_clause
                                | nowait_clause
                                | allocate_clause
                                | depend_with_modifier_clause
                                | uses_allocators_clause
                                | num_threads_clause
                                | default_clause                    
                                | shared_clause
                                | copyin_clause
                                | reduction_clause
                                | proc_bind_clause                       
                                | lastprivate_clause 
                                | linear_clause
                                | schedule_clause
                                | collapse_clause
                                | ordered_clause                        
                                | order_clause
                                | safelen_clause
                                | simdlen_clause
                                | aligned_clause
                                | nontemporal_clause
                                ;
target_parallel_do_simd_directive : TARGET PARALLEL DO SIMD{
                        current_directive = new OpenMPDirective(OMPD_target_parallel_do_simd);
                                         }
                     target_parallel_do_simd_clause_optseq 
                                  ;
target_parallel_do_simd_clause_optseq : /* empty */
                                      | target_parallel_do_simd_clause_seq
                                      ;
target_parallel_do_simd_clause_seq : target_parallel_do_simd_clause
                                   | target_parallel_do_simd_clause_seq target_parallel_do_simd_clause
                                   | target_parallel_do_simd_clause_seq ',' target_parallel_do_simd_clause
                                   ;
target_parallel_do_simd_clause : if_target_parallel_simd_clause
                               | device_clause
                               | private_clause
                               | firstprivate_clause
                               | in_reduction_clause
                               | map_clause
                               | is_device_ptr_clause
                               | defaultmap_clause
                               | nowait_clause
                               | allocate_clause
                               | depend_with_modifier_clause
                               | uses_allocators_clause
                               | num_threads_clause
                               | default_clause                    
                               | shared_clause
                               | copyin_clause
                               | reduction_clause
                               | proc_bind_clause                       
                               | lastprivate_clause 
                               | linear_clause
                               | schedule_clause
                               | collapse_clause
                               | ordered_clause                        
                               | order_clause
                               | safelen_clause
                               | simdlen_clause
                               | aligned_clause
                               | nontemporal_clause
                               ;
target_parallel_loop_directive : TARGET PARALLEL LOOP{
                        current_directive = new OpenMPDirective(OMPD_target_parallel_loop);
                                         }
                     target_parallel_loop_clause_optseq 
                               ;
target_parallel_loop_clause_optseq : /* empty */
                                   | target_parallel_loop_clause_seq
                                   ;
target_parallel_loop_clause_seq : target_parallel_loop_clause
                                | target_parallel_loop_clause_seq target_parallel_loop_clause
                                | target_parallel_loop_clause_seq ',' target_parallel_loop_clause
                                ;
target_parallel_loop_clause : if_target_parallel_clause
                            | device_clause
                            | private_clause
                            | firstprivate_clause
                            | in_reduction_clause
                            | map_clause
                            | is_device_ptr_clause
                            | defaultmap_clause
                            | nowait_clause
                            | allocate_clause
                            | depend_with_modifier_clause
                            | uses_allocators_clause
                            | num_threads_clause
                            | default_clause             
                            | shared_clause
                            | copyin_clause
                            | reduction_clause
                            | proc_bind_clause                   
                            | lastprivate_clause 
                            | collapse_clause
                            | bind_clause
                            | order_clause 
                            ;
target_simd_directive : TARGET SIMD{
                        current_directive = new OpenMPDirective(OMPD_target_simd);
                                         }
                     target_simd_clause_optseq 
                      ;
target_simd_clause_optseq : /* empty */
                          | target_simd_clause_seq
                          ;
target_simd_clause_seq : target_simd_clause
                       | target_simd_clause_seq target_simd_clause
                       | target_simd_clause_seq ',' target_simd_clause
                       ;
target_simd_clause : if_target_simd_clause
                   | device_clause
                   | private_clause
                   | firstprivate_clause
                   | in_reduction_clause
                   | map_clause
                   | is_device_ptr_clause
                   | defaultmap_clause
                   | nowait_clause
                   | allocate_clause
                   | depend_with_modifier_clause
                   | uses_allocators_clause
                   | safelen_clause
                   | simdlen_clause
                   | linear_clause
                   | aligned_clause
                   | nontemporal_clause
                   | lastprivate_clause
                   | reduction_clause
                   | collapse_clause
                   | order_clause
                   ;
target_teams_directive : TARGET TEAMS{
                        current_directive = new OpenMPDirective(OMPD_target_teams);
                                         }
                     target_teams_clause_optseq 
                       ;
target_teams_clause_optseq : /* empty */
                           | target_teams_clause_seq
                           ;
target_teams_clause_seq : target_teams_clause
                        | target_teams_clause_seq target_teams_clause
                        | target_teams_clause_seq ',' target_teams_clause
                        ;
target_teams_clause : if_target_clause
                    | device_clause
                    | private_clause
                    | firstprivate_clause
                    | in_reduction_clause
                    | map_clause
                    | is_device_ptr_clause
                    | defaultmap_clause
                    | nowait_clause
                    | allocate_clause
                    | depend_with_modifier_clause
                    | uses_allocators_clause
                    | num_teams_clause
                    | thread_limit_clause
                    | default_clause
                    | shared_clause
                    | reduction_default_only_clause
                    ;
target_teams_distribute_directive : TARGET TEAMS DISTRIBUTE{
                        current_directive = new OpenMPDirective(OMPD_target_teams_distribute);
                                         }
                     target_teams_distribute_clause_optseq 
                                  ;
target_teams_distribute_clause_optseq : /* empty */
                                      | target_teams_distribute_clause_seq
                                      ;
target_teams_distribute_clause_seq : target_teams_distribute_clause
                                   | target_teams_distribute_clause_seq target_teams_distribute_clause
                                   | target_teams_distribute_clause_seq ',' target_teams_distribute_clause
                                   ;
target_teams_distribute_clause : if_target_clause
                               | device_clause
                               | private_clause
                               | firstprivate_clause
                               | in_reduction_clause
                               | map_clause
                               | is_device_ptr_clause
                               | defaultmap_clause
                               | nowait_clause
                               | allocate_clause
                               | depend_with_modifier_clause
                               | uses_allocators_clause
                               | num_teams_clause
                               | thread_limit_clause
                               | default_clause                   
                               | shared_clause
                               | reduction_default_only_clause
                               | lastprivate_distribute_clause
                               | collapse_clause
                               | dist_schedule_clause
                               ;
target_teams_distribute_simd_directive : TARGET TEAMS DISTRIBUTE SIMD{
                        current_directive = new OpenMPDirective(OMPD_target_teams_distribute_simd);
                                         }
                     target_teams_distribute_simd_clause_optseq 
                                       ;
target_teams_distribute_simd_clause_optseq : /* empty */
                                           | target_teams_distribute_simd_clause_seq
                                           ;
target_teams_distribute_simd_clause_seq : target_teams_distribute_simd_clause
                                        | target_teams_distribute_simd_clause_seq target_teams_distribute_simd_clause
                                        | target_teams_distribute_simd_clause_seq ',' target_teams_distribute_simd_clause
                                        ;
target_teams_distribute_simd_clause : if_target_simd_clause
                                    | device_clause
                                    | private_clause
                                    | firstprivate_clause
                                    | in_reduction_clause
                                    | map_clause
                                    | is_device_ptr_clause
                                    | defaultmap_clause
                                    | nowait_clause
                                    | allocate_clause
                                    | depend_with_modifier_clause
                                    | uses_allocators_clause 
                                    | num_teams_clause
                                    | thread_limit_clause
                                    | default_clause
                                    | shared_clause
                                    | reduction_clause
                                    | lastprivate_clause
                                    | collapse_clause
                                    | dist_schedule_clause
                                    | safelen_clause
                                    | simdlen_clause
                                    | linear_clause
                                    | aligned_clause
                                    | nontemporal_clause
                                    | order_clause
                                    ;
target_teams_loop_directive : TARGET TEAMS LOOP{
                        current_directive = new OpenMPDirective(OMPD_target_teams_loop);
                                         }
                     target_teams_loop_clause_optseq 
                            ;
target_teams_loop_clause_optseq : /* empty */
                                | target_teams_loop_clause_seq
                                ;
target_teams_loop_clause_seq : target_teams_loop_clause
                             | target_teams_loop_clause_seq target_teams_loop_clause
                             | target_teams_loop_clause_seq ',' target_teams_loop_clause
                             ;
target_teams_loop_clause : if_target_clause
                         | device_clause
                         | private_clause
                         | firstprivate_clause
                         | in_reduction_clause
                         | map_clause
                         | is_device_ptr_clause
                         | defaultmap_clause
                         | nowait_clause
                         | allocate_clause
                         | depend_with_modifier_clause
                         | uses_allocators_clause 
                         | num_teams_clause
                         | thread_limit_clause
                         | default_clause
                         | shared_clause
                         | reduction_default_only_clause                                 
                         | bind_clause
                         | collapse_clause
                         | order_clause
                         | lastprivate_clause
                         ;
target_teams_distribute_parallel_for_directive : TARGET TEAMS DISTRIBUTE PARALLEL FOR{
                        current_directive = new OpenMPDirective(OMPD_target_teams_distribute_parallel_for);
                                         }
                     target_teams_distribute_parallel_for_clause_optseq 
                                               ;
target_teams_distribute_parallel_for_clause_optseq : /* empty */
                                                   | target_teams_distribute_parallel_for_clause_seq
                                                   ;
target_teams_distribute_parallel_for_clause_seq : target_teams_distribute_parallel_for_clause
                                                | target_teams_distribute_parallel_for_clause_seq target_teams_distribute_parallel_for_clause
                                                | target_teams_distribute_parallel_for_clause_seq ',' target_teams_distribute_parallel_for_clause
                                                ;
target_teams_distribute_parallel_for_clause : if_target_parallel_clause
                                            | device_clause
                                            | private_clause
                                            | firstprivate_clause
                                            | in_reduction_clause
                                            | map_clause
                                            | is_device_ptr_clause
                                            | defaultmap_clause
                                            | nowait_clause
                                            | allocate_clause
                                            | depend_with_modifier_clause
                                            | uses_allocators_clause 
                                            | num_teams_clause
                                            | thread_limit_clause
                                            | default_clause                                 
                                            | shared_clause
                                            | reduction_clause                            
                                            | num_threads_clause                   
                                            | copyin_clause                            
                                            | proc_bind_clause                      
                                            | lastprivate_clause 
                                            | linear_clause
                                            | schedule_clause
                                            | collapse_clause
                                            | ordered_clause                                
                                            | order_clause 
                                            | dist_schedule_clause
                                            ;
target_teams_distribute_parallel_do_directive : TARGET TEAMS DISTRIBUTE PARALLEL DO{
                        current_directive = new OpenMPDirective(OMPD_target_teams_distribute_parallel_do);
                                         }
                     target_teams_distribute_parallel_do_clause_optseq 
                                              ;
target_teams_distribute_parallel_do_clause_optseq : /* empty */
                                                  | target_teams_distribute_parallel_do_clause_seq
                                                  ;
target_teams_distribute_parallel_do_clause_seq : target_teams_distribute_parallel_do_clause
                                               | target_teams_distribute_parallel_do_clause_seq target_teams_distribute_parallel_do_clause
                                               | target_teams_distribute_parallel_do_clause_seq ',' target_teams_distribute_parallel_do_clause
                                               ;
target_teams_distribute_parallel_do_clause : if_target_parallel_clause
                                           | device_clause
                                           | private_clause
                                           | firstprivate_clause
                                           | in_reduction_clause
                                           | map_clause
                                           | is_device_ptr_clause
                                           | defaultmap_clause
                                           | nowait_clause
                                           | allocate_clause
                                           | depend_with_modifier_clause
                                           | uses_allocators_clause 
                                           | num_teams_clause
                                           | thread_limit_clause
                                           | default_clause                                 
                                           | shared_clause
                                           | reduction_clause                            
                                           | num_threads_clause                   
                                           | copyin_clause                            
                                           | proc_bind_clause                      
                                           | lastprivate_clause 
                                           | linear_clause
                                           | schedule_clause
                                           | collapse_clause
                                           | ordered_clause                                
                                           | order_clause 
                                           | dist_schedule_clause
                                           ;
target_teams_distribute_parallel_for_simd_directive : TARGET TEAMS DISTRIBUTE PARALLEL FOR SIMD{
                        current_directive = new OpenMPDirective(OMPD_target_teams_distribute_parallel_for_simd);
                                         }
                     target_teams_distribute_parallel_for_simd_clause_optseq 
                                                    ;
target_teams_distribute_parallel_for_simd_clause_optseq : /* empty */
                                                        | target_teams_distribute_parallel_for_simd_clause_seq
                                                        ;
target_teams_distribute_parallel_for_simd_clause_seq : target_teams_distribute_parallel_for_simd_clause
                                                     | target_teams_distribute_parallel_for_simd_clause_seq target_teams_distribute_parallel_for_simd_clause
                                                     | target_teams_distribute_parallel_for_simd_clause_seq ',' target_teams_distribute_parallel_for_simd_clause
                                                     ;
target_teams_distribute_parallel_for_simd_clause : if_target_parallel_simd_clause
                                                 | device_clause
                                                 | private_clause
                                                 | firstprivate_clause
                                                 | in_reduction_clause
                                                 | map_clause
                                                 | is_device_ptr_clause
                                                 | defaultmap_clause
                                                 | nowait_clause
                                                 | allocate_clause
                                                 | depend_with_modifier_clause
                                                 | uses_allocators_clause 
                                                 | num_teams_clause
                                                 | thread_limit_clause
                                                 | default_clause                                     
                                                 | shared_clause
                                                 | reduction_clause
                                                 | num_threads_clause
                                                 | copyin_clause                               
                                                 | proc_bind_clause                                  
                                                 | lastprivate_clause 
                                                 | linear_clause
                                                 | schedule_clause
                                                 | collapse_clause
                                                 | ordered_clause                          
                                                 | order_clause 
                                                 | dist_schedule_clause
                                                 | safelen_clause
                                                 | simdlen_clause
                                                 | aligned_clause
                                                 | nontemporal_clause
                                                 ;
target_teams_distribute_parallel_do_simd_directive : TARGET TEAMS DISTRIBUTE PARALLEL DO SIMD{
                        current_directive = new OpenMPDirective(OMPD_target_teams_distribute_parallel_do_simd);
                                         }
                     target_teams_distribute_parallel_do_simd_clause_optseq 
                                                   ;
target_teams_distribute_parallel_do_simd_clause_optseq : /* empty */
                                                       | target_teams_distribute_parallel_do_simd_clause_seq
                                                       ;
target_teams_distribute_parallel_do_simd_clause_seq : target_teams_distribute_parallel_do_simd_clause
                                                    | target_teams_distribute_parallel_do_simd_clause_seq target_teams_distribute_parallel_do_simd_clause
                                                    | target_teams_distribute_parallel_do_simd_clause_seq ',' target_teams_distribute_parallel_do_simd_clause
                                                     ;
target_teams_distribute_parallel_do_simd_clause : if_target_parallel_simd_clause
                                                | device_clause
                                                | private_clause
                                                | firstprivate_clause
                                                | in_reduction_clause
                                                | map_clause
                                                | is_device_ptr_clause
                                                | defaultmap_clause
                                                | nowait_clause
                                                | allocate_clause
                                                | depend_with_modifier_clause
                                                | uses_allocators_clause 
                                                | num_teams_clause
                                                | thread_limit_clause
                                                | default_clause                                     
                                                | shared_clause
                                                | reduction_clause
                                                | num_threads_clause
                                                | copyin_clause                               
                                                | proc_bind_clause                                  
                                                | lastprivate_clause 
                                                | linear_clause
                                                | schedule_clause
                                                | collapse_clause
                                                | ordered_clause                          
                                                | order_clause 
                                                | dist_schedule_clause
                                                | safelen_clause
                                                | simdlen_clause
                                                | aligned_clause
                                                | nontemporal_clause
                                                ;
/*YAYING*/
for_directive : FOR {
                        current_directive = new OpenMPDirective(OMPD_for);
                     }
                     for_clause_optseq 
              ;
do_directive : DO {
                     current_directive = new OpenMPDirective(OMPD_do);
                  }
                  do_clause_optseq
             ;
do_paired_directive : DO {
                            current_directive = new OpenMPDirective(OMPD_do);
                         }
                         do_paried_clause_optseq
                    ;
simd_directive : SIMD {
                         current_directive = new OpenMPDirective(OMPD_simd);
                      }
                      simd_clause_optseq 
               ;
for_simd_directive : FOR SIMD {
                                current_directive = new OpenMPDirective(OMPD_for_simd);
                              }
                              for_simd_clause_optseq
                   ;
do_simd_directive : DO SIMD {
                               current_directive = new OpenMPDirective(OMPD_do_simd);
                             }
                             do_simd_clause_optseq
                  ;
do_simd_paired_directive : DO SIMD {
                                     current_directive = new OpenMPDirective(OMPD_do_simd);
                                   }
                                   do_simd_paried_clause_optseq
                         ;
parallel_for_simd_directive : PARALLEL FOR SIMD { 
                                current_directive = new OpenMPDirective(OMPD_parallel_for_simd);
                            }
                            parallel_for_simd_clause_optseq
                            ;
parallel_do_simd_directive : PARALLEL DO SIMD { 
                                current_directive = new OpenMPDirective(OMPD_parallel_do_simd);
                           }
                           parallel_for_simd_clause_optseq
                           ;
declare_simd_directive : DECLARE SIMD {
                          current_directive = new OpenMPDeclareSimdDirective;
                        }
                       declare_simd_clause_optseq
                       ;
declare_simd_fortran_directive : DECLARE SIMD {
                                    current_directive = new OpenMPDeclareSimdDirective();
                               } '(' proc_name ')'
                               declare_simd_clause_optseq
                               ;
proc_name : /* empty */
          | EXPR_STRING { ((OpenMPDeclareSimdDirective*)current_directive)->addProcName($1); }
          ;
distribute_directive : DISTRIBUTE {
                        current_directive = new OpenMPDirective(OMPD_distribute);
                     }
                     distribute_clause_optseq
                     ;
distribute_simd_directive : DISTRIBUTE SIMD {
                              current_directive = new OpenMPDirective(OMPD_distribute_simd);
                          }
                          distribute_simd_clause_optseq
                          ;
distribute_parallel_for_directive : DISTRIBUTE PARALLEL FOR {
                                       current_directive = new OpenMPDirective(OMPD_distribute_parallel_for);
                                  }
                                  distribute_parallel_for_clause_optseq
                                  ;
distribute_parallel_do_directive : DISTRIBUTE PARALLEL DO {
                                       current_directive = new OpenMPDirective(OMPD_distribute_parallel_do);
                                 }
                                 distribute_parallel_do_clause_optseq
                                 ;
distribute_parallel_for_simd_directive : DISTRIBUTE PARALLEL FOR SIMD {
                                             current_directive = new OpenMPDirective(OMPD_distribute_parallel_for_simd);
                                       }
                                       distribute_parallel_for_simd_clause_optseq
                                       ;
distribute_parallel_do_simd_directive : DISTRIBUTE PARALLEL DO SIMD {
                                             current_directive = new OpenMPDirective(OMPD_distribute_parallel_do_simd);
                                      }
                                      distribute_parallel_do_simd_clause_optseq
                                      ;
parallel_for_directive : PARALLEL FOR {
                         current_directive = new OpenMPDirective(OMPD_parallel_for);
                       }
                       parallel_for_clause_optseq
                       ;
parallel_do_directive : PARALLEL DO {
                         current_directive = new OpenMPDirective(OMPD_parallel_do);
                       }
                      parallel_do_clause_optseq
                      ;
parallel_loop_directive : PARALLEL LOOP {
                         current_directive = new OpenMPDirective(OMPD_parallel_loop);
                        }
                        parallel_loop_clause_optseq
                        ;
parallel_sections_directive : PARALLEL SECTIONS {
                               current_directive = new OpenMPDirective(OMPD_parallel_sections);
                            }
                            parallel_sections_clause_optseq
                            ;
parallel_workshare_directive : PARALLEL WORKSHARE {
                               if (user_set_lang == Lang_Fortran || auto_lang == Lang_Fortran) { 
                                   current_directive = new OpenMPDirective(OMPD_parallel_workshare); } else {
                                       yyerror("parallel workshare is only supported in Fortran");
                                       YYABORT;
                               }
                             }
                             parallel_workshare_clause_optseq
                             ;
parallel_master_directive : PARALLEL MASTER {
                               current_directive = new OpenMPDirective(OMPD_parallel_master);
                          }
                          parallel_master_clause_optseq
                          ;
master_taskloop_directive : MASTER TASKLOOP {
                               current_directive = new OpenMPDirective(OMPD_master_taskloop);
                          }
                          master_taskloop_clause_optseq
                          ;
master_taskloop_simd_directive : MASTER TASKLOOP SIMD {
                                    current_directive = new OpenMPDirective(OMPD_master_taskloop_simd);
                               }
                               master_taskloop_simd_clause_optseq
                               ;
parallel_master_taskloop_directive : PARALLEL MASTER TASKLOOP {
                                          current_directive = new OpenMPDirective(OMPD_parallel_master_taskloop);
                                   }
                                   parallel_master_taskloop_clause_optseq
                                   ; 
parallel_master_taskloop_simd_directive : PARALLEL MASTER TASKLOOP SIMD {
                                          current_directive = new OpenMPDirective(OMPD_parallel_master_taskloop_simd);
                                        }
                                        parallel_master_taskloop_simd_clause_optseq
                                        ; 
loop_directive : LOOP {
                        current_directive = new OpenMPDirective(OMPD_loop);
                     }
               loop_clause_optseq
               ;
scan_directive : SCAN {
                        current_directive = new OpenMPDirective(OMPD_scan);
                      }
               scan_clause_optseq
               ;
sections_directive : SECTIONS {
                        current_directive = new OpenMPDirective(OMPD_sections);
                     }
                   sections_clause_optseq
                   ;
sections_paired_directive : SECTIONS {
                               current_directive = new OpenMPDirective(OMPD_sections);
                            }
                          sections_paired_clause_optseq
                          ;
section_directive : SECTION {
                        current_directive = new OpenMPDirective(OMPD_section);
                  }
                  ;
single_directive : SINGLE {
                        current_directive = new OpenMPDirective(OMPD_single);
                 }
                 single_clause_optseq
                 ;
single_paired_directive : SINGLE {
                               current_directive = new OpenMPDirective(OMPD_single);
                        }
                        single_paired_clause_optseq
                        ;
workshare_directive : WORKSHARE {
                         if (user_set_lang == Lang_Fortran || auto_lang == Lang_Fortran) {
                             current_directive = new OpenMPDirective(OMPD_workshare); } else {
                                 yyerror("workshare is only supported in Fortran");
                                 YYABORT;
                             }
                    }
                    ;
workshare_paired_directive : WORKSHARE {
                               current_directive = new OpenMPDirective(OMPD_workshare);
                           }
                           workshare_paired_clause_optseq
                           ;
cancel_directive : CANCEL {
                        current_directive = new OpenMPDirective(OMPD_cancel);
                 }
                 cancel_clause_optseq
                 ;
//cancel_fortran_directive : CANCEL {
//                              current_directive = new OpenMPDirective(OMPD_cancel);
//                         }
//                         cancel_clause_fortran_optseq
//                         ;
cancellation_point_directive : CANCELLATION POINT {
                                current_directive = new OpenMPDirective(OMPD_cancellation_point);
                                }
                             cancellation_point_clause_optseq
                             ;
//cancellation_point_fortran_directive : CANCELLATION POINT {
//                                         current_directive = new OpenMPDirective(OMPD_cancellation_point);
//                                     }
//                                     cancellation_point_clause_fortran_optseq
//                                     ;
teams_directive : TEAMS {
                        current_directive = new OpenMPDirective(OMPD_teams);
                }
                teams_clause_optseq
                ;

allocate_directive : ALLOCATE {
                        current_directive = new OpenMPAllocateDirective();
                   } allocate_list
                   allocate_clause_optseq
                   ;
allocate_list : '('directive_varlist')'
              ;

directive_variable : EXPR_STRING { ((OpenMPAllocateDirective*)current_directive)->addAllocateList($1); }
                   ;
directive_varlist : directive_variable
                  | directive_varlist ',' directive_variable
                  ;

threadprivate_directive : THREADPRIVATE { current_directive = new OpenMPThreadprivateDirective(); } '('threadprivate_list')'
                        ;
threadprivate_variable : EXPR_STRING { ((OpenMPThreadprivateDirective*)current_directive)->addThreadprivateList($1); }
                       ;
threadprivate_list : threadprivate_variable
                   | threadprivate_list ',' threadprivate_variable
                   ;

declare_reduction_directive : DECLARE REDUCTION { current_directive = new OpenMPDeclareReductionDirective(); } '(' reduction_list ')' declare_reduction_clause_optseq
                            ;

reduction_list : reduction_identifiers ':' typername_list ':' combiner
               ;

reduction_identifiers : '+'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("+"); }
                      | '-'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("-"); }
                      | '*'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("*"); }
                      | '&'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("&"); }
                      | '|'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("|"); }
                      | '^'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("^"); }
                      | LOGAND{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("&&"); }
                      | LOGOR{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("||"); }
                      ; 

typername_variable : EXPR_STRING { ((OpenMPDeclareReductionDirective*)current_directive)->addTypenameList($1); }
                   ;
typername_list : typername_variable
               | typername_list ',' typername_variable
               ;
combiner : EXPR_STRING { ((OpenMPDeclareReductionDirective*)current_directive)->setCombiner($1); }
         ;

declare_mapper_directive : DECLARE MAPPER { current_directive = new OpenMPDeclareMapperDirective(OMPD_DECLARE_MAPPER_IDENTIFIER_unspecified); } '(' mapper_list ')' declare_mapper_clause_optseq
                         ;

mapper_list : mapper_identifier_optseq 
            ;

mapper_identifier_optseq : type_var
                         | mapper_identifier ':' type_var
                         ;
 
mapper_identifier : IDENTIFIER_DEFAULT { ((OpenMPDeclareMapperDirective*)current_directive)->setIdentifier(OMPD_DECLARE_MAPPER_IDENTIFIER_default); }
                  | EXPR_STRING { ((OpenMPDeclareMapperDirective*)current_directive)->setIdentifier(OMPD_DECLARE_MAPPER_IDENTIFIER_user); ((OpenMPDeclareMapperDirective*)current_directive)->setUserDefinedIdentifier($1); }
                  ;
         
type_var : EXPR_STRING { 
               if (user_set_lang == Lang_C || auto_lang == Lang_C) { 
                   const char * _type_var = $1;
                   std::string type_var = std::string(_type_var);
                   int length = type_var.length() - 1;
                   for (int i = length; i >= 0; i--) {
                       if (type_var[i] == ' ' || type_var[i] == '*') { 
                           std::string _type = type_var.substr(0, i + 1);
                           std::string _var = type_var.substr(i + 1, length - i);
                           const char* type = _type.c_str();
                           const char* var = _var.c_str();
                           ((OpenMPDeclareMapperDirective*)current_directive)->setDeclareMapperType(type);
                           ((OpenMPDeclareMapperDirective*)current_directive)->setDeclareMapperVar(var);
                           break;
                       }
                   }
               } else {
                   yyerror("The syntax should be \"type :: var\" in Fortran"); 
                   YYABORT; 
               }
         } 
         | declare_mapper_type DOUBLE_COLON declare_mapper_var { if (user_set_lang == Lang_C || auto_lang == Lang_C) yyerror("The syntax should be \"type var\" in C"); YYABORT; }
         ;
         
declare_mapper_type : EXPR_STRING { ((OpenMPDeclareMapperDirective*)current_directive)->setDeclareMapperType($1); }
                    ;
                    
declare_mapper_var : EXPR_STRING { ((OpenMPDeclareMapperDirective*)current_directive)->setDeclareMapperVar($1); }
                   ;

parallel_clause_optseq : /* empty */
                       | parallel_clause_seq
                       ;
teams_clause_optseq : /* empty */
                    | teams_clause_seq
                    ;

for_clause_optseq : /*empty*/
                  | for_clause_seq
                  ;
do_clause_optseq : /*empty*/
                 | do_clause_seq
                 ;
do_paried_clause_optseq : /*empty*/
                        | nowait_clause
                        ;
simd_clause_optseq : /*empty*/
                   | simd_clause_seq
                   ;
for_simd_clause_optseq : /*empty*/
                       | for_simd_clause_seq
                       ;
do_simd_clause_optseq : /*empty*/
                      | do_simd_clause_seq
                      ;
do_simd_paried_clause_optseq : /*empty*/
                             | nowait_clause
                             ;
parallel_for_simd_clause_optseq : /*empty*/
                                | parallel_for_simd_clause_seq
                                ;
declare_simd_clause_optseq : /*empty*/
                           | declare_simd_clause_seq
                           ;
distribute_clause_optseq : /*empty*/
                         | distribute_clause_seq
                         ;
distribute_simd_clause_optseq : /*empty*/
                              | distribute_simd_clause_seq
                              ;
distribute_parallel_for_clause_optseq : /*empty*/
                                      | distribute_parallel_for_clause_seq
                                      ;
distribute_parallel_do_clause_optseq : /*empty*/
                                     | distribute_parallel_do_clause_seq
                                     ;
distribute_parallel_for_simd_clause_optseq : /*empty*/
                                           | distribute_parallel_for_simd_clause_seq
                                           ;
distribute_parallel_do_simd_clause_optseq : /*empty*/
                                          | distribute_parallel_do_simd_clause_seq
                                          ;
parallel_for_clause_optseq : /*empty*/
                           | parallel_for_clause_seq
                           ;
parallel_do_clause_optseq : /*empty*/
                          | parallel_do_clause_seq
                          ;
parallel_loop_clause_optseq : /*empty*/
                            | parallel_loop_clause_seq
                            ;
parallel_sections_clause_optseq : /*empty*/
                                | parallel_sections_clause_seq
                                ;
parallel_workshare_clause_optseq : /*empty*/
                                 | parallel_workshare_clause_seq
                                 ;
parallel_master_clause_optseq : /*empty*/
                              | parallel_master_clause_seq
                              ;
master_taskloop_clause_optseq : /*empty*/
                              | master_taskloop_clause_seq
                              ;
master_taskloop_simd_clause_optseq : /*empty*/
                                   | master_taskloop_simd_clause_seq
                                   ;
parallel_master_taskloop_clause_optseq : /*empty*/
                                       | parallel_master_taskloop_clause_seq
                                       ;
parallel_master_taskloop_simd_clause_optseq : /*empty*/
                                            | parallel_master_taskloop_simd_clause_seq
                                            ;
loop_clause_optseq : /*empty*/
                   | loop_clause_seq
                   ;
scan_clause_optseq : scan_clause_seq
                   ;
sections_clause_optseq : /*empty*/
                       | sections_clause_seq
                       ;
sections_paired_clause_optseq : /*empty*/
                              | nowait_clause
                              ;
single_clause_optseq : /*empty*/
                     | single_clause_seq
                     ;
single_paired_clause_optseq : /*empty*/
                            | single_paired_clause_seq
                            ;
workshare_paired_clause_optseq : /*empty*/
                               | nowait_clause
                               ;
cancel_clause_optseq : /*empty*/
                     | cancel_clause_seq
                     ;
//cancel_clause_fortran_optseq : /*empty*/
//                             | cancel_clause_fortran_seq
//                             ;
cancellation_point_clause_optseq : /*empty*/
                                 | cancellation_point_clause_seq
                                 ;
//cancellation_point_clause_fortran_optseq : /*empty*/
//                                         | cancellation_point_clause_fortran_seq
                                         ;
allocate_clause_optseq : /*empty*/
                       | allocate_clause_seq
                       ;
allocate_clause_seq : allocate_directive_clause
                    | allocate_clause_seq allocate_directive_clause
                    | allocate_clause_seq ',' allocate_directive_clause
                    ; 



declare_reduction_clause_optseq :  /*empty*/
                                | declare_reduction_clause_seq
                                ;
declare_mapper_clause_optseq : /*empty*/
                             | declare_mapper_clause_seq
                             ;
declare_mapper_clause_seq : declare_mapper_clause
                          | declare_mapper_clause_seq declare_mapper_clause
                          | declare_mapper_clause_seq ',' declare_mapper_clause
                          ; 
parallel_clause_seq : parallel_clause
                    | parallel_clause_seq parallel_clause
                    | parallel_clause_seq ',' parallel_clause
                    ;

teams_clause_seq : teams_clause
                 | teams_clause_seq teams_clause
                 | teams_clause_seq ',' teams_clause
                 ;

for_clause_seq : for_clause
               | for_clause_seq for_clause
               | for_clause_seq ',' for_clause
               ;

do_clause_seq : do_clause
              | do_clause_seq do_clause
              | do_clause_seq ',' do_clause
              ;

simd_clause_seq : simd_clause
                | simd_clause_seq simd_clause
                | simd_clause_seq ',' simd_clause
                ;

for_simd_clause_seq : for_simd_clause
                    | for_simd_clause_seq for_simd_clause
                    | for_simd_clause_seq ',' for_simd_clause
                    ;
do_simd_clause_seq : do_simd_clause
                   | do_simd_clause_seq do_simd_clause
                   | do_simd_clause_seq ',' do_simd_clause
                   ;
parallel_for_simd_clause_seq : parallel_for_simd_clause
                             | parallel_for_simd_clause_seq parallel_for_simd_clause
                             | parallel_for_simd_clause_seq ',' parallel_for_simd_clause
                             ;
declare_simd_clause_seq : declare_simd_clause
                        | declare_simd_clause_seq declare_simd_clause
                        | declare_simd_clause_seq ',' declare_simd_clause
                        ;
distribute_clause_seq : distribute_clause
                      | distribute_clause_seq distribute_clause
                      | distribute_clause_seq ',' distribute_clause
                      ;
distribute_simd_clause_seq : distribute_simd_clause
                           | distribute_simd_clause_seq distribute_simd_clause
                           | distribute_simd_clause_seq ',' distribute_simd_clause
                           ;
distribute_parallel_for_clause_seq : distribute_parallel_for_clause
                                   | distribute_parallel_for_clause_seq distribute_parallel_for_clause
                                   | distribute_parallel_for_clause_seq ',' distribute_parallel_for_clause
                                   ;
distribute_parallel_do_clause_seq : distribute_parallel_do_clause
                                  | distribute_parallel_do_clause_seq distribute_parallel_do_clause
                                  | distribute_parallel_do_clause_seq ',' distribute_parallel_do_clause
                                  ;
distribute_parallel_for_simd_clause_seq : distribute_parallel_for_simd_clause
                                        | distribute_parallel_for_simd_clause_seq distribute_parallel_for_simd_clause
                                        | distribute_parallel_for_simd_clause_seq ',' distribute_parallel_for_simd_clause
                                        ;
distribute_parallel_do_simd_clause_seq : distribute_parallel_do_simd_clause
                                       | distribute_parallel_do_simd_clause_seq distribute_parallel_do_simd_clause
                                       | distribute_parallel_do_simd_clause_seq ',' distribute_parallel_do_simd_clause
                                       ;
parallel_for_clause_seq : parallel_for_clause
                        | parallel_for_clause_seq parallel_for_clause
                        | parallel_for_clause_seq ',' parallel_for_clause
                        ;
parallel_do_clause_seq : parallel_do_clause
                       | parallel_do_clause_seq parallel_do_clause
                       | parallel_do_clause_seq ',' parallel_do_clause
                       ;
parallel_loop_clause_seq : parallel_loop_clause
                         | parallel_loop_clause_seq parallel_loop_clause
                         | parallel_loop_clause_seq ',' parallel_loop_clause
                         ;
parallel_sections_clause_seq : parallel_sections_clause
                             | parallel_sections_clause_seq parallel_sections_clause
                             | parallel_sections_clause_seq ',' parallel_sections_clause
                             ;
parallel_workshare_clause_seq : parallel_workshare_clause
                              | parallel_workshare_clause_seq parallel_workshare_clause
                              | parallel_workshare_clause_seq ',' parallel_workshare_clause
                              ;
parallel_master_clause_seq : parallel_master_clause
                           | parallel_master_clause_seq parallel_master_clause
                           | parallel_master_clause_seq ',' parallel_master_clause
                           ;
master_taskloop_clause_seq : master_taskloop_clause
                           | master_taskloop_clause_seq master_taskloop_clause
                           | master_taskloop_clause_seq ',' master_taskloop_clause
                           ;
master_taskloop_simd_clause_seq : master_taskloop_simd_clause
                                | master_taskloop_simd_clause_seq master_taskloop_simd_clause
                                | master_taskloop_simd_clause_seq ',' master_taskloop_simd_clause
                                ;
parallel_master_taskloop_clause_seq : parallel_master_taskloop_clause
                                    | parallel_master_taskloop_clause_seq parallel_master_taskloop_clause
                                    | parallel_master_taskloop_clause_seq ',' parallel_master_taskloop_clause
                                    ;
parallel_master_taskloop_simd_clause_seq : parallel_master_taskloop_simd_clause
                                         | parallel_master_taskloop_simd_clause_seq parallel_master_taskloop_simd_clause
                                         | parallel_master_taskloop_simd_clause_seq ',' parallel_master_taskloop_simd_clause
                                         ;
loop_clause_seq : loop_clause
                | loop_clause_seq loop_clause
                | loop_clause_seq ',' loop_clause
                ;
scan_clause_seq : scan_clause
                ;
sections_clause_seq : sections_clause
                    | sections_clause_seq sections_clause
                    | sections_clause_seq ',' sections_clause
                    ;
//sections_clause_fortran_seq : sections_fortran_clause
//                            | sections_clause_fortran_seq sections_fortran_clause
//                            | sections_clause_fortran_seq ',' sections_fortran_clause
//                            ;
single_clause_seq : single_clause
                  | single_clause_seq single_clause
                  | single_clause_seq ',' single_clause
                  ;
single_paired_clause_seq : single_paired_clause
                         | single_paired_clause_seq single_paired_clause
                         | single_paired_clause_seq ',' single_paired_clause
                         ;
cancel_clause_seq : construct_type_clause
                  | construct_type_clause if_cancel_clause
                  | construct_type_clause ',' if_cancel_clause
                  ;
//cancel_clause_fortran_seq : construct_type_clause_fortran
//                          | construct_type_clause_fortran if_cancel_clause
//                          | construct_type_clause_fortran ',' if_cancel_clause
//                          ;
cancellation_point_clause_seq : construct_type_clause
                              ;
//cancellation_point_clause_fortran_seq : construct_type_clause_fortran
//                                      ;
allocate_directive_clause : allocator_clause
                          | align_clause
                          ;
declare_reduction_clause_seq : initializer_clause
                             ;
declare_mapper_clause : map_clause
                      ;
parallel_clause : if_parallel_clause
                | num_threads_clause
                | default_clause
                | private_clause
                | firstprivate_clause
                | shared_clause
                | copyin_clause
                | reduction_clause
                | proc_bind_clause
                | allocate_clause
                ;
teams_clause : num_teams_clause
             | thread_limit_clause
             | default_clause
             | private_clause
             | firstprivate_clause
             | shared_clause
             | reduction_default_only_clause
             | allocate_clause
             ;

for_clause : private_clause
           | firstprivate_clause
           | lastprivate_clause
           | linear_clause
           | reduction_clause
           | schedule_clause
           | collapse_clause
           | ordered_clause
           | nowait_clause
           | allocate_clause
           | order_clause
           ;

do_clause : private_clause
          | firstprivate_clause
          | lastprivate_clause
          | linear_clause
          | reduction_clause
          | schedule_clause
          | collapse_clause
          | ordered_clause
          | allocate_clause
          | order_clause
          ;

simd_clause : if_simd_clause
            | safelen_clause
            | simdlen_clause
            | linear_clause
            | aligned_clause
            | nontemporal_clause
            | private_clause
            | lastprivate_clause
            | reduction_clause
            | collapse_clause
            | order_clause
            ;

for_simd_clause : if_simd_clause
                | safelen_clause
                | simdlen_clause
                | linear_clause
                | aligned_clause
                | private_clause
                | firstprivate_clause 
                | lastprivate_clause
                | reduction_clause
                | schedule_clause
                | collapse_clause
                | ordered_clause
                | nowait_clause
                | allocate_clause
                | order_clause
                | nontemporal_clause
                ;
do_simd_clause : if_simd_clause
               | safelen_clause
               | simdlen_clause
               | linear_clause
               | aligned_clause
               | private_clause 
               | firstprivate_clause 
               | lastprivate_clause
               | reduction_clause
               | schedule_clause
               | collapse_clause
               | ordered_clause
               | allocate_clause
               | order_clause
               | nontemporal_clause
               ;
parallel_for_simd_clause : if_parallel_simd_clause
                         | num_threads_clause
                         | default_clause
                         | private_clause
                         | firstprivate_clause
                         | shared_clause
                         | copyin_clause
                         | reduction_clause
                         | proc_bind_clause
                         | allocate_clause
                         | lastprivate_clause 
                         | linear_clause
                         | schedule_clause
                         | collapse_clause
                         | ordered_clause
                         | order_clause
                         | safelen_clause
                         | simdlen_clause
                         | aligned_clause
                         | nontemporal_clause
                         ;
 
declare_simd_clause : simdlen_clause
                    | linear_clause
                    | aligned_clause
                    | uniform_clause
                    | inbranch_clause
                    | notinbranch_clause
                    ;
 
distribute_clause : private_clause
                  | firstprivate_clause 
                  | lastprivate_distribute_clause
                  | collapse_clause
                  | dist_schedule_clause
                  | allocate_clause
                  ;
distribute_simd_clause : private_clause
                       | firstprivate_clause 
                       | lastprivate_clause
                       | collapse_clause
                       | dist_schedule_clause
                       | allocate_clause
                       | if_simd_clause
                       | safelen_clause
                       | simdlen_clause
                       | linear_clause
                       | aligned_clause
                       | nontemporal_clause
                       | reduction_clause
                       | order_clause
                       ;
distribute_parallel_for_clause : if_parallel_clause
                               | num_threads_clause
                               | default_clause
                               | private_clause
                               | firstprivate_clause
                               | shared_clause
                               | copyin_clause
                               | reduction_clause
                               | proc_bind_clause
                               | allocate_clause
                               | lastprivate_clause 
                               | linear_clause
                               | schedule_clause
                               | collapse_clause
                               | ordered_clause
                               | nowait_clause
                               | order_clause 
                               | dist_schedule_clause
                               ;
distribute_parallel_do_clause : if_parallel_clause
                              | num_threads_clause
                              | default_clause
                              | private_clause
                              | firstprivate_clause
                              | shared_clause
                              | copyin_clause
                              | reduction_clause
                              | proc_bind_clause
                              | allocate_clause
                              | lastprivate_clause 
                              | linear_clause
                              | schedule_clause
                              | collapse_clause
                              | ordered_clause
                              | order_clause 
                              | dist_schedule_clause
                              ;
distribute_parallel_for_simd_clause : if_parallel_simd_clause
                                    | num_threads_clause
                                    | default_clause
                                    | private_clause
                                    | firstprivate_clause
                                    | shared_clause
                                    | copyin_clause
                                    | reduction_clause
                                    | proc_bind_clause
                                    | allocate_clause
                                    | lastprivate_clause 
                                    | linear_clause
                                    | schedule_clause
                                    | collapse_clause
                                    | ordered_clause
                                    | nowait_clause
                                    | order_clause 
                                    | dist_schedule_clause
                                    | safelen_clause
                                    | simdlen_clause
                                    | aligned_clause
                                    | nontemporal_clause
                                    ;
distribute_parallel_do_simd_clause : if_parallel_simd_clause
                                   | num_threads_clause
                                   | default_clause
                                   | private_clause
                                   | firstprivate_clause
                                   | shared_clause
                                   | copyin_clause
                                   | reduction_clause
                                   | proc_bind_clause
                                   | allocate_clause
                                   | lastprivate_clause 
                                   | linear_clause
                                   | schedule_clause
                                   | collapse_clause
                                   | ordered_clause
                                   | order_clause 
                                   | dist_schedule_clause
                                   | safelen_clause
                                   | simdlen_clause
                                   | aligned_clause
                                   | nontemporal_clause
                                   ;
parallel_for_clause : if_parallel_clause
                    | num_threads_clause
                    | default_clause
                    | private_clause
                    | firstprivate_clause
                    | shared_clause
                    | copyin_clause
                    | reduction_clause
                    | proc_bind_clause
                    | allocate_clause
                    | lastprivate_clause 
                    | linear_clause
                    | schedule_clause
                    | collapse_clause
                    | ordered_clause
                    | nowait_clause
                    | order_clause 
                    ;
parallel_do_clause : if_parallel_clause
                   | num_threads_clause
                   | default_clause
                   | private_clause
                   | firstprivate_clause
                   | shared_clause
                   | copyin_clause
                   | reduction_clause
                   | proc_bind_clause
                   | allocate_clause
                   | lastprivate_clause 
                   | linear_clause
                   | schedule_clause
                   | collapse_clause
                   | ordered_clause
                   | order_clause 
                   ;
parallel_loop_clause : if_parallel_clause
                     | num_threads_clause
                     | default_clause
                     | private_clause
                     | firstprivate_clause
                     | shared_clause
                     | copyin_clause
                     | reduction_clause
                     | proc_bind_clause
                     | allocate_clause
                     | lastprivate_clause 
                     | collapse_clause
                     | bind_clause
                     | order_clause 
                     ;
parallel_sections_clause : if_parallel_clause
                         | num_threads_clause
                         | default_clause
                         | private_clause
                         | firstprivate_clause
                         | shared_clause
                         | copyin_clause
                         | reduction_clause
                         | proc_bind_clause
                         | allocate_clause
                         | lastprivate_clause 
                         ;
parallel_workshare_clause : if_parallel_clause
                          | num_threads_clause
                          | default_clause
                          | private_clause
                          | firstprivate_clause
                          | shared_clause
                          | copyin_clause
                          | reduction_clause
                          | proc_bind_clause
                          | allocate_clause
                          ;
parallel_master_clause : if_parallel_clause
                       | num_threads_clause
                       | default_clause
                       | private_clause
                       | firstprivate_clause
                       | shared_clause
                       | copyin_clause
                       | reduction_clause
                       | proc_bind_clause
                       | allocate_clause
                       ;
master_taskloop_clause : if_taskloop_clause
                       | shared_clause
                       | private_clause
                       | firstprivate_clause
                       | lastprivate_clause
                       | reduction_clause
                       | in_reduction_clause
                       | default_clause
                       | grainsize_clause
                       | num_tasks_clause
                       | collapse_clause
                       | final_clause
                       | priority_clause
                       | untied_clause
                       | mergeable_clause
                       | nogroup_clause
                       | allocate_clause
                       ;
master_taskloop_simd_clause : if_taskloop_simd_clause
                            | shared_clause
                            | private_clause
                            | firstprivate_clause
                            | lastprivate_clause
                            | reduction_clause
                            | in_reduction_clause
                            | default_clause
                            | grainsize_clause
                            | num_tasks_clause
                            | collapse_clause
                            | final_clause
                            | priority_clause
                            | untied_clause
                            | mergeable_clause
                            | nogroup_clause
                            | allocate_clause
                            | safelen_clause
                            | simdlen_clause
                            | linear_clause
                            | aligned_clause
                            | nontemporal_clause
                            | order_clause 
                            ;
parallel_master_taskloop_clause : if_parallel_taskloop_clause
                                | num_threads_clause
                                | default_clause
                                | private_clause
                                | firstprivate_clause
                                | shared_clause
                                | copyin_clause
                                | reduction_clause
                                | proc_bind_clause
                                | allocate_clause
                                | lastprivate_clause 
                                | nowait_clause 
                                | grainsize_clause
                                | num_tasks_clause
                                | collapse_clause
                                | final_clause
                                | priority_clause
                                | untied_clause
                                | mergeable_clause
                                | nogroup_clause
                                ;
parallel_master_taskloop_simd_clause : if_parallel_taskloop_simd_clause
                                     | num_threads_clause
                                     | default_clause
                                     | private_clause
                                     | firstprivate_clause
                                     | shared_clause
                                     | copyin_clause
                                     | reduction_clause
                                     | proc_bind_clause
                                     | allocate_clause
                                     | lastprivate_clause 
                                     | nowait_clause 
                                     | grainsize_clause
                                     | num_tasks_clause
                                     | collapse_clause
                                     | final_clause
                                     | priority_clause
                                     | untied_clause
                                     | mergeable_clause
                                     | nogroup_clause
                                     | safelen_clause
                                     | simdlen_clause
                                     | linear_clause
                                     | aligned_clause
                                     | nontemporal_clause
                                     | order_clause
                                     ;
loop_clause : bind_clause
            | collapse_clause
            | order_clause
            | private_clause
            | lastprivate_clause
            | reduction_default_only_clause
            ;
scan_clause : inclusive_clause
            | exclusive_clause
            ;
sections_clause : private_clause
                | firstprivate_clause
                | lastprivate_clause
                | reduction_clause
                | allocate_clause
                | fortran_nowait_clause
                ;
single_clause : private_clause
              | firstprivate_clause
              | fortran_copyprivate_clause
              | allocate_clause
              | fortran_nowait_clause
              ;
single_paired_clause : copyprivate_clause
                     | nowait_clause
                     ;
construct_type_clause : PARALLEL { current_clause = current_directive->addOpenMPClause(OMPC_parallel); }
                      | SECTIONS { current_clause = current_directive->addOpenMPClause(OMPC_sections); }
                      | FOR { if (user_set_lang != Lang_Fortran || auto_lang != Lang_Fortran) {current_clause = current_directive->addOpenMPClause(OMPC_for);} else {yyerror("cancel or cancellation direcitve does not support for clause in Fortran"); YYABORT; } }
                      | DO { if (user_set_lang == Lang_Fortran || auto_lang == Lang_Fortran) {current_clause = current_directive->addOpenMPClause(OMPC_do);} else {yyerror("cancel or cancellation direcitve does not support DO clause in C"); YYABORT; } }
                      | TASKGROUP { current_clause = current_directive->addOpenMPClause(OMPC_taskgroup); }
                      ;
//construct_type_clause_fortran : PARALLEL { current_clause = current_directive->addOpenMPClause(OMPC_parallel); }
//                              | SECTIONS { current_clause = current_directive->addOpenMPClause(OMPC_sections); }
//                              | DO { current_clause = current_directive->addOpenMPClause(OMPC_do); }
//                              | TASKGROUP { current_clause = current_directive->addOpenMPClause(OMPC_taskgroup); }
//                              ;
if_parallel_clause : IF '(' if_parallel_parameter ')' { ; }
                   ;

if_parallel_parameter : PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); }
                        expression { ; }
                      | EXPR_STRING {
                        current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                        current_clause->addLangExpr($1);
                        }
                      ;
if_task_clause : IF '(' if_task_parameter ')' { ; }
               ;

if_task_parameter : TASK ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_task); } expression { ; }
                  | EXPR_STRING {
                        current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                        current_clause->addLangExpr($1);
                        }
                  ;
if_taskloop_clause : IF '(' if_taskloop_parameter ')' { ; }
                   ;

if_taskloop_parameter : TASKLOOP ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_taskloop); } expression { ; }
                      | EXPR_STRING {
                            current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                            current_clause->addLangExpr($1);
                        }
                      ;
if_target_data_clause : IF '(' if_target_data_parameter ')' { ; }
                      ;

if_target_data_parameter : TARGET DATA ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_data); } expression { ; }
                         | EXPR_STRING {
                               current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                               current_clause->addLangExpr($1);
                           } 
                         ;
if_target_parallel_clause : IF '(' if_target_parallel_parameter ')' { ; }
                          ;

if_target_parallel_parameter : TARGET ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target); } expression { ; }
                             | PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); } expression { ; }
                             | EXPR_STRING {
                               current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                               current_clause->addLangExpr($1);
                           } 
                             ;
if_target_simd_clause : IF '(' if_target_simd_parameter ')' { ; }
                      ;

if_target_simd_parameter : TARGET ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target); } expression { ; }
                         | SIMD ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd); } expression { ; }
                         | EXPR_STRING {
                               current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                               current_clause->addLangExpr($1);
                           } 
                             ;
if_target_enter_data_clause : IF '(' if_target_enter_data_parameter ')' { ; }
                            ;

if_target_enter_data_parameter : TARGET ENTER DATA ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_enter_data); } expression { ; }
                               | EXPR_STRING {
                                     current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                     current_clause->addLangExpr($1);
                                 }
                               ;
if_target_exit_data_clause : IF '(' if_target_exit_data_parameter ')' { ; }
                           ;

if_target_exit_data_parameter : TARGET EXIT DATA ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_exit_data); } expression { ; }
                              | EXPR_STRING {
                                    current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                    current_clause->addLangExpr($1);
                                }
                              ;
if_target_clause : IF '(' if_target_parameter ')' { ; }
                 ;

if_target_parameter : TARGET ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target); } expression { ; }
                    | EXPR_STRING {
                          current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                          current_clause->addLangExpr($1);
                      }
                    ;
if_target_update_clause : IF '(' if_target_update_parameter ')' { ; }
                        ;

if_target_update_parameter : TARGET UPDATE ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_update); } expression { ; }
                           | EXPR_STRING {
                                 current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                 current_clause->addLangExpr($1);
                             }
                           ;
if_taskloop_simd_clause : IF '(' if_taskloop_simd_parameter ')' { ; }
                        ;

if_taskloop_simd_parameter : TASKLOOP ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_taskloop); } expression { ; }
                           | SIMD ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd); } expression { ; }
                           | EXPR_STRING {
                                 current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                 current_clause->addLangExpr($1);
                             }
                           ;
if_simd_clause : IF '(' if_simd_parameter ')' { ; }
               ;
if_simd_parameter : SIMD ':' {current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd);} expression { ; }
                  | EXPR_STRING {
                        current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                        current_clause->addLangExpr($1);
                        }
                  ;
if_parallel_simd_clause : IF '(' if_parallel_simd_parameter ')' { ; }
                        ;
if_parallel_simd_parameter : SIMD ':' {current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd);} expression { ; }
                           | PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); } expression { ; }
                           | EXPR_STRING {
                                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                current_clause->addLangExpr($1);
                           }
                           ;
if_target_parallel_simd_clause : IF '(' if_target_parallel_simd_parameter ')' { ; }
                               ;
if_target_parallel_simd_parameter : SIMD ':' {current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd);} expression { ; }
                                  | PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); } expression { ; }
                                  | TARGET ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target); } expression { ; }
                                  | EXPR_STRING {
                                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                current_clause->addLangExpr($1);
                           }
                                  ;
if_cancel_clause : IF '(' if_cancel_parameter ')' { ; }
                 ;
if_cancel_parameter : CANCEL ':' {current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_cancel);} expression { ; }
                    | EXPR_STRING {
                        current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                        current_clause->addLangExpr($1);
                        }
                    ;
if_parallel_taskloop_clause : IF '(' if_parallel_taskloop_parameter ')' { ; }
                            ;
if_parallel_taskloop_parameter : PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); } expression { ; }
                               | TASKLOOP ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_taskloop); } expression { ; }
                               | EXPR_STRING {
                               current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                               current_clause->addLangExpr($1);
                                }
                               ;
if_parallel_taskloop_simd_clause : IF '(' if_parallel_taskloop_simd_parameter ')' { ; }
                                 ;
if_parallel_taskloop_simd_parameter : PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); } expression { ; }
                                    | TASKLOOP ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_taskloop); } expression { ; }
                                    | SIMD ':' {current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd);} expression { ; }
                                    | EXPR_STRING {
                                      current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                                      current_clause->addLangExpr($1);
                                    }
                                    ;
/*if_clause : IF '(' if_parameter ')' { ; }
          ;

if_parameter : EXPR_STRING {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                current_clause->addLangExpr($1);
                }
             ;
*/
num_threads_clause: NUM_THREADS {
                            current_clause = current_directive->addOpenMPClause(OMPC_num_threads);
                         } '(' expression ')'
                  ;
num_teams_clause: NUM_TEAMS {
                            current_clause = current_directive->addOpenMPClause(OMPC_num_teams);
                         } '(' expression ')'
                ;
align_clause: ALIGN {
                  current_clause = current_directive->addOpenMPClause(OMPC_align);
                  } '(' expression ')'
            ;
                
thread_limit_clause: THREAD_LIMIT { current_clause = current_directive->addOpenMPClause(OMPC_thread_limit); } '(' expression ')'
                   ;
copyin_clause: COPYIN {
                current_clause = current_directive->addOpenMPClause(OMPC_copyin);
                } '(' var_list ')'
             ;

default_clause : DEFAULT '(' default_parameter ')' { } 
               ;

default_parameter : SHARED { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_shared); }
                  | NONE { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_none); }
                  | FIRSTPRIVATE { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_firstprivate); }
                  | PRIVATE { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_private); }
                  ;

default_variant_clause : DEFAULT '(' default_variant_directive ')' { }
                       ;

default_variant_directive : { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_variant);
                            current_parent_directive = current_directive;
                            current_parent_clause = current_clause; } variant_directive {
                            ((OpenMPDefaultClause*)current_parent_clause)->setVariantDirective(current_directive);
                            current_directive = current_parent_directive;
                            current_clause = current_parent_clause;
                            current_parent_directive = NULL;
                            current_parent_clause = NULL;
                            std::cout << "A construct directive has been added to DEFAULT clause.\n";
                            }
                          ;

proc_bind_clause : PROC_BIND '(' proc_bind_parameter ')' { } ;

proc_bind_parameter : MASTER { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_master); }
                    | CLOSE { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_close); }
                    | SPREAD { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_spread); }
                    ;
bind_clause : BIND '(' bind_parameter ')' { } ;

bind_parameter : TEAMS { current_clause = current_directive->addOpenMPClause(OMPC_bind, OMPC_BIND_teams); }
               | PARALLEL { current_clause = current_directive->addOpenMPClause(OMPC_bind, OMPC_BIND_parallel); }
               | THREAD { current_clause = current_directive->addOpenMPClause(OMPC_bind, OMPC_BIND_thread); }
               ;
allocate_clause : ALLOCATE '(' allocate_parameter ')' ;

allocate_parameter : EXPR_STRING  { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_unspecified); current_clause->addLangExpr($1);  }
                   | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_unspecified); current_clause->addLangExpr($1); } var_list
                   | allocator_parameter ':' { ; } var_list
                   ;
allocator_parameter : DEFAULT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_default); }
                    | LARGE_CAP_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_large_cap); }
                    | CONST_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cons_mem); }
                    | HIGH_BW_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_high_bw); }
                    | LOW_LAT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_low_lat); }
                    | CGROUP_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cgroup); }
                    | PTEAM_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_pteam); }
                    | THREAD_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_thread); }
                    | EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_user, $1); }
                    ;

private_clause : PRIVATE {
                current_clause = current_directive->addOpenMPClause(OMPC_private);
                    } '(' var_list ')' { 
               }
               ;

firstprivate_clause : FIRSTPRIVATE {
                         current_clause = current_directive->addOpenMPClause(OMPC_firstprivate);
                        } '(' var_list ')' {
                    }
                    ;

copyprivate_clause : COPYPRIVATE {
                           current_clause = current_directive->addOpenMPClause(OMPC_copyprivate);
                        } '(' var_list ')' {
                   }
                   ;
fortran_copyprivate_clause : COPYPRIVATE {
                                 if (user_set_lang == Lang_C || auto_lang == Lang_C) {current_clause = current_directive->addOpenMPClause(OMPC_copyprivate);} else {yyerror("Single does not support copyprivate_clause in Fortran."); YYABORT;}
                               } '(' var_list ')' {
                           }
                           ;
lastprivate_clause : LASTPRIVATE '(' lastprivate_parameter ')' ;

lastprivate_parameter : EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_lastprivate, OMPC_LASTPRIVATE_MODIFIER_unspecified); current_clause->addLangExpr($1); }
                      | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_lastprivate, OMPC_LASTPRIVATE_MODIFIER_unspecified); current_clause->addLangExpr($1); } var_list
                      | lastprivate_modifier ':'{;} var_list
                      ;

lastprivate_distribute_clause : LASTPRIVATE {
                         current_clause = current_directive->addOpenMPClause(OMPC_lastprivate);
                        } '(' var_list ')' {
                    }

lastprivate_modifier : MODIFIER_CONDITIONAL { current_clause = current_directive->addOpenMPClause(OMPC_lastprivate,OMPC_LASTPRIVATE_MODIFIER_conditional); }
                     ;

linear_clause : LINEAR '(' linear_parameter ')'
              | LINEAR '(' linear_parameter ':' EXPR_STRING { ((OpenMPLinearClause*)current_clause)->setUserDefinedStep($5); ((OpenMPLinearClause*)current_clause)->mergeLinear(current_directive, current_clause); } ')' 
              ;

linear_parameter : EXPR_STRING  { current_clause = current_directive->addOpenMPClause(OMPC_linear, OMPC_LINEAR_MODIFIER_unspecified); current_clause->addLangExpr($1); }
                 | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_linear, OMPC_LINEAR_MODIFIER_unspecified); current_clause->addLangExpr($1); } var_list
                 | linear_modifier '(' var_list ')'
                 ;
linear_modifier : MODOFIER_VAL { current_clause = current_directive->addOpenMPClause(OMPC_linear,OMPC_LINEAR_MODIFIER_val); }
                | MODOFIER_REF { if (user_set_lang == Lang_unknown && auto_lang == Lang_C){ auto_lang = Lang_Cplusplus; } if (user_set_lang == Lang_C) {yyerror("REF modifier is not supportted in C."); YYABORT; } else { current_clause = current_directive->addOpenMPClause(OMPC_linear, OMPC_LINEAR_MODIFIER_ref); } }
                | MODOFIER_UVAL { if (user_set_lang == Lang_unknown && auto_lang == Lang_C){ auto_lang = Lang_Cplusplus; } if (user_set_lang == Lang_C) {yyerror("UVAL modifier is not supportted in C."); YYABORT;} else { current_clause = current_directive->addOpenMPClause(OMPC_linear, OMPC_LINEAR_MODIFIER_uval); } }
                ;

aligned_clause : ALIGNED '(' aligned_parameter ')'
               | ALIGNED '(' aligned_parameter ':' EXPR_STRING { ((OpenMPAlignedClause*)current_clause)->setUserDefinedAlignment($5);} ')'
               ;


aligned_parameter : EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1);  }
                  | EXPR_STRING ',' {current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1); } var_list
                  ;

initializer_clause : INITIALIZER '('initializer_expr')';

initializer_expr : OMP_PRIV '=' expr;

expr: EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_initializer, OMPC_INITIALIZER_PRIV_user, $1); };

safelen_clause: SAFELEN { current_clause = current_directive->addOpenMPClause(OMPC_safelen); } '(' var_list ')' {
                        }
              ;

simdlen_clause: SIMDLEN { current_clause = current_directive->addOpenMPClause(OMPC_simdlen); } '(' var_list ')' {
                        }
              ;

nontemporal_clause: NONTEMPORAL { current_clause = current_directive->addOpenMPClause(OMPC_nontemporal); } '(' var_list ')' {
                        }
                      ;

collapse_clause: COLLAPSE { current_clause = current_directive->addOpenMPClause(OMPC_collapse); } '(' expression ')' {
                        }
               ;

ordered_clause: ORDERED { current_clause = current_directive->addOpenMPClause(OMPC_ordered); } '(' expression ')'
              | ORDERED { current_clause = current_directive->addOpenMPClause(OMPC_ordered); }
              ;
partial_clause: PARTIAL { current_clause = current_directive->addOpenMPClause(OMPC_partial); } '(' expression ')'
              | PARTIAL { current_clause = current_directive->addOpenMPClause(OMPC_partial); }
              ;
fortran_nowait_clause: NOWAIT { if (user_set_lang == Lang_C || auto_lang == Lang_C) {current_clause = current_directive->addOpenMPClause(OMPC_nowait);} else {yyerror("Sections does not support nowait clause in Fortran."); YYABORT;} }
                     ;
nowait_clause: NOWAIT { current_clause = current_directive->addOpenMPClause(OMPC_nowait); }
             ;
full_clause: FULL { current_clause = current_directive->addOpenMPClause(OMPC_full); }
             ;
order_clause: ORDER '(' order_parameter ')' { }
            ;

order_parameter : CONCURRENT { current_clause = current_directive->addOpenMPClause(OMPC_order, OMPC_ORDER_concurrent); }
                ;

uniform_clause: UNIFORM { current_clause = current_directive->addOpenMPClause(OMPC_uniform); } '(' var_list ')'
              ;

inbranch_clause: INBRANCH { current_clause = current_directive->addOpenMPClause(OMPC_inbranch); }
               ;

notinbranch_clause: NOTINBRANCH { current_clause = current_directive->addOpenMPClause(OMPC_notinbranch); }
                  ;
inclusive_clause: INCLUSIVE { current_clause = current_directive->addOpenMPClause(OMPC_inclusive); } '(' var_list ')'
                ;
exclusive_clause: EXCLUSIVE { current_clause = current_directive->addOpenMPClause(OMPC_exclusive); } '(' var_list ')'
                ;
allocator_clause: ALLOCATOR '(' allocator1_parameter ')';
allocator1_parameter : DEFAULT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_default); }
                     | LARGE_CAP_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_large_cap); }
                     | CONST_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_cons_mem); }
                     | HIGH_BW_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_high_bw); }
                     | LOW_LAT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_low_lat); }
                     | CGROUP_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_cgroup); }
                     | PTEAM_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_pteam); }
                     | THREAD_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_thread); }
                     | EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_user, $1); }
                     ;

dist_schedule_clause : DIST_SCHEDULE '(' dist_schedule_parameter ')' {}
                     ;
dist_schedule_parameter : STATIC { current_clause = current_directive->addOpenMPClause(OMPC_dist_schedule,OMPC_DIST_SCHEDULE_KIND_static); }
                        | STATIC { current_clause = current_directive->addOpenMPClause(OMPC_dist_schedule,OMPC_DIST_SCHEDULE_KIND_static); } ',' EXPR_STRING { ((OpenMPDistScheduleClause*)current_clause)->setChunkSize($4); }
                        ;
schedule_clause : SCHEDULE { firstParameter = OMPC_SCHEDULE_MODIFIER_unspecified; secondParameter = OMPC_SCHEDULE_MODIFIER_unspecified; }'(' schedule_parameter ')' {
                }
                ;

schedule_parameter : schedule_kind {}
                   | schedule_modifier ':' schedule_kind
                   ;


schedule_kind : schedule_enum_kind { }
              | schedule_enum_kind ','  EXPR_STRING { ((OpenMPScheduleClause*)current_clause)->setChunkSize($3); }
              ;

schedule_modifier : schedule_enum_modifier ',' schedule_modifier2
                  | schedule_enum_modifier
                  ;

schedule_modifier2 : MODIFIER_MONOTONIC { if (firstParameter == OMPC_SCHEDULE_MODIFIER_simd) {secondParameter = OMPC_SCHEDULE_MODIFIER_monotonic;} else{yyerror("Two modifiers are incorrect"); YYABORT; } }
                   | MODIFIER_NONMONOTONIC { if (firstParameter == OMPC_SCHEDULE_MODIFIER_simd){secondParameter = OMPC_SCHEDULE_MODIFIER_nonmonotonic;}else{yyerror("Two modifiers are incorrect"); YYABORT; } }
                   | MODIFIER_SIMD { if (firstParameter == OMPC_SCHEDULE_MODIFIER_simd){yyerror("Two modifiers are incorrect"); YYABORT; } else{secondParameter = OMPC_SCHEDULE_MODIFIER_simd;} }
                   ;
schedule_enum_modifier : MODIFIER_MONOTONIC { firstParameter = OMPC_SCHEDULE_MODIFIER_monotonic; }
                       | MODIFIER_NONMONOTONIC { firstParameter = OMPC_SCHEDULE_MODIFIER_nonmonotonic; }
                       | MODIFIER_SIMD { firstParameter = OMPC_SCHEDULE_MODIFIER_simd; }
                       ;

schedule_enum_kind : STATIC { if (current_directive!= NULL)current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_static); }
                   | DYNAMIC { if (current_directive!= NULL)current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_dynamic); }
                   | GUIDED { if (current_directive!= NULL)current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_guided); }
                   | AUTO { if (current_directive!= NULL)current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_auto); }
                   | RUNTIME { if (current_directive!= NULL)current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_runtime); }
                   ;  
shared_clause : SHARED {
                current_clause = current_directive->addOpenMPClause(OMPC_shared);
                    } '(' var_list ')'
              ;

reduction_clause : REDUCTION { firstParameter = OMPC_REDUCTION_MODIFIER_unspecified; } '(' reduction_parameter ':' var_list ')' {
                 }
                 ;

reduction_parameter : reduction_identifier {}
                    | reduction_modifier ',' reduction_identifier
                    ;

reduction_identifier : reduction_enum_identifier {}
                     | EXPR_STRING { current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_user, $1); }
                     ;

reduction_modifier : MODIFIER_INSCAN { firstParameter = OMPC_REDUCTION_MODIFIER_inscan; }
                   | MODIFIER_TASK { firstParameter = OMPC_REDUCTION_MODIFIER_task; }
                   | MODIFIER_DEFAULT { firstParameter = OMPC_REDUCTION_MODIFIER_default; }
                   ;

reduction_enum_identifier : '+'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_plus); }
                          | '-'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_minus); }
                          | '*'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_mul); }
                          | '&'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitand); }
                          | '|'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitor); }
                          | '^'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitxor); }
                          | LOGAND{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_logand); }
                          | LOGOR{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_logor); }
                          | MAX{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_max); }
                          | MIN{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_min); }
                          ;

reduction_default_only_clause : REDUCTION { firstParameter = OMPC_REDUCTION_MODIFIER_unspecified; } '(' reduction_default_only_parameter ':' var_list ')' {
                              }
                              ;

reduction_default_only_parameter : reduction_identifier {}
                                 | reduction_default_only_modifier ',' reduction_identifier
                                 ;

reduction_default_only_modifier : MODIFIER_DEFAULT { firstParameter = OMPC_REDUCTION_MODIFIER_default; }
                                ;

%%

int yyerror(const char *s) {
    // printf(" %s!\n", s);
    fprintf(stderr,"error: %s\n",s);
    current_directive = NULL;
    return 0;
}
 
int yywrap()
{
    return 1;
} 

// Standalone ompparser
OpenMPDirective* parseOpenMP(const char* _input, void * _exprParse(const char*)) {
    OpenMPBaseLang base_lang = Lang_C;
    current_directive = NULL;
    std::string input_string;
    const char *input = _input;
    std::regex fortran_regex ("[!c*][$][Oo][Mm][Pp]");
    input_string = std::string(input, 5);
    if (user_set_lang == Lang_unknown){
        auto_lang = Lang_C;
        exprParse = _exprParse;
        if (std::regex_match(input_string, fortran_regex)) {
            base_lang = Lang_Fortran;
            auto_lang = Lang_Fortran;
            input_string = std::string(input);
            std::transform(input_string.begin(), input_string.end(), input_string.begin(), ::tolower);
            input = input_string.c_str();
        };
    } else {
        base_lang = user_set_lang;
        exprParse = _exprParse;
        if (std::regex_match(input_string, fortran_regex)) {
            if (user_set_lang != Lang_Fortran){
                yyerror("The language is set to C/C++, but the input is Fortran.");
                return NULL;
            }
        } else {
            if (user_set_lang == Lang_Fortran){
                yyerror("The language is set to Fortran, but the input is C/C++.");
                return NULL;
            }
        };
    }
    start_lexer(input);
    yyparse();
    end_lexer();
    if (current_directive) {
        current_directive->setBaseLang(base_lang);
    };
    return current_directive;
}
