/*
 * Copyright (c) 2018-2020, High Performance Computing Architecture and System
 * research laboratory at University of North Carolina at Charlotte (HPCAS@UNCC)
 * and Lawrence Livermore National Security, LLC.
 *
 * SPDX-License-Identifier: (BSD-3-Clause)
 */

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
OpenMPClause * OpenMPDirective::addOpenMPClause(int k, ... ) {

    OpenMPClauseKind kind = (OpenMPClauseKind)k;
    // Check whether the given kind of clause exists first.
    // If not, create an empty vector.
    if (clauses.count(kind) == 0) {
        clauses.insert(std::pair<OpenMPClauseKind, std::vector<OpenMPClause*>*>(kind, new std::vector<OpenMPClause*>));
    };
    std::vector<OpenMPClause*>* current_clauses = getClauses(kind);
    va_list args;
    va_start(args, k);
    OpenMPClause * new_clause = NULL;

    switch (kind) {
        case OMPC_num_threads:
        case OMPC_private:
        case OMPC_firstprivate:
        case OMPC_shared:
        case OMPC_num_teams:
        case OMPC_thread_limit:
        case OMPC_copyin:
        case OMPC_collapse:
        case OMPC_ordered:
        case OMPC_nowait:
        case OMPC_safelen:
        case OMPC_simdlen:
        case OMPC_nontemporal:
        case OMPC_uniform:
        case OMPC_inbranch:
        case OMPC_notinbranch :
        case OMPC_copyprivate : 
        case OMPC_parallel:    
        case OMPC_sections: 
        case OMPC_for:
        case OMPC_do:
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
        case OMPC_threads:
        case OMPC_simd:
        case OMPC_acq_rel : 
        case OMPC_seq_cst : 
        case OMPC_release : 
        case OMPC_acquire : 
        case OMPC_relaxed : 
        case OMPC_read : 
        case OMPC_write : 
        case OMPC_update : 
        case OMPC_capture : 
        case OMPC_hint :
        case OMPC_destroy:
        
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
               if (kind == OMPC_seq_cst) {
                    std::cerr << "Cannot have two seq_cst clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
               if (kind == OMPC_acq_rel) {
                    std::cerr << "Cannot have two acq_rel clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
               if (kind == OMPC_release) {
                    std::cerr << "Cannot have two release clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
               if (kind == OMPC_acquire) {
                    std::cerr << "Cannot have two acquire clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
               if (kind == OMPC_relaxed) {
                    std::cerr << "Cannot have two relaxed clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
               if (kind == OMPC_hint) {
                    std::cerr << "Cannot have two hint clause for the directive " << kind << ", ignored\n";
                } else {
                    /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
                    new_clause = current_clauses->at(0);
                }
            }
            break;
        }
        case OMPC_if : {
            OpenMPIfClauseModifier modifier = (OpenMPIfClauseModifier) va_arg(args,int);
            char * user_defined_modifier = NULL;
            if (modifier == OMPC_IF_MODIFIER_user)  user_defined_modifier = va_arg(args, char*);
            new_clause = OpenMPIfClause::addIfClause(this, modifier, user_defined_modifier);
            break;
        }
        case OMPC_default : {
            OpenMPDefaultClauseKind default_kind = (OpenMPDefaultClauseKind) va_arg(args, int);
            new_clause = OpenMPDefaultClause::addDefaultClause(this, default_kind);
            break;
        }
        case OMPC_order : {
            OpenMPOrderClauseKind order_kind = (OpenMPOrderClauseKind) va_arg(args, int);
            new_clause = OpenMPOrderClause::addOrderClause(this, order_kind);
            break;
        }
        case OMPC_ext_implementation_defined_requirement : {
            new_clause = OpenMPExtImplementationDefinedRequirementClause::addExtImplementationDefinedRequirementClause(this);
            break;
        }
        case OMPC_match: {
            new_clause = OpenMPMatchClause::addMatchClause(this);
            break;
        }

        case OMPC_reduction: {
            OpenMPReductionClauseModifier modifier = (OpenMPReductionClauseModifier) va_arg(args, int);
            OpenMPReductionClauseIdentifier identifier = (OpenMPReductionClauseIdentifier) va_arg(args, int);
            char * user_defined_identifier = NULL;
            if (identifier == OMPC_REDUCTION_IDENTIFIER_user) user_defined_identifier = va_arg(args, char *);
            new_clause = OpenMPReductionClause::addReductionClause(this, modifier, identifier, user_defined_identifier);
            break;
        }
        case OMPC_proc_bind: {
            OpenMPProcBindClauseKind proc_bind_kind = (OpenMPProcBindClauseKind) va_arg(args, int);
            new_clause = OpenMPProcBindClause::addProcBindClause(this, proc_bind_kind);
            break;
        } 
        case OMPC_uses_allocators: {
            new_clause = OpenMPUsesAllocatorsClause::addUsesAllocatorsClause(this);
            break;
        }
        case OMPC_bind: {
            OpenMPBindClauseBinding bind_binding = (OpenMPBindClauseBinding) va_arg(args, int);
            new_clause = OpenMPBindClause::addBindClause(this, bind_binding);
            break;
        }

        case OMPC_lastprivate: {
            OpenMPLastprivateClauseModifier modifier = (OpenMPLastprivateClauseModifier) va_arg(args,int);
            new_clause = OpenMPLastprivateClause::addLastprivateClause(this, modifier);
            break;
        }

        case OMPC_linear: {
            OpenMPLinearClauseModifier modifier = (OpenMPLinearClauseModifier) va_arg(args,int);
            new_clause = OpenMPLinearClause::addLinearClause(this, modifier);
            break;
        }
        case OMPC_aligned:{
            new_clause = OpenMPAlignedClause::addAlignedClause(this);
            break;
        }
        case OMPC_dist_schedule: {
            OpenMPDistScheduleClauseKind dist_schedule_kind = (OpenMPDistScheduleClauseKind) va_arg(args,int);
            new_clause = OpenMPDistScheduleClause::addDistScheduleClause(this, dist_schedule_kind);
            break;
        }
        case OMPC_schedule: {
            OpenMPScheduleClauseModifier modifier1 = (OpenMPScheduleClauseModifier) va_arg(args, int);
            OpenMPScheduleClauseModifier modifier2 = (OpenMPScheduleClauseModifier) va_arg(args, int);
            OpenMPScheduleClauseKind schedule_kind = (OpenMPScheduleClauseKind) va_arg(args, int);
            char * user_defined_kind = NULL;
            if (schedule_kind == OMPC_SCHEDULE_KIND_user) user_defined_kind = va_arg(args, char *);
            new_clause = OpenMPScheduleClause::addScheduleClause(this, modifier1, modifier2, schedule_kind, user_defined_kind);

            break;
        }
        case OMPC_device : {
            OpenMPDeviceClauseModifier modifier = (OpenMPDeviceClauseModifier) va_arg(args, int);
            new_clause = OpenMPDeviceClause::addDeviceClause(this, modifier);
            break;
}

        case OMPC_initializer:{
            OpenMPInitializerClausePriv priv = (OpenMPInitializerClausePriv) va_arg(args, int);
            char* user_defined_priv = NULL;
            if (priv == OMPC_INITIALIZER_PRIV_user) user_defined_priv = va_arg(args, char *);
            new_clause = OpenMPInitializerClause::addInitializerClause(this, priv, user_defined_priv);
            break;
        }
        case OMPC_allocate : {
            OpenMPAllocateClauseAllocator allocator = (OpenMPAllocateClauseAllocator) va_arg(args, int);
            char* user_defined_allocator = NULL;
            if (allocator == OMPC_ALLOCATE_ALLOCATOR_user) user_defined_allocator = va_arg(args, char *);
            new_clause = OpenMPAllocateClause::addAllocateClause(this, allocator, user_defined_allocator);
            break;
        }
        case OMPC_allocator : {
            OpenMPAllocatorClauseAllocator allocator = (OpenMPAllocatorClauseAllocator) va_arg(args, int);
            char* user_defined_allocator = NULL;
            if (allocator == OMPC_ALLOCATOR_ALLOCATOR_user) user_defined_allocator = va_arg(args, char *);
            new_clause = OpenMPAllocatorClause::addAllocatorClause(this, allocator, user_defined_allocator);
            
            break;
        }
        case OMPC_atomic_default_mem_order : {
            OpenMPAtomicDefaultMemOrderClauseKind atomic_default_mem_order_kind = (OpenMPAtomicDefaultMemOrderClauseKind) va_arg(args, int);
            new_clause = OpenMPAtomicDefaultMemOrderClause::addAtomicDefaultMemOrderClause(this, atomic_default_mem_order_kind);
            
            break;
        }
        case OMPC_in_reduction : {
            OpenMPInReductionClauseIdentifier identifier = (OpenMPInReductionClauseIdentifier) va_arg(args, int);
            char * user_defined_identifier = NULL;
            if (identifier == OMPC_IN_REDUCTION_IDENTIFIER_user) user_defined_identifier = va_arg(args, char *);
            new_clause = OpenMPInReductionClause::addInReductionClause(this, identifier, user_defined_identifier);
            break;
        }
        case OMPC_depobj_update : {
            OpenMPDepobjUpdateClauseDependeceType type = (OpenMPDepobjUpdateClauseDependeceType) va_arg(args, int);
            new_clause = OpenMPDepobjUpdateClause::addDepobjUpdateClause(this, type);            
            break;
        }
        case OMPC_depend : {
            OpenMPDependClauseModifier modifier = (OpenMPDependClauseModifier) va_arg(args, int);
            OpenMPDependClauseType type = (OpenMPDependClauseType) va_arg(args, int);
            new_clause = OpenMPDependClause::addDependClause(this, modifier, type);
            break;
        }
        case OMPC_affinity : {
            OpenMPAffinityClauseModifier modifier = (OpenMPAffinityClauseModifier) va_arg(args, int);
            new_clause = OpenMPAffinityClause::addAffinityClause(this, modifier);
            break;
        }
        case OMPC_to : {
            OpenMPToClauseKind to_kind = (OpenMPToClauseKind) va_arg(args, int);
            new_clause = OpenMPToClause::addToClause(this, to_kind);
            break;
        }
        case OMPC_from : {
            OpenMPFromClauseKind from_kind = (OpenMPFromClauseKind) va_arg(args, int);
            new_clause = OpenMPFromClause::addFromClause(this, from_kind);
            break;
        }

        case OMPC_device_type : {
            OpenMPDeviceTypeClauseKind device_type_kind = (OpenMPDeviceTypeClauseKind) va_arg(args, int);
            new_clause = OpenMPDeviceTypeClause::addDeviceTypeClause(this, device_type_kind);
            break;
        }
 
        case OMPC_defaultmap : {
            OpenMPDefaultmapClauseBehavior behavior = (OpenMPDefaultmapClauseBehavior) va_arg(args,int);
            OpenMPDefaultmapClauseCategory category = (OpenMPDefaultmapClauseCategory) va_arg(args,int);
            new_clause = OpenMPDefaultmapClause::addDefaultmapClause(this, behavior, category);
            break;        
        }
        case OMPC_task_reduction : {
            OpenMPTaskReductionClauseIdentifier identifier = (OpenMPTaskReductionClauseIdentifier) va_arg(args, int);
            char * user_defined_identifier = NULL;
            if (identifier == OMPC_TASK_REDUCTION_IDENTIFIER_user) user_defined_identifier = va_arg(args, char *);
            new_clause = OpenMPTaskReductionClause::addTaskReductionClause(this, identifier, user_defined_identifier);
            break;
        }
        case OMPC_map: {
            OpenMPMapClauseModifier modifier1 = (OpenMPMapClauseModifier)va_arg(args, int);
            OpenMPMapClauseModifier modifier2 = (OpenMPMapClauseModifier)va_arg(args, int);
            OpenMPMapClauseModifier modifier3 = (OpenMPMapClauseModifier)va_arg(args, int);
            OpenMPMapClauseType type = (OpenMPMapClauseType)va_arg(args, int);
            std::string mapper_identifier = (std::string)va_arg(args, char*);
            new_clause = OpenMPMapClause::addMapClause(this, modifier1, modifier2, modifier3, type, mapper_identifier);
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
    if (new_clause != NULL && new_clause->getClausePosition() == -1) {
        this->getClausesInOriginalOrder()->push_back(new_clause);
        new_clause->setClausePosition(this->getClausesInOriginalOrder()->size()-1);
    };
    return new_clause;
};

OpenMPClause* OpenMPMapClause::addMapClause(OpenMPDirective *directive, OpenMPMapClauseModifier modifier1, OpenMPMapClauseModifier modifier2, OpenMPMapClauseModifier modifier3, OpenMPMapClauseType type, std::string mapper_identifier) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_map);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPMapClause(modifier1, modifier2, modifier3, type, mapper_identifier);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_map] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            if (((OpenMPMapClause*)(*it))->getModifier1() == modifier1 && ((OpenMPMapClause*)(*it))->getModifier2() == modifier2 &&((OpenMPMapClause*)(*it))->getModifier3() == modifier3&&
                ((OpenMPMapClause*)(*it))->getType() == type) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPMapClause (modifier1, modifier2, modifier3, type, mapper_identifier);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPTaskReductionClause::addTaskReductionClause(OpenMPDirective *directive, OpenMPTaskReductionClauseIdentifier identifier, char * user_defined_identifier) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_task_reduction);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPTaskReductionClause(identifier);
        if (identifier == OMPC_TASK_REDUCTION_IDENTIFIER_user && user_defined_identifier) {
            ((OpenMPTaskReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
        };
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_task_reduction] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            std::string current_user_defined_identifier_expression;
            if (user_defined_identifier) {
                current_user_defined_identifier_expression = std::string(user_defined_identifier);
            };
            if (((OpenMPTaskReductionClause*)(*it))->getIdentifier() == identifier &&
            current_user_defined_identifier_expression.compare(((OpenMPTaskReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPTaskReductionClause(identifier);
        if (identifier == OMPC_TASK_REDUCTION_IDENTIFIER_user)
            ((OpenMPTaskReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
        current_clauses->push_back(new_clause);
        }
    return new_clause;
};

OpenMPClause* OpenMPDefaultmapClause::addDefaultmapClause(OpenMPDirective *directive, OpenMPDefaultmapClauseBehavior behavior, OpenMPDefaultmapClauseCategory category) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_defaultmap);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDefaultmapClause(behavior,category);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_defaultmap] = current_clauses;
    } else {
    for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
        if (((OpenMPDefaultmapClause*)(*it))->getBehavior() == behavior&&((OpenMPDefaultmapClause*)(*it))->getCategory() == category) {
             new_clause = (*it);
             return new_clause;
        }
    }
             new_clause = new OpenMPDefaultmapClause(behavior,category);
             current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPDeviceTypeClause::addDeviceTypeClause(OpenMPDirective *directive, OpenMPDeviceTypeClauseKind device_type_kind) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_device_type);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        current_clauses = new std::vector<OpenMPClause *>();
        (*all_clauses)[OMPC_device_type] = current_clauses;
    };
    new_clause = new OpenMPDeviceTypeClause(device_type_kind);
    current_clauses->push_back(new_clause);

    return new_clause;
};

OpenMPClause* OpenMPProcBindClause::addProcBindClause(OpenMPDirective *directive, OpenMPProcBindClauseKind proc_bind_kind) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_proc_bind);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPProcBindClause(proc_bind_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_proc_bind] = current_clauses;
    } else { /* could be an error since if clause may only appear once */
        std::cerr << "Cannot have two procbind clause for the directive " << directive->getKind() << ", ignored\n";
    };
    return new_clause;
};

