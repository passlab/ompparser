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
static int firstParameter;
static int secondParameter;
static int thirdParameter;
std::string fourthParameter;
std::vector<const char*>* iterator_definition = new std::vector<const char*>();
std::vector<const char*>* depend_iterator_definition = new std::vector<const char*>();
std::vector<std::vector<const char*>* > depend_iterators_definition_class;
static const char* trait_score = "";

/* Treat the entire expression as a string for now */
extern void openmp_parse_expr();
static int openmp_error(const char*);
static const char* orig_str;
void * (*exprParse)(const char*) = NULL;

bool b_within_variable_list  = false;  // a flag to indicate if the program is now processing a list of variables
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


%token  OMP PARALLEL FOR DECLARE DISTRIBUTE LOOP SCAN SECTIONS SECTION SINGLE CANCEL TASKGROUP CANCELLATION POINT THREAD VARIANT THREADPRIVATE METADIRECTIVE MAPPER
        IF NUM_THREADS DEFAULT PRIVATE FIRSTPRIVATE SHARED COPYIN REDUCTION PROC_BIND ALLOCATE SIMD TASK LASTPRIVATE WHEN MATCH
        LINEAR SCHEDULE COLLAPSE NOWAIT ORDER ORDERED MODIFIER_CONDITIONAL MODIFIER_MONOTONIC MODIFIER_NOMONOTONIC STATIC DYNAMIC GUIDED AUTO RUNTIME MODOFIER_VAL MODOFIER_REF MODOFIER_UVAL MODIFIER_SIMD
        SAFELEN SIMDLEN ALIGNED NONTEMPORAL UNIFORM INBRANCH NOTINBRANCH DIST_SCHEDULE BIND INCLUSIVE EXCLUSIVE COPYPRIVATE ALLOCATOR INITIALIZER OMP_PRIV IDENTIFIER_DEFAULT/*YAYING*/
        NONE MASTER CLOSE SPREAD MODIFIER_INSCAN MODIFIER_TASK MODIFIER_DEFAULT 
        PLUS MINUS STAR BITAND BITOR BITXOR LOGAND LOGOR EQV NEQV MAX MIN
        DEFAULT_MEM_ALLOC LARGE_CAP_MEM_ALLOC CONST_MEM_ALLOC HIGH_BW_MEM_ALLOC LOW_LAT_MEM_ALLOC CGROUP_MEM_ALLOC
        PTEAM_MEM_ALLOC THREAD_MEM_ALLOC
        TEAMS
        NUM_TEAMS THREAD_LIMIT
        END USER CONSTRUCT DEVICE IMPLEMENTATION CONDITION SCORE VENDOR
        KIND HOST NOHOST ANY CPU GPU FPGA ISA ARCH EXTENSION
        AMD ARM BSC CRAY FUJITSU GNU IBM INTEL LLVM PGI TI UNKNOWN
        FINAL UNTIED MERGEABLE IN_REDUCTION DEPEND PRIORITY AFFINITY DETACH MODIFIER_ITERATOR DEPOBJ FINAL_CLAUSE IN INOUT MUTEXINOUTSET OUT
        TASKLOOP GRAINSIZE NUM_TASKS NOGROUP TASKYIELD REQUIRES REVERSE_OFFLOAD UNIFIED_ADDRESS UNIFIED_SHARED_MEMORY ATOMIC_DEFAULT_MEM_ORDER DYNAMIC_ALLOCATORS SEQ_CST ACQ_REL RELAXED
        USE_DEVICE_PTR USE_DEVICE_ADDR TARGET DATA ENTER EXIT ANCESTOR DEVICE_NUM IS_DEVICE_PTR
        DEFAULTMAP BEHAVIOR_ALLOC BEHAVIOR_TO BEHAVIOR_FROM BEHAVIOR_TOFROM BEHAVIOR_FIRSTPRIVATE BEHAVIOR_NONE BEHAVIOR_DEFAULT CATEGORY_SCALAR CATEGORY_AGGREGATE CATEGORY_POINTER UPDATE TO FROM TO_MAPPER FROM_MAPPER USES_ALLOCATORS
 LINK DEVICE_TYPE MAP MAP_MODIFIER_ALWAYS MAP_MODIFIER_CLOSE MAP_MODIFIER_MAPPER MAP_TYPE_TO MAP_TYPE_FROM MAP_TYPE_TOFROM MAP_TYPE_ALLOC MAP_TYPE_RELEASE MAP_TYPE_DELETE EXT_ BARRIER TASKWAIT FLUSH RELEASE ACQUIRE ATOMIC READ WRITE CAPTURE HINT CRITICAL SOURCE SINK DESTROY THREADS
%token <itype> ICONSTANT
%token <stype> EXPRESSION ID_EXPRESSION EXPR_STRING VAR_STRING TASK_REDUCTION
/* associativity and precedence */
%left '<' '>' '=' "!=" "<=" ">="
%left '+' '-'
%left '*' '/' '%'

%type <stype> expression

/* start point for the parsing */
%start openmp_directive

%%

/* lang-dependent expression is only used in clause, at this point, the current_clause object should already be created. */
expression : EXPR_STRING { std::cout << $1 << "\n"; current_clause->addLangExpr($1); /*void * astnode = exprParse)($1);*/ }
variable :   EXPR_STRING { std::cout << $1 << "\n"; current_clause->addLangExpr($1); } /* we use expression for variable so far */

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
                 | simd_directive
                 | teams_directive
                 | for_simd_directive
                 | declare_directive
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
                 ;

variant_directive : parallel_directive
                 | metadirective_directive
                 | declare_variant_directive
                 | for_directive
                 | simd_directive
                 | teams_directive
                 | for_simd_directive
                 | declare_directive
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
                         | metadirective_directive
                         | teams_directive
                         | sections_directive
                         | simd_directive
                         | single_directive
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

