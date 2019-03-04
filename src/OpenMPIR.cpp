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
    std::vector<OpenMPClause*>* current_clauses = getClauses(kind);
    va_list args;
    va_start(args, kind);
    OpenMPClause * new_clause = NULL;

    switch (kind) {
        case OMPC_if : {
            OpenMPIfClauseKind ifKind = (OpenMPIfClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPIfClause(ifKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else { /* could be an error since if clause may only appear once */
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
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
                new_clause = new OpenMPIfClause(ifKind);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_default : {
            OpenMPDefaultClauseKind defaultKind = (OpenMPDefaultClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPDefaultClause(defaultKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else { /* could be an error since if clause may only appear once */
                std::cerr << "Cannot have two default clause for the directive " << kind << ", ignored\n";
            }
            break;
        }
        case OMPC_num_threads :
        case OMPC_private :
        case OMPC_firstprivate :
        case OMPC_shared :
        case OMPC_num_teams :
 	    case OMPC_thread_limit :
        case OMPC_copyin :
	    case OMPC_collapse :
        case OMPC_ordered :
        case OMPC_order :
        case OMPC_nowait :
        case OMPC_safelen :
        case OMPC_simdlen :
        case OMPC_aligned :
        case OMPC_nontemporal : {

            if (current_clauses->size() == 0) {
                new_clause = new OpenMPClause(kind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                if (kind == OMPC_num_threads) {
                    std::cerr << "Cannot have two num_threads clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
            }

            break;
        }
        case OMPC_reduction : {
            OpenMPReductionClauseModifier modifier = (OpenMPReductionClauseModifier) va_arg(args, int);
            OpenMPReductionClauseIdentifier identifier = (OpenMPReductionClauseIdentifier) va_arg(args, int);
            char * userDefinedIdentifier = NULL;
            if (identifier == OMPC_REDUCTION_IDENTIFIER_user) userDefinedIdentifier = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPReductionClause(modifier, identifier);
                if (identifier == OMPC_REDUCTION_IDENTIFIER_user)
                    ((OpenMPReductionClause*)new_clause)->setUserDefinedIdentifier(userDefinedIdentifier);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPReductionClause*)(*it))->getModifier() == modifier &&
                        ((OpenMPReductionClause*)(*it))->getIdentifier() == identifier &&
                        strcasecmp(userDefinedIdentifier, ((OpenMPReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPReductionClause(modifier, identifier);
                if (identifier == OMPC_REDUCTION_IDENTIFIER_user)
                    ((OpenMPReductionClause*)new_clause)->setUserDefinedIdentifier(userDefinedIdentifier);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_proc_bind : {
            OpenMPProcBindClauseKind pbKind = (OpenMPProcBindClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPProcBindClause(pbKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else { /* could be an error since if clause may only appear once */
                std::cerr << "Cannot have two proc_bind clauses for the directive " << kind << ", ignored\n";
            }
            break;
        }

	case OMPC_lastprivate : {
	    OpenMPLastprivateClauseModifier modifier = (OpenMPLastprivateClauseModifier) va_arg(args,int);
            char * user_defined_modifier = NULL;
            if (modifier == OMPC_LASTPRIVATE_MODIFIER_user)  user_defined_modifier = va_arg(args, char*);
	    if (current_clauses->size() == 0) {
	        new_clause = new OpenMPLastprivateClause(modifier);
                if (modifier == OMPC_LASTPRIVATE_MODIFIER_user)
                    ((OpenMPLastprivateClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
	        current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
		clauses[kind] = current_clauses;
           	} else{
	            for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                        if (((OpenMPLastprivateClause*)(*it))->getModifier() == modifier&&
                            strcasecmp(user_defined_modifier, ((OpenMPLastprivateClause*)(*it))->getUserDefinedModifier()) == 0) {
                           new_clause = (*it);
                           goto end;
                    }
               }
                    new_clause = new OpenMPLastprivateClause(modifier);
                    if (modifier == OMPC_LASTPRIVATE_MODIFIER_user)
                       ((OpenMPLastprivateClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
                    current_clauses->push_back(new_clause);
	
	         }
                 break;
				 }

        case OMPC_linear :      {
	    OpenMPLinearClauseModifier modifier = (OpenMPLinearClauseModifier) va_arg(args,int);
            char * user_defined_modifier = NULL;
            if (modifier == OMPC_LINEAR_MODIFIER_user)  user_defined_modifier = va_arg(args, char*);
	    if (current_clauses->size() == 0) {
	        new_clause = new OpenMPLinearClause(modifier);
                if (modifier == OMPC_LINEAR_MODIFIER_user)
                    ((OpenMPLinearClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
	        current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
		clauses[kind] = current_clauses;
           	} else{
	            for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                        if (((OpenMPLinearClause*)(*it))->getModifier() == modifier&&
                            strcasecmp(user_defined_modifier, ((OpenMPLinearClause*)(*it))->getUserDefinedModifier()) == 0) {
                           new_clause = (*it);
                           goto end;
                    }
               }
                    new_clause = new OpenMPLinearClause(modifier);
                    if (modifier == OMPC_LINEAR_MODIFIER_user)
                       ((OpenMPLinearClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
                    current_clauses->push_back(new_clause);
	
	         }
                 break;        
        }

        case OMPC_schedule : {
            OpenMPScheduleClauseModifier modifier = (OpenMPScheduleClauseModifier) va_arg(args, int);
            OpenMPScheduleClauseKind schedulekind = (OpenMPScheduleClauseKind) va_arg(args, int);
            char * user_defined_kind = NULL;
            if (schedulekind == OMPC_SCHEDULE_KIND_user) user_defined_kind = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPScheduleClause(modifier, schedulekind);
                if (schedulekind == OMPC_SCHEDULE_KIND_user)
                    ((OpenMPScheduleClause*)new_clause)->setUserDefinedKind(user_defined_kind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPScheduleClause*)(*it))->getModifier() == modifier &&
                        ((OpenMPScheduleClause*)(*it))->getKind() == schedulekind &&
                        strcasecmp(user_defined_kind, ((OpenMPScheduleClause*)(*it))->getUserDefinedKind()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPScheduleClause(modifier, schedulekind);
                if (schedulekind == OMPC_SCHEDULE_KIND_user)
                    ((OpenMPScheduleClause*)new_clause)->setUserDefinedKind(user_defined_kind);
                current_clauses->push_back(new_clause);
            }
            break;
        }

        case OMPC_allocate : {
            OpenMPAllocateClauseAllocator allocator = (OpenMPAllocateClauseAllocator) va_arg(args, int);
            char* user_defined_allocator = NULL;
            if (allocator == OMPC_ALLOCATE_ALLOCATOR_user) user_defined_allocator = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPAllocateClause(allocator);
                if (allocator == OMPC_ALLOCATE_ALLOCATOR_user)
                    ((OpenMPAllocateClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_allocator_expression;
                    if (user_defined_allocator) {
                        current_user_defined_allocator_expression = std::string(user_defined_allocator);
                    };
                    if (((OpenMPAllocateClause*)(*it))->getAllocator() == allocator && current_user_defined_allocator_expression.compare(((OpenMPAllocateClause*)(*it))->getUserDefinedAllocator()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could find the matching object for this clause */
                new_clause = new OpenMPAllocateClause(allocator);
                if (allocator == OMPC_ALLOCATE_ALLOCATOR_user)
                    ((OpenMPAllocateClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_when: {
            if (current_clauses->size() == 0) {
                current_clauses = new std::vector<OpenMPClause *>();
                clauses[kind] = current_clauses;
            };
            new_clause = new OpenMPWhenClause();
            current_clauses->push_back(new_clause);
            break;
        }
    }
end:
    va_end(args);
    return new_clause;
}

std::string OpenMPDirective::generatePragmaString() {

    std::string result = "omp ";

    result += this->toString();

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses = this->getAllClauses();
    if (clauses != NULL) {
        std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            std::vector<OpenMPClause*>* current_clauses = it->second;
            std::vector<OpenMPClause*>::iterator clauseIter;
            for (clauseIter = current_clauses->begin(); clauseIter != current_clauses->end(); clauseIter++) {
                result += (*clauseIter)->toString();
            }
        }
        result = result.substr(0, result.size()-1);
    }

    return result;
};

std::string OpenMPDirective::toString() {

    std::string result;

    switch (this->getKind()) {
        case OMPD_parallel:
            result += "parallel ";
            break;
        case OMPD_teams:
	    result += "teams " ;
	    break;
        case OMPD_for:
            result += "for ";
            break;
        case OMPD_simd:
            result += "simd ";
            break;
        default:
            printf("The directive enum is not supported yet.\n");
    };

    return result;
}

std::string OpenMPClause::expressionToString() {

    std::string result;
    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        for (it = expr->begin(); it != expr->end(); it++) {
            result += std::string(*it) + ", ";
        };
        result = result.substr(0, result.size()-2);
    }

    return result;
}


std::string OpenMPClause::toString() {

    std::string result;

    switch (this->getKind()) {
        case OMPC_allocate:
            result += "allocate ";
            break;
        case OMPC_private:
            result += "private ";
            break;
        case OMPC_firstprivate:
            result += "firstprivate ";
            break;
        case OMPC_shared:
            result += "shared ";
            break;
	case OMPC_num_teams:
            result += "num_teams ";
            break;
        case OMPC_num_threads:
            result += "num_threads ";
            break;
	case OMPC_thread_limit:
            result += "thread_limit ";;
            break;
	case OMPC_default:
            result += "default ";
            break;
	case OMPC_lastprivate:
            result += "lastprivate ";
            break;
        case OMPC_linear:
            result += "linear ";
            break;
        case OMPC_reduction:
            result += "reduction ";
            break;
        case OMPC_schedule:
            result += "schedule ";
            break;
        case OMPC_collapse:
            result += "collapse ";
            break;
        case OMPC_ordered:
            result += "ordered ";
            break;
        case OMPC_nowait:
            result += "nowait ";
            break;
        case OMPC_order:
            result += "order ";
            break;
	case OMPC_safelen:
            result += "safelen ";
            break;
	case OMPC_simdlen:
            result += "simdlen ";
            break;
	case OMPC_aligned:
            result += "aligned ";
            break;
	case OMPC_nontemporal:
            result += "nontemporal ";
            break;
        default:
            printf("The clause enum is not supported yet.\n");
    }

    std::string clause_string = "(";
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    }

    return result;

}

void OpenMPDirective::generateDOT() {

    std::string directive_kind = this->toString();

    std::string current_line = "graph OpenMPIR_" + directive_kind + " {\n";

    std::string filename = "OpenMPIR_" + directive_kind.substr(0, directive_kind.size()-1) + ".dot";
    std::ofstream output(filename.c_str());

    output << current_line.c_str();

    current_line = "";

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses = this->getAllClauses();
    if (clauses != NULL) {
        std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            std::vector<OpenMPClause*>* current_clauses = it->second;
            std::vector<OpenMPClause*>::iterator clauseIter;
            for (clauseIter = current_clauses->begin(); clauseIter != current_clauses->end(); clauseIter++) {
                (*clauseIter)->generateDOT(output, directive_kind);
            }
        }
    }

    if (current_line == "") {
        output << directive_kind.c_str() << "\n";
    }

    output << "}\n";

};

void OpenMPClause::generateDOT(std::ofstream& dot_file, std::string directive_kind) {

    std::string current_line;
    std::string clause_kind;
    switch (this->getKind()) {
        case OMPC_private:
            clause_kind = "private ";
            break;
        case OMPC_firstprivate:
            clause_kind = "firstprivate ";
            break;
        case OMPC_shared:
            clause_kind = "shared ";
            break;
  case OMPC_num_threads:
            clause_kind = "num_threads ";
            break;
  case OMPC_num_teams:
            clause_kind = "num_teams ";
            break;
case OMPC_thread_limit:
            clause_kind = "thread_limit ";
            break;
case OMPC_default:
            clause_kind = "default ";
            break;
	case OMPC_lastprivate:
            clause_kind += "lastprivate ";
            break;
        case OMPC_linear:
            clause_kind += "linear ";
            break;
        case OMPC_reduction:
            clause_kind += "reduction ";
            break;
        case OMPC_schedule:
            clause_kind += "schedule ";
            break;
        case OMPC_collapse:
            clause_kind += "collapse ";
            break;
        case OMPC_ordered:
            clause_kind += "ordered ";
            break;
        case OMPC_nowait:
            clause_kind += "nowait ";
            break;
        case OMPC_allocate:
            clause_kind += "allocate ";
            break;
        case OMPC_order:
            clause_kind += "order ";
            break;	
	case OMPC_safelen:
            clause_kind += "safelen ";
            break;
	case OMPC_simdlen:
            clause_kind += "simdlen ";
            break;
	case OMPC_aligned:
            clause_kind += "aligned ";
            break;
	case OMPC_nontemporal:
            clause_kind += "nontemporal ";
            break;
        default:
            printf("The clause enum is not supported yet.\n");
    }
    current_line = "\t" + directive_kind + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int idx = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind.substr(0, clause_kind.size()-1) + "_expr" + std::to_string(idx);
            idx += 1;
            current_line = "\t\t" + clause_kind + "-- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = "\t\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    }

}