OpenMPClause* OpenMPBindClause::addBindClause(OpenMPDirective *directive, OpenMPBindClauseBinding bind_binding) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_bind);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPBindClause(bind_binding);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_bind] = current_clauses;
    } else { /* could be an error since if clause may only appear once */
        std::cerr << "Cannot have two bind clause for the directive " << directive->getKind() << ", ignored\n";
    };

    return new_clause;
};

OpenMPClause* OpenMPLinearClause::addLinearClause(OpenMPDirective *directive, OpenMPLinearClauseModifier modifier) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_linear);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPLinearClause(modifier);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_linear] = current_clauses;
        } 
        else {            
            new_clause = new OpenMPLinearClause(modifier);
            current_clauses->push_back(new_clause);
        };
    (*all_clauses)[OMPC_linear] = current_clauses;
    return new_clause;
};

OpenMPClause* OpenMPExtImplementationDefinedRequirementClause::addExtImplementationDefinedRequirementClause(OpenMPDirective *directive) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_ext_implementation_defined_requirement);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPExtImplementationDefinedRequirementClause();
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_ext_implementation_defined_requirement] = current_clauses;
    } else {            
            new_clause = new OpenMPExtImplementationDefinedRequirementClause();
            current_clauses->push_back(new_clause);
    };
    (*all_clauses)[OMPC_ext_implementation_defined_requirement] = current_clauses;
    return new_clause;
};

