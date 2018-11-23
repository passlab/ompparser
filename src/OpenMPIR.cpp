#include "OpenMPIR.h"
#include <stdarg.h>

using namespace std;

/**
 *
 * @param kind
 * @param ..., parameters for the clause, the number of max number of parameters is determined by the kind since each clause
 * expects a fixed set of parameters.
 * @return
 */
OpenMPClause * OpenMPDirective::addOpenMPClause(OpenMPClauseKind kind, ... ) {

    // Check whether the given kind of clause exists first.
    // If not, create an empty vector.
    if (clauses.count(kind) == 0) {
        clauses.insert(std::pair<OpenMPClauseKind, std::vector<OpenMPClause*>*>(kind, new std::vector<OpenMPClause*>));
    };
    std::vector<OpenMPClause*>* currentClauses = getClauses(kind);
    va_list args;
    va_start(args, kind);
    OpenMPClause * newClause = NULL;

    switch (kind) {
        case OMPC_if : {
            OpenMPIfClauseKind ifKind = (OpenMPIfClauseKind) va_arg(args, int);
            if (currentClauses->size() == 0) {
                newClause = new OpenMPIfClause(ifKind);
                currentClauses = new std::vector<OpenMPClause*>();
                currentClauses->push_back(newClause);
                clauses[kind] = currentClauses;
            } else { /* could be an error since if clause may only appear once */
                for(std::vector<OpenMPClause*>::iterator it = currentClauses->begin(); it != currentClauses->end(); ++it) {
                    if (((OpenMPIfClause*)(*it))->getIfClauseKind() == ifKind) {
                        std::cerr << "Cannot have two if clauses (of the same kind) for the directive " << kind << "\n";
                        goto end;
                    }
                }
                /* we found two clauses, but of different ifClauseKind, e.g. one is parallel and the other is for,
                 * this happens only in combined constructs.
                 * TODO: We should do error check here, or other place to see whether
                 * the directive is a combined construct that accepts the specified ifClauseKind
                 */
                newClause = new OpenMPIfClause(ifKind);
                currentClauses->push_back(newClause);
            }
        }
        case OMPC_default : {
            OpenMPDefaultClauseKind defaultKind = (OpenMPDefaultClauseKind) va_arg(args, int);
            if (currentClauses->size() == 0) {
                newClause = new OpenMPDefaultClause(defaultKind);
                currentClauses = new std::vector<OpenMPClause*>();
                currentClauses->push_back(newClause);
                clauses[kind] = currentClauses;
            } else { /* could be an error since if clause may only appear once */
                std::cerr << "Cannot have two default clause for the directive " << kind << ", ignored\n";
            }
        }
        case OMPC_num_threads :
        case OMPC_private :
        case OMPC_firstprivate :
        case OMPC_shared :
        case OMPC_copyin : {

            if (currentClauses->size() == 0) {
                newClause = new OpenMPClause(kind);
                currentClauses = new std::vector<OpenMPClause*>();
                currentClauses->push_back(newClause);
                clauses[kind] = currentClauses;
            } else {
                if (kind == OMPC_num_threads) {
                    std::cerr << "Cannot have two num_threads clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    newClause = currentClauses->at(0);
                }
            }

            break;
        }
        case OMPC_reduction : {
            OpenMPReductionClauseModifier modifier = (OpenMPReductionClauseModifier) va_arg(args, int);
            OpenMPReductionClauseIdentifier identifier = (OpenMPReductionClauseIdentifier) va_arg(args, int);
            char * userDefinedIdentifier = NULL;
            if (identifier == OMPC_REDUCTION_IDENTIFIER_user) userDefinedIdentifier = va_arg(args, char *);
            if (currentClauses->size() == 0) {
                newClause = new OpenMPReductionClause(modifier, identifier);
                if (identifier == OMPC_REDUCTION_IDENTIFIER_user)
                    ((OpenMPReductionClause*)newClause)->setUserDefinedIdentifier(userDefinedIdentifier);
                currentClauses = new std::vector<OpenMPClause*>();
                currentClauses->push_back(newClause);
                clauses[kind] = currentClauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = currentClauses->begin(); it != currentClauses->end(); ++it) {
                    if (((OpenMPReductionClause*)(*it))->getModifier() == modifier &&
                        ((OpenMPReductionClause*)(*it))->getIdentifier() == identifier &&
                        strcasecmp(userDefinedIdentifier, ((OpenMPReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                        newClause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                newClause = new OpenMPReductionClause(modifier, identifier);
                if (identifier == OMPC_REDUCTION_IDENTIFIER_user)
                    ((OpenMPReductionClause*)newClause)->setUserDefinedIdentifier(userDefinedIdentifier);
                currentClauses->push_back(newClause);
            }
        }
        case OMPC_proc_bind : {
            OpenMPProcBindClauseKind pbKind = (OpenMPProcBindClauseKind) va_arg(args, int);
            if (currentClauses->size() == 0) {
                newClause = new OpenMPProcBindClause(pbKind);
                currentClauses = new std::vector<OpenMPClause*>();
                currentClauses->push_back(newClause);
                clauses[kind] = currentClauses;
            } else { /* could be an error since if clause may only appear once */
                std::cerr << "Cannot have two proc_bind clauses for the directive " << kind << ", ignored\n";
            }
        }
        case OMPC_allocate : {
            OpenMPAllocateClauseAllocator allocator = (OpenMPAllocateClauseAllocator) va_arg(args, int);
            char * userDefinedAllocator = NULL;
            if (allocator == OMPC_ALLOCATE_ALLOCATOR_user) userDefinedAllocator = va_arg(args, char *);
            if (currentClauses->size() == 0) {
                newClause = new OpenMPAllocateClause(allocator);
                if (allocator == OMPC_ALLOCATE_ALLOCATOR_user)
                    ((OpenMPAllocateClause*)newClause)->setUserDefinedAllocator(userDefinedAllocator);
                currentClauses = new std::vector<OpenMPClause*>();
                currentClauses->push_back(newClause);
                clauses[kind] = currentClauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = currentClauses->begin(); it != currentClauses->end(); ++it) {
                    if (((OpenMPAllocateClause*)(*it))->getAllocator() == allocator &&
                            strcasecmp(userDefinedAllocator, ((OpenMPAllocateClause*)(*it))->getUserDefinedAllocator()) == 0) {
                        newClause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                newClause = new OpenMPAllocateClause(allocator);
                if (allocator == OMPC_ALLOCATE_ALLOCATOR_user)
                    ((OpenMPAllocateClause*)newClause)->setUserDefinedAllocator(userDefinedAllocator);
                currentClauses->push_back(newClause);
            }
        }
    }
end:
    va_end(args);
    return newClause;
}




