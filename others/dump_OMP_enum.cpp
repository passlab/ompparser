
/* this file is used to dump the enum definition of all the OpenMP directives and clauses.
 * we borrow code from clang/LLVM for that. Check OpenMPKinds.h and OpenMPKinds.def to see 
 * how the enum is implemented usign macro expansion. 
 * to see those enum definition, do the follow processing command usign g++:
 * g++ -E -Isrc dump_OMP_enum.cpp >dump_OMP_enum.E
 */
#include <OpenMPKinds.h>
int main( int argc, const char* argv[] )
{
    return 0;
}