variant_func_id : '(' EXPR_STRING { std::cout << $2 << " - variant func id.\n"; ((OpenMPDeclareVariantDirective*)current_directive)->setVariantFuncID($2); } ')'
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
taskloop_simd_directive :  TASKLOOP SIMD {
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
depobj : EXPR_STRING { std::cout << $1 << "\n"; ((OpenMPDepobjDirective*)current_directive)->addDepobj($1); printf("test1\n"); } 
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

critical_name : EXPR_STRING { std::cout << $1 << " - name in critical clause.\n"; ((OpenMPCriticalDirective*)current_directive)->setCriticalName($1); }
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

extended_variable : EXPR_STRING { std::cout << $1 << "\n"; ((OpenMPDeclareTargetDirective*)current_directive)->addExtendedList($1); }
                  ;
declare_target_extended_list : extended_variable
                             | declare_target_extended_list ',' extended_variable
                             ;
flush_clause_optseq : /* empty */
                    | '(' flush_list ')'
                    | flush_clause_seq
                    ;
flush_list : extended_variable
           | flush_list ',' extended_variable
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
                  /*Do we need to care about the expression-stmt and the structure-stmt? Page235*/

atomic_clause_optseq: /* empty */
                    | atomic_clause_seq
                    | atomic_clause_class
                    | atomic_clause_seq ',' atomic_clause_class
                    | atomic_clause_seq atomic_clause_class
                    ;
atomic_clause_class : atomic_clause
                    | atomic_clause atomic_clause_seq
                    | atomic_clause ',' atomic_clause_seq
		    ;
atomic_clause_seq : atomic_clause_seq_general
                  | atomic_clause_seq atomic_clause_seq_general
                  | atomic_clause_seq ',' atomic_clause_seq_general
                  ;
atomic_clause_seq_general : memory_order_clause
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

seq_cst_clause : SEQ_CST {current_clause = current_directive->addOpenMPClause(OMPC_seq_cst); }
               ;
acq_rel_clause : ACQ_REL {current_clause = current_directive->addOpenMPClause(OMPC_acq_rel); }
               ;
release_clause : RELEASE {current_clause = current_directive->addOpenMPClause(OMPC_release); }
               ;
acquire_clause : ACQUIRE {current_clause = current_directive->addOpenMPClause(OMPC_acquire); }
               ;
relaxed_clause : RELAXED {current_clause = current_directive->addOpenMPClause(OMPC_relaxed); }
               ;

taskwait_clause_optseq : /* empty */
                       | taskwait_clause_seq
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
taskgroup_clause_seq : taskgroup_clause
                     | taskgroup_clause_seq taskgroup_clause
                     | taskgroup_clause_seq ',' taskgroup_clause
                     ;

task_clause : if_clause
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
taskloop_clause : if_clause
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
taskloop_simd_clause : if_clause
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
                | ext_implementation_defined_requirement       
                ;
target_data_clause : if_clause
                   | device_clause
                   | map_clause
                   | use_device_ptr_clause
                   | use_device_addr_clause
                   ;
target_enter_data_clause: if_clause
                        | device_clause
                        | map_clause
                        | depend_with_modifier_clause
                        | nowait_clause
                        ;
target_exit_data_clause: if_clause
                       | device_clause
                       | map_clause
                       | depend_with_modifier_clause
                       | nowait_clause
                       ;
target_clause: if_clause
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
             ;
target_update_clause: motion_clause
                    | target_update_other_clause
                    ;
motion_clause: to_clause
             | from_clause
             ;
target_update_other_clause: if_clause
                          | device_clause
                          | depend_with_modifier_clause
                          | nowait_clause
                          ;
declare_target_clause : to_clause
                      | link_clause
                      | device_type_clause
                      ;
taskwait_clause : depend_with_modifier_clause
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
                        | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_in_reduction,OMPC_IN_REDUCTION_IDENTIFIER_user, $1); }
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

 depend_with_modifier_clause : DEPEND { firstParameter = OMPC_DEPEND_MODIFIER_unknown; } '(' depend_parameter ')' {
}
                             ;

depend_parameter : dependence_type ':' var_list 
                 | depend_modifier ',' dependence_type':' var_list
                 ;
dependence_type : depend_enum_type
                ;
depend_modifier : MODIFIER_ITERATOR { firstParameter = OMPC_DEPEND_MODIFIER_iterator;
                              }'('depend_iterators_definition')'
                ;
depend_iterators_definition : depend_iterator_specifier
                            |depend_iterators_definition ',' depend_iterator_specifier
                            ;
depend_iterator_specifier : EXPR_STRING EXPR_STRING { std::cout << $1 << "\n"; depend_iterator_definition->push_back($1);depend_iterator_definition->push_back($2); } '=' depend_range_specification
                          | EXPR_STRING{depend_iterator_definition->push_back("");std::cout << $1 << "\n";depend_iterator_definition->push_back($1); }  '=' depend_range_specification 
                          ;
depend_range_specification : EXPR_STRING { std::cout << $1 << " : range begins.\n"; depend_iterator_definition->push_back($1); } ':' EXPR_STRING { std::cout << $4 << " : range ends.\n";depend_iterator_definition->push_back($4); } depend_range_step 
                           ;
depend_range_step : /*null*/ {depend_iterator_definition->push_back(""); depend_iterators_definition_class.push_back(depend_iterator_definition); depend_iterator_definition=new std::vector<const char*>(); }
                  | ':' EXPR_STRING { std::cout << $2 << " - range step.\n";depend_iterator_definition->push_back($2);depend_iterators_definition_class.push_back(depend_iterator_definition); depend_iterator_definition=new std::vector<const char*>(); }
                  ;
