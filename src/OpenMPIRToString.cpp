#include "OpenMPIR.h"
#include <stdarg.h>


std::string OpenMPDirective::generatePragmaString(std::string prefix, std::string beginning_symbol, std::string ending_symbol) {

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
            break;
        }
        case OMPD_threadprivate: {
            std::vector<const char*>* list = ((OpenMPThreadprivateDirective*)this)->getThreadprivateList();
            std::vector<const char*>::iterator list_item;
            result += "(";
            for (list_item = list->begin(); list_item != list->end(); list_item++){
                 result += *list_item;
                 result += ",";
            }
            result = result.substr(0, result.size()-1); 
            result += ")";
            break;
        }
        case OMPD_declare_reduction:{
            std::vector<const char*>* list = ((OpenMPDeclareReductionDirective*)this)->getTypenameList();
            std::vector<const char*>::iterator list_item;
            std::string id = ((OpenMPDeclareReductionDirective*)this)->getIdentifier();
            std::string combiner = ((OpenMPDeclareReductionDirective*)this)->getCombiner();
            result += "( ";
            result += id;
            result += " : ";
            for (list_item = list->begin(); list_item != list->end(); list_item++){
                 result += *list_item;
                 result += ",";
            }
            result = result.substr(0, result.size()-1); 
            result += " : ";
            result += combiner;
            result += " ) ";
            break;
        }
        case OMPD_declare_mapper:{
            std::string id = ((OpenMPDeclareMapperDirective*)this)->getIdentifier();
            std::string type_var = ((OpenMPDeclareMapperDirective*)this)->getTypeVar();
            result += "( ";
            if(id != ""){
              result += id;
              result += ":";
            }
            result += type_var;
            result += " )";
            break;
        }
        case OMPD_declare_simd:{
            std::string proc_name = ((OpenMPDeclareSimdDirective*)this)->getProcName();
            if(proc_name != ""){
              result += "(";
              result += proc_name;
              result += ") ";
            }
            break;
        }
        case OMPD_end: {
            result += ((OpenMPEndDirective*)this)->getPairedDirective()->generatePragmaString("", "", "");
            break;
        }
        case OMPD_declare_target: {
            std::vector<std::string>* list = ((OpenMPDeclareTargetDirective*)this)->getExtendedList();
            if(list->size() > 0){
                std::vector<std::string>::iterator list_item;
                result += "(";
                for (list_item = list->begin(); list_item != list->end(); list_item++){
                   result += *list_item;
                   result += ",";
                }
                result = result.substr(0, result.size()-1); 
                result += ") ";
             }
                break;
        }
        case OMPD_critical: {
            std::string name = ((OpenMPCriticalDirective*)this)->getCriticalName();
            if(name!=""){
               result += "(";
               result += name;
               result += ") ";
            }
            break;
        }
        case OMPD_flush: {
            std::vector<std::string>* list = ((OpenMPFlushDirective*)this)->getFlushList();
            if(list->size() > 0){
            std::vector<std::string>::iterator list_item;
            result += "(";
            for (list_item = list->begin(); list_item != list->end(); list_item++){
                 result += *list_item;
                 result += ",";
            }
            result = result.substr(0, result.size()-1); 
            result += ") ";}
            break;
        }
        case OMPD_depobj: {
            std::string depobj = ((OpenMPDepobjDirective*)this)->getDepobj();
            result += "(";
            result += depobj;
            result += ") ";
            break;
        }
        default:
            ;
    };

    std::vector<OpenMPClause*>* clauses = this->getClausesInOriginalOrder();
    if (clauses->size() != 0) {
        std::vector<OpenMPClause*>::iterator iter;
        for (iter = clauses->begin(); iter != clauses->end(); iter++) {
            result += (*iter)->toString();
        }
        result = result.substr(0, result.size()-1);
    }

    if(this->getKind() == OMPD_atomic){
        std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses_atomic_clauses = ((OpenMPAtomicDirective*)this)->getAllAtomicClauses();
        if (clauses_atomic_clauses->size() != 0) {
            result += " ";
            std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >::iterator it;
            for (it = clauses_atomic_clauses->begin(); it != clauses_atomic_clauses->end(); it++) {
                std::vector<OpenMPClause*>* current_clauses = it->second;
                std::vector<OpenMPClause*>::iterator clauseIter;
                for (clauseIter = current_clauses->begin(); clauseIter != current_clauses->end(); clauseIter++) {
                    result += (*clauseIter)->toString();
                }
            }
            result = result.substr(0, result.size()-1);
        }

        std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >* clauses_atomic_after = ((OpenMPAtomicDirective*)this)->getAllClausesAtomicAfter();
    
        if (clauses_atomic_after->size() != 0) {
            result += " ";
            std::map<OpenMPClauseKind, std::vector<OpenMPClause*>* >::iterator it;
            for (it = clauses_atomic_after->begin(); it != clauses_atomic_after->end(); it++) {
                std::vector<OpenMPClause*>* current_clauses = it->second;
                std::vector<OpenMPClause*>::iterator clauseIter;
                for (clauseIter = current_clauses->begin(); clauseIter != current_clauses->end(); clauseIter++) {
                    result += (*clauseIter)->toString();
                }
            }
            result = result.substr(0, result.size()-1);
        }
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
        case OMPD_requires: {
            result += "requires ";
            break;
        }
        case OMPD_teams:
            result += "teams " ;
            break;
        case OMPD_for:
            result += "for ";
            break;
        case OMPD_do:
            result += "do ";
            break;
        case OMPD_simd:
            result += "simd ";
            break;
        case OMPD_for_simd:
            result += "for simd ";
            break;
        case OMPD_do_simd:
            result += "do simd ";
            break;
        case OMPD_parallel_for_simd:
            result += "parallel for simd ";
            break;
        case OMPD_parallel_do_simd:
            result += "parallel do simd ";
            break;
        case OMPD_declare_simd:
            result += "declare simd ";
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
        case OMPD_distribute_parallel_do:
            result += "distribute parallel do ";
            break;
        case OMPD_distribute_parallel_for_simd:
            result += "distribute parallel for simd ";
            break;
        case OMPD_distribute_parallel_do_simd:
            result += "distribute parallel do simd ";
            break;
        case OMPD_parallel_for:
            result += "parallel for ";
            break;
        case OMPD_parallel_do:
            result += "parallel do ";
            break;
        case OMPD_parallel_loop:
            result += "parallel loop ";
            break;
        case OMPD_parallel_sections:
            result += "parallel sections ";
            break;
        case OMPD_parallel_workshare:
            result += "parallel workshare ";
            break;
        case OMPD_parallel_master:
            result += "parallel master ";
            break;
        case OMPD_master_taskloop:
            result += "master taskloop ";
            break;
        case OMPD_master_taskloop_simd:
            result += "master taskloop simd ";
            break;
        case OMPD_parallel_master_taskloop:
            result += "parallel master taskloop ";
            break;
        case OMPD_parallel_master_taskloop_simd:
            result += "parallel master taskloop simd ";
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
        case OMPD_workshare:
            result += "workshare ";
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
            break;
        case OMPD_threadprivate:
            result += "threadprivate ";
            break;
        case OMPD_declare_reduction:
            result += "declare reduction ";
            break;
        case OMPD_declare_mapper:
            result += "declare mapper ";
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
        case OMPD_flush:
            result += "flush ";
            break;
        case OMPD_atomic:
            result += "atomic ";
            break;
        case OMPD_critical:
            result += "critical ";
            break;
        case OMPD_ordered:
            result += "ordered ";
            break;
        case OMPD_depobj:
            result += "depobj ";
            break;
        case OMPD_teams_distribute:
            result += "teams distribute ";
            break;
        case OMPD_teams_distribute_simd:
            result += "teams distribute simd ";
            break;
        case OMPD_teams_distribute_parallel_for:
            result += "teams distribute parallel for ";
            break;
        case OMPD_teams_distribute_parallel_for_simd:
            result += "teams distribute parallel for simd ";
            break;
        case OMPD_teams_loop:
            result += "teams loop ";
            break;
        case OMPD_target_parallel:
            result += "target parallel ";
            break;
        case OMPD_target_parallel_for:
            result += "target parallel for ";
            break;
        case OMPD_target_parallel_for_simd:
            result += "target parallel for simd ";
            break;
        case OMPD_target_parallel_loop:
            result += "target parallel loop ";
            break;
        case OMPD_target_simd:
            result += "target simd ";
            break;
        case OMPD_target_teams:
            result += "target teams ";
            break;
        case OMPD_target_teams_distribute:
            result += "target teams distribute ";
            break;
        case OMPD_target_teams_distribute_simd:
            result += "target teams distribute simd ";
            break;
        case OMPD_target_teams_loop:
            result += "target teams loop ";
            break;
        case OMPD_target_teams_distribute_parallel_for:
            result += "target teams distribute parallel for ";
            break;
        case OMPD_target_teams_distribute_parallel_for_simd:
            result += "target teams distribute parallel for simd ";
            break; 
        case OMPD_teams_distribute_parallel_do:
            result += "teams distribute parallel do ";
            break;
        case OMPD_teams_distribute_parallel_do_simd:
            result += "teams distribute parallel do simd ";
            break;
        case OMPD_target_parallel_do:
            result += "target parallel do ";
            break;
        case OMPD_target_parallel_do_simd:
            result += "target parallel do simd ";
            break;
        case OMPD_target_teams_distribute_parallel_do:
            result += "target teams distribute parallel do ";
            break;
        case OMPD_target_teams_distribute_parallel_do_simd:
            result += "target teams distribute parallel do simd ";
            break; 
        default:
            printf("The directive enum is not supported yet.\n");
    };

    return result;
};

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
};

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
        case OMPC_initializer:
            result += "initializer ";
            break;
        case OMPC_for:
            result += "for ";
            break;
        case OMPC_do:
            result += "do ";
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
        case OMPC_acq_rel:
            result += "acq_rel ";
            break;
        case OMPC_release:
            result += "release ";
            break;
        case OMPC_acquire:
            result += "acquire ";
            break;
        case OMPC_read:
            result += "read ";
            break;
        case OMPC_write:
            result += "write ";
            break;
        case OMPC_update:
            result += "update ";
            break;
        case OMPC_capture:
            result += "capture ";
            break;
        case OMPC_seq_cst:
            result += "seq_cst ";
            break;
        case OMPC_relaxed:
            result += "relaxed ";
            break;
        case OMPC_hint:
            result += "hint ";
            break;
        case OMPC_threads:
            result += "threads ";
            break;
        case OMPC_simd:
            result += "simd ";
            break;
        case OMPC_destroy:
            result += "destroy ";
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

};

