#include <stdio.h>
#include <OpenMPAttribute.h>
#include <string.h>

extern OpenMPDirective* parseOpenMP(const char*);
/*
void output(OpenMPDirective*);

void output(OpenMPDirective* node) {
    print("Directive: ");
    print(node->getKind());
    if (strcmp(node->getType(), "directive") == 0) {
        printf("\n    directive: ");
    }
    else if (strcmp(node->getType(), "clause") == 0) {
        printf("\n        clause: ");
    }
    else if (strcmp(node->getType(), "parameter") == 0) {
        printf("\n            parameter: ");
    }
    else {
        printf("\nroot: ");
    };

    printf(node->getVal());
    std::vector<openMPNode*>* children = node->getChildren();
    if (children != NULL) {
        std::vector<openMPNode*>::iterator it;
        for (it = children->begin(); it != children->end(); it++) {
            output(*it);
        }
    }
}
*/

int main( int argc, const char* argv[] )
{
    // const char * input = "omp parallel for num_threads (3+5) private(a,b,c) shared (d,e,f)";
    const char* input = "omp parallel private (a+b)";
    // const char* input = "omp parallel for reduction (+:a,b,c) reduction (whatever:foo(x):goo(y+8)) reduction (2+3*6-8) // Some comments.";

    //OpenMPDirective* pfor = OpenMP_ParseDirective(OpenMPString);
    //parallel_for->generateDOT("pfile.dot");

    OpenMPDirective* openMPAST = parseOpenMP(input);
    
    //output(openMPAST);
    printf("\n");

    /* for future features, ignore now
    char * map = "map(to:A[0:100],B)";

    OpenMPClause* mapclause = OpenMP_ParseClause(OpenMPString);
    */

    return 0;
}