void OpenMPExtImplementationDefinedRequirementClause::mergeExtImplementationDefinedRequirement(OpenMPDirective *directive, OpenMPClause* current_clause) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_ext_implementation_defined_requirement);
    OpenMPClause* new_clause = NULL;

    for (std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end()-1; it++) {
        if (((OpenMPExtImplementationDefinedRequirementClause*)(*it))->getImplementationDefinedRequirement() == ((OpenMPExtImplementationDefinedRequirementClause*)current_clause)->getImplementationDefinedRequirement()) {
            directive->getClausesInOriginalOrder()->pop_back();
            std::cerr << "You can not have 2 same ext_implementation_defined_requirement clauses, ignored\n";
            break;
        }
    }
};

void OpenMPLinearClause::mergeLinear(OpenMPDirective *directive, OpenMPClause* current_clause) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_linear);
    OpenMPClause* new_clause = NULL;

    for (std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end()-1; it++) {
          
        if (((OpenMPLinearClause*)(*it))->getModifier() == ((OpenMPLinearClause*)current_clause)->getModifier() && ((OpenMPLinearClause*)(*it))->getUserDefinedStep() == ((OpenMPLinearClause*)current_clause)->getUserDefinedStep()) {
            std::vector<const char *>* expressions_previous_clause = ((OpenMPLinearClause*)(*it))->getExpressions();
            std::vector<const char *>* expressions_current_clause = current_clause->getExpressions();

            for (std::vector<const char *>::iterator it_expr_current = expressions_current_clause->begin(); it_expr_current != expressions_current_clause->end(); it_expr_current++) {
                bool not_normalize = false;
                for (std::vector<const char *>::iterator it_expr_previous = expressions_previous_clause->begin(); it_expr_previous != expressions_previous_clause->end(); it_expr_previous++) {
                    if (strcmp(*it_expr_current, *it_expr_previous) == 0){
                        not_normalize = true;
                        break;
                    }
                }
                if (!not_normalize) {
                    expressions_previous_clause->push_back(*it_expr_current);
                }
            }
            current_clauses->pop_back();
            directive->getClausesInOriginalOrder()->pop_back();

            break;
        }
    }
};

