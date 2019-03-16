/* OpenMP C/C++/Fortran Grammar */

%name-prefix "openmp_"
%defines
%error-verbose

%{
/* DQ (2/10/2014): IF is conflicting with Boost template IF. */
#undef IF

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "OpenMPIR.h"
#include <string.h>

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
static int fourthParameter;

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


%token  OMP PARALLEL FOR METADIRECTIVE DECLARE DISTRIBUTE LOOP SCAN SECTIONS SECTION SINGLE CANCEL TASKGROUP CANCELLATION POINT THREAD VARIANT
        IF NUM_THREADS DEFAULT PRIVATE FIRSTPRIVATE SHARED COPYIN REDUCTION PROC_BIND ALLOCATE SIMD TASK LASTPRIVATE WHEN MATCH
        LINEAR SCHEDULE COLLAPSE NOWAIT ORDER ORDERED MODIFIER_CONDITIONAL MODIFIER_MONOTONIC MODIFIER_NOMONOTONIC STATIC DYNAMIC GUIDED AUTO RUNTIME MODOFIER_VAL MODOFIER_REF MODOFIER_UVAL MODIFIER_SIMD
        SAFELEN SIMDLEN ALIGNED NONTEMPORAL UNIFORM INBRANCH NOTINBRANCH DIST_SCHEDULE BIND INCLUSIVE EXCLUSIVE COPYPRIVATE ALLOCATOR/*YAYING*/
        NONE MASTER CLOSE SPREAD MODIFIER_INSCAN MODIFIER_TASK MODIFIER_DEFAULT 
        PLUS MINUS STAR BITAND BITOR BITXOR LOGAND LOGOR EQV NEQV MAX MIN
        DEFAULT_MEM_ALLOC LARGE_CAP_MEM_ALLOC CONST_MEM_ALLOC HIGH_BW_MEM_ALLOC LOW_LAT_MEM_ALLOC CGROUP_MEM_ALLOC
        PTEAM_MEM_ALLOC THREAD_MEM_ALLOC
        TEAMS
        NUM_TEAMS THREAD_LIMIT
        END USER CONSTRUCT DEVICE IMPLEMENTATION CONDITION SCORE
        KIND HOST NOHOST CPU GPU FPGA ISA
        AMD

%token <itype> ICONSTANT
%token <stype> EXPRESSION ID_EXPRESSION EXPR_STRING VAR_STRING
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
                 ;

metadirective_directive : METADIRECTIVE {
                        current_directive = new OpenMPDirective(OMPD_metadirective);
                     }
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
                | default_clause
                ;

when_clause : WHEN { current_clause = current_directive->addOpenMPClause(OMPC_when); }
                '(' context_selector_specification ':' {
                current_parent_directive = current_directive;
                current_parent_clause = current_clause;
                } when_variant_directive { /*current_directive->setParentConstruct(current_clause);*/
                /*((OpenMPWhenClause*)current_parent_clause)->setVariantDirective(current_directive);*/
                current_directive = current_parent_directive;
                current_clause = current_parent_clause;
                current_parent_directive = NULL;
                current_parent_clause = NULL;
                } ')' { } ;

when_variant_directive : variant_directive {((OpenMPWhenClause*)current_parent_clause)->setVariantDirective(current_directive);}
                | { ; }
                ;

context_selector_specification : trait_set_selector
                | context_selector_specification trait_set_selector
                | context_selector_specification ',' trait_set_selector
                ;

trait_set_selector : trait_set_selector_name { } '=' '{' trait_selector_list '}'
                ;

trait_set_selector_name : USER { }
                | CONSTRUCT {
                    current_parent_directive = current_directive;
                    current_parent_clause = current_clause;}
                | DEVICE { }
                | IMPLEMENTATION { }
                ;

trait_selector_list : trait_selector
                | trait_selector_list trait_selector
                | trait_selector_list ',' trait_selector
                ;

trait_selector : condition_selector
                | construct_selector {
                    ((OpenMPVariantClause*)current_parent_clause)->addConstructDirective(current_directive);
                    current_directive = current_parent_directive;
                    current_clause = current_parent_clause;
                    current_parent_directive = NULL;
                    current_parent_clause = NULL;
                    std::cout << "A construct directive has been added to WHEN clause.\n"; 
                }
                | device_selector_list
                | implementation_selector
                ;

condition_selector : CONDITION '(' EXPR_STRING { std::cout << $3 << " - condition \n"; ((OpenMPVariantClause*)current_clause)->setUserCondition($3); } ')'
                ;

device_selector_list : device_selector
                     | device_selector_list device_selector
                     ;

device_selector : context_kind
                | context_isa
                ;

context_kind : KIND '(' context_kind_name ')'
             ;

context_kind_name : HOST { std::cout << "host - device \n"; ((OpenMPVariantClause*)current_clause)->setContextKind(OMPC_CONTEXT_KIND_host); }
                  | NOHOST { std::cout << "nohost - device \n"; ((OpenMPVariantClause*)current_clause)->setContextKind(OMPC_CONTEXT_KIND_nohost); }
                  | CPU { std::cout << "cpu - device \n"; ((OpenMPVariantClause*)current_clause)->setContextKind(OMPC_CONTEXT_KIND_cpu); }
                  | GPU { std::cout << "gpu - device \n"; ((OpenMPVariantClause*)current_clause)->setContextKind(OMPC_CONTEXT_KIND_gpu); }
                  | FPGA { std::cout << "fpga - device \n"; ((OpenMPVariantClause*)current_clause)->setContextKind(OMPC_CONTEXT_KIND_fpga); }
                  ;

context_isa : ISA '(' EXPR_STRING { std::cout << $3 << " - isa \n"; ((OpenMPVariantClause*)current_clause)->setIsaExpression($3); } ')'
            ;

implementation_selector : context_vendor_name
                        | EXPR_STRING {}
                        ;

context_vendor_name : AMD { ((OpenMPVariantClause*)current_clause)->setImplementationKind(OMPC_CONTEXT_VENDOR_amd); }
                    ;

construct_selector : parallel_selector
                   ;

parallel_selector : PARALLEL { current_directive = new OpenMPDirective(OMPD_parallel); }
                | PARALLEL '(' { current_directive = new OpenMPDirective(OMPD_parallel); } parallel_selector_parameter ')'
                ;

parallel_selector_parameter : trait_score ':' parallel_clause_optseq
                | parallel_clause_optseq
                ;

trait_score : SCORE '(' expression ')'
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
distribute_parallel_for_clause_optseq: /*empty*/
	               | distribute_parallel_for_clause_seq 
                       ;
distribute_parallel_for_simd_clause_optseq: /*empty*/
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

parallel_clause : if_clause
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
construct_type_clause : parallel_clause
                      | sections_clause
                      | for_clause
                      | taskgroup_clause
                      ;
if_clause: IF '(' if_parameter ')' { ; }
                    ;

if_parameter :  PARALLEL ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_parallel);
                } expression { ; }
              | SIMD ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_simd);
                } expression { ; }
              | TASK ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_task);
                } expression { ; }
              | CANCEL ':' {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_cancel);
                } expression { ; }
              | EXPR_STRING {
                current_clause = current_directive->addOpenMPClause(OMPC_if, OMPC_IF_unspecified);
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
                    | default_variant_directive
                    ;


default_variant_directive : { current_clause = current_directive->addOpenMPClause(OMPC_default, OMPC_DEFAULT_unknown);
                            current_parent_directive = current_directive;
                            current_parent_clause = current_clause; } variant_directive {
                            ((OpenMPDefaultClause*)current_parent_clause)->setVariantDirective(current_directive);
                            current_directive = current_parent_directive;
                            current_clause = current_parent_clause;
                            current_parent_directive = NULL;
                            current_parent_clause = NULL;
                            std::cout << "A construct directive has been added to DEFAULT clause.\n"; }
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

allocate_parameter :   EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_allocate); current_clause->addLangExpr($1);  }
                     | EXPR_STRING ',' {std::cout << $1 << "\n";
                         current_clause = current_directive->addOpenMPClause(OMPC_allocate); current_clause->addLangExpr($1); } var_list
                     | allocator_parameter ':' { ; } var_list
                      ;