std::string OpenMPExtImplementationDefinedRequirementClause::toString() {
    std::string result = "";
    std::string parameter_string;

    std::string ext_user_defined_implementation = this->getImplementationDefinedRequirement();
    result += "ext_" + ext_user_defined_implementation;
    result += " ";
    return result;
};

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
    std::vector<vector<const char*>* >* depend_iterators_definition_class = this->getDependIteratorsDefinitionClass();
    std::string result = "depend ";

    std::string clause_string = "(";

    OpenMPDependClauseModifier modifier = this->getModifier();

    OpenMPDependClauseType type = this->getType();
    if(modifier != OMPC_DEPEND_MODIFIER_unknown) {
        switch (modifier) {
            case OMPC_DEPEND_MODIFIER_iterator: {
                clause_string += "iterator";
                clause_string += " ( ";
                for (int i = 0; i < depend_iterators_definition_class->size(); i++) {  
                    clause_string += depend_iterators_definition_class->at(i)->at(0);
                    if (strcmp(depend_iterators_definition_class->at(i)->at(0), "\0") != 0) {
                        clause_string += " ";
                    };
                    clause_string += depend_iterators_definition_class->at(i)->at(1);
                    clause_string += "=";
                    clause_string += depend_iterators_definition_class->at(i)->at(2);
                    clause_string += ":";
                    clause_string += depend_iterators_definition_class->at(i)->at(3);
                    if ((string)depend_iterators_definition_class->at(i)->at(4) != "") {
                        clause_string += ":";
                        clause_string += depend_iterators_definition_class->at(i)->at(4);
                    };
                    clause_string += ", ";
                };
                clause_string = clause_string.substr(0, clause_string.size()-2);

                clause_string += " )";
            }
            default:
            ;
        }
    }

    if (clause_string.size() > 1) {
        clause_string += ", ";
    };
    switch (type) {
        case OMPC_DEPENDENCE_TYPE_in:
            clause_string += "in";
            break;
        case OMPC_DEPENDENCE_TYPE_out:
            clause_string += "out";
            break;
        case OMPC_DEPENDENCE_TYPE_inout:
            clause_string += "inout";
            break;
        case OMPC_DEPENDENCE_TYPE_mutexinoutset:
            clause_string += "mutexinoutset";
            break;
        case OMPC_DEPENDENCE_TYPE_depobj:
            clause_string += "depobj";
            break;
        case OMPC_DEPENDENCE_TYPE_source:
            clause_string += "source";
            break;
        case OMPC_DEPENDENCE_TYPE_sink:
            clause_string += "sink";
            break;
        default:
            ;
    }

    if (clause_string.size() > 1&&type!=OMPC_DEPENDENCE_TYPE_source) {
        clause_string += " : ";
    };
    if(type==OMPC_DEPENDENCE_TYPE_sink){clause_string += this->getDependenceVector();}
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };
    return result;
};