OpenMPClause* OpenMPReductionClause::addReductionClause(OpenMPDirective *directive, OpenMPReductionClauseModifier modifier, OpenMPReductionClauseIdentifier identifier, char * user_defined_identifier) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_reduction);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPReductionClause(modifier, identifier);
        if (identifier == OMPC_REDUCTION_IDENTIFIER_user && user_defined_identifier) {
            ((OpenMPReductionClause *) new_clause)->setUserDefinedIdentifier(user_defined_identifier);
        };
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_reduction] = current_clauses;
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
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPReductionClause(modifier, identifier);
        if (identifier == OMPC_REDUCTION_IDENTIFIER_user)
            ((OpenMPReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
        current_clauses->push_back(new_clause);
    }
    return new_clause;

};

OpenMPClause* OpenMPFromClause::addFromClause(OpenMPDirective *directive, OpenMPFromClauseKind from_kind) {
   
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_from);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPFromClause(from_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_from] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            if (((OpenMPFromClause*)(*it))->getKind() == from_kind){
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPFromClause(from_kind);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPToClause::addToClause(OpenMPDirective *directive, OpenMPToClauseKind to_kind) {
   
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_to);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPToClause(to_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_to] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            if (((OpenMPToClause*)(*it))->getKind() == to_kind){
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPToClause(to_kind);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPAffinityClause::addAffinityClause(OpenMPDirective *directive, OpenMPAffinityClauseModifier modifier) {
   
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_affinity);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPAffinityClause(modifier);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_affinity] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            if (((OpenMPAffinityClause*)(*it))->getModifier() == modifier){
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPAffinityClause(modifier);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPDependClause::addDependClause(OpenMPDirective *directive, OpenMPDependClauseModifier modifier, OpenMPDependClauseType type) {
   
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_depend);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDependClause(modifier, type);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_depend] = current_clauses;
    } else {
        new_clause = new OpenMPDependClause(modifier, type);
        current_clauses->push_back(new_clause);
    }
    (*all_clauses)[OMPC_depend] = current_clauses;
    return new_clause;
};
void OpenMPDependClause::mergeDepend(OpenMPDirective *directive, OpenMPClause* current_clause) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_depend);
    OpenMPClause* new_clause = NULL;
    
    if (current_clauses->size() == true) return;

    for (std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end()-1; it++) {
        if (((OpenMPDependClause*)(*it))->getModifier() != OMPC_DEPEND_MODIFIER_unspecified && ((OpenMPDependClause*)(*it))->getModifier() == ((OpenMPDependClause*)current_clause)->getModifier() && ((OpenMPDependClause*)(*it))->getType() == ((OpenMPDependClause*)current_clause)->getType()) {
            bool normalize = true;
            std::vector<vector<const char*>* >* depend_iterators_definition_previous = ((OpenMPDependClause*)(*it))->getDependIteratorsDefinitionClass();
            std::vector<vector<const char*>* >* depend_iterators_definition_current = ((OpenMPDependClause*)current_clause)->getDependIteratorsDefinitionClass();
            if (depend_iterators_definition_previous->size() == depend_iterators_definition_current->size()){
                for (std::vector<vector<const char*>* >::iterator it_expr_current_outer = depend_iterators_definition_current->begin(); it_expr_current_outer != depend_iterators_definition_current->end(); it_expr_current_outer++) {
                    for (std::vector<vector<const char*>* >::iterator it_expr_previous_outer = depend_iterators_definition_previous->begin(); it_expr_previous_outer != depend_iterators_definition_previous->end(); it_expr_previous_outer++) {
                        bool merge = false;
                        if (strcmp((*it_expr_current_outer)->at(0), (*it_expr_previous_outer)->at(0)) == 0 && strcmp((*it_expr_current_outer)->at(1), (*it_expr_previous_outer)->at(1)) == 0 && strcmp((*it_expr_current_outer)->at(2), (*it_expr_previous_outer)->at(2)) == 0 && strcmp((*it_expr_current_outer)->at(3), (*it_expr_previous_outer)->at(3)) == 0 && strcmp((*it_expr_current_outer)->at(4), (*it_expr_previous_outer)->at(4)) == 0){
                            bool merge = true;
                            break;
                        }
                        if (it_expr_previous_outer == depend_iterators_definition_previous->end()-1 && merge == false) {
                            normalize = false;
                        }
                    }
                    if (normalize == false) break;
                }
                if (normalize == true) {
                    std::vector<const char *>* expressions_previous = ((OpenMPDependClause*)(*it))->getExpressions();
                    std::vector<const char *>* expressions_current = current_clause->getExpressions();
                    for (std::vector<const char *>::iterator it_expr_current = expressions_current->begin(); it_expr_current != expressions_current->end(); it_expr_current++) {
                        bool para_merge = true;
                        for (std::vector<const char *>::iterator it_expr_previous = expressions_previous->begin(); it_expr_previous != expressions_previous->end(); it_expr_previous++) {
                            if (strcmp(*it_expr_current, *it_expr_previous) == 0 ){ 
                                para_merge = false;
                            }
                        }
                        if (para_merge == true) expressions_previous->push_back(*it_expr_current);
                    }
                    current_clauses->pop_back();
                    directive->getClausesInOriginalOrder()->pop_back();
                    break;
                }
            }
                  
        }
        else if (((OpenMPDependClause*)(*it))->getModifier() == OMPC_DEPEND_MODIFIER_unspecified && ((OpenMPDependClause*)(*it))->getModifier() == ((OpenMPDependClause*)current_clause)->getModifier() && ((OpenMPDependClause*)(*it))->getType() == ((OpenMPDependClause*)current_clause)->getType()) {
            std::vector<const char *>* expressions_previous = ((OpenMPDependClause*)(*it))->getExpressions();
            std::vector<const char *>* expressions_current = current_clause->getExpressions();
            for (std::vector<const char *>::iterator it_expr_current = expressions_current->begin(); it_expr_current != expressions_current->end(); it_expr_current++) {
                bool para_merge = true;
                for (std::vector<const char *>::iterator it_expr_previous = expressions_previous->begin(); it_expr_previous != expressions_previous->end(); it_expr_previous++) {
                    if (strcmp(*it_expr_current, *it_expr_previous) == 0 ){ 
                        para_merge = false;
                    }
                }
                if (para_merge == true) expressions_previous->push_back(*it_expr_current);
            }
            current_clauses->pop_back();
            directive->getClausesInOriginalOrder()->pop_back();
            break;
        }
    }
};