allocator_parameter : DEFAULT_MEM_ALLOC           { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_default); }
						  | LARGE_CAP_MEM_ALLOC		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_large_cap); }
						  | CONST_MEM_ALLOC 		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cons_mem); }
						  | HIGH_BW_MEM_ALLOC 		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_high_bw); }
						  | LOW_LAT_MEM_ALLOC 		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_low_lat); }
						  | CGROUP_MEM_ALLOC 		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cgroup); }
						  | PTEAM_MEM_ALLOC 		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_pteam); }
						  | THREAD_MEM_ALLOC 		{ current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_thread); }
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
                      | EXPR_STRING  ',' {std::cout << $1 << "\n";} {
                         current_clause = current_directive->addOpenMPClause(OMPC_lastprivate); current_clause->addLangExpr($1); } var_list
                      | lastprivate_modifier ':'{;} var_list
		      ;

lastprivate_modifier : MODIFIER_CONDITIONAL { current_clause = current_directive->addOpenMPClause(OMPC_lastprivate,OMPC_LASTPRIVATE_MODIFIER_conditional);}
	             | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_lastprivate, OMPC_LASTPRIVATE_MODIFIER_user, $1); }
                     ;

linear_clause : LINEAR '('  linear_parameter ')'
              | LINEAR '('  linear_parameter ':' var_list')'
	      ;

