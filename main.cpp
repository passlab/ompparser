#include <stdio.h>
#include <OpenMPIR.h>
#include <string.h>
#include <iostream>

extern OpenMPDirective* parseOpenMP(const char*);

void output(OpenMPDirective*);

void output(OpenMPDirective* node) {
    
	// get graph
	//node->generateDOT(); // node->getLabel()

    std::cout << "\nDirective: " << node->getKind() << "\n";
    /*
    std::vector<OpenMPClause*>* clauses = node->getClauses();
    if (clauses != NULL) {
        std::vector<OpenMPClause*>::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            //std::cout << "    Clause: " << (*it)->getLabel() << "\n"; // (*it)->getKind()
			//std::cout << "        1st UDT Parameter: " << (*it)->getCustomFirstParameter() << "\n";
			//std::cout << "        2nd UDT Parameter: " << (*it)->getCustomSecondParameter()  << "\n";
            std::vector<const char*>* expr = (*it)->getExpr();
            if (expr != NULL) {
                std::vector<const char*>::iterator itExpr;
                for (itExpr = expr->begin(); itExpr != expr->end(); itExpr++) {
                    std::cout << "        Parameter: " << *itExpr << "\n";
                }
            }

        }
    }
    */
}

int main( int argc, const char* argv[] ) {

    const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) num_threads (3*5+4/(7+10)) firstprivate (foo(x), y), shared (a, b, c[1:10]), copyin (a[foo(goo(x)):20],a,y) default (none) if (parallel : b) proc_bind (master) reduction (inscan, + : a, foo(x)) reduction (abc : x, y, z) allocate (omp_high_bw_mem_alloc : m, n[1:5]) allocate (no, allo, cator)";

	//const char* input = "omp parallel reduction (tasktest : x11, y, z) allocate (user_defined_test : m, n[1:5]) allocate (omp_high_bw_mem_alloc : m, n[1:5]) reduction (inscan, max : a, foo(x))";

	//const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) num_threads (3*5+4/(7+10)) allocate (omp_user_defined_mem_alloc : m, n[1:5]) allocate (no, allo, cator)";
	
	
    OpenMPDirective* openMPAST = parseOpenMP(input);
    
    //output(openMPAST);
    printf("\n");


    return 0;
}