OpenMPClause* OpenMPDepobjUpdateClause::addDepobjUpdateClause(OpenMPDirective *directive, OpenMPDepobjUpdateClauseDependeceType type) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_depobj_update);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDepobjUpdateClause(type);                
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_depobj_update] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            if (((OpenMPDepobjUpdateClause*)(*it))->getType() == type ) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPDepobjUpdateClause(type);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPInReductionClause::addInReductionClause(OpenMPDirective *directive, OpenMPInReductionClauseIdentifier identifier, char * user_defined_identifier) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_in_reduction);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPInReductionClause(identifier);
        if (identifier == OMPC_IN_REDUCTION_IDENTIFIER_user && user_defined_identifier) {
            ((OpenMPInReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
        };
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_in_reduction] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            std::string current_user_defined_identifier_expression;
            if (user_defined_identifier) {
                current_user_defined_identifier_expression = std::string(user_defined_identifier);
            };
            if (((OpenMPInReductionClause*)(*it))->getIdentifier() == identifier &&
            current_user_defined_identifier_expression.compare(((OpenMPInReductionClause*)(*it))->getUserDefinedIdentifier()) == 0) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could fine the matching object for this clause */
        new_clause = new OpenMPInReductionClause(identifier);
        if (identifier == OMPC_IN_REDUCTION_IDENTIFIER_user)
            ((OpenMPInReductionClause*)new_clause)->setUserDefinedIdentifier(user_defined_identifier);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPAtomicDefaultMemOrderClause::addAtomicDefaultMemOrderClause(OpenMPDirective *directive, OpenMPAtomicDefaultMemOrderClauseKind atomic_default_mem_order_kind) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_atomic_default_mem_order);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPAtomicDefaultMemOrderClause(atomic_default_mem_order_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_atomic_default_mem_order] = current_clauses;
    } else { /* could be an error since if clause may only appear once */
        std::cerr << "Cannot have two atomic_default_mem_orders clause for the directive " << directive->getKind() << ", ignored\n";
    }
    return new_clause;
};

