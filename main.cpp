#include <stdio.h>
#include <OpenMPAttribute.h>
#include <string.h>
#include <iostream>

extern OpenMPDirective* parseOpenMP(const char*);

void output(OpenMPDirective*);

void output(OpenMPDirective* node) {
    printf("Directive: ");
    //printf(node->getKind());
    std::cout << node->getKind();
    printf("\n");

    std::vector<OpenMPClause*>* clauses = node->getClauses();
    if (clauses != NULL) {
        std::vector<OpenMPClause*>::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            //printf(*it->getKind());
            //std::cout << *it->getKind() << "\n";
            std::cout << "    Clause: " << "\n";

        }
    }
}


int main( int argc, const char* argv[] )
{
    // const char * input = "omp parallel for num_threads (3+5) private(a,b,c) shared (d,e,f)";
    const char* input = "omp parallel private (a+b) private (123) private (foo(x))";
    // const char* input = "omp parallel for reduction (+:a,b,c) reduction (whatever:foo(x):goo(y+8)) reduction (2+3*6-8) // Some comments.";

    //OpenMPDirective* pfor = OpenMP_ParseDirective(OpenMPString);
    //parallel_for->generateDOT("pfile.dot");

    OpenMPDirective* openMPAST = parseOpenMP(input);
    
    output(openMPAST);
    printf("\n");

    /* for future features, ignore now
    char * map = "map(to:A[0:100],B)";

    OpenMPClause* mapclause = OpenMP_ParseClause(OpenMPString);
    */

    return 0;
}