linear_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_linear); current_clause->addLangExpr($1);  }
                 | EXPR_STRING ','  {std::cout << $1 << "\n";} {current_clause = current_directive->addOpenMPClause(OMPC_linear); current_clause->addLangExpr($1); } var_list
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
                  | EXPR_STRING ','  {std::cout << $1 << "\n";} {current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1); } var_list
		  ;

safelen_clause: SAFELEN { current_clause = current_directive->addOpenMPClause(OMPC_safelen);} '(' var_list ')' {
						}
					  ;

simdlen_clause: SIMDLEN { current_clause = current_directive->addOpenMPClause(OMPC_simdlen);} '(' var_list ')' {
						}
					  ;

nontemporal_clause: NONTEMPORAL { current_clause = current_directive->addOpenMPClause(OMPC_nontemporal);} '(' var_list ')' {
						}
					  ;

collapse_clause: COLLAPSE { current_clause = current_directive->addOpenMPClause(OMPC_collapse);} '(' var_list ')' {
						}
					  ;

ordered_clause: ORDERED {current_clause = current_directive->addOpenMPClause(OMPC_ordered);} '(' var_list ')'
              | ORDERED {current_clause = current_directive->addOpenMPClause(OMPC_ordered);}
			  ;

nowait_clause: NOWAIT {current_clause = current_directive->addOpenMPClause(OMPC_nowait);}
			  ;

parallel_clause: PARALLEL {current_clause = current_directive->addOpenMPClause(OMPC_parallel);}
			  ;

sections_clause: SECTIONS {current_clause = current_directive->addOpenMPClause(OMPC_sections);}
			  ;

for_clause: FOR {current_clause = current_directive->addOpenMPClause(OMPC_for);}
			  ;

taskgroup_clause: TASKGROUP {current_clause = current_directive->addOpenMPClause(OMPC_taskgroup);}
			  ;

order_clause: ORDER  {current_clause = current_directive->addOpenMPClause(OMPC_order);} '(' var_list ')'
		          ;

uniform_clause: UNIFORM  {current_clause = current_directive->addOpenMPClause(OMPC_uniform);} '(' var_list ')'
		          ;

inbranch_clause: INBRANCH {current_clause = current_directive->addOpenMPClause(OMPC_inbranch);}
                          ;

notinbranch_clause: NOTINBRANCH {current_clause = current_directive->addOpenMPClause(OMPC_notinbranch);}
                          ;
inclusive_clause: INCLUSIVE  {current_clause = current_directive->addOpenMPClause(OMPC_inclusive);} '(' var_list ')'
		          ;
exclusive_clause: EXCLUSIVE  {current_clause = current_directive->addOpenMPClause(OMPC_exclusive);} '(' var_list ')'
		          ;
dist_schedule_clause : DIST_SCHEDULE '('dist_schedule_parameter')'{}
                     ;