depend_enum_type : IN { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_in, depend_iterators_definition_class); }
                 | OUT { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_out, depend_iterators_definition_class); }
                 | INOUT { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_inout, depend_iterators_definition_class); }
                 | MUTEXINOUTSET { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_mutexinoutset); }
                 | DEPOBJ { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_depobj, depend_iterators_definition_class); }
                 ;

depend_depobj_clause : DEPEND { firstParameter = OMPC_DEPEND_MODIFIER_unknown; }'(' dependence_depobj_parameter ')' {
}
                     ;
dependence_depobj_parameter : dependence_depobj_type ':' expression
                            ;
dependence_depobj_type :  IN             { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_in, depend_iterators_definition_class); }
                       | OUT            { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_out, depend_iterators_definition_class); }
                       | INOUT          { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_inout, depend_iterators_definition_class); }
                       | MUTEXINOUTSET  { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_mutexinoutset); }
                       ;
depend_ordered_clause : DEPEND { firstParameter = OMPC_DEPEND_MODIFIER_unknown; }'(' dependence_ordered_parameter ')' {
}
                      ;
dependence_ordered_parameter : dependence_ordered_type
                             ;
dependence_ordered_type :  SOURCE { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_source, depend_iterators_definition_class); }
                        | SINK { current_clause = current_directive->addOpenMPClause(OMPC_depend, firstParameter, OMPC_DEPENDENCE_TYPE_sink, depend_iterators_definition_class); }':' EXPR_STRING{ std::cout << $4 << "\n"; ((OpenMPDependClause*)current_clause)->addDependenceVector($4);  }
                        ;

priority_clause: PRIORITY {
                            current_clause = current_directive->addOpenMPClause(OMPC_priority);
                         } '(' expression ')'
               ;

affinity_clause: AFFINITY '(' affinity_parameter ')' ;

affinity_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_affinity); current_clause->addLangExpr($1);  }
                   | EXPR_STRING ',' {std::cout << $1 << "\n";current_clause = current_directive->addOpenMPClause(OMPC_affinity); current_clause->addLangExpr($1); } var_list
                   | affinity_modifier ':' var_list
                   ;

affinity_modifier : MODIFIER_ITERATOR { current_clause = current_directive->addOpenMPClause(OMPC_affinity, OMPC_AFFINITY_MODIFIER_iterator); 
                              }'('iterators_definition')'{}
                  ;
iterators_definition : iterator_specifier
                     | iterators_definition ',' iterator_specifier                       
                     ;
iterator_specifier : EXPR_STRING EXPR_STRING { std::cout << $1 << "\n"; iterator_definition->push_back($1);iterator_definition->push_back($2); } '=' range_specification
                   | EXPR_STRING{iterator_definition->push_back("");std::cout << $1 << "\n";iterator_definition->push_back($1); }  '=' range_specification 
                   ;
range_specification : EXPR_STRING { std::cout << $1 << " : range begins.\n"; iterator_definition->push_back($1); } ':' EXPR_STRING { std::cout << $4 << " : range ends.\n";iterator_definition->push_back($4); } range_step 
                    ;
range_step : /*null*/ {iterator_definition->push_back(""); ((OpenMPAffinityClause*)current_clause)->addIteratorsDefinitionClass(iterator_definition); iterator_definition=new std::vector<const char*>(); }
           | ':' EXPR_STRING { std::cout << $2 << " - range step.\n";iterator_definition->push_back($2); ((OpenMPAffinityClause*)current_clause)->addIteratorsDefinitionClass(iterator_definition);iterator_definition=new std::vector<const char*>(); }
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
ext_implementation_defined_requirement: EXT_ EXPR_STRING {
                                        ((OpenMPRequiresDirective*)current_directive)->addUserDefinedImplementation($2);
                                      }
                                      ;
device_clause : DEVICE '(' device_parameter ')' ;

device_parameter :   EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_device); current_clause->addLangExpr($1);  }
                 | EXPR_STRING ',' {std::cout << $1 << "\n";
                         current_clause = current_directive->addOpenMPClause(OMPC_device); current_clause->addLangExpr($1); } var_list
                 | device_modifier_parameter ':' var_list
                 ;

device_modifier_parameter : ANCESTOR     { current_clause = current_directive->addOpenMPClause(OMPC_device, OMPC_DEVICE_MODIFIER_ancestor); }
                          | DEVICE_NUM{ current_clause = current_directive->addOpenMPClause(OMPC_device, OMPC_DEVICE_MODIFIER_device_num); }
                          ;

use_device_ptr_clause : USE_DEVICE_PTR {
                current_clause = current_directive->addOpenMPClause(OMPC_use_device_ptr);
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
defaultmap_clause : DEFAULTMAP{ firstParameter = OMPC_DEFAULTMAP_BEHAVIOR_unknown; } '('  defaultmap_parameter ')'
                  ;
defaultmap_parameter : defaultmap_behavior
                     | defaultmap_behavior ':' defaultmap_category
                     ;

defaultmap_behavior : BEHAVIOR_ALLOC { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_alloc; }
                    | BEHAVIOR_TO { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_alloc; }
                    | BEHAVIOR_FROM { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_from; }
                    | BEHAVIOR_TOFROM {firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_tofrom; }
                    | BEHAVIOR_FIRSTPRIVATE { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_firstprivate; }
                    | BEHAVIOR_NONE { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_none; }
                    | BEHAVIOR_DEFAULT { firstParameter=OMPC_DEFAULTMAP_BEHAVIOR_default; }
                    ;

defaultmap_category : CATEGORY_SCALAR { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap, firstParameter,OMPC_DEFAULTMAP_CATEGORY_scalar); }
                    | CATEGORY_AGGREGATE { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap, firstParameter,OMPC_DEFAULTMAP_CATEGORY_aggregate); }
                    | CATEGORY_POINTER { current_clause = current_directive->addOpenMPClause(OMPC_defaultmap,firstParameter,OMPC_DEFAULTMAP_CATEGORY_pointer); }
                    ;
