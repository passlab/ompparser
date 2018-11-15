/* OpenMP C/C++/Fortran Grammar */

%name-prefix "omp_"
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
extern int omp_lex(); 

/*A customized initialization function for the scanner, str is the string to be scanned.*/
extern void omp_lexer_init(const char* str);

/* Standalone ompparser */
extern void start_lexer(const char* input);
extern void end_lexer(void);

//The directive/clause that are being parsed
static OpenMPDirective* currentDirective = NULL;
static OpenMPClause * currentClause = NULL;
static int firstParameter;
static int secondParameter;
static int thirdParameter;
static int fourthParameter;

/* Treat the entire expression as a string for now */
extern void omp_parse_expr();
static int omp_error(const char*);
static const char* orig_str;

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

%token  OMP PARALLEL
        IF NUM_THREADS DEFAULT PRIVATE FIRSTPRIVATE SHARED COPYIN REDUCTION PROC_BIND ALLOCATE
        NONE MASTER CLOSE SPREAD INSCAN TASK
        PLUS MINUS STAR BITAND BITOR BITXOR LOGAND LOGOR EQV NEQV MAX MIN
        ALLOCATOR_DEFAULT ALLOCATOR_LARGE_CAP ALLOCATOR_CONS_MEM ALLOCATOR_HIGH_BW ALLOCATOR_LOW_LAT ALLOCATOR_CGROUP
        ALLOCATOR_PTEAM ALLOCATOR_THREAD
%token <itype> ICONSTANT
%token <stype> EXPRESSION ID_EXPRESSION EXPR_STRING
/* associativity and precedence */
%left '<' '>' '=' "!=" "<=" ">="
%left '+' '-'
%left '*' '/' '%'

%type <stype> expression

/* start point for the parsing */
%start openmp_directive

%%

/* lang-dependent expression is only used in clause, at this point, the currentClause object should already be created. */
expression : EXPR_STRING { currentClause->addLangExpr($1); }
variable :   EXPR_STRING { currentClause->addLangExpr($1); } /* we use expression for variable so far */

expr_list : expression
        | expr_list ',' expression
        ;

var_list : variable
        | var_list ',' variable
        ;

openmp_directive : parallel_directive 
                 ;

parallel_directive : OMP PARALLEL {
                       currentDirective = new OpenMPDirective(OMPD_parallel);
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

if_clause: IF '(' if_parameter ')' { ; }
                    ;

if_parameter :  PARALLEL ':' {
                currentClause = currentDirective->addOpenMPClause(OMPC_if, OMPC_IF_parallel);
                } expression { ; }
              | SIMD ':' {
                currentClause = currentDirective->addOpenMPClause(OMPC_if, OMPC_IF_simd);
                } expression { ; }
              | SIMD ':' {
                currentClause = currentDirective->addOpenMPClause(OMPC_if, OMPC_IF_task);
                } expression { ; }
              | EXPR_STRING {
                currentClause = currentDirective->addOpenMPClause(OMPC_if);
                currentClause->addLangExpr($1);
                }
				;

num_threads_clause: NUM_THREADS {
                            currentClause = currentDirective->addOpenMPClause(OMPC_num_threads);
                         } '(' expression ')'
                      ;

copyin_clause: COPYIN {
                currentClause = currentDirective->addOpenMPClause(OMPC_copyin);
				} '(' var_list ')'
			  ;

proc_bind_clause : PROC_BIND '(' proc_bind_parameter ')' { } ;

proc_bind_parameter : MASTER    { currentClause = currentDirective->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_master); }
                    | CLOSE   { currentClause = currentDirective->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_close); }
                    | SPREAD  { currentClause = currentDirective->addOpenMPClause(OMPC_proc_bind, MPC_PROC_BIND_spread); }
                    ;

allocate_clause : ALLOCATE '(' allocate_parameter ')' ;

allocate_parameter :   VAR_STRING  { currentClause = currentDirective->addOpenMPClause(OMPC_allocate); currentClause->addLangExpr($2);  }
                     | VAR_STRING "," {
                         currentClause = currentDirective->addOpenMPClause(OMPC_allocate); currentClause->addLangExpr($2); }
                       var_list
                     | allocator_parameter ':' { ; } var_list
                      ;

allocator_parameter : DEFAULT_MEM_ALLOC 		    { currentClause = currentDirective->addOpenMPClause(OMPC_allocate, MPC_ALLOCATE_default_storage); }
						  | LARGE_CAP_MEM_ALLOC		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_large_capacity); }
						  | CONST_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, MPC_ALLOCATE_constant_memory); }
						  | HIGH_BW_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_high_bandwidth); }
						  | LOW_LAT_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_low_latency); }
						  | CGROUP_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_group_access); }
						  | PTEAM_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_team_access); }
						  | THREAD_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_thread_access); }
						  | VAR_STRING              { currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_user_defined;
						                              currentClause->setUserDefinedAllocator($1); }
						;

private_clause : PRIVATE {
                currentClause = currentDirective->addOpenMPClause(OMPC_private);
					} '(' var_list ')' {
					}
				  ;

firstprivate_clause : FIRSTPRIVATE {
                currentClause = currentDirective->addOpenMPClause(OMPC_firstprivate);
						} '(' var_list ')' {
						}
					  ;

shared_clause : SHARED {
                currentClause = currentDirective->addOpenMPClause(OMPC_shared);
					} '(' var_list ')'
				  ;

reduction_clause : REDUCTION { firstParameter = unknown; } '(' reduction_parameter ':' var_list ')' {
					}
					;

reduction_parameter : reduction_identifier {}
					| reduction_modifier ',' reduction_identifier {}
					;

reduction_identifier : reduction_enum_identifier {	}
					| user_defined_first_parameter
				  ;

reduction_modifier : MODIFIER_INSCAN 	{ firstParameter = OMPC_REDUCTION_MODIFIER_inscan; }
					| MODIFIER_TASK 	{ firstParameter = OMPC_REDUCTION_MODIFIER_task; }
					| MODIFIER_DEFAULT 	{ firstParameter = OMPC_REDUCTION_MODIFIER_default); }
					;

reduction_enum_identifier :  '+'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_plus); }
						   | '-'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_minus); }
						   | '*'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_mul); }
						   | '&'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_bitand); }
						   | '|'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_bitor); }
						   | '^'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_bitxor); }
						   | LOGAND		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_logand); }
						   | LOGOR		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_logor); }
						   | MAX		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_max); }
						   | MIN		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_reduction_min); }
						;

%%

int yyerror(const char *s) {
    // printf(" %s!\n", s);
	fprintf(stderr,"error: %s\n",s);
	writeOutput (s);
    assert(0);
    return 0; // we want to the program to stop on error
}
 
int yywrap()
{
	return 1;
} 
