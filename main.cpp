#include <stdio.h>
#include <OpenMPAttribute.h>
#include <string.h>

extern openMPNode* parseOpenMP(const char*);

void output(openMPNode*);

void output(openMPNode* node) {
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


int main( int argc, const char* argv[] )
{
    const char * parallel_for = "parallel for parallel for num_threads (3+5) private(a,b,c) shared (d,e,f)";

    const char* input = "omp parallel for reduction (+:a,b,c)";

    //OpenMPDirective* pfor = OpenMP_ParseDirective(OpenMPString);
    //parallel_for->generateDOT("pfile.dot");

    openMPNode* openMPAST = parseOpenMP(input);
    
    output(openMPAST);
    printf("\n");

    /* for future features, ignore now
    char * map = "map(to:A[0:100],B)";

    OpenMPClause* mapclause = OpenMP_ParseClause(OpenMPString);
    */

    return 0;
}