OpenMPClause* OpenMPAllocatorClause::addAllocatorClause(OpenMPDirective *directive, OpenMPAllocatorClauseAllocator allocator, char* user_defined_allocator) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_allocator);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPAllocatorClause(allocator);
        if (allocator == OMPC_ALLOCATOR_ALLOCATOR_user)
            ((OpenMPAllocatorClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_allocator] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            std::string current_user_defined_allocator_expression;
            if (user_defined_allocator) {
                current_user_defined_allocator_expression = std::string(user_defined_allocator);
            };
            if (((OpenMPAllocatorClause*)(*it))->getAllocator() == allocator && current_user_defined_allocator_expression.compare(((OpenMPAllocatorClause*)(*it))->getUserDefinedAllocator()) == 0) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could find the matching object for this clause */
        new_clause = new OpenMPAllocatorClause(allocator);
        if (allocator == OMPC_ALLOCATOR_ALLOCATOR_user)
            ((OpenMPAllocatorClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPAllocateClause::addAllocateClause(OpenMPDirective *directive, OpenMPAllocateClauseAllocator allocator, char* user_defined_allocator) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_allocate);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPAllocateClause(allocator);
        if (allocator == OMPC_ALLOCATE_ALLOCATOR_user)
            ((OpenMPAllocateClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_allocate] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            std::string current_user_defined_allocator_expression;
            if (user_defined_allocator) {
                current_user_defined_allocator_expression = std::string(user_defined_allocator);
            };
            if (((OpenMPAllocateClause*)(*it))->getAllocator() == allocator && current_user_defined_allocator_expression.compare(((OpenMPAllocateClause*)(*it))->getUserDefinedAllocator()) == 0) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could find the matching object for this clause */
        new_clause = new OpenMPAllocateClause(allocator);
        if (allocator == OMPC_ALLOCATE_ALLOCATOR_user)
            ((OpenMPAllocateClause*)new_clause)->setUserDefinedAllocator(user_defined_allocator);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPInitializerClause::addInitializerClause(OpenMPDirective *directive, OpenMPInitializerClausePriv priv, char* user_defined_priv) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_initializer);
    OpenMPClause* new_clause = NULL;

     if (current_clauses->size() == 0) {
        new_clause = new OpenMPInitializerClause(priv);
        if (priv == OMPC_INITIALIZER_PRIV_user)
            ((OpenMPInitializerClause*)new_clause)->setUserDefinedPriv(user_defined_priv);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_initializer] = current_clauses;
     } else {
         for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            std::string current_user_defined_priv_expression;
            if (user_defined_priv) {
                current_user_defined_priv_expression = std::string(user_defined_priv);
            };
            if (((OpenMPInitializerClause*)(*it))->getPriv() == priv && current_user_defined_priv_expression.compare(((OpenMPInitializerClause*)(*it))->getUserDefinedPriv()) == 0) {
                new_clause = (*it);
                return new_clause;
            }
        }
        /* could find the matching object for this clause */
        new_clause = new OpenMPInitializerClause(priv);
        if (priv == OMPC_INITIALIZER_PRIV_user)
            ((OpenMPInitializerClause*)new_clause)->setUserDefinedPriv(user_defined_priv);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPDeviceClause::addDeviceClause(OpenMPDirective *directive, OpenMPDeviceClauseModifier modifier) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_device);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDeviceClause(modifier);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_device] = current_clauses;
    } else {
        for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            if (((OpenMPDeviceClause*)(*it))->getModifier() == modifier) {
                new_clause = (*it);
                return new_clause;
            }
        }
    /* could find the matching object for this clause */
        new_clause = new OpenMPDeviceClause(modifier);
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPScheduleClause::addScheduleClause(OpenMPDirective *directive, OpenMPScheduleClauseModifier modifier1, OpenMPScheduleClauseModifier modifier2, OpenMPScheduleClauseKind schedule_kind, char * user_defined_kind) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_schedule);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPScheduleClause(modifier1,modifier2, schedule_kind);
        if (schedule_kind == OMPC_SCHEDULE_KIND_user)
            ((OpenMPScheduleClause*)new_clause)->setUserDefinedKind(user_defined_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_schedule] = current_clauses;
    } else {
        std::cerr << "Cannot have two schedule clause for the directive " << directive->getKind() << ", ignored\n";
    }
    return new_clause;
};

