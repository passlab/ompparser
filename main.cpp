#include <stdio.h>
#include <OpenMPAttribute.h>

extern openMPNode* parseOpenMP(const char*);

int main( int argc, const char* argv[] )
{
    const char * parallel_for = "parallel for parallel for num_threads (3+5) private(a,b,c) shared (d,e,f)";

    const char* input = "omp parallel for reduction (+:a,b,c)";

    //OpenMPDirective* pfor = OpenMP_ParseDirective(OpenMPString);
    //parallel_for->generateDOT("pfile.dot");

    openMPNode* openMPAST = parseOpenMP(input);
    
    /* for future features, ignore now
    char * map = "map(to:A[0:100],B)";

    OpenMPClause* mapclause = OpenMP_ParseClause(OpenMPString);
    */

    return 0;
}

