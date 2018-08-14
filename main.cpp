#include <stdio.h>
#include <OpenMPAttribute.h>
#include <string.h>
#include <iostream>

extern OpenMPDirective* parseOpenMP(const char*);

// void omp_set_default_allocator (const omp_allocator_t *allocator);
void output(OpenMPDirective*);

void output(OpenMPDirective* node) {
    
	// get graph
	node->generateDOT(); // node->getLabel()

    std::cout << "\nDirective: " << node->getKind() << "\n";

    std::vector<OpenMPClause*>* clauses = node->getClauses();
    if (clauses != NULL) {
        std::vector<OpenMPClause*>::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            std::cout << "    Clause: " << (*it)->getKind() << "\n";
            std::vector<const char*>* expr = (*it)->getExpr();
            if (expr != NULL) {
                std::vector<const char*>::iterator itExpr;
                for (itExpr = expr->begin(); itExpr != expr->end(); itExpr++) {
                    std::cout << "        Parameter: " << *itExpr << "\n";
                }
            }

        }
    }
}


int main( int argc, const char* argv[] )
{
	// const omp_allocator_t *allocator;
    // const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) num_threads (3*5+4/(7+10)) num_threads(2) firstprivate (foo(x), y), shared (a, b, c[1:10]), copyin (a[foo(goo(x)):20],a,y) default (shared), default (none) if (a) if (parallel : b) proc_bind (master) proc_bind(close) proc_bind(spread) reduction (inscan, + : a, foo(x)) reduction (- : x, y, z) allocate (omp_user_defined_mem_alloc : m, n[1:5]) allocate (no, allo, cator)";

	// at this point allocator will pass like an ordinary string, but when it gets to parser, the parser will cast to an allocator
	// and initialize an allocator whose handle is passed to the ALLOCATOR clause.
	// TODO - create and pass allocator object from here
	const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) num_threads (3*5+4/(7+10)) allocate (omp_high_bw_mem_alloc : m, n[1:5]) allocate (no, allo, cator)";
	
    OpenMPDirective* openMPAST = parseOpenMP(input);
    
    output(openMPAST);
    printf("\n");

    /* for future features, ignore now
    char * map = "map(to:A[0:100],B)";

    OpenMPClause* mapclause = OpenMP_ParseClause(OpenMPString);
    */

    return 0;
}

