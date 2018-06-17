#include <stdio.h>
#include <OpenMPAttribute.h>
int main( int argc, const char* argv[] )
{
    char * parallel_for = "parallel for parallel for num_threads (3+5) private(a,b,c) shared (d,e,f)";

    OpenMPDirective* pfor = OpenMP_ParseDirective(OpenMPString);
    parallel_for->generateDOT("pfile.dot");

    /* for future features, ignore now
    char * map = "map(to:A[0:100],B)";

    OpenMPClause* mapclause = OpenMP_ParseClause(OpenMPString);
    */

    return 0;
}

