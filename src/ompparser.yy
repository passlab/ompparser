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


%token  OMP PARALLEL FOR METADIRECTIVE
        IF NUM_THREADS DEFAULT PRIVATE FIRSTPRIVATE SHARED COPYIN REDUCTION PROC_BIND ALLOCATE SIMD TASK LASTPRIVATE  WHEN
        LINEAR SCHEDULE COLLAPSE NOWAIT ORDER ORDERED MODIFIER_CONDITIONAL MODIFIER_MONOTONIC MODIFIER_NOMONOTONIC STATIC DYNAMIC GUIDED AUTO RUNTIME MODOFIER_VAL MODOFIER_REF MODOFIER_UVAL MODIFIER_SIMD
        SAFELEN SIMDLEN ALIGNED NONTEMPORAL/*YAYING*/
        NONE MASTER CLOSE SPREAD MODIFIER_INSCAN MODIFIER_TASK MODIFIER_DEFAULT 
        PLUS MINUS STAR BITAND BITOR BITXOR LOGAND LOGOR EQV NEQV MAX MIN
        DEFAULT_MEM_ALLOC LARGE_CAP_MEM_ALLOC CONST_MEM_ALLOC HIGH_BW_MEM_ALLOC LOW_LAT_MEM_ALLOC CGROUP_MEM_ALLOC
        PTEAM_MEM_ALLOC THREAD_MEM_ALLOC
        TEAMS
        NUM_TEAMS THREAD_LIMIT
        END USER CONSTRUCT DEVICE IMPLEMENTATION CONDITION SCORE

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
		 | for_directive
		 | simd_directive
                 | teams_directive
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
                '(' context_selector_specification ':' { /*((OpenMPWhenClause*)current_clause)->setContextSelector(current_clause->getExpressions()->back());*/
                current_parent_directive = current_directive;
                current_parent_clause = current_clause;
                } when_sub_directive { current_directive->setParentConstruct(current_clause);
                ((OpenMPWhenClause*)current_parent_clause)->setSubDirective(current_directive);
                current_directive = current_parent_directive;
                current_clause = current_parent_clause;
                current_parent_directive = NULL;
                current_parent_clause = NULL;
                } ')' { } ;

when_sub_directive : openmp_directive
                | { ; }
                ;

context_selector_specification : trait_set_selector
                | context_selector_specification trait_set_selector
                | context_selector_specification ',' trait_set_selector
                ;

trait_set_selector : trait_set_selector_name '=' '{' trait_selector_list '}'
                ;

trait_set_selector_name : USER
                | CONSTRUCT
                | DEVICE
                | IMPLEMENTATION
                ;

trait_selector_list : trait_selector
                | trait_selector_list trait_selector
                | trait_selector_list ',' trait_selector
                ;

trait_selector : condition_selector
                | parallel_selector
                ;

condition_selector : CONDITION '(' expression ')'
                ;

parallel_selector : PARALLEL
                | PARALLEL '(' parallel_selector_parameter ')'
                ;

parallel_selector_parameter : trait_score ':' parallel_clause_optseq
                | parallel_clause_optseq
                ;

trait_score : SCORE '(' expression ')'
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
                    | default_sub_directive
                    ;


default_sub_directive : openmp_directive
                    ;

proc_bind_clause : PROC_BIND '(' proc_bind_parameter ')' { } ;

proc_bind_parameter : MASTER    { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_master); }
                    | CLOSE   { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_close); }
                    | SPREAD  { current_clause = current_directive->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_spread); }
                    ;

allocate_clause : ALLOCATE '(' allocate_parameter ')' ;

allocate_parameter :   EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_allocate); current_clause->addLangExpr($1);  }
                     | EXPR_STRING ',' {std::cout << $1 << "\n";
                         current_clause = current_directive->addOpenMPClause(OMPC_allocate); current_clause->addLangExpr($1); } var_list
                     | allocator_parameter ':' { ; } var_list
                      ;

allocator_parameter : DEFAULT_MEM_ALLOC 		    { current_clause = current_directive->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_default); }
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
                | EXPR_STRING { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_linear, OMPC_LINEAR_MODIFIER_user, $1); }
                ;

aligned_clause : ALIGNED '('  aligned_parameter ')'
               | ALIGNED '('  aligned_parameter ':' var_list')'
	       ;

aligned_parameter : EXPR_STRING  { std::cout << $1 << "\n"; current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1);  }
                  | EXPR_STRING ','  {std::cout << $1 << "\n";} {current_clause = current_directive->addOpenMPClause(OMPC_aligned); current_clause->addLangExpr($1); } var_list
                  | linear_modifier '(' var_list ')'
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

order_clause: ORDER  {current_clause = current_directive->addOpenMPClause(OMPC_order);} '(' var_list ')'
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