uses_allocators_clause : USES_ALLOCATORS  { current_clause = current_directive->addOpenMPClause(OMPC_uses_allocators); } '(' uses_allocators_parameter ')' ;
uses_allocators_parameter : allocators_list
                          | allocators_list ','uses_allocators_parameter
                          ;
allocators_list : allocators_list_parameter
                |allocators_list_parameter '(' EXPR_STRING ')'
                ;

allocators_list_parameter : DEFAULT_MEM_ALLOC      
                          | LARGE_CAP_MEM_ALLOC
                          | CONST_MEM_ALLOC
                          | HIGH_BW_MEM_ALLOC
                          | LOW_LAT_MEM_ALLOC
                          | CGROUP_MEM_ALLOC
                          | PTEAM_MEM_ALLOC
                          | THREAD_MEM_ALLOC
                          | EXPR_STRING
                          ;
to_clause: TO '(' to_parameter ')' ;
to_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_to); current_clause->addLangExpr($1);  }
             | EXPR_STRING ',' {std::cout << $1 << "\n";current_clause = current_directive->addOpenMPClause(OMPC_to); current_clause->addLangExpr($1); } var_list
             | to_mapper ':' var_list
             ;
to_mapper : TO_MAPPER {current_clause = current_directive->addOpenMPClause(OMPC_to, OMPC_TO_mapper);
                              }'('EXPR_STRING')'{std::cout << $4 << "\n"; ((OpenMPToClause*)current_clause)->setMapperIdentifier($4); }
          ;

from_clause: FROM '(' from_parameter ')' ;
from_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_from); current_clause->addLangExpr($1);  }
               | EXPR_STRING ',' {std::cout << $1 << "\n";current_clause = current_directive->addOpenMPClause(OMPC_from); current_clause->addLangExpr($1); } var_list
               | from_mapper ':' var_list
               ;
from_mapper : FROM_MAPPER { current_clause = current_directive->addOpenMPClause(OMPC_from, OMPC_FROM_mapper); 
                              }'('EXPR_STRING')'{std::cout << $4 << "\n"; ((OpenMPFromClause*)current_clause)->setMapperIdentifier($4); }
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

map_clause : MAP {firstParameter = OMPC_MAP_MODIFIER_unknown; secondParameter = OMPC_MAP_MODIFIER_unknown; thirdParameter = OMPC_MAP_MODIFIER_unknown; }'(' map_parameter')';

map_parameter : EXPR_STRING {current_clause = current_directive->addOpenMPClause(OMPC_map); current_clause->addLangExpr($1); }
              | EXPR_STRING ',' { current_clause = current_directive->addOpenMPClause(OMPC_map); current_clause->addLangExpr($1); } var_list
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

map_modifier1 : MAP_MODIFIER_ALWAYS {firstParameter = OMPC_MAP_MODIFIER_always; }
              | MAP_MODIFIER_CLOSE  {firstParameter = OMPC_MAP_MODIFIER_close; }
              | map_modifier_mapper {firstParameter = OMPC_MAP_MODIFIER_mapper; }
              ;
map_modifier2 : MAP_MODIFIER_ALWAYS {secondParameter = OMPC_MAP_MODIFIER_always; }
              | MAP_MODIFIER_CLOSE  {secondParameter = OMPC_MAP_MODIFIER_close; }
              | map_modifier_mapper {secondParameter = OMPC_MAP_MODIFIER_mapper; }
              ;
map_modifier3 : MAP_MODIFIER_ALWAYS {if (firstParameter==OMPC_MAP_MODIFIER_always||secondParameter==OMPC_MAP_MODIFIER_always) { yyerror("ALWAYS modifier can appear in the map clause only once\n"); YYABORT; } else thirdParameter = OMPC_MAP_MODIFIER_always; }
              | MAP_MODIFIER_CLOSE  {if (firstParameter==OMPC_MAP_MODIFIER_close||secondParameter==OMPC_MAP_MODIFIER_close) { yyerror("CLOSE modifier can appear in the map clause only once\n"); YYABORT; } else thirdParameter = OMPC_MAP_MODIFIER_close; }
              | map_modifier_mapper {if (firstParameter==OMPC_MAP_MODIFIER_mapper||secondParameter==OMPC_MAP_MODIFIER_mapper) { yyerror("MAPPER modifier can appear in the map clause only once\n"); YYABORT; } else thirdParameter = OMPC_MAP_MODIFIER_mapper; }
              ;
map_type : MAP_TYPE_TO {current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_to, fourthParameter); }
         | MAP_TYPE_FROM {current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_from, fourthParameter); }
         | MAP_TYPE_TOFROM {current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_tofrom, fourthParameter); }
         | MAP_TYPE_ALLOC {current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_alloc, fourthParameter); }
         | MAP_TYPE_RELEASE {current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_release, fourthParameter); }
         | MAP_TYPE_DELETE {current_clause = current_directive->addOpenMPClause(OMPC_map, firstParameter, secondParameter,thirdParameter, OMPC_MAP_TYPE_delete, fourthParameter); }
         ;
 map_modifier_mapper : MAP_MODIFIER_MAPPER '('EXPR_STRING')'{std::cout << $3 << "\n";fourthParameter=$3; }                       
                     ;

task_reduction_clause : TASK_REDUCTION '(' task_reduction_identifier ':' var_list ')' {
                      }
                      ;
task_reduction_identifier : task_reduction_enum_identifier
                          | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_user, $1); }
                          ;