OpenMPClause* OpenMPDistScheduleClause::addDistScheduleClause(OpenMPDirective *directive, OpenMPDistScheduleClauseKind dist_schedule_kind) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_dist_schedule);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDistScheduleClause(dist_schedule_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_dist_schedule] = current_clauses;
    } else {
        std::cerr << "Cannot have two dist_schedule clause for the directive " << directive->getKind() << ", ignored\n";
    }
    return new_clause;
};
        
OpenMPClause* OpenMPLastprivateClause::addLastprivateClause(OpenMPDirective *directive, OpenMPLastprivateClauseModifier modifier) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_lastprivate);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPLastprivateClause(modifier);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_lastprivate] = current_clauses;
        } else {
            for(std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {

                if (((OpenMPLastprivateClause*)(*it))->getModifier() == modifier) {
                    new_clause = (*it);
                    return new_clause;
                };
            };
            new_clause = new OpenMPLastprivateClause(modifier);
            current_clauses->push_back(new_clause);
        }
    return new_clause;
};

OpenMPClause* OpenMPIfClause::addIfClause(OpenMPDirective *directive, OpenMPIfClauseModifier modifier, char * user_defined_modifier) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_if);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPIfClause(modifier);
        if (modifier == OMPC_IF_MODIFIER_user) {
            ((OpenMPIfClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
        };
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_if] = current_clauses;
    } else {
        for (std::vector<OpenMPClause*>::iterator it = current_clauses->begin(); it != current_clauses->end(); ++it) {
            std::string current_user_defined_modifier_expression;
            if (user_defined_modifier) {
                current_user_defined_modifier_expression = std::string(user_defined_modifier);
            };
            if (((OpenMPIfClause*)(*it))->getModifier() == modifier &&
                current_user_defined_modifier_expression.compare(((OpenMPIfClause*)(*it))->getUserDefinedModifier()) == 0) {
                new_clause = (*it);
                return new_clause;
            };
        };
        new_clause = new OpenMPIfClause(modifier);
        if (modifier == OMPC_IF_MODIFIER_user) {
            ((OpenMPIfClause*)new_clause)->setUserDefinedModifier(user_defined_modifier);
        }
        current_clauses->push_back(new_clause);
    }
    return new_clause;
};