dist_schedule_parameter : STATIC {current_clause = current_directive->addOpenMPClause(OMPC_dist_schedule,OMPC_DISTSCHEDULE_KIND_static);}
                        | STATIC {current_clause = current_directive->addOpenMPClause(OMPC_dist_schedule,OMPC_DISTSCHEDULE_KIND_static);} ',' var_list
	                ;
schedule_clause: SCHEDULE {firstParameter = OMPC_SCHEDULE_KIND_unknown;secondParameter = OMPC_SCHEDULE_KIND_unknown;}'(' schedule_parameter ')' {
					}
					;

schedule_parameter : schedule_kind {}
		   | schedule_modifier ':' schedule_kind
		   ;


schedule_kind : schedule_enum_kind {	}
             | schedule_enum_kind ',' var_list
	     | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter,secondParameter, OMPC_SCHEDULE_KIND_user, $1); }
	     ;

schedule_modifier : schedule_enum_modifier ',' schedule_modifier2
                  | schedule_enum_modifier
                  ;
schedule_modifier2 : MODIFIER_MONOTONIC {secondParameter = OMPC_SCHEDULE_MODIFIER_monotonic;}
                       | MODIFIER_NOMONOTONIC {secondParameter = OMPC_SCHEDULE_MODIFIER_nonmonotonic;}
                       | MODIFIER_SIMD {secondParameter = OMPC_SCHEDULE_MODIFIER_simd;}
                       ;
schedule_enum_modifier : MODIFIER_MONOTONIC {firstParameter = OMPC_SCHEDULE_MODIFIER_monotonic;}
                       | MODIFIER_NOMONOTONIC {firstParameter = OMPC_SCHEDULE_MODIFIER_nonmonotonic;}
                       | MODIFIER_SIMD {firstParameter = OMPC_SCHEDULE_MODIFIER_simd;}
                       ;
schedule_enum_kind : STATIC     { current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_static); }
                   | DYNAMIC    {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_dynamic);}
                   | GUIDED     {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_guided);}
                   | AUTO       {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_auto);}
                   | RUNTIME    {current_clause = current_directive->addOpenMPClause(OMPC_schedule, firstParameter, secondParameter, OMPC_SCHEDULE_KIND_runtime);}
                   ;  
shared_clause : SHARED {
                current_clause = current_directive->addOpenMPClause(OMPC_shared);
					} '(' var_list ')'
				  ;

reduction_clause : REDUCTION { firstParameter = OMPC_REDUCTION_IDENTIFIER_unknown; } '(' reduction_parameter ':' var_list ')' {
					}
					;

reduction_parameter : reduction_identifier {}
					| reduction_modifier ',' reduction_identifier
					;

reduction_identifier : reduction_enum_identifier {	}
					| EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_user, $1); }
				  ;

reduction_modifier : MODIFIER_INSCAN 	{ firstParameter = OMPC_REDUCTION_MODIFIER_inscan; }
		   | MODIFIER_TASK 	{ firstParameter = OMPC_REDUCTION_MODIFIER_task; }
		   | MODIFIER_DEFAULT 	{ firstParameter = OMPC_REDUCTION_MODIFIER_default; }
		   ;

reduction_enum_identifier :  '+'		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_plus); }
						   | '-'		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_minus); }
						   | '*'		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_mul); }
						   | '&'		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitand); }
						   | '|'		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitor); }
						   | '^'		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitxor); }
						   | LOGAND		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_logand); }
						   | LOGOR		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_logor); }
						   | MAX		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_max); }
						   | MIN		{ current_clause = current_directive->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_min); }
						;

%%

int yyerror(const char *s) {
    // printf(" %s!\n", s);
	fprintf(stderr,"error: %s\n",s);
	// writeOutput (s);
    assert(0);
    return 0; // we want to the program to stop on error
}
 
int yywrap()
{
	return 1;
} 

// Standalone ompparser
OpenMPDirective* parseOpenMP(const char* input, void * _exprParse(const char*)) {
    
    printf("Start parsing...\n");
    exprParse = _exprParse;
    start_lexer(input);
    int res = yyparse();
    end_lexer();
    
    return current_directive;
}
