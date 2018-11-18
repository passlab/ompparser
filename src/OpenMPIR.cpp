#include "OpenMPIR.h"
#include <algorithm>

using namespace std;

/**
 * a list of language expressions, variables, etc that are not parsed by the ompparser
 */
/*
void OpenMPClause::addLangExpr(const char *expression, int line = 0, int col = 0) {
    //TODO: Here we need to do certain normlization, if an expression already exists, we ignore
	expressions.push_back(expression);
    locations.push_back(SourceLocation(line, col));
}
*/

    /*
class OpenMPDirective : public SourceLocation  {
    public:
        OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, ...) {
            switch kind {
                case OMPC_if: {
                    if (kind not in &getClauses()) {
                        &getClauses[kind] = new std::vector<OpenMPIfClause*>;
                    }
                    current_clauses = &getClauses[kind];
                    current_clauses->append(new OpenMPIfClause());
                }
            }
        }
}
    */
OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, ...) {
    printf(".........\n");
    return NULL;
}