task_reduction_enum_identifier : '+'{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_plus); }
                               | '-'{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_minus); }
                               | '*'{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_mul); }
                               | '&'{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_bitand); }
                               | '|'{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_bitor); }
                               | '^'{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_bitxor); }
                               | LOGAND{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_logand); }
                               | LOGOR{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_logor); }
                               | MAX{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_max); }
                               | MIN{ current_clause = current_directive->addOpenMPClause(OMPC_task_reduction,OMPC_TASK_REDUCTION_IDENTIFIER_min); }
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
threads_clause : THREADS{
                            current_clause = current_directive->addOpenMPClause(OMPC_threads);
                         } 
               ;
simd_ordered_clause : SIMD{
                            current_clause = current_directive->addOpenMPClause(OMPC_simd);
                         } 
                    ;
/*YAYING*/
for_directive :  FOR {
                        current_directive = new OpenMPDirective(OMPD_for);
                     }
                     for_clause_optseq 
                   ;

simd_directive :  SIMD {
                        current_directive = new OpenMPDirective(OMPD_simd);
                     }
                     simd_clause_optseq 
                   ;
for_simd_directive :  FOR SIMD{
                        current_directive = new OpenMPDirective(OMPD_for_simd);
                     }
                     for_simd_clause_optseq
                   ;
declare_directive :  DECLARE{
                        current_directive = new OpenMPDirective(OMPD_declare);
                     }
                     declare_clause_optseq
                   ;

distribute_directive : DISTRIBUTE{
                        current_directive = new OpenMPDirective(OMPD_distribute);
                     }
                     distribute_clause_optseq
                   ;
distribute_simd_directive : DISTRIBUTE SIMD{
                        current_directive = new OpenMPDirective(OMPD_distribute_simd);
                     }
                     distribute_simd_clause_optseq
                     ;
distribute_parallel_for_directive : DISTRIBUTE PARALLEL FOR{
                        current_directive = new OpenMPDirective(OMPD_distribute_parallel_for);
                     }
                     distribute_parallel_for_clause_optseq
                     ;

distribute_parallel_for_simd_directive: DISTRIBUTE PARALLEL FOR SIMD{
                        current_directive = new OpenMPDirective(OMPD_distribute_parallel_for_simd);
                     }
                     distribute_parallel_for_simd_clause_optseq
                     ;
loop_directive : LOOP{
                        current_directive = new OpenMPDirective(OMPD_loop);
                     }
                     loop_clause_optseq
                     ;
scan_directive : SCAN{
                        current_directive = new OpenMPDirective(OMPD_scan);
                     }
                     scan_clause_optseq
                     ;
sections_directive : SECTIONS{
                        current_directive = new OpenMPDirective(OMPD_sections);
                     }
                     sections_clause_optseq
                     ;
section_directive : SECTION{
                        current_directive = new OpenMPDirective(OMPD_section);
                     }
                     ;
single_directive : SINGLE{
                        current_directive = new OpenMPDirective(OMPD_single);
                     }
                     single_clause_optseq
                     ;
cancel_directive : CANCEL{
                        current_directive = new OpenMPDirective(OMPD_cancel);
                     }
                     cancel_clause_optseq
                     ;
cancellation_point_directive : CANCELLATION POINT{
                        current_directive = new OpenMPDirective(OMPD_cancellation_point);
                     }
                     cancellation_point_clause_optseq
                     ;

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
allocate_list: '('directive_varlist')'
             ;


directive_variable :   EXPR_STRING { std::cout << $1 << "\n"; ((OpenMPAllocateDirective*)current_directive)->addAllocateList($1); }
                   ;
directive_varlist : directive_variable
                  | directive_varlist ',' directive_variable
                  ;

threadprivate_directive : THREADPRIVATE {current_directive = new OpenMPThreadprivateDirective(); } '('threadprivate_list')'
                        ;
threadprivate_variable :   EXPR_STRING { std::cout << $1 << "\n"; ((OpenMPThreadprivateDirective*)current_directive)->addThreadprivateList($1); }
                       ;
threadprivate_list : threadprivate_variable
                   | threadprivate_list ',' threadprivate_variable
                   ;

declare_reduction_directive : DECLARE REDUCTION {current_directive = new OpenMPDeclareReductionDirective(); } '(' reduction_list ')' declare_reduction_clause_optseq
                            ;

reduction_list : reduction_identifiers ':' typername_list ':' combiner;

reduction_identifiers: '+'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("+"); }
                     | '-'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("-"); }
                     | '*'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("*"); }
                     | '&'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("&"); }
                     | '|'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("|"); }
                     | '^'{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("^"); }
                     | LOGAND{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("&&"); }
                     | LOGOR{ ((OpenMPDeclareReductionDirective*)current_directive)->setIdentifier("||"); }
                     ; 

typername_variable : EXPR_STRING { std::cout << $1 << "\n"; ((OpenMPDeclareReductionDirective*)current_directive)->addTypenameList($1); }
                   ;
typername_list : typername_variable
               | typername_list ',' typername_variable
               ;
combiner : EXPR_STRING { std::cout << $1 << "\n"; ((OpenMPDeclareReductionDirective*)current_directive)->setCombiner($1); }
         ;

declare_mapper_directive : DECLARE MAPPER {current_directive = new OpenMPDeclareMapperDirective(); } '(' mapper_list ')' declare_mapper_clause_optseq
                         ;

mapper_list : mapper_identifier_optseq 
            ;

mapper_identifier_optseq : type_var
                         | mapper_identifier ':' type_var
                         ;
 
mapper_identifier : IDENTIFIER_DEFAULT {((OpenMPDeclareMapperDirective*)current_directive)->setIdentifier("default"); }
                  | EXPR_STRING {((OpenMPDeclareMapperDirective*)current_directive)->setIdentifier($1); }
                  ;

type_var : EXPR_STRING {std::cout<< $1 <<"\n";((OpenMPDeclareMapperDirective*)current_directive)->setTypeVar($1); }
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