OpenMPClause* OpenMPDefaultClause::addDefaultClause(OpenMPDirective *directive, OpenMPDefaultClauseKind default_kind) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_default);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPDefaultClause(default_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_default] = current_clauses;
    } else { /* could be an error since if clause may only appear once */
        std::cerr << "Cannot have two default clause for the directive " << directive->getKind() << ", ignored\n";
    };

    return new_clause;
};

OpenMPClause* OpenMPOrderClause::addOrderClause(OpenMPDirective *directive, OpenMPOrderClauseKind order_kind) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_order);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPOrderClause(order_kind);
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_order] = current_clauses;
    } else { /* could be an error since if clause may only appear once */
        std::cerr << "Cannot have two order clause for the directive " << directive->getKind() << ", ignored\n";
    };

    return new_clause;
};

OpenMPClause* OpenMPAlignedClause::addAlignedClause(OpenMPDirective *directive) {

    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_aligned);
    OpenMPClause* new_clause = NULL;

    if (current_clauses->size() == 0) {
        new_clause = new OpenMPAlignedClause();
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_aligned] = current_clauses;
    } 

    return new_clause;
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
};

OpenMPClause* OpenMPMatchClause::addMatchClause(OpenMPDirective *directive) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_match);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        new_clause = new OpenMPMatchClause();
        current_clauses = new std::vector<OpenMPClause*>();
        current_clauses->push_back(new_clause);
        (*all_clauses)[OMPC_match] = current_clauses;
    } else {
        /* we can have multiple clause and we merge them together now, thus we return the object that is already created */
        new_clause = current_clauses->at(0);
        }
    return new_clause;
};

OpenMPClause* OpenMPUsesAllocatorsClause::addUsesAllocatorsClause(OpenMPDirective *directive) {
    std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* all_clauses = directive->getAllClauses();
    std::vector<OpenMPClause*>* current_clauses = directive->getClauses(OMPC_uses_allocators);
    OpenMPClause* new_clause = NULL;
    if (current_clauses->size() == 0) {
        current_clauses = new std::vector<OpenMPClause *>();
        (*all_clauses)[OMPC_uses_allocators] = current_clauses;
    };
    new_clause = new OpenMPUsesAllocatorsClause();
    current_clauses->push_back(new_clause);

    return new_clause;
};
