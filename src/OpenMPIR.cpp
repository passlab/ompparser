#include "OpenMPIR.h"
#include <stdarg.h>


void OpenMPClause::addLangExpr(const char *expression, int line, int col) {
    // Since the size of expression vector is supposed to be small, brute force is used here.
    for (int i = 0; i < this->expressions.size(); i++) {
        if (!strcmp(expressions.at(i), expression)) {
            return;
        };
    };
    expressions.push_back(expression);
    locations.push_back(SourceLocation(line, col));
};

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
        case OMPC_if: {
        OpenMPIfClauseModifier modifier = (OpenMPIfClauseModifier) va_arg(args,int);
            char * user_defined_modifier = NULL;
            if (modifier == OMPC_IF_MODIFIER_user)  user_defined_modifier = va_arg(args, char*);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPIfClause(modifier);
                if (modifier == OMPC_IF_MODIFIER_user) {
                    ((OpenMPIfClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
                };
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for (std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_modifier_expression;
                    if (user_defined_modifier) {
                        current_user_defined_modifier_expression = std::string(user_defined_modifier);
                    };
                    if (((OpenMPIfClause*)(*it))->getModifier() == modifier &&
                        current_user_defined_modifier_expression.compare(((OpenMPIfClause*)(*it))->getUserDefinedModifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    };
                };
                new_clause = new OpenMPIfClause(modifier);
                if (modifier == OMPC_IF_MODIFIER_user) {
                    ((OpenMPIfClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
                }
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
        case OMPC_num_threads:
        case OMPC_private:
        case OMPC_firstprivate:
        case OMPC_shared:
        case OMPC_num_teams:
        case OMPC_thread_limit:
        case OMPC_copyin:
        case OMPC_collapse:
        case OMPC_ordered:
        case OMPC_order:
        case OMPC_nowait:
        case OMPC_safelen:
        case OMPC_simdlen:
        case OMPC_aligned:
        case OMPC_nontemporal:
        case OMPC_uniform:
        case OMPC_inbranch:
        case OMPC_notinbranch :
        case OMPC_copyprivate : 
        case OMPC_parallel:    
        case OMPC_sections: 
        case OMPC_for: 
        case OMPC_taskgroup:
        case OMPC_inclusive:
        case OMPC_exclusive:
        case OMPC_use_device_ptr :
        case OMPC_use_device_addr :
        case OMPC_grainsize :
        case OMPC_num_tasks :
        case OMPC_nogroup :
        case OMPC_final :
        case OMPC_untied :
        case OMPC_mergeable :
        case OMPC_priority :
        case OMPC_detach :
        case OMPC_reverse_offload :
        case OMPC_unified_address :
        case OMPC_unified_shared_memory :
        case OMPC_dynamic_allocators :
        case OMPC_is_device_ptr :
        case OMPC_link :
         {
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
        case OMPC_match: {
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPMatchClause();
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

        case OMPC_reduction: {
            OpenMPReductionClauseModifier modifier = (OpenMPReductionClauseModifier) va_arg(args, int);
            OpenMPReductionClauseIdentifier identifier = (OpenMPReductionClauseIdentifier) va_arg(args, int);
            char * user_defined_identifier = NULL;
            if (identifier == OMPC_REDUCTION_IDENTIFIER_user) user_defined_identifier = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPReductionClause(modifier, identifier);
                if (identifier == OMPC_REDUCTION_IDENTIFIER_user && user_defined_identifier) {
                    ((OpenMPReductionClause *) new_clause)->setUserDefinedIdentifier(user_defined_identifier);
                };
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for (std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); it++) {
                    std::string current_user_defined_identifier_expression;
                    if (user_defined_identifier) {
                        current_user_defined_identifier_expression = std::string(user_defined_identifier);
                    };
                    if (((OpenMPReductionClause*)(*it))->getModifier() == modifier &&
                    ((OpenMPReductionClause*)(*it))->getIdentifier() == identifier &&
                    current_user_defined_identifier_expression.compare(((OpenMPReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }

                /* could fine the matching object for this clause */
                new_clause = new OpenMPReductionClause(modifier, identifier);
                if (identifier == OMPC_REDUCTION_IDENTIFIER_user)
                    ((OpenMPReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_proc_bind: {
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
  
        case OMPC_bind: {
            OpenMPBindClauseKind bKind = (OpenMPBindClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPBindClause(bKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else { /* could be an error since if clause may only appear once */
                std::cerr << "Cannot have two bind clauses for the directive " << kind << ", ignored\n";
            }
            break;
        }

        case OMPC_lastprivate: {
            OpenMPLastprivateClauseModifier modifier = (OpenMPLastprivateClauseModifier) va_arg(args,int);
            char * user_defined_modifier = NULL;
            if (modifier == OMPC_LASTPRIVATE_MODIFIER_user)  user_defined_modifier = va_arg(args, char*);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPLastprivateClause(modifier);
                if (modifier == OMPC_LASTPRIVATE_MODIFIER_user) {
                    ((OpenMPLastprivateClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
                };
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
            clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_modifier_expression;
                    if (user_defined_modifier) {
                        current_user_defined_modifier_expression = std::string(user_defined_modifier);
                    };
                    if (((OpenMPLastprivateClause*)(*it))->getModifier() == modifier&&
                        current_user_defined_modifier_expression.compare(((OpenMPLastprivateClause*)(*it))->getUserDefinedModifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    };
                };
                new_clause = new OpenMPLastprivateClause(modifier);
                if (modifier == OMPC_LASTPRIVATE_MODIFIER_user) {
                    ((OpenMPLastprivateClause *) new_clause)->setUserDefinedModifier(user_defined_modifier);
                };
                current_clauses->push_back(new_clause);
            }
            break;
        }

        case OMPC_linear: {
            OpenMPLinearClauseModifier modifier = (OpenMPLinearClauseModifier) va_arg(args,int);
            char * user_defined_modifier = NULL;
            if (modifier == OMPC_LINEAR_MODIFIER_user)  user_defined_modifier = va_arg(args, char*);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPLinearClause(modifier);
                if (modifier == OMPC_LINEAR_MODIFIER_user) {
                    ((OpenMPLinearClause *) new_clause)->setUserDefinedModifier(user_defined_modifier);
                };
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_modifier_expression;
                    if (user_defined_modifier) {
                        current_user_defined_modifier_expression = std::string(user_defined_modifier);
                    };
                    if (((OpenMPLinearClause*)(*it))->getModifier() == modifier &&
                    current_user_defined_modifier_expression.compare(((OpenMPLinearClause*)(*it))->getUserDefinedModifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                new_clause = new OpenMPLinearClause(modifier);
                if (modifier == OMPC_LINEAR_MODIFIER_user) {
                    ((OpenMPLinearClause *) new_clause)->setUserDefinedModifier(user_defined_modifier);
                };
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_dist_schedule: {
            OpenMPDistscheduleClauseKind dist_schedule_kind = (OpenMPDistscheduleClauseKind) va_arg(args,int);
            char * user_defined_kind = NULL;
            if (dist_schedule_kind == OMPC_DISTSCHEDULE_KIND_user) user_defined_kind = va_arg(args, char*);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPDistscheduleClause(dist_schedule_kind);
                if (dist_schedule_kind == OMPC_DISTSCHEDULE_KIND_user) {
                    ((OpenMPDistscheduleClause *) new_clause)->setUserDefinedKind(user_defined_kind);
                }
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_kind_expression;
                    if (user_defined_kind) {
                        current_user_defined_kind_expression = std::string(user_defined_kind);
                    };
                    if (((OpenMPDistscheduleClause*)(*it))->getKind() == dist_schedule_kind &&
                    current_user_defined_kind_expression.compare(((OpenMPDistscheduleClause*)(*it))->getUserDefinedKind()) == 0) {
                        new_clause = (*it);
                        goto end;
                    };
                }
                new_clause = new OpenMPDistscheduleClause(dist_schedule_kind);
                if (dist_schedule_kind == OMPC_DISTSCHEDULE_KIND_user) {
                    ((OpenMPDistscheduleClause*)new_clause)->setUserDefinedKind(user_defined_kind);
                }
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_schedule: {
            OpenMPScheduleClauseModifier modifier1 = (OpenMPScheduleClauseModifier) va_arg(args, int);
            OpenMPScheduleClauseModifier modifier2 = (OpenMPScheduleClauseModifier) va_arg(args, int);
            OpenMPScheduleClauseKind schedulekind = (OpenMPScheduleClauseKind) va_arg(args, int);
            char * user_defined_kind = NULL;
            if (schedulekind == OMPC_SCHEDULE_KIND_user) user_defined_kind = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPScheduleClause(modifier1,modifier2, schedulekind);
                if (schedulekind == OMPC_SCHEDULE_KIND_user)
                    ((OpenMPScheduleClause*)new_clause)->setUserDefinedKind(user_defined_kind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_kind_expression;
                    if (user_defined_kind) {
                        current_user_defined_kind_expression = std::string(user_defined_kind);
                    };
                    if (((OpenMPScheduleClause*)(*it))->getModifier1() == modifier1 && ((OpenMPScheduleClause*)(*it))->getModifier2() == modifier2 &&
                        ((OpenMPScheduleClause*)(*it))->getKind() == schedulekind &&
                        current_user_defined_kind_expression.compare(((OpenMPScheduleClause*)(*it))->getUserDefinedKind()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPScheduleClause(modifier1,modifier2, schedulekind);
                if (schedulekind == OMPC_SCHEDULE_KIND_user)
                    ((OpenMPScheduleClause*)new_clause)->setUserDefinedKind(user_defined_kind);
                current_clauses->push_back(new_clause);
            }
            break;
        }
         case OMPC_device : {
            OpenMPDeviceClauseModifier modifier = (OpenMPDeviceClauseModifier) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPDeviceClause(modifier);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPDeviceClause*)(*it))->getModifier() == modifier) {
                        new_clause = (*it);
                        goto end;
                    }
                }
  /* could find the matching object for this clause */
                new_clause = new OpenMPDeviceClause(modifier);
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
        case OMPC_allocator : {
            OpenMPAllocatorClauseAllocator allocator = (OpenMPAllocatorClauseAllocator) va_arg(args, int);
            char* user_defined_allocator = NULL;
            if (allocator == OMPC_ALLOCATOR_ALLOCATOR_user) user_defined_allocator = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPAllocatorClause(allocator);
                if (allocator == OMPC_ALLOCATOR_ALLOCATOR_user)
                    ((OpenMPAllocatorClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_allocator_expression;
                    if (user_defined_allocator) {
                        current_user_defined_allocator_expression = std::string(user_defined_allocator);
                    };
                    if (((OpenMPAllocatorClause*)(*it))->getAllocator() == allocator && current_user_defined_allocator_expression.compare(((OpenMPAllocatorClause*)(*it))->getUserDefinedAllocator()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could find the matching object for this clause */
                new_clause = new OpenMPAllocatorClause(allocator);
                if (allocator == OMPC_ALLOCATOR_ALLOCATOR_user)
                    ((OpenMPAllocatorClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
                current_clauses->push_back(new_clause);
            }
            break;
        }
/*xinyao*/
        case OMPC_atomic_default_mem_order : {
            OpenMPAtomicDefaultMemOrderClauseKind atomic_default_mem_order_kind = (OpenMPAtomicDefaultMemOrderClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPAtomicDefaultMemOrderClause(atomic_default_mem_order_kind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else { /* could be an error since if clause may only appear once */
                std::cerr << "Cannot have two atomic_default_mem_orders clause for the directive " << kind << ", ignored\n";
            }
            break;
        }
       case OMPC_in_reduction : {
            OpenMPInReductionClauseIdentifier identifier = (OpenMPInReductionClauseIdentifier) va_arg(args, int);
            char * user_defined_identifier = NULL;
            if (identifier == OMPC_IN_REDUCTION_IDENTIFIER_user) user_defined_identifier = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPInReductionClause(identifier);
                if (identifier == OMPC_IN_REDUCTION_IDENTIFIER_user && user_defined_identifier) {
                    ((OpenMPInReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
                 };
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_identifier_expression;
                    if (user_defined_identifier) {
                        current_user_defined_identifier_expression = std::string(user_defined_identifier);
                    };
                    if (((OpenMPInReductionClause*)(*it))->getIdentifier() == identifier &&
                    current_user_defined_identifier_expression.compare(((OpenMPInReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPInReductionClause(identifier);
                if (identifier == OMPC_IN_REDUCTION_IDENTIFIER_user)
                    ((OpenMPInReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_depend : {
            OpenMPDependClauseModifier modifier = (OpenMPDependClauseModifier) va_arg(args, int);
            OpenMPDependClauseType type = (OpenMPDependClauseType) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPDependClause(modifier, type);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPDependClause*)(*it))->getModifier() == modifier &&
                        ((OpenMPDependClause*)(*it))->getType() == type) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPDependClause(modifier, type);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_affinity : {
            OpenMPAffinityClauseModifier modifier = (OpenMPAffinityClauseModifier) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPAffinityClause(modifier);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPAffinityClause*)(*it))->getModifier() == modifier){
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPAffinityClause(modifier);
                current_clauses->push_back(new_clause);
            }
            break;
        }
case OMPC_to : {
            OpenMPToClauseKind toKind = (OpenMPToClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPToClause(toKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPToClause*)(*it))->getKind() == toKind){
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPToClause(toKind);
                current_clauses->push_back(new_clause);
            }
            break;
        }
case OMPC_from : {
            OpenMPFromClauseKind fromKind = (OpenMPFromClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPFromClause(fromKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    if (((OpenMPFromClause*)(*it))->getKind() == fromKind){
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPFromClause(fromKind);
                current_clauses->push_back(new_clause);
            }
            break;
        }
 case OMPC_device_type : {
            OpenMPDeviceTypeClauseKind devicetypeKind = (OpenMPDeviceTypeClauseKind) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPDeviceTypeClause(devicetypeKind);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } 
            break;
        }
 
 case OMPC_defaultmap :      {
    OpenMPDefaultmapClauseBehavior behavior = (OpenMPDefaultmapClauseBehavior) va_arg(args,int);
            OpenMPDefaultmapClauseCategory category = (OpenMPDefaultmapClauseCategory) va_arg(args,int);
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDefaultmapClause(behavior,category);
        current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
clauses[kind] = current_clauses;
           } else{
            for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                        if (((OpenMPDefaultmapClause*)(*it))->getBehavior() == behavior&&((OpenMPDefaultmapClause*)(*it))->getCategory() == category) {
                           new_clause = (*it);
                           goto end;
                    }
               }
                    new_clause = new OpenMPDefaultmapClause(behavior,category);
                    current_clauses->push_back(new_clause);

         }
                 break;        
        }
        case OMPC_task_reduction : {
            OpenMPTaskReductionClauseIdentifier identifier = (OpenMPTaskReductionClauseIdentifier) va_arg(args, int);
            char * user_defined_identifier = NULL;
            if (identifier == OMPC_TASK_REDUCTION_IDENTIFIER_user) user_defined_identifier = va_arg(args, char *);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPTaskReductionClause(identifier);
                if (identifier == OMPC_TASK_REDUCTION_IDENTIFIER_user && user_defined_identifier) {
                    ((OpenMPTaskReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
                 };
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                    std::string current_user_defined_identifier_expression;
                    if (user_defined_identifier) {
                        current_user_defined_identifier_expression = std::string(user_defined_identifier);
                    };
                    if (((OpenMPTaskReductionClause*)(*it))->getIdentifier() == identifier &&
                    current_user_defined_identifier_expression.compare(((OpenMPTaskReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPTaskReductionClause(identifier);
                if (identifier == OMPC_TASK_REDUCTION_IDENTIFIER_user)
                    ((OpenMPTaskReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_map: {
            OpenMPMapClauseModifier modifier1 = (OpenMPMapClauseModifier) va_arg(args, int);
            OpenMPMapClauseModifier modifier2 = (OpenMPMapClauseModifier) va_arg(args, int);
            OpenMPMapClauseModifier modifier3 = (OpenMPMapClauseModifier) va_arg(args, int);
            OpenMPMapClauseType type = (OpenMPMapClauseType) va_arg(args, int);
            if (current_clauses->size() == 0) {
                new_clause = new OpenMPMapClause(modifier1,modifier2,modifier3, type);
                current_clauses = new std::vector<OpenMPClause*>();
                current_clauses->push_back(new_clause);
                clauses[kind] = current_clauses;
            } else {
                for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
                   
                    
                    if (((OpenMPMapClause*)(*it))->getModifier1() == modifier1 && ((OpenMPMapClause*)(*it))->getModifier2() == modifier2 &&((OpenMPMapClause*)(*it))->getModifier3() == modifier3&&
                        ((OpenMPMapClause*)(*it))->getType() == type) {
                        new_clause = (*it);
                        goto end;
                    }
                }
                /* could fine the matching object for this clause */
                new_clause = new OpenMPMapClause(modifier1,modifier2,modifier3, type);
                current_clauses->push_back(new_clause);
            }
            break;
        }
        case OMPC_when: {

            new_clause = OpenMPWhenClause::addWhenClause(this);
            break;
        }
        default: {
            ;
        }
    };
end:
    va_end(args);
    return new_clause;
}

std::string OpenMPDirective::generatePragmaString(std::string prefix, std::string beginning_symbol, std::string ending_symbol, bool output_score) {

    if (this->getBaseLang() == Lang_Fortran && prefix == "#pragma omp ") {
        prefix = "!$omp ";
    };
    std::string result = prefix;

    result += this->toString();

    result += beginning_symbol;

    switch (this->getKind()) {

        case OMPD_declare_variant: {
            result += "(" + ((OpenMPDeclareVariantDirective *) this)->getVariantFuncID() + ") ";
            break;
        }
        case OMPD_allocate: {
            std::vector<const char *> *list = ((OpenMPAllocateDirective *) this)->getAllocateList();
            std::vector<const char *>::iterator list_item;

            result += "(";
            for (list_item = list->begin(); list_item != list->end(); list_item++) {
                result += *list_item;
                result += ",";
            }
            result = result.substr(0, result.size() - 1);
            result += ") ";
            break;}
        case OMPD_threadprivate: {
            std::vector<const char*>* list = ((OpenMPThreadprivateDirective*)this)->getThreadprivateList();
            std::vector<const char*>::iterator list_item;
            result += "(";
            for (list_item = list->begin(); list_item != list->end(); list_item++){
                 result += *list_item;
                 result += ",";
            }
            result = result.substr(0, result.size()-1); 
            result += ") ";
            break;
        }
        case OMPD_end: {
            result += ((OpenMPEndDirective*)this)->getPairedDirective()->generatePragmaString("", "", "");
            break;
        }
        case OMPD_requires: {
            std::vector<std::string> *ext_user_defined_implementation = ((OpenMPRequiresDirective*)this)->getUserDefinedImplementation();

            if(ext_user_defined_implementation->size() > 0) {
                std::vector<std::string>::iterator list_item;
                for (list_item = ext_user_defined_implementation->begin(); list_item != ext_user_defined_implementation->end(); list_item++) {
                    result += "ext_" + *list_item;
                    result += " ";
                };
            };
            break;
        }
        default:
            ;
    };

    if (output_score) {
        std::string trait_score = this->getTraitScore();
        if (trait_score != "") {
            result += "score(" + trait_score + ") : ";
        };
    };

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses = this->getAllClauses();
    if (clauses->size() != 0) {
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

    result += ending_symbol;

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
        case OMPD_for_simd:
            result += "for simd ";
            break;
        case OMPD_declare:
            result += "declare ";
            break;
        case OMPD_distribute:
            result += "distribute ";
            break;
        case OMPD_distribute_simd:
            result += "distribute simd ";
            break;
        case OMPD_distribute_parallel_for:
            result += "distribute parallel for ";
            break;
        case OMPD_distribute_parallel_for_simd:
            result += "distribute parallel for simd ";
            break;
        case OMPD_loop:
            result += "loop ";
            break;
        case OMPD_scan:
            result += "scan ";
            break;
        case OMPD_sections:
            result += "sections ";
            break;
        case OMPD_section:
            result += "section ";
            break;
        case OMPD_single:
            result += "single ";
            break;
        case OMPD_cancel:
            result += "cancel ";
            break;
        case OMPD_cancellation_point:
            result += "cancellation point ";
            break;
        case OMPD_metadirective:
            result += "metadirective ";
            break;
        case OMPD_declare_variant:
            result += "declare variant ";
            break;
        case OMPD_allocate:
            result += "allocate ";
            break;
        case OMPD_task:
            result += "task ";
            break;
        case OMPD_taskloop:
            result += "taskloop ";
            break;
        case OMPD_taskloop_simd:
            result += "taskloop simd ";
            break;
        case OMPD_target_data:
            result += "target data ";
            break;
        case OMPD_taskyield:
            result += "taskyield ";
            break;
        case OMPD_requires:
            result += "requires ";
            break;
        case OMPD_target_enter_data:
            result += "target enter data ";
            break;
        case OMPD_target_exit_data:
            result += "target exit data ";
            break;
        case OMPD_target:
            result += "target ";
            break;
        case OMPD_target_update:
            result += "target update ";
            break;
        case OMPD_declare_target:
            result += "declare target ";
            break;
        case OMPD_end_declare_target:
            result += "end declare target ";
            break;
        case OMPD_master:
            result += "master ";
        case OMPD_threadprivate:
            result += "threadprivate ";
            break;
        case OMPD_end:
            result += "end ";
            break;
        case OMPD_barrier:
            result += "barrier ";
            break;
        case OMPD_taskwait:
            result += "taskwait ";
            break;
        case OMPD_taskgroup:
            result += "taskgroup ";
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
        case OMPC_allocator:
            result += "allocator ";
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
        case OMPC_copyin:
            result += "copyin ";
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
        case OMPC_uniform:
            result += "uniform ";
            break;
        case OMPC_inbranch:
            result += "inbranch ";
            break;
        case OMPC_notinbranch:
            result += "notinbranch ";
            break;
        case OMPC_bind:
            result += "bind ";
            break; 
        case OMPC_inclusive:
            result += "inclusive ";
            break;        
        case OMPC_exclusive:
            result += "exclusive ";
            break;
        case OMPC_copyprivate:
            result += "copyprivate ";
            break;        
        case OMPC_parallel:
            result += "parallel ";
            break;
        case OMPC_sections:
            result += "sections ";
            break;
        case OMPC_for:
            result += "for ";
            break;
        case OMPC_taskgroup:
            result += "taskgroup ";
            break;
        case OMPC_if:
            result += "if ";
            break;
        case OMPC_final:
            result += "final ";
            break;
        case OMPC_untied:
            result += "untied ";
            break;
        case OMPC_mergeable:
            result += "mergeable ";
            break;
        case OMPC_priority:
            result += "priority ";
            break;
        case OMPC_detach:
            result += "detach ";
            break;
        case OMPC_reverse_offload:
            result += "reverse_offload ";
            break;
        case OMPC_unified_address:
            result += "unified_address ";
            break;
        case OMPC_unified_shared_memory:
            result += "unified_shared_memory ";
            break;
        case OMPC_dynamic_allocators:
            result += "dynamic_allocators ";
            break;
        case OMPC_use_device_ptr:
            result += "use_device_ptr ";
            break;
        case OMPC_use_device_addr:
            result += "use_device_addr ";
            break;
        case OMPC_is_device_ptr:
            result += "is_device_ptr ";
            break;
        case OMPC_grainsize:
            result += "grainsize ";
            break;
        case OMPC_num_tasks:
            result += "num_tasks ";
            break;
        case OMPC_nogroup:
            result += "nogroup ";
            break;
        case OMPC_link:
            result += "link ";
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

/*xinyao*/
std::string OpenMPAtomicDefaultMemOrderClause::toString() {
    std::string result = "atomic_default_mem_order(";
    std::string parameter_string;
    OpenMPAtomicDefaultMemOrderClauseKind kind = this->getKind();
    switch (kind) {
        case OMPC_ATOMIC_DEFAULT_MEM_ORDER_seq_cst:
            parameter_string = "seq_cst";
            break;
        case OMPC_ATOMIC_DEFAULT_MEM_ORDER_acq_rel:
            parameter_string = "acq_rel";
            break;
        case OMPC_ATOMIC_DEFAULT_MEM_ORDER_relaxed:
            parameter_string = "relaxed";
            break;
        default:
            std::cout << "The parameter of tomic_default_mem_order clause is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        result += parameter_string + ") ";
    }
    else {
        result = result.substr(0, result.size()-2);
    }

    return result;
}
void OpenMPAtomicDefaultMemOrderClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "atomic_default_mem_order_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPAtomicDefaultMemOrderClauseKind kind = this->getKind();
    std::string parameter_string;
    switch (kind) {
       case OMPC_ATOMIC_DEFAULT_MEM_ORDER_seq_cst:
            parameter_string = "seq_cst";
            break;
        case OMPC_ATOMIC_DEFAULT_MEM_ORDER_acq_rel:
            parameter_string = "acq_rel";
            break;
        case OMPC_ATOMIC_DEFAULT_MEM_ORDER_relaxed:
            parameter_string = "relaxed";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_kind";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
};
void OpenMPInReductionClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "in_reduction_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPInReductionClauseIdentifier identifier = this->getIdentifier();
    std::string parameter_string;
    switch (identifier) {
        case OMPC_IN_REDUCTION_IDENTIFIER_plus:
            parameter_string = "+";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_minus:
            parameter_string  = "-";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_mul:
            parameter_string = "*";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_bitand:
            parameter_string = "&";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_bitor:
            parameter_string = "|";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_bitxor:
            parameter_string = "^";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_logand:
            parameter_string = "&&";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_logor:
            parameter_string = "||";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_min:
            parameter_string = "min";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_max:
            parameter_string = "max";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_user:
            parameter_string = this->getUserDefinedIdentifier();
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_identifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};

std::string OpenMPInReductionClause::toString() {

    std::string result = "in_reduction ";
    std::string clause_string = "(";
    OpenMPInReductionClauseIdentifier identifier = this->getIdentifier();
    switch (identifier) {
        case OMPC_IN_REDUCTION_IDENTIFIER_plus:
            clause_string += "+";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_minus:
            clause_string += "-";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_mul:
            clause_string += "*";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_bitand:
            clause_string += "&";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_bitor:
            clause_string += "|";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_bitxor:
            clause_string += "^";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_logand:
            clause_string += "&&";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_logor:
            clause_string += "||";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_min:
            clause_string += "min";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_max:
            clause_string += "max";
            break;
        case OMPC_IN_REDUCTION_IDENTIFIER_user:
            clause_string += this->getUserDefinedIdentifier();
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

std::string OpenMPDependClause::toString() {

    std::string result = "depend ";
    std::string clause_string = "(";
    OpenMPDependClauseModifier modifier = this->getModifier();
    OpenMPDependClauseType type = this->getType();
    switch (modifier) {
        case OMPC_DEPEND_MODIFIER_iterator:
            clause_string += "iterator";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ", ";
    };
    switch (type) {
        case OMPC_DEPEND_TYPE_in:
            clause_string += "in";
            break;
        case OMPC_DEPEND_TYPE_out:
            clause_string += "out";
            break;
        case OMPC_DEPEND_TYPE_inout:
            clause_string += "inout";
            break;
        case OMPC_DEPEND_TYPE_mutexinoutset:
            clause_string += "mutexinoutset";
            break;
        case OMPC_DEPEND_TYPE_depobj:
            clause_string += "depobj";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

void OpenMPDependClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "depend_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDependClauseModifier modifier = this->getModifier();
    OpenMPDependClauseType type = this->getType();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_DEPEND_MODIFIER_iterator:
            parameter_string = "iterator";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (type) {
        case OMPC_DEPEND_TYPE_in:
            parameter_string = "in";
            break;
        case OMPC_DEPEND_TYPE_out:
            parameter_string  = "out";
            break;
        case OMPC_DEPEND_TYPE_inout:
            parameter_string = "inout";
            break;
        case OMPC_DEPEND_TYPE_mutexinoutset:
            parameter_string = "mutexinoutset";
            break;
        case OMPC_DEPEND_TYPE_depobj:
            parameter_string = "depobj";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_type";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

   std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };
};
std::string OpenMPAffinityClause::toString() {

    std::string result = "affinity ";
    std::string clause_string = "(";
    OpenMPAffinityClauseModifier modifier = this->getModifier();
    switch (modifier) {
        case OMPC_AFFINITY_MODIFIER_iterator:
            clause_string += "iterator";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

void OpenMPAffinityClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "affinity_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPAffinityClauseModifier modifier = this->getModifier();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_AFFINITY_MODIFIER_iterator:
            parameter_string = "iterator";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
 std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};


std::string OpenMPToClause::toString() {

    std::string result = "to ";
    std::string clause_string = "(";
    OpenMPToClauseKind to_kind = this->getKind();
    switch (to_kind) {
        case OMPC_TO_mapper:
            clause_string += "mapper";
            break;
        default:
            ;
    }
   if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

void OpenMPToClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "to_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPToClauseKind kind = this->getKind();
    std::string parameter_string;
    switch (kind) {
        case OMPC_TO_mapper:
            parameter_string = "mapper";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_kind";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
 std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};


std::string OpenMPFromClause::toString() {

    std::string result = "from ";
    std::string clause_string = "(";
    OpenMPFromClauseKind from_kind = this->getKind();
    switch (from_kind) {
        case OMPC_FROM_mapper:
            clause_string += "mapper";
            break;
        default:
            ;
    }
   if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

void OpenMPFromClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "from_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPFromClauseKind kind = this->getKind();
    std::string parameter_string;
    switch (kind) {
        case OMPC_FROM_mapper:
            parameter_string = "mapper";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_kind";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
 std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};

std::string OpenMPDefaultmapClause::toString() {

    std::string result = "defaultmap ";
    std::string clause_string = "(";
    OpenMPDefaultmapClauseBehavior behavior = this->getBehavior();
    OpenMPDefaultmapClauseCategory category = this->getCategory();
    switch (behavior) {
        case OMPC_DEFAULTMAP_BEHAVIOR_alloc:
            clause_string += "alloc";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_to:
            clause_string += "to";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_from:
            clause_string += "from";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_tofrom:
            clause_string += "tofrom";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_firstprivate:
            clause_string += "firstprivate";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_none:
            clause_string += "none";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_default:
            clause_string += "default";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ": ";
    };
    switch (category) {
        case OMPC_DEFAULTMAP_CATEGORY_scalar:
            clause_string += "scalar";
            break;
        case OMPC_DEFAULTMAP_CATEGORY_aggregate:
            clause_string += "aggregate";
            break;
        case OMPC_DEFAULTMAP_CATEGORY_pointer:
            clause_string += "pointer";
            break;
        default:
            ;
    }
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};
void OpenMPDefaultmapClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "defaultmap_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDefaultmapClauseBehavior behavior = this->getBehavior();
    OpenMPDefaultmapClauseCategory category = this->getCategory();
    std::string parameter_string;
    switch (behavior) {
        case OMPC_DEFAULTMAP_BEHAVIOR_alloc:
            parameter_string = "alloc";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_to:
            parameter_string = "to";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_from:
            parameter_string = "from";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_tofrom:
            parameter_string = "tofrom";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_firstprivate:
            parameter_string = "firstprivate";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_none:
            parameter_string = "none";
            break;
        case OMPC_DEFAULTMAP_BEHAVIOR_default:
            parameter_string = "default";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_behavior";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (category) {
        case OMPC_DEFAULTMAP_CATEGORY_scalar:
            parameter_string = "scalar";
            break;
        case OMPC_DEFAULTMAP_CATEGORY_aggregate:
            parameter_string  = "aggregate";
            break;
        case OMPC_DEFAULTMAP_CATEGORY_pointer:
            parameter_string = "pointer";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_category";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

   std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };
};

std::string OpenMPDeviceClause::toString() {

    std::string result = "device ";
    std::string clause_string = "(";
    OpenMPDeviceClauseModifier modifier = this->getModifier();
    switch (modifier) {
        case OMPC_DEVICE_MODIFIER_ancestor:
            clause_string += "ancestor";
            break;
        case OMPC_DEVICE_MODIFIER_device_num:
            clause_string += "device_num";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

void OpenMPDeviceClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "device_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDeviceClauseModifier modifier = this->getModifier();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_DEVICE_MODIFIER_ancestor:
            parameter_string = "ancestor";
            break;
        case OMPC_DEVICE_MODIFIER_device_num:
            parameter_string = "device num";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
 std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};


std::string OpenMPDeviceTypeClause::toString() {

    std::string result = "device_type (";
    std::string parameter_string;
    OpenMPDeviceTypeClauseKind device_type_kind = this->getDeviceTypeClauseKind();
    switch (device_type_kind) {
        case OMPC_DEVICE_TYPE_host:
            parameter_string = "host";
            break;
        case OMPC_DEVICE_TYPE_nohost:
            parameter_string = "nohost";
            break;
        case OMPC_DEVICE_TYPE_any:
            parameter_string = "any";
            break;
        default:
            std::cout << "The parameter of device_type clause is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        result += parameter_string + ") ";
    }
    else {
        result = result.substr(0, result.size()-2);
    }

    return result;
}

void OpenMPDeviceTypeClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "device_type_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDeviceTypeClauseKind device_type_kind = this->getDeviceTypeClauseKind();
    std::string parameter_string;
    switch (device_type_kind) {
        case OMPC_DEVICE_TYPE_host:
            parameter_string = "host";
            break;
        case OMPC_DEVICE_TYPE_nohost:
            parameter_string = "nohost";
            break;
        case OMPC_DEVICE_TYPE_any:
            parameter_string = "any";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_kind";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
};

void OpenMPTaskReductionClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "task_reduction_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPTaskReductionClauseIdentifier identifier = this->getIdentifier();
    std::string parameter_string;
    switch (identifier) {
        case OMPC_TASK_REDUCTION_IDENTIFIER_plus:
            parameter_string = "+";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_minus:
            parameter_string  = "-";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_mul:
            parameter_string = "*";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_bitand:
            parameter_string = "&";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_bitor:
            parameter_string = "|";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_bitxor:
            parameter_string = "^";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_logand:
            parameter_string = "&&";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_logor:
            parameter_string = "||";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_min:
            parameter_string = "min";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_max:
            parameter_string = "max";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_user:
            parameter_string = this->getUserDefinedIdentifier();
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_identifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};

std::string OpenMPTaskReductionClause::toString() {

    std::string result = "task_reduction ";
    std::string clause_string = "(";
    OpenMPTaskReductionClauseIdentifier identifier = this->getIdentifier();
    switch (identifier) {
        case OMPC_TASK_REDUCTION_IDENTIFIER_plus:
            clause_string += "+";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_minus:
            clause_string += "-";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_mul:
            clause_string += "*";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_bitand:
            clause_string += "&";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_bitor:
            clause_string += "|";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_bitxor:
            clause_string += "^";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_logand:
            clause_string += "&&";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_logor:
            clause_string += "||";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_min:
            clause_string += "min";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_max:
            clause_string += "max";
            break;
        case OMPC_TASK_REDUCTION_IDENTIFIER_user:
            clause_string += this->getUserDefinedIdentifier();
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

std::string OpenMPMapClause::toString() {

    std::string result = "map ";
    std::string clause_string = "(";
    OpenMPMapClauseModifier modifier1 = this->getModifier1();
    OpenMPMapClauseModifier modifier2 = this->getModifier2();
    OpenMPMapClauseModifier modifier3 = this->getModifier3();
    OpenMPMapClauseType type = this->getType();
    switch (modifier1) {
        case OMPC_MAP_MODIFIER_always:
            clause_string += "always";
            break;
        case OMPC_MAP_MODIFIER_close:
            clause_string += "close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            clause_string += "mapper";
            break;
        default:
            ;
    }
    switch (modifier2) {
        case OMPC_MAP_MODIFIER_always:
            clause_string += ",always";
            break;
        case OMPC_MAP_MODIFIER_close:
            clause_string += ",close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            clause_string += ",mapper";
            break;
        default:
            ;
    }
    switch (modifier3) {
        case OMPC_MAP_MODIFIER_always:
            clause_string += ",always";
            break;
        case OMPC_MAP_MODIFIER_close:
            clause_string += ",close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            clause_string += ",mapper";
            break;
        default:
            ;
    }
    
    clause_string += ",";  
    switch (type) {
        case OMPC_MAP_TYPE_to:
            clause_string += "to";
            break;
        case OMPC_MAP_TYPE_from:
            clause_string += "from";
            break;
        case OMPC_MAP_TYPE_tofrom:
            clause_string += "tofrom";
            break;
        case OMPC_MAP_TYPE_alloc:
            clause_string += "alloc";
            break;
        case OMPC_MAP_TYPE_release:
            clause_string += "release";
            break;
        case OMPC_MAP_TYPE_delete:
            clause_string += "delete";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

void OpenMPMapClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "map_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPMapClauseModifier modifier1 = this->getModifier1();
    OpenMPMapClauseModifier modifier2 = this->getModifier2();
    OpenMPMapClauseModifier modifier3 = this->getModifier3();
    OpenMPMapClauseType Type = this->getType();
    std::string parameter_string;
    switch (modifier1) {
        case OMPC_MAP_MODIFIER_always:
            parameter_string += "always";
            break;
        case OMPC_MAP_MODIFIER_close:
            parameter_string += "close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            parameter_string += "mapper";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier1";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (modifier2) {
        case OMPC_MAP_MODIFIER_always:
            parameter_string += "always";
            break;
        case OMPC_MAP_MODIFIER_close:
            parameter_string += "close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            parameter_string += "mapper";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier2";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (modifier3) {
        case OMPC_MAP_MODIFIER_always:
            parameter_string += "always";
            break;
        case OMPC_MAP_MODIFIER_close:
            parameter_string += "close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            parameter_string += "mapper";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier3";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (type) {
        case OMPC_MAP_TYPE_to:
            parameter_string += "to";
            break;
        case OMPC_MAP_TYPE_from:
            parameter_string += "from";
            break;
        case OMPC_MAP_TYPE_tofrom:
            parameter_string += "tofrom";
            break;
        case OMPC_MAP_TYPE_alloc:
            parameter_string += "alloc";
            break;
        case OMPC_MAP_TYPE_release:
            parameter_string += "release";
            break;
        case OMPC_MAP_TYPE_delete:
            parameter_string += "delete";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_type";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
/**/

std::string OpenMPReductionClause::toString() {

    std::string result = "reduction ";
    std::string clause_string = "(";
    OpenMPReductionClauseModifier modifier = this->getModifier();
    OpenMPReductionClauseIdentifier identifier = this->getIdentifier();
    switch (modifier) {
        case OMPC_REDUCTION_MODIFIER_default:
            clause_string += "default";
            break;
        case OMPC_REDUCTION_MODIFIER_inscan:
            clause_string += "inscan";
            break;
        case OMPC_REDUCTION_MODIFIER_task:
            clause_string += "task";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ", ";
    };
    switch (identifier) {
        case OMPC_REDUCTION_IDENTIFIER_plus:
            clause_string += "+";
            break;
        case OMPC_REDUCTION_IDENTIFIER_minus:
            clause_string += "-";
            break;
        case OMPC_REDUCTION_IDENTIFIER_mul:
            clause_string += "*";
            break;
        case OMPC_REDUCTION_IDENTIFIER_bitand:
            clause_string += "&";
            break;
        case OMPC_REDUCTION_IDENTIFIER_bitor:
            clause_string += "|";
            break;
        case OMPC_REDUCTION_IDENTIFIER_bitxor:
            clause_string += "^";
            break;
        case OMPC_REDUCTION_IDENTIFIER_logand:
            clause_string += "&&";
            break;
        case OMPC_REDUCTION_IDENTIFIER_logor:
            clause_string += "||";
            break;
        case OMPC_REDUCTION_IDENTIFIER_min:
            clause_string += "min";
            break;
        case OMPC_REDUCTION_IDENTIFIER_max:
            clause_string += "max";
            break;
        case OMPC_REDUCTION_IDENTIFIER_user:
            clause_string += this->getUserDefinedIdentifier();
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += " : ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

std::string OpenMPLastprivateClause::toString() {

    std::string result = "lastprivate ";
    std::string clause_string = "(";
    OpenMPLastprivateClauseModifier modifier = this->getModifier();
    switch (modifier) {
        case OMPC_LASTPRIVATE_MODIFIER_conditional:
            clause_string += "conditional";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ": ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 2) {
        result += clause_string;
    };

    return result;
};


std::string OpenMPLinearClause::toString() {

    std::string result = "linear ";
    std::string clause_string = "(";
    bool flag = false;
    OpenMPLinearClauseModifier modifier = this->getModifier();
    switch (modifier) {
        case OMPC_LINEAR_MODIFIER_val:
            clause_string += "val";
            flag = true;
            break;
        case OMPC_LINEAR_MODIFIER_ref:
            clause_string += "ref";
            flag = true;
            break;
        case OMPC_LINEAR_MODIFIER_uval:
            clause_string += "uval";
            flag = true;
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += "( ";
    };
   clause_string += this->expressionToString();
    if (flag == true) {
        clause_string += ") ";
    };
    if(this->getUserDefinedStep() != ""){
        clause_string += ":";
        clause_string += this->getUserDefinedStep();
    }
    if (clause_string.size() > 2) {
        clause_string += ") ";
    };
    if (clause_string.size() > 3) {
        result += clause_string;
    };


    return result;
};
std::string OpenMPDistscheduleClause::toString() {

    std::string result = "dist_schedule ";
    std::string clause_string = "(";
    OpenMPDistscheduleClauseKind kind = this->getKind();
    switch (kind) {
        case OMPC_DISTSCHEDULE_KIND_static:
            clause_string += "static";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ": ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 2) {
        result += clause_string;
    };

    return result;
};
std::string OpenMPScheduleClause::toString() {

    std::string result = "schedule ";
    std::string clause_string = "(";
    OpenMPScheduleClauseModifier modifier1 = this->getModifier1();
    OpenMPScheduleClauseModifier modifier2 = this->getModifier2();
    OpenMPScheduleClauseKind kind = this->getKind();
    switch (modifier1) {
        case OMPC_SCHEDULE_MODIFIER_monotonic:
            clause_string += "monotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_nonmonotonic:
            clause_string += "nonmonotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_simd:
            clause_string += "simd";
            break;
        default:
            ;
    }
    switch (modifier2) {
        case OMPC_SCHEDULE_MODIFIER_monotonic:
            clause_string += ",monotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_nonmonotonic:
            clause_string += ",nonmonotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_simd:
            clause_string += ",simd";
            break;
        default:
            ;
    }
    clause_string += ":";  
    switch (kind) {
        case OMPC_SCHEDULE_KIND_static:
            clause_string += "static";
            break;
        case OMPC_SCHEDULE_KIND_dynamic:
            clause_string += "dynamic";
            break;
        case OMPC_SCHEDULE_KIND_guided:
            clause_string += "guided";
            break;
        case OMPC_SCHEDULE_KIND_auto:
            clause_string += "auto";
            break;
        case OMPC_SCHEDULE_KIND_runtime:
            clause_string += "runtime";
            break;
        default:
            ;
    }
    if (this->expressionToString() != ""){
    clause_string += ", ";
    clause_string += this->expressionToString();}
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};
std::string OpenMPIfClause::toString() {

    std::string result = "if ";
    std::string clause_string = "(";
    OpenMPIfClauseModifier modifier = this->getModifier();
    switch (modifier) {
        case OMPC_IF_MODIFIER_parallel:
            clause_string += "parallel";
            break;
        case OMPC_IF_MODIFIER_simd:
            clause_string += "simd";
            break;
        case OMPC_IF_MODIFIER_task:
            clause_string += "task";
            break;
        case OMPC_IF_MODIFIER_taskloop:
            clause_string += "taskloop";
            break;
        case OMPC_IF_MODIFIER_cancel:
            clause_string += "cancel";
            break;
        case OMPC_IF_MODIFIER_target_data:
            clause_string += "target data";
            break;
        case OMPC_IF_MODIFIER_target_enter_data:
            clause_string += "target enter data";
            break;
        case OMPC_IF_MODIFIER_target_exit_data:
            clause_string += "target exit data";
            break;
        case OMPC_IF_MODIFIER_target:
            clause_string += "target";
            break;
        case OMPC_IF_MODIFIER_target_update:
            clause_string += "target update";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ": ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 2) {
        result += clause_string;
    };

    return result;
};
std::string OpenMPAllocateClause::toString() {

    std::string result = "allocate ";
    std::string clause_string = "(";
    OpenMPAllocateClauseAllocator allocator = this->getAllocator();
    switch (allocator) {
        case OMPC_ALLOCATE_ALLOCATOR_default:
            clause_string += "omp_default_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_large_cap:
            clause_string += "omp_large_cap_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_cons_mem:
            clause_string += "omp_const_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_high_bw:
            clause_string += "omp_high_bw_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_low_lat:
            clause_string += "omp_low_lat_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_cgroup:
            clause_string += "omp_cgroup_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_pteam:
            clause_string += "omp_pteam_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_thread:
            clause_string += "omp_thread_mem_alloc";
            break;
        default:
            ;
    }
    if (clause_string.size() > 1) {
        clause_string += ": ";
    };
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 2) {
        result += clause_string;
    };

    return result;
};
std::string OpenMPAllocatorClause::toString() {

    std::string result = "allocator ";
    std::string clause_string = "(";
    OpenMPAllocatorClauseAllocator allocator = this->getAllocator();
    switch (allocator) {
        case OMPC_ALLOCATOR_ALLOCATOR_default:
            clause_string += "omp_default_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_large_cap:
            clause_string += "omp_large_cap_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_cons_mem:
            clause_string += "omp_const_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_high_bw:
            clause_string += "omp_high_bw_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_low_lat:
            clause_string += "omp_low_lat_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_cgroup:
            clause_string += "omp_cgroup_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_pteam:
            clause_string += "omp_pteam_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_thread:
            clause_string += "omp_thread_mem_alloc";
            break;
        default:
            ;
    }
    clause_string += ") ";
    if (clause_string.size() > 2) {
        result += clause_string;
    };

    return result;
};
//2 or more combined directive
void OpenMPDirective::generateDOT() {
    std::string directive_kind;
    OpenMPDirectiveKind kind = this->getKind();
    switch(kind) {
        case OMPD_cancellation_point :
                directive_kind = "cancellation_point " ;
                break;
        case OMPD_for_simd:
                directive_kind = "for_simd ";
                break;
        case OMPD_distribute_simd:
                directive_kind = "distribute_simd ";
                break;
        case OMPD_distribute_parallel_for:
                directive_kind = "distribute_parallel_for ";
                break;
        case OMPD_distribute_parallel_for_simd:
                directive_kind = "distribute_parallel_for_simd ";
                break;
        case OMPD_teams:
                directive_kind = "teams ";
                break;
        case OMPD_requires:
                directive_kind = "requires ";
                break;
        case OMPD_task:
                directive_kind = "task ";
                break;
        case OMPD_taskloop:
                directive_kind = "taskloop ";
                break;
        case OMPD_taskloop_simd:
                directive_kind = "taskloop_simd ";
                break;
        case OMPD_taskyield:
                directive_kind = "taskyield ";
                break;
        case OMPD_target_data:
                directive_kind = "target_data ";
                break;
        case OMPD_target_enter_data:
                directive_kind = "target_enter_data ";
                break;
        case OMPD_target_exit_data:
                directive_kind = "target_exit_data ";
                break;
        case OMPD_target:
                directive_kind = "target ";
                break;
        case OMPD_target_update:
                directive_kind = "target_update ";
                break;
        case OMPD_declare_target:
                directive_kind = "declare_target ";
                break;
        case OMPD_end_declare_target:
                directive_kind = "end_declare_target ";
                break;
        case OMPD_master:
                directive_kind = "master ";
                break;
        case OMPD_barrier:
                directive_kind = "barrier ";
                break;
        case OMPD_taskwait:
                directive_kind = "taskwait ";
                break;
        case OMPD_taskgroup:
                directive_kind = "taskgroup ";
                break;
        default:
                directive_kind = this->toString();
    }
    std::string current_line;

    current_line = "graph OpenMPIR_" + directive_kind + " {\n";

    std::string filename = "OpenMPIR_" + directive_kind.substr(0, directive_kind.size() - 1) + ".dot";
    std::ofstream output(filename.c_str());

    output << current_line.c_str();
    output << "\t" << directive_kind.c_str() << "\n";
    switch (kind) {
        case OMPD_allocate: {
            std::string indent = std::string(1, '\t');
            std::vector<const char *> *list = ((OpenMPAllocateDirective *) this)->getAllocateList();
            std::vector<const char *>::iterator list_item;
            int list_index = 0;
            std::string list_name;
            std::string expr_name;
            std::string tkind = "allocate";
            list_name = tkind + "_directive_list_" + std::to_string(list_index);
            current_line = indent + tkind + " -- " + list_name + "\n";
            output << current_line.c_str();
            for (list_item = list->begin(); list_item != list->end(); list_item++) {
                expr_name = list_name + "_expr" + std::to_string(list_index);
                list_index += 1;
                current_line = indent + indent + list_name + " -- " + expr_name + "\n";
                output << current_line.c_str();
                current_line =
                    indent + indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*list_item) +
                    "\"]\n";
                output << current_line.c_str();
            }
        }
        default: {
            ;
        }
    };

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses = this->getAllClauses();
    if (clauses != NULL) {
        std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            int clause_index = 0;
            std::vector<OpenMPClause*>* current_clauses = it->second;
            std::vector<OpenMPClause*>::iterator clause_iter;
            for (clause_iter = current_clauses->begin(); clause_iter != current_clauses->end(); clause_iter++) {
                (*clause_iter)->generateDOT(output, 1, clause_index, directive_kind);
                clause_index += 1;
            }
        }
    };

    output << "}\n";

};

// Unfinished yet.
void OpenMPDirective::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string directive_kind = this->toString();

    std::string current_line;

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses = this->getAllClauses();
    if (clauses != NULL) {
        std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >::iterator it;
        for (it = clauses->begin(); it != clauses->end(); it++) {
            int clause_index = 0;
            std::vector<OpenMPClause*>* current_clauses = it->second;
            std::vector<OpenMPClause*>::iterator clauseIter;
            for (clauseIter = current_clauses->begin(); clauseIter != current_clauses->end(); clauseIter++) {
                (*clauseIter)->generateDOT(dot_file, depth+1, clause_index, parent_node);
                clause_index += 1;
            };
        };
    };

    if (current_line == "") {
        dot_file << directive_kind.c_str() << "\n";
    };
};

void OpenMPClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string clause_kind;
    std::string indent = std::string(depth, '\t');

    switch (this->getKind()) {
        case OMPC_private:
            clause_kind += "private";
            break;
        case OMPC_firstprivate:
            clause_kind += "firstprivate";
            break;
        case OMPC_shared:
            clause_kind += "shared";
            break;
        case OMPC_num_threads:
            clause_kind += "num_threads";
            break;
        case OMPC_num_teams:
            clause_kind += "num_teams";
            break;
        case OMPC_thread_limit:
            clause_kind += "thread_limit";
            break;
        case OMPC_default:
            clause_kind += "default";
            break;
        case OMPC_collapse:
            clause_kind += "collapse";
            break;
        case OMPC_ordered:
            clause_kind += "ordered";
            break;
        case OMPC_nowait:
            clause_kind += "nowait";
            break;
        case OMPC_allocate:
            clause_kind += "allocate";
            break;
        case OMPC_order:
            clause_kind += "order";
            break;
        case OMPC_safelen:
            clause_kind += "safelen";
            break;
        case OMPC_simdlen:
            clause_kind += "simdlen";
            break;
        case OMPC_aligned:
            clause_kind += "aligned";
            break;
        case OMPC_nontemporal:
            clause_kind += "nontemporal";
            break;
        case OMPC_uniform:
            clause_kind += "uniform";
            break;
        case OMPC_inbranch:
            clause_kind += "inbranch";
            break;
        case OMPC_notinbranch:
            clause_kind += "notinbranch";
            break;
        case OMPC_proc_bind:
            clause_kind += "proc_bind";
            break;
        case OMPC_bind:
            clause_kind += "bind";
            break; 
        case OMPC_inclusive:
            clause_kind += "inclusive";
            break;        
        case OMPC_exclusive:
            clause_kind += "exclusive";
            break;
        case OMPC_copyprivate:
            clause_kind += "copyprivate";
            break;
        case OMPC_parallel:
            clause_kind += "parallel";
            break;
        case OMPC_sections:
            clause_kind += "sections";
            break;
        case OMPC_for:
            clause_kind += "for";
            break;
        case OMPC_taskgroup:
            clause_kind += "taskgroup";
            break;
        case OMPC_if:
            clause_kind += "if";
            break;
        case OMPC_reverse_offload:
            clause_kind += "reverse_offload";
            break;
        case OMPC_unified_address:
            clause_kind += "unified_address";
            break;
        case OMPC_unified_shared_memory:
            clause_kind += "unified_shared_memory";
            break;
        case OMPC_dynamic_allocators:
            clause_kind += "dynamic_allocators";
            break;
        case OMPC_final:
            clause_kind += "final";
            break;
        case OMPC_untied:
            clause_kind += "untied";
            break;
        case OMPC_mergeable:
            clause_kind += "mergeable";
            break;
        case OMPC_priority:
            clause_kind += "priority";
            break;
        case OMPC_detach:
            clause_kind += "detach";
            break;
        case OMPC_grainsize:
            clause_kind += "grainsize";
            break;
        case OMPC_num_tasks:
            clause_kind += "num_tasks";
            break;
        case OMPC_nogroup:
            clause_kind += "nogroup";
            break;
        case OMPC_map:
            clause_kind += "map";
            break;
        case OMPC_use_device_ptr:
            clause_kind += "use_device_ptr";
            break;
        case OMPC_use_device_addr:
            clause_kind += "use_device_addr";
            break;
        case OMPC_is_device_ptr:
            clause_kind += "is_device_ptr";
            break;
        case OMPC_link:
            clause_kind += "link";
            break;
        case OMPC_device_type:
            clause_kind += "device_type";
            break;
        default:
            printf("The clause enum is not supported yet.\n");
    }
    clause_kind += "_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};

void OpenMPReductionClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "reduction_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPReductionClauseModifier modifier = this->getModifier();
    OpenMPReductionClauseIdentifier identifier = this->getIdentifier();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_REDUCTION_MODIFIER_default:
            parameter_string = "default";
            break;
        case OMPC_REDUCTION_MODIFIER_inscan:
            parameter_string = "inscan";
            break;
        case OMPC_REDUCTION_MODIFIER_task:
            parameter_string = "task";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (identifier) {
        case OMPC_REDUCTION_IDENTIFIER_plus:
            parameter_string = "+";
            break;
        case OMPC_REDUCTION_IDENTIFIER_minus:
            parameter_string  = "-";
            break;
        case OMPC_REDUCTION_IDENTIFIER_mul:
            parameter_string = "*";
            break;
        case OMPC_REDUCTION_IDENTIFIER_bitand:
            parameter_string = "&";
            break;
        case OMPC_REDUCTION_IDENTIFIER_bitor:
            parameter_string = "|";
            break;
        case OMPC_REDUCTION_IDENTIFIER_bitxor:
            parameter_string = "^";
            break;
        case OMPC_REDUCTION_IDENTIFIER_logand:
            parameter_string = "&&";
            break;
        case OMPC_REDUCTION_IDENTIFIER_logor:
            parameter_string = "||";
            break;
        case OMPC_REDUCTION_IDENTIFIER_min:
            parameter_string = "min";
            break;
        case OMPC_REDUCTION_IDENTIFIER_max:
            parameter_string = "max";
            break;
        case OMPC_REDUCTION_IDENTIFIER_user:
            parameter_string = this->getUserDefinedIdentifier();
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_identifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};

OpenMPClause* OpenMPWhenClause::addWhenClause(OpenMPDirective *directive) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_when);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        current_clauses = new std::vector<OpenMPClause *>();
        (*all_clauses)[OMPC_when] = current_clauses;
    };
    new_clause = new OpenMPWhenClause();
    current_clauses->push_back(new_clause);

    return new_clause;
}

std::string OpenMPVariantClause::toString() {

    std::string result;
    std::string clause_string;
    std::string parameter_string;
    std::vector<OpenMPDirective*>* parameter_directives;
    OpenMPDirective* variant_directive = NULL;
    OpenMPClauseKind clause_kind = this->getKind();
    switch (clause_kind) {
        case OMPC_when:
            result = "when";
            break;
        case OMPC_match:
            result = "match";
            break;
        default:
            std::cout << "The variant clause is not supported.\n";
    };
    result += " (";

    // check user
    parameter_string = this->getUserCondition();
    if (parameter_string != "") {
        clause_string += "user = {condition(" + parameter_string + ")}" + ", ";
    };

    // check construct
    parameter_directives = this->getConstructDirective();
    if (parameter_directives->size() != 0) {
        clause_string += "construct = {";
        std::vector<OpenMPDirective*>::iterator iter;
        for (iter = parameter_directives->begin(); iter != parameter_directives->end(); iter++) {
            clause_string += (*iter)->generatePragmaString("", "(", ")", true) + ", ";
        };
        clause_string = clause_string.substr(0, clause_string.size()-2);
        clause_string += "}, ";
    }

    result += clause_string;
    parameter_string.clear();


    // check device
    clause_string.clear();
    // check device_kind
    OpenMPClauseContextKind context_kind = this->getContextKind();
    switch (context_kind) {
        case OMPC_CONTEXT_KIND_host:
            parameter_string = "host";
            break;
        case OMPC_CONTEXT_KIND_nohost:
            parameter_string = "nohost";
            break;
        case OMPC_CONTEXT_KIND_any:
            parameter_string = "any";
            break;
        case OMPC_CONTEXT_KIND_cpu:
            parameter_string = "cpu";
            break;
        case OMPC_CONTEXT_KIND_gpu:
            parameter_string = "gpu";
            break;
        case OMPC_CONTEXT_KIND_fpga:
            parameter_string = "fpga";
            break;
        case OMPC_CONTEXT_KIND_unknown:
            break;
        default:
            std::cout << "The context kind is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        clause_string += "kind(" + parameter_string + "), ";
    };

    // check device isa
    parameter_string = this->getIsaExpression();
    if (parameter_string.size() > 0) {
        clause_string += "isa(" + parameter_string + "), ";
    };

    // check device arch
    parameter_string = this->getArchExpression();
    if (parameter_string.size() > 0) {
        clause_string += "arch(" + parameter_string + "), ";
    };

    if (clause_string.size() > 0) {
        result += "device = {" + clause_string.substr(0, clause_string.size()-2) + "}, ";
    };

    // check implementation
    clause_string.clear();
    parameter_string.clear();
    // check implementation vendor
    OpenMPClauseContextVendor context_vendor = this->getImplementationKind();
    switch (context_vendor) {
        case OMPC_CONTEXT_VENDOR_amd:
            parameter_string = "amd";
            break;
        case OMPC_CONTEXT_VENDOR_arm:
            parameter_string = "arm";
            break;
        case OMPC_CONTEXT_VENDOR_bsc:
            parameter_string = "bsc";
            break;
        case OMPC_CONTEXT_VENDOR_cray:
            parameter_string = "cray";
            break;
        case OMPC_CONTEXT_VENDOR_fujitsu:
            parameter_string = "fujitsu";
            break;
        case OMPC_CONTEXT_VENDOR_gnu:
            parameter_string = "gnu";
            break;
        case OMPC_CONTEXT_VENDOR_ibm:
            parameter_string = "ibm";
            break;
        case OMPC_CONTEXT_VENDOR_intel:
            parameter_string = "intel";
            break;
        case OMPC_CONTEXT_VENDOR_llvm:
            parameter_string = "llvm";
            break;
        case OMPC_CONTEXT_VENDOR_pgi:
            parameter_string = "pgi";
            break;
        case OMPC_CONTEXT_VENDOR_ti:
            parameter_string = "ti";
            break;
        case OMPC_CONTEXT_VENDOR_unknown:
            parameter_string = "unknown";
            break;
        case OMPC_CONTEXT_VENDOR_unspecified:
            break;
        default:
            std::cout << "The context vendor is not supported.\n";
    };
    if (parameter_string.size() > 0) {
        clause_string += "vendor(" + parameter_string + "), ";
    };

    // check implementation extension
    parameter_string = this->getExtensionExpression();
    if (parameter_string.size() > 0) {
        clause_string += "extension(" + parameter_string + "), ";
    };

    if (clause_string.size() > 0) {
        result += "implementation = {" + clause_string.substr(0, clause_string.size()-2) + "}, ";
    };

    clause_string.clear();
    result = result.substr(0, result.size()-2);

    if (clause_kind == OMPC_when) {
        clause_string = " : ";
        variant_directive = ((OpenMPWhenClause*)this)->getVariantDirective();
        if (variant_directive != NULL) {
            clause_string += variant_directive->generatePragmaString("");
        };
    };

    result += clause_string + ") ";

    return result;
};

std::string OpenMPDefaultClause::toString() {

    std::string result = "default (";
    std::string parameter_string;
    OpenMPDefaultClauseKind default_kind = this->getDefaultClauseKind();
    switch (default_kind) {
        case OMPC_DEFAULT_shared:
            parameter_string = "shared";
            break;
        case OMPC_DEFAULT_private:
            parameter_string = "private";
            break;
        case OMPC_DEFAULT_firstprivate:
            parameter_string = "firstprivate";
            break;
        case OMPC_DEFAULT_none:
            parameter_string = "none";
            break;
        case OMPC_DEFAULT_variant:
            parameter_string = this->getVariantDirective()->generatePragmaString("", "", "");
            break;
        default:
            std::cout << "The parameter of default clause is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        result += parameter_string + ") ";
    }
    else {
        result = result.substr(0, result.size()-2);
    }

    return result;
}

std::string OpenMPProcBindClause::toString() {

    std::string result = "proc_bind (";
    std::string parameter_string;
    OpenMPProcBindClauseKind proc_bind_kind = this->getProcBindClauseKind();
    switch (proc_bind_kind) {
        case OMPC_PROC_BIND_close:
            parameter_string = "close";
            break;
        case OMPC_PROC_BIND_master:
            parameter_string = "master";
            break;
        case OMPC_PROC_BIND_spread:
            parameter_string = "spread";
            break;
        default:
            std::cout << "The parameter of proc_bind clause is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        result += parameter_string + ") ";
    }
    else {
        result = result.substr(0, result.size()-2);
    }

    return result;
}


void OpenMPLastprivateClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "lastprivate_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPLastprivateClauseModifier modifier = this->getModifier();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_LASTPRIVATE_MODIFIER_conditional:
            parameter_string = "conditional";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };


    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
void OpenMPLinearClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "linear_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPLinearClauseModifier modifier = this->getModifier();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_LINEAR_MODIFIER_val:
            parameter_string = "val";
            break;
        case OMPC_LINEAR_MODIFIER_ref:
            parameter_string = "ref";
            break;
        case OMPC_LINEAR_MODIFIER_uval:
            parameter_string = "uval";
            break;
        default:

            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };


    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };
    std:string step = this->getUserDefinedStep();
    if (step != "") {
        parameter_string = step;
        std::string step_name = clause_kind + "_liner_step";
        current_line = indent + clause_kind + " -- " + step_name + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + step_name + " [label = \"" + step_name + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };    

};

void OpenMPScheduleClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "schedule_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPScheduleClauseModifier modifier1 = this->getModifier1();
    OpenMPScheduleClauseModifier modifier2 = this->getModifier2();
    OpenMPScheduleClauseKind kind = this->getKind();
    std::string parameter_string;
    switch (modifier1) {
        case OMPC_SCHEDULE_MODIFIER_monotonic:
            parameter_string += "monotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_nonmonotonic:
            parameter_string += "nonmonotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_simd:
            parameter_string += "simd";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier1";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (modifier2) {
        case OMPC_SCHEDULE_MODIFIER_monotonic:
            parameter_string += "monotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_nonmonotonic:
            parameter_string += "nonmonotonic";
            break;
        case OMPC_SCHEDULE_MODIFIER_simd:
            parameter_string += "simd";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier2";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };
    parameter_string.clear();
    switch (kind) {
        case OMPC_SCHEDULE_KIND_static:
            parameter_string += "static";
            break;
        case OMPC_SCHEDULE_KIND_dynamic:
            parameter_string += "dynamic";
            break;
        case OMPC_SCHEDULE_KIND_guided:
            parameter_string += "guided";
            break;
        case OMPC_SCHEDULE_KIND_auto:
            parameter_string += "auto";
            break;
        case OMPC_SCHEDULE_KIND_runtime:
            parameter_string += "runtime";
            break;
        default:
            ;
    };

    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_kind";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
void OpenMPDistscheduleClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "dist_schedule_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDistscheduleClauseKind kind = this->getKind();
    std::string parameter_string;
    switch (kind) {
        case OMPC_DISTSCHEDULE_KIND_static:
            parameter_string = "static";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_kind";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };


    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
void OpenMPIfClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "if_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPIfClauseModifier modifier = this->getModifier();
    std::string parameter_string;
    switch (modifier) {
        case OMPC_IF_MODIFIER_parallel:
            parameter_string += "parallel";
            break;
        case OMPC_IF_MODIFIER_simd:
            parameter_string += "simd";
            break;
        case OMPC_IF_MODIFIER_task:
            parameter_string += "task";
            break;
        case OMPC_IF_MODIFIER_taskloop:
            parameter_string += "taskloop";
            break;
        case OMPC_IF_MODIFIER_cancel:
            parameter_string += "cancel";
            break;
        case OMPC_IF_MODIFIER_target_data:
            parameter_string += "target data";
            break;
        case OMPC_IF_MODIFIER_target_enter_data:
            parameter_string += "target enter data";
            break;
        case OMPC_IF_MODIFIER_target_exit_data:
            parameter_string += "target exit data";
            break;
        case OMPC_IF_MODIFIER_target:
            parameter_string += "target";
            break;
        case OMPC_IF_MODIFIER_target_update:
            parameter_string += "target update";
            break;
        default:
            ;
    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_modifier";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };


    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
void OpenMPAllocateClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "allocate_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPAllocateClauseAllocator allocator = this->getAllocator();
    std::string parameter_string;
    switch (allocator) {
        case OMPC_ALLOCATE_ALLOCATOR_default:
            parameter_string += "omp_default_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_large_cap:
            parameter_string += "omp_large_cap_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_cons_mem:
            parameter_string += "omp_const_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_high_bw:
            parameter_string += "omp_high_bw_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_low_lat:
            parameter_string += "omp_low_lat_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_cgroup:
            parameter_string += "omp_cgroup_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_pteam:
            parameter_string += "omp_pteam_mem_alloc";
            break;
        case OMPC_ALLOCATE_ALLOCATOR_thread:
            parameter_string += "omp_thread_mem_alloc";
            break;
        default:
            ;

    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_allocator";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };


    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
void OpenMPAllocatorClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "allocator_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPAllocatorClauseAllocator allocator = this->getAllocator();
    std::string parameter_string;
    switch (allocator) {
        case OMPC_ALLOCATOR_ALLOCATOR_default:
            parameter_string += "omp_default_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_large_cap:
            parameter_string += "omp_large_cap_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_cons_mem:
            parameter_string += "omp_const_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_high_bw:
            parameter_string += "omp_high_bw_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_low_lat:
            parameter_string += "omp_low_lat_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_cgroup:
            parameter_string += "omp_cgroup_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_pteam:
            parameter_string += "omp_pteam_mem_alloc";
            break;
        case OMPC_ALLOCATOR_ALLOCATOR_thread:
            parameter_string += "omp_thread_mem_alloc";
            break;
        default:
            ;

    }
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_allocator";
        current_line = indent + clause_kind + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };


    std::vector<const char*>* expr = this->getExpressions();
    if (expr != NULL) {
        std::vector<const char*>::iterator it;
        int expr_index = 0;
        std::string expr_name;
        for (it = expr->begin(); it != expr->end(); it++) {
            expr_name = clause_kind + "_expr" + std::to_string(expr_index);
            expr_index += 1;
            current_line = indent + clause_kind + " -- " + expr_name + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*it) + "\"]\n";
            dot_file << current_line.c_str();
        };
    };

};