std::string OpenMPDepobjUpdateClause::toString() {

    std::string result = "update ";
    std::string clause_string = "(";
    OpenMPDepobjUpdateClauseDependeceType type = this->getType();
    switch (type) {
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_source:
            clause_string += "source";
            break;
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_in:
            clause_string += "in";
            break;
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_out:
            clause_string += "out";
            break;
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_inout:
            clause_string += "inout";
            break;
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_mutexinoutset:
            clause_string += "mutexinoutset";
            break;
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_depobj:
            clause_string += "depobj";
            break;
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_sink:
            clause_string += "sink";
            break;
        default:
            ;
    }
    clause_string += ") ";
    if (clause_string.size() > 3) {
        result += clause_string;
    };

    return result;
};

std::string OpenMPAffinityClause::toString() {
    std::vector<vector<const char*>* >* iterators_definition_class = this->getIteratorsDefinitionClass();
    std::string result = "affinity ";
    std::string clause_string = "(";
    OpenMPAffinityClauseModifier modifier = this->getModifier();
    switch (modifier) {
        case OMPC_AFFINITY_MODIFIER_iterator:
            clause_string += "iterator";
            clause_string += " ( ";
            for (int i = 0; i <iterators_definition_class->size(); i++){  
                clause_string += iterators_definition_class->at(i)->at(0);
                if (strcmp(iterators_definition_class->at(i)->at(0), "\0") != 0) { clause_string +=" "; };
                clause_string +=iterators_definition_class->at(i)->at(1);
                clause_string +="=";
                clause_string +=iterators_definition_class->at(i)->at(2);
                clause_string +=":";
                clause_string +=iterators_definition_class->at(i)->at(3);
                if ((string)iterators_definition_class->at(i)->at(4) != "") {
                    clause_string +=":";clause_string +=iterators_definition_class->at(i)->at(4);
                }
                clause_string += ", ";
            };
            clause_string = clause_string.substr(0, clause_string.size()-2);
            clause_string += " )";
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

std::string OpenMPToClause::toString() {

    std::string result = "to ";
    std::string clause_string = "(";
    OpenMPToClauseKind to_kind = this->getKind();
    switch (to_kind) {
        case OMPC_TO_mapper:
            clause_string += "mapper";
            clause_string += "(";
            clause_string += this->getMapperIdentifier();
            clause_string += ")";
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

std::string OpenMPFromClause::toString() {

    std::string result = "from ";
    std::string clause_string = "(";
    OpenMPFromClauseKind from_kind = this->getKind();
    switch (from_kind) {
        case OMPC_FROM_mapper:
            clause_string += "mapper";
            clause_string += "(";
            clause_string += this->getMapperIdentifier();
            clause_string += ")";
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
    if (category != OMPC_DEFAULTMAP_CATEGORY_unspecified) {
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
        case OMPC_DEFAULTMAP_CATEGORY_allocatable:
            clause_string += "allocatable";
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
            clause_string += "(";
            clause_string += this->getMapperIdentifier();
            clause_string += ")";
            break;
        default:
            ;
    }
    switch (modifier2) {
        case OMPC_MAP_MODIFIER_always:
            clause_string += " always";
            break;
        case OMPC_MAP_MODIFIER_close:
            clause_string += " close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            clause_string += " mapper";
            clause_string += "(";
            clause_string += this->getMapperIdentifier();
            clause_string += ")";
            break;
        default:
            ;
    }
    switch (modifier3) {
        case OMPC_MAP_MODIFIER_always:
            clause_string += " always";
            break;
        case OMPC_MAP_MODIFIER_close:
            clause_string += " close";
            break;
        case OMPC_MAP_MODIFIER_mapper:
            clause_string += " mapper";
            clause_string += "(";
            clause_string += this->getMapperIdentifier();
            clause_string += ")";
            break;
        default:
            ;
    }

    switch (type) {
        case OMPC_MAP_TYPE_to:
            clause_string += " to";
            break;
        case OMPC_MAP_TYPE_from:
            clause_string += " from";
            break;
        case OMPC_MAP_TYPE_tofrom:
            clause_string += " tofrom";
            break;
        case OMPC_MAP_TYPE_alloc:
            clause_string += " alloc";
            break;
        case OMPC_MAP_TYPE_release:
            clause_string += " release";
            break;
        case OMPC_MAP_TYPE_delete:
            clause_string += " delete";
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
    clause_string += ") ";
    result += clause_string;
    return result;
};

std::string OpenMPAlignedClause::toString() {

    std::string result = "aligned ";
    std::string clause_string = "(";
    clause_string += this->expressionToString();
    if(this->getUserDefinedAlignment() != ""){
        clause_string += ":";
        clause_string += this->getUserDefinedAlignment();
    }
        clause_string += ") ";
        result += clause_string;
    return result;
};

std::string OpenMPDistScheduleClause::toString() {

    std::string result = "dist_schedule ";
    std::string clause_string = "(";
    OpenMPDistScheduleClauseKind kind = this->getKind();
    switch (kind) {
        case OMPC_DIST_SCHEDULE_KIND_static:
            clause_string += "static";
            break;
        default:
            ;
    }
    if (this->getChunkSize() != "") {
        clause_string += ", ";
    clause_string += this->getChunkSize();}
    clause_string += ") ";
    result += clause_string;
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
    if (clause_string.size() > 1){
        clause_string += ":"; 
    }
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
    if (this->getChunkSize() != ""){
        clause_string += ", ";
        clause_string += this->getChunkSize();
    }
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

std::string OpenMPInitializerClause::toString() {

    std::string result = "initializer ";
    std::string clause_string = "(";
    std::string priv = this->getUserDefinedPriv();
    clause_string += "omp_priv=" + priv;
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
    if (this->getUserDefinedAllocator() != ""){
        clause_string += this->getUserDefinedAllocator();
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
   if (this->getUserDefinedAllocator() != ""){
        clause_string += this->getUserDefinedAllocator();
    }
    clause_string += this->expressionToString();
    clause_string += ") ";
    if (clause_string.size() > 2) {
        result += clause_string;
    };


    return result;
};

std::string OpenMPVariantClause::toString() {

    std::string result;
    std::string clause_string;
    std::string parameter_string;
    std::string beginning_symbol;
    std::string ending_symbol;
    std::pair<std::string, std::string>* parameter_pair_string = NULL;
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
    parameter_pair_string = this->getUserCondition();
    if (parameter_pair_string->first != "") {
        clause_string += "user = {condition(score(" + parameter_pair_string->first + "): " + parameter_pair_string->second + ")}" + ", ";
    }
    else if (parameter_pair_string->second != "") {
        clause_string += "user = {condition(" + parameter_pair_string->second + ")}" + ", ";
    };

    // check construct
    std::vector<std::pair<std::string, OpenMPDirective*>>* parameter_pair_directives = this->getConstructDirective();
    if (parameter_pair_directives->size() != 0) {
        clause_string += "construct = {";
        std::vector<std::pair<std::string, OpenMPDirective*>>::iterator iter;
        for (iter = parameter_pair_directives->begin(); iter != parameter_pair_directives->end(); iter++) {
            if ((*iter).first != "") {
                beginning_symbol = "score(" + (*iter).first + "): ";
                ending_symbol = ")";
            }
            else if ((*iter).second->getAllClauses()->size() != 0){
                beginning_symbol = "(";
                ending_symbol = ")";
            }
            else {
                beginning_symbol = "";
                ending_symbol = "";
            };
            clause_string += (*iter).second->generatePragmaString("", beginning_symbol, ending_symbol) + ", ";
        };
        clause_string = clause_string.substr(0, clause_string.size()-2);
        clause_string += "}, ";
    }

    result += clause_string;
    parameter_string.clear();

    // check device
    clause_string.clear();
    // check device arch
    parameter_pair_string = this->getArchExpression();
    if (parameter_pair_string->first.size() > 0) {
        clause_string += "arch(score(" + parameter_pair_string->first + "): " + parameter_pair_string->second + "), ";
    }
    else if (parameter_pair_string->second.size() > 0) {
        clause_string += "arch(" + parameter_pair_string->second + "), ";
    };

    // check device isa
    parameter_pair_string = this->getIsaExpression();
    if (parameter_pair_string->first.size() > 0) {
        clause_string += "isa(score(" + parameter_pair_string->first + "): " + parameter_pair_string->second + "), ";
    }
    else if (parameter_pair_string->second.size() > 0) {
        clause_string += "isa(" + parameter_pair_string->second + "), ";
    };

    // check device_kind
    std::pair<std::string, OpenMPClauseContextKind>* context_kind = this->getContextKind();
    switch (context_kind->second) {
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

    if (context_kind->first.size() > 0) {
        clause_string += "kind(score(" + context_kind->first + "): " + parameter_string + "), ";
    }
    else if (parameter_string.size() > 0) {
        clause_string += "kind(" + parameter_string + "), ";
    };

    if (clause_string.size() > 0) {
        result += "device = {" + clause_string.substr(0, clause_string.size()-2) + "}, ";
    };

    // check implementation
    clause_string.clear();
    parameter_string.clear();
    // check implementation vendor
    std::pair<std::string, OpenMPClauseContextVendor>* context_vendor = this->getImplementationKind();
    switch (context_vendor->second) {
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
    if (context_vendor->first.size() > 0) {
        clause_string += "vendor(score(" + context_vendor->first + "): " + parameter_string + "), ";
    }
    else if (parameter_string.size() > 0) {
        clause_string += "vendor(" + parameter_string + "), ";
    };

    // check implementation extension
    parameter_pair_string = this->getExtensionExpression();
    if (parameter_pair_string->first.size() > 0) {
        clause_string += "extension(score(" + parameter_pair_string->first + "): " + parameter_pair_string->second + "), ";
    }
    else if (parameter_pair_string->second.size() > 0) {
        clause_string += "extension(" + parameter_pair_string->second + "), ";
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
};

std::string OpenMPOrderClause::toString() {

    std::string result = "order (";
    std::string parameter_string;
    OpenMPOrderClauseKind order_kind = this->getOrderClauseKind();
    switch (order_kind) {
        case OMPC_ORDER_concurrent:
            parameter_string = "concurrent";
            break;
        default:
            std::cout << "The parameter of order clause is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        result += parameter_string + ") ";
    }
    else {
        result = result.substr(0, result.size()-2);
    }

    return result;
};

std::string OpenMPBindClause::toString() {

    std::string result = "bind (";
    std::string parameter_string;
    OpenMPBindClauseBinding bind_binding = this->getBindClauseBinding();
    switch (bind_binding) {
        case OMPC_BIND_teams:
            parameter_string = "teams";
            break;
        case OMPC_BIND_parallel:
            parameter_string = "parallel";
            break;
        case OMPC_BIND_thread:
            parameter_string = "thread";
            break;
        default:
            std::cout << "The parameter of bind clause is not supported.\n";
    };

    if (parameter_string.size() > 0) {
        result += parameter_string + ") ";
    }
    else {
        result = result.substr(0, result.size()-2);
    }

    return result;
};

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
};

std::string OpenMPUsesAllocatorsClause::toString() {
    std::vector<usesAllocatorParameter*>* usesAllocatorsAllocatorSequence = this->getUsesAllocatorsAllocatorSequence();
    std::string result = "uses_allocators ";
    std::string parameter_string;
    parameter_string += "(";
    for (int i = 0; i < usesAllocatorsAllocatorSequence->size(); i++) { 
        switch (usesAllocatorsAllocatorSequence->at(i)->getUsesAllocatorsAllocator()) {
            case OMPC_USESALLOCATORS_ALLOCATOR_default: {
                parameter_string += "omp_default_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_large_cap: {
                parameter_string += "omp_large_cap_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_cons_mem: {
                parameter_string += "omp_const_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_high_bw: {
                parameter_string += "omp_high_bw_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_low_lat: {
                parameter_string += "omp_low_lat_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_cgroup: {
                parameter_string += "omp_cgroup_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_pteam: {
                parameter_string += "omp_pteam_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_thread: {
                parameter_string += "omp_thread_mem_alloc";
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            case OMPC_USESALLOCATORS_ALLOCATOR_user: {
                parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorUser();
                if (usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray() != "") {
                    parameter_string += "(";
                    parameter_string += usesAllocatorsAllocatorSequence->at(i)->getAllocatorTraitsArray();
                    parameter_string += ")";
                }
                if (i < usesAllocatorsAllocatorSequence->size()-1) {
                    parameter_string += ",";
                }
            }
            break;
            default:
                   ;
    }
}
    result += parameter_string;
    result += " ) ";
    return result;
};