simd_clause_optseq : /*empty*/
                   | simd_clause_seq
                   ;
for_simd_clause_optseq : /*empty*/
                       | for_simd_clause_seq
                       ;
declare_clause_optseq : /*empty*/
                      | declare_clause_seq
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
distribute_parallel_for_simd_clause_optseq : /*empty*/
                                           | distribute_parallel_for_simd_clause_seq
                                           ;
loop_clause_optseq : /*empty*/
                   | loop_clause_seq
                   ;
scan_clause_optseq : /*empty*/
                   | scan_clause_seq
                   ;
sections_clause_optseq : /*empty*/
                       | sections_clause_seq
                       ;
single_clause_optseq : /*empty*/
                     | single_clause_seq
                     ;
cancel_clause_optseq : /*empty*/
                     | cancel_clause_seq
                     ;
cancellation_point_clause_optseq : /*empty*/
                                 | cancellation_point_clause_seq
                                 ;
allocate_clause_optseq :  /*empty*/
                       | allocate_clause_seq
                       ;
declare_reduction_clause_optseq :  /*empty*/
                                | declare_reduction_clause_seq
                                ;
declare_mapper_clause_optseq :  /*empty*/
                             | declare_mapper_clause_seq
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
               | for_clause_seq "," for_clause
               ;

simd_clause_seq : simd_clause
                | simd_clause_seq simd_clause
                | simd_clause_seq "," simd_clause
                ;

for_simd_clause_seq : for_simd_clause
                    | for_simd_clause_seq for_simd_clause
                    | for_simd_clause_seq "," for_simd_clause
                    ;
declare_clause_seq : declare_clause
                   | declare_clause_seq declare_clause
                   | declare_clause_seq "," declare_clause
                   ;
distribute_clause_seq : distribute_clause
                      | distribute_clause_seq distribute_clause
                      | distribute_clause_seq "," declare_clause
                      ;
distribute_simd_clause_seq : distribute_simd_clause
                           | distribute_simd_clause_seq distribute_simd_clause
                           | distribute_simd_clause_seq "," distribute_simd_clause
                           ;
distribute_parallel_for_clause_seq : distribute_parallel_for_clause
                                   | distribute_parallel_for_clause_seq distribute_parallel_for_clause
                                   | distribute_parallel_for_clause_seq "," distribute_parallel_for_clause
                                   ;
distribute_parallel_for_simd_clause_seq : distribute_parallel_for_simd_clause
                                        | distribute_parallel_for_simd_clause_seq distribute_parallel_for_simd_clause
                                        | distribute_parallel_for_simd_clause_seq "," distribute_parallel_for_simd_clause
                                        ;
loop_clause_seq : loop_clause
                | loop_clause_seq loop_clause
                | loop_clause_seq "," loop_clause
                ;
scan_clause_seq : scan_clause
                ;
sections_clause_seq : sections_clause
                   | sections_clause_seq sections_clause
                   | sections_clause_seq "," sections_clause
                   ;

single_clause_seq : single_clause
                  | single_clause_seq single_clause
                  | single_clause_seq "," single_clause
                  ;
cancel_clause_seq : construct_type_clause
                  | construct_type_clause if_clause
                  | construct_type_clause "," if_clause
                  ;
cancellation_point_clause_seq : construct_type_clause
                              ;
allocate_clause_seq :  allocator_clause
                    ;
declare_reduction_clause_seq : initializer_clause
                             ;
declare_mapper_clause_seq : map_clause
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
                | reduction_clause
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

simd_clause : if_clause
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

for_simd_clause : if_clause
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
 
declare_clause : simdlen_clause
               | linear_clause
               | aligned_clause
               | uniform_clause
               | inbranch_clause
               | notinbranch_clause
               ;
 
distribute_clause : private_clause
                  | firstprivate_clause 
                  | lastprivate_clause
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
                  | if_clause
                  | safelen_clause
                  | simdlen_clause
                  | linear_clause
                  | aligned_clause
                  | nontemporal_clause
                  | reduction_clause
                  | order_clause
                  ;
distribute_parallel_for_clause : if_clause
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
distribute_parallel_for_simd_clause: if_clause
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
loop_clause : bind_clause
            | collapse_clause
            | order_clause
            | private_clause
            | lastprivate_clause
            | reduction_clause
            ;
scan_clause : inclusive_clause
            | exclusive_clause
            ;
sections_clause : private_clause
                | firstprivate_clause
                | lastprivate_clause
                | reduction_clause
                | allocate_clause
                | nowait_clause
                ;
single_clause : private_clause
              | firstprivate_clause
              | copyprivate_clause
              | allocate_clause
              | nowait_clause
              ;
construct_type_clause : PARALLEL { current_clause = current_directive->addOpenMPClause(OMPC_parallel); }
                      | SECTIONS { current_clause = current_directive->addOpenMPClause(OMPC_sections); }
                      | FOR { current_clause = current_directive->addOpenMPClause(OMPC_for); }
                      | TASKGROUP { current_clause = current_directive->addOpenMPClause(OMPC_taskgroup); }
                      ;
if_parallel_clause : IF '(' if_parallel_parameter ')' { ; }
                   ;

if_parallel_parameter : PARALLEL ':' { current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_parallel); }
                        expression { ; }
                      | EXPR_STRING {
                        current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                        current_clause->addLangExpr($1);
                        }
                      ;

if_clause : IF '(' if_parameter ')' { ; }
          ;

