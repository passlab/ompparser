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
static OpenMPDirective* currentDirective = NULL;
static OpenMPClause* currentClause = NULL;
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

%token  OMP PARALLEL METADIRECTIVE
        IF NUM_THREADS DEFAULT PRIVATE FIRSTPRIVATE SHARED COPYIN REDUCTION PROC_BIND ALLOCATE SIMD TASK WHEN
        NONE MASTER CLOSE SPREAD MODIFIER_INSCAN MODIFIER_TASK MODIFIER_DEFAULT
        PLUS MINUS STAR BITAND BITOR BITXOR LOGAND LOGOR EQV NEQV MAX MIN
        DEFAULT_MEM_ALLOC LARGE_CAP_MEM_ALLOC CONST_MEM_ALLOC HIGH_BW_MEM_ALLOC LOW_LAT_MEM_ALLOC CGROUP_MEM_ALLOC
        PTEAM_MEM_ALLOC THREAD_MEM_ALLOC
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

/* lang-dependent expression is only used in clause, at this point, the currentClause object should already be created. */
expression : EXPR_STRING { std::cout << $1 << "\n"; currentClause->addLangExpr($1); /*void * astnode = exprParse)($1);*/ }
variable :   EXPR_STRING { std::cout << $1 << "\n"; currentClause->addLangExpr($1); } /* we use expression for variable so far */

/*expr_list : expression
        | expr_list ',' expression
        ;
*/
var_list : variable
        | var_list ',' variable
        ;

openmp_directive : parallel_directive
                | metadirective_directive
                 ;

metadirective_directive : METADIRECTIVE {
                        currentDirective = new OpenMPDirective(OMPD_metadirective);
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

when_clause : WHEN { currentClause = currentDirective->addOpenMPClause(OMPC_when); }
                '(' context_selector_specification ':' { /*((OpenMPWhenClause*)currentClause)->setContextSelector(currentClause->getExpressions()->back());*/
                current_parent_directive = currentDirective;
                current_parent_clause = currentClause;
                } sub_directive { currentDirective->setParentConstruct(currentClause);
                ((OpenMPWhenClause*)current_parent_clause)->setSubDirective(currentDirective);
                currentDirective = current_parent_directive;
                currentClause = current_parent_clause;
                current_parent_directive = NULL;
                current_parent_clause = NULL;
                } ')' { } ;

sub_directive : openmp_directive;

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
              | TASK ':' {
                currentClause = currentDirective->addOpenMPClause(OMPC_if, OMPC_IF_task);
                } expression { ; }
              | EXPR_STRING {
                currentClause = currentDirective->addOpenMPClause(OMPC_if, OMPC_IF_unspecified);
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

default_clause : DEFAULT '(' default_parameter ')' { } ;

default_parameter : SHARED { currentClause = currentDirective->addOpenMPClause(OMPC_default, OMPC_DEFAULT_shared); }
                    | NONE { currentClause = currentDirective->addOpenMPClause(OMPC_default, OMPC_DEFAULT_none); }
                    | FIRSTPRIVATE { currentClause = currentDirective->addOpenMPClause(OMPC_default, OMPC_DEFAULT_firstprivate); }
                    | PRIVATE { currentClause = currentDirective->addOpenMPClause(OMPC_default, OMPC_DEFAULT_private); }
                    | sub_directive
                    ;


proc_bind_clause : PROC_BIND '(' proc_bind_parameter ')' { } ;

proc_bind_parameter : MASTER    { currentClause = currentDirective->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_master); }
                    | CLOSE   { currentClause = currentDirective->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_close); }
                    | SPREAD  { currentClause = currentDirective->addOpenMPClause(OMPC_proc_bind, OMPC_PROC_BIND_spread); }
                    ;

allocate_clause : ALLOCATE '(' allocate_parameter ')' ;

allocate_parameter :   EXPR_STRING  { std::cout << $1 << "\n"; currentClause = currentDirective->addOpenMPClause(OMPC_allocate); currentClause->addLangExpr($1);  }
                     | EXPR_STRING ',' {std::cout << $1 << "\n";
                         currentClause = currentDirective->addOpenMPClause(OMPC_allocate); currentClause->addLangExpr($1); } var_list
                     | allocator_parameter ':' { ; } var_list
                      ;

allocator_parameter : DEFAULT_MEM_ALLOC 		    { currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_default); }
						  | LARGE_CAP_MEM_ALLOC		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_large_cap); }
						  | CONST_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cons_mem); }
						  | HIGH_BW_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_high_bw); }
						  | LOW_LAT_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_low_lat); }
						  | CGROUP_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_cgroup); }
						  | PTEAM_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_pteam); }
						  | THREAD_MEM_ALLOC 		{ currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_thread); }
						  | EXPR_STRING { std::cout << $1 << "\n"; currentClause = currentDirective->addOpenMPClause(OMPC_allocate, OMPC_ALLOCATE_ALLOCATOR_user, $1); }
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

reduction_clause : REDUCTION { firstParameter = OMPC_REDUCTION_IDENTIFIER_unknown; } '(' reduction_parameter ':' var_list ')' {
					}
					;

reduction_parameter : reduction_identifier {}
					| reduction_modifier ',' reduction_identifier
					;

reduction_identifier : reduction_enum_identifier {	}
					| EXPR_STRING { std::cout << $1 << "\n"; currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_user, $1); }
				  ;

reduction_modifier : MODIFIER_INSCAN 	{ firstParameter = OMPC_REDUCTION_MODIFIER_inscan; }
					| MODIFIER_TASK 	{ firstParameter = OMPC_REDUCTION_MODIFIER_task; }
					| MODIFIER_DEFAULT 	{ firstParameter = OMPC_REDUCTION_MODIFIER_default; }
					;

reduction_enum_identifier :  '+'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_plus); }
						   | '-'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_minus); }
						   | '*'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_mul); }
						   | '&'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitand); }
						   | '|'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitor); }
						   | '^'		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_bitxor); }
						   | LOGAND		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_logand); }
						   | LOGOR		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_logor); }
						   | MAX		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_max); }
						   | MIN		{ currentClause = currentDirective->addOpenMPClause(OMPC_reduction, firstParameter, OMPC_REDUCTION_IDENTIFIER_min); }
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
    
    return currentDirective;
}