if_parameter : SIMD ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_simd);
                } expression { ; }
              | TASK ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_task);
                } expression { ; }
              | TASKLOOP ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_taskloop);
                } expression { ; }
              | CANCEL ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_cancel);
                } expression { ; }
              | TARGET DATA ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_data);
                } expression { ; }
              | TARGET ENTER DATA ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_enter_data);
                } expression { ; }
              | TARGET EXIT DATA ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_exit_data);
                } expression { ; }
              | TARGET ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target);
                } expression { ; }
              | TARGET UPDATE ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_target_update);
                }expression { ; }
              | EXPR_STRING {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_MODIFIER_unspecified);
                current_clause->addLangExpr($1);
                }
              ;

num_threads_clause: NUM_THREADS {
                            current_clause = current_directive->addOpenMPClause(OMPC_num_threads);
                         } '(' expression ')'
                      ;
num_teams_clause: NUM_TEAMS {
                            current_clause = current_directive->addOpenMPClause(OMPC_num_teams);
                         } '(' expression ')'
                      ;
thread_limit_clause: THREAD_LIMIT {
                            current_clause = current_directive->addOpenMPClause(OMPC_thread_limit);
                         } '(' expression ')'
                      ;
copyin_clause: COPYIN {
                current_clause = current_directive->addOpenMPClause(OMPC_copyin);
                } '(' var_list ')'
              ;

default_clause : DEFAULT '(' default_parameter ')' { } ;

default_parameter : SHARED { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_shared); }
                  | NONE { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_none); }
                  | FIRSTPRIVATE { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_firstprivate); }
                  | PRIVATE { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_private); }
                  ;

default_variant_clause : DEFAULT '(' default_variant_directive ')' { } ;


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

proc_bind_parameter : MASTER  { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_master); }
                    | CLOSE   { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_close); }
                    | SPREAD  { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_spread); }
                    ;
bind_clause : BIND '(' bind_parameter ')' { } ;

bind_parameter : TEAMS      { current_clause = current_directive->addOpenMPClause(OMPC_bind, OMPC_BIND_teams); }
               | PARALLEL   { current_clause = current_directive->addOpenMPClause(OMPC_bind, OMPC_BIND_parallel); }
               | THREAD     { current_clause = current_directive->addOpenMPClause(OMPC_bind, OMPC_BIND_thread); }
               ;
allocate_clause : ALLOCATE '(' allocate_parameter ')' ;

allocate_parameter :   EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_unknown); current_clause->addLangExpr($1);  }
                     | EXPR_STRING ',' {std::cout << $1 << "\n";
                         current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_unknown); current_clause->addLangExpr($1); } var_list
                     | allocator_parameter ':' { ; } var_list
                      ;
allocator_parameter : DEFAULT_MEM_ALLOC           { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_default); }
                          | LARGE_CAP_MEM_ALLOC{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_large_cap); }
                          | CONST_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cons_mem); }
                          | HIGH_BW_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_high_bw); }
                          | LOW_LAT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_low_lat); }
                          | CGROUP_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cgroup); }
                          | PTEAM_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_pteam); }
                          | THREAD_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_thread); }
                          | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_user, $1); }
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

lastprivate_clause : LASTPRIVATE '(' lastprivate_parameter')';

lastprivate_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_lastprivate); current_clause->addLangExpr($1);  }
                      | EXPR_STRING  ',' {std::cout << $1 << "\n"; } {
                         current_clause = current_directive->addOpenMPClause(OMPC_lastprivate); current_clause->addLangExpr($1); } var_list
                      | lastprivate_modifier ':'{ ; } var_list
              ;

lastprivate_modifier : MODIFIER_CONDITIONAL { current_clause = current_directive->addOpenMPClause(OMPC_lastprivate,OMPC_LASTPRIVATE_MODIFIER_conditional); }
                 | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_lastprivate, OMPC_LASTPRIVATE_MODIFIER_user, $1); }
                     ;

linear_clause : LINEAR '('  linear_parameter ')'
              | LINEAR '('  linear_parameter ':'EXPR_STRING  { std::cout << $5 << "\n"; ((OpenMPLinearClause*)current_clause)->setUserDefinedStep($5); } ')' 
      ;


linear_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_linear); current_clause->addLangExpr($1);  }
                 | EXPR_STRING ','  {std::cout << $1 << "\n"; } {current_clause = current_directive->addOpenMPClause(OMPC_linear); current_clause->addLangExpr($1); } var_list
                 | linear_modifier '(' var_list ')'
         ;

linear_modifier : MODOFIER_VAL { current_clause = current_directive->addOpenMPClause(OMPC_linear,OMPC_LINEAR_MODIFIER_val); }
                | MODOFIER_REF { current_clause = current_directive->addOpenMPClause(OMPC_linear,OMPC_LINEAR_MODIFIER_ref); }
                | MODOFIER_UVAL { current_clause = current_directive->addOpenMPClause(OMPC_linear,OMPC_LINEAR_MODIFIER_uval); }
                ;

aligned_clause : ALIGNED '('  aligned_parameter ')'
               | ALIGNED '('  aligned_parameter ':' var_list')'
           ;


aligned_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1);  }
                  | EXPR_STRING ','  {std::cout << $1 << "\n"; } {current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1); } var_list
          ;
initializer_clause : INITIALIZER '('initializer_expr')';

initializer_expr : OMP_PRIV {std::cout << "omp_priv = "; } '=' expr;

expr: EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_initializer, OMPC_INITIALIZER_PRIV_user, $1); };

safelen_clause: SAFELEN { current_clause = current_directive->addOpenMPClause(OMPC_safelen); } '(' var_list ')' {
                        }
                      ;

simdlen_clause: SIMDLEN { current_clause = current_directive->addOpenMPClause(OMPC_simdlen); } '(' var_list ')' {
                        }
                      ;

nontemporal_clause: NONTEMPORAL { current_clause = current_directive->addOpenMPClause(OMPC_nontemporal); } '(' var_list ')' {
                        }
                      ;

collapse_clause: COLLAPSE { current_clause = current_directive->addOpenMPClause(OMPC_collapse); } '(' var_list ')' {
                        }
                      ;

ordered_clause: ORDERED {current_clause = current_directive->addOpenMPClause(OMPC_ordered); } '(' var_list ')'
              | ORDERED {current_clause = current_directive->addOpenMPClause(OMPC_ordered); }
              ;

nowait_clause: NOWAIT {current_clause = current_directive->addOpenMPClause(OMPC_nowait); }
              ;

order_clause: ORDER  {current_clause = current_directive->addOpenMPClause(OMPC_order); } '(' var_list ')'
                  ;

uniform_clause: UNIFORM  {current_clause = current_directive->addOpenMPClause(OMPC_uniform); } '(' var_list ')'
                  ;

inbranch_clause: INBRANCH {current_clause = current_directive->addOpenMPClause(OMPC_inbranch); }
                          ;

notinbranch_clause: NOTINBRANCH {current_clause = current_directive->addOpenMPClause(OMPC_notinbranch); }
                          ;
inclusive_clause: INCLUSIVE  {current_clause = current_directive->addOpenMPClause(OMPC_inclusive); } '(' var_list ')'
                  ;
exclusive_clause: EXCLUSIVE  {current_clause = current_directive->addOpenMPClause(OMPC_exclusive); } '(' var_list ')'
                  ;
allocator_clause: ALLOCATOR '('allocator1_parameter')';
allocator1_parameter : DEFAULT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_default); }
                     | LARGE_CAP_MEM_ALLOC{ current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_large_cap); }
                     | CONST_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_cons_mem); }
                     | HIGH_BW_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_high_bw); }
                     | LOW_LAT_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_low_lat); }
                     | CGROUP_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_cgroup); }
                     | PTEAM_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_pteam); }
                     | THREAD_MEM_ALLOC { current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_thread); }
                     | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_allocator, OMPC_ALLOCATOR_ALLOCATOR_user, $1); }
                     ;

dist_schedule_clause : DIST_SCHEDULE '('dist_schedule_parameter')'{}
                     ;
dist_schedule_parameter : STATIC {current_clause = current_directive->addOpenMPClause(OMPC_dist_schedule,OMPC_DISTSCHEDULE_KIND_static); }
                        | STATIC {current_clause = current_directive->addOpenMPClause(OMPC_dist_schedule,OMPC_DISTSCHEDULE_KIND_static); } ',' var_list
                        ;
schedule_clause : SCHEDULE {firstParameter = OMPC_SCHEDULE_KIND_unknown;secondParameter = OMPC_SCHEDULE_KIND_unknown; }'(' schedule_parameter ')' {
                }
                ;

schedule_parameter : schedule_kind {}
                   | schedule_modifier ':' schedule_kind
                   ;


schedule_kind : schedule_enum_kind { }
              | schedule_enum_kind ',' var_list {}
              ;

schedule_modifier : schedule_enum_modifier ',' schedule_modifier2
                  | schedule_enum_modifier
                  ;
schedule_modifier2 : MODIFIER_MONOTONIC {secondParameter = OMPC_SCHEDULE_MODIFIER_monotonic; }
                   | MODIFIER_NOMONOTONIC {secondParameter = OMPC_SCHEDULE_MODIFIER_nonmonotonic; }
                   | MODIFIER_SIMD {secondParameter = OMPC_SCHEDULE_MODIFIER_simd; }
                   ;
schedule_enum_modifier : MODIFIER_MONOTONIC {firstParameter = OMPC_SCHEDULE_MODIFIER_monotonic; }
                       | MODIFIER_NOMONOTONIC {firstParameter = OMPC_SCHEDULE_MODIFIER_nonmonotonic; }
                       | MODIFIER_SIMD {firstParameter = OMPC_SCHEDULE_MODIFIER_simd; }
                       ;
schedule_enum_kind : STATIC     {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_static); }
                   | DYNAMIC    {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_dynamic); }
                   | GUIDED     {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_guided); }
                   | AUTO       {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_auto); }
                   | RUNTIME    {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_runtime); }
                   ;  
shared_clause : SHARED {
                current_clause = current_directive->addOpenMPClause(OMPC_shared);
                    } '(' var_list ')'
                  ;

reduction_clause : REDUCTION { firstParameter = OMPC_REDUCTION_MODIFIER_unknown; } '(' reduction_parameter ':' var_list ')' {
                 }
                 ;

reduction_parameter : reduction_identifier {}
                    | reduction_modifier ',' reduction_identifier
                    ;

reduction_identifier : reduction_enum_identifier {}
                    | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_user, $1); }
                  ;

reduction_modifier : MODIFIER_INSCAN { firstParameter = OMPC_REDUCTION_MODIFIER_inscan; }
                   | MODIFIER_TASK { firstParameter = OMPC_REDUCTION_MODIFIER_task; }
                   | MODIFIER_DEFAULT { firstParameter = OMPC_REDUCTION_MODIFIER_default; }
                   ;

reduction_enum_identifier :  '+'{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_plus); }
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
    
    printf("Start parsing...\n");
    OpenMPBaseLang base_lang = Lang_C;
    exprParse = _exprParse;
    current_directive = NULL;
    std::string input_string;
    const char *input = _input;
    // Since we can't guarantee the input has been preprocessed, it should be checked here.
    std::regex fortran_regex ("[!][$][Oo][Mm][Pp]");
    input_string = std::string(input, 5);
    if (std::regex_match(input_string, fortran_regex)) {
        base_lang = Lang_Fortran;
        input_string = std::string(input);
        std::transform(input_string.begin(), input_string.end(), input_string.begin(), ::tolower);
        input = input_string.c_str();
    };
    start_lexer(input);
    int res = yyparse();
    end_lexer();
    if (current_directive) {
        current_directive->setBaseLang(base_lang);
    };
    
    return current_directive;
}
