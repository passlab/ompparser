/*
 * Copyright (c) 2018-2020, High Performance Computing Architecture and System
 * research laboratory at University of North Carolina at Charlotte (HPCAS@UNCC)
 * and Lawrence Livermore National Security, LLC.
 *
 * SPDX-License-Identifier: (BSD-3-Clause)
 */

#include "OpenMPIR.h"
#include <stdarg.h>


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

void OpenMPDepobjUpdateClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "update_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDepobjUpdateClauseDependeceType type = this->getType();
    std::string parameter_string;
    switch (type) {
        case OMPC_DEPOBJ_UPDATE_DEPENDENCE_TYPE_source:
            parameter_string = "source";
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
        case OMPC_DEPENDENCE_TYPE_in:
            parameter_string = "in";
            break;
        case OMPC_DEPENDENCE_TYPE_out:
            parameter_string  = "out";
            break;
        case OMPC_DEPENDENCE_TYPE_inout:
            parameter_string = "inout";
            break;
        case OMPC_DEPENDENCE_TYPE_mutexinoutset:
            parameter_string = "mutexinoutset";
            break;
        case OMPC_DEPENDENCE_TYPE_depobj:
            parameter_string = "depobj";
            break;
        case OMPC_DEPENDENCE_TYPE_source:
            parameter_string = "source";
            break;
        case OMPC_DEPENDENCE_TYPE_sink:
            parameter_string = "sink";
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
    std:string mapper_identifier = this->getMapperIdentifier();
    if (mapper_identifier != "") {
    std::string node_id =  clause_kind+ "_kind";
        parameter_string = mapper_identifier;
        std::string mapper_identifier_name = clause_kind + "_mapper_identifier";
        current_line = indent + node_id + " -- " + mapper_identifier_name + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + mapper_identifier_name + " [label = \"" + mapper_identifier_name + "\\n " + parameter_string + "\"]\n";
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
    std:string mapper_identifier = this->getMapperIdentifier();
    if (mapper_identifier != "") {
    std::string node_id =  clause_kind+ "_kind";
        parameter_string = mapper_identifier;
        std::string mapper_identifier_name = clause_kind + "_mapper_identifier";
        current_line = indent + node_id + " -- " + mapper_identifier_name + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + mapper_identifier_name + " [label = \"" + mapper_identifier_name + "\\n " + parameter_string + "\"]\n";
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
        case OMPC_DEFAULTMAP_CATEGORY_allocatable:
            parameter_string = "allocatable";
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
        case OMPD_do_simd:
                directive_kind = "do_simd ";
                break;
        case OMPD_parallel_for_simd:
                directive_kind = "parallel_for_simd ";
                break;
        case OMPD_declare_simd:
                directive_kind = "declare_simd ";
                break;
        case OMPD_distribute_simd:
                directive_kind = "distribute_simd ";
                break;
        case OMPD_distribute_parallel_for:
                directive_kind = "distribute_parallel_for ";
                break;
        case OMPD_distribute_parallel_do:
                directive_kind = "distribute_parallel_do ";
                break;
        case OMPD_distribute_parallel_for_simd:
                directive_kind = "distribute_parallel_for_simd ";
                break;
        case OMPD_distribute_parallel_do_simd:
                directive_kind = "distribute_parallel_do_simd ";
                break;
        case OMPD_parallel_for:
                directive_kind = "parallel_for ";
                break;
        case OMPD_parallel_do:
                directive_kind = "parallel_do ";
                break;
        case OMPD_parallel_loop:
                directive_kind = "parallel_loop ";
                break;
        case OMPD_parallel_sections:
                directive_kind = "parallel_sections ";
                break;
        case OMPD_parallel_workshare:
                directive_kind += "parallel_workshare ";
                break;
        case OMPD_parallel_master:
                directive_kind += "parallel_master ";
                break;
        case OMPD_master_taskloop:
                directive_kind += "master_taskloop ";
                break;
        case OMPD_master_taskloop_simd:
                directive_kind += "master_taskloop_simd ";
                break;
        case OMPD_parallel_master_taskloop:
                directive_kind += "parallel_master_taskloop ";
                break;
        case OMPD_parallel_master_taskloop_simd:
                directive_kind += "parallel_master_taskloop_simd ";
                break;
        case OMPD_declare_reduction:
                directive_kind = "declare_reduction ";
                break;
        case OMPD_declare_mapper:
                directive_kind = "declare_mapper ";
                break;
        case OMPD_taskloop_simd:
                directive_kind = "taskloop_simd ";
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
        case OMPD_target_update:
                directive_kind = "target_update ";
                break;
        case OMPD_declare_target:
                directive_kind = "declare_target ";
                break;
        case OMPD_end_declare_target:
                directive_kind = "end_declare_target ";
                break;
        case OMPD_teams_distribute:
                directive_kind += "teams_distribute ";
                break;
        case OMPD_teams_distribute_simd:
                directive_kind += "teams_distribute_simd ";
                break;
        case OMPD_teams_distribute_parallel_for:
                directive_kind += "teams_distribute_parallel_for ";
                break;
        case OMPD_teams_distribute_parallel_for_simd:
                directive_kind += "teams_distribute_parallel_for_simd ";
                break;
        case OMPD_teams_loop:
                directive_kind += "teams_loop ";
                break;
        case OMPD_target_parallel:
                directive_kind += "target_parallel ";
                break;
        case OMPD_target_parallel_for:
                directive_kind += "target_parallel_for ";
                break;
        case OMPD_target_parallel_for_simd:
                directive_kind += "target_parallel_for_simd ";
                break;
        case OMPD_target_parallel_loop:
                directive_kind += "target_parallel_loop ";
                break;
        case OMPD_target_simd:
                directive_kind += "target_simd ";
                break;
        case OMPD_target_teams:
                directive_kind += "target_teams ";
                break;
        case OMPD_target_teams_distribute:
                directive_kind += "target_teams_distribute ";
                break;
        case OMPD_target_teams_distribute_simd:
                directive_kind += "target_teams_distribute_simd ";
                break;
        case OMPD_target_teams_loop:
                directive_kind += "target_teams_loop ";
                break;
        case OMPD_target_teams_distribute_parallel_for:
                directive_kind += "target_teams_distribute_parallel_for ";
                break;
        case OMPD_target_teams_distribute_parallel_for_simd:
                directive_kind += "target_teams_distribute_parallel_for simd ";
                break;
        case OMPD_teams_distribute_parallel_do:
                directive_kind += "teams distribute parallel do ";
                break;
        case OMPD_teams_distribute_parallel_do_simd:
                directive_kind += "teams distribute parallel do simd ";
                break;
        case OMPD_target_parallel_do:
                directive_kind += "target parallel do ";
                break;
        case OMPD_target_parallel_do_simd:
                directive_kind += "target parallel do simd ";
                break;
        case OMPD_target_teams_distribute_parallel_do:
                directive_kind += "target teams distribute parallel do ";
                break;
        case OMPD_target_teams_distribute_parallel_do_simd:
                directive_kind += "target teams distribute parallel do simd ";
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
    std::string node_id = "";
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
           break;
        }
        case OMPD_threadprivate: {
            std::string indent = std::string(1, '\t');
            std::vector<const char *> *list = ((OpenMPThreadprivateDirective*)this)->getThreadprivateList();
            std::vector<const char *>::iterator list_item;
            int list_index = 0;
            std::string list_name;
            std::string expr_name;
            std::string tkind = "threadprivate";
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
           break;
        }
        case OMPD_declare_reduction: {
            std::string indent = std::string(1, '\t');
            std::vector<const char*>* list = ((OpenMPDeclareReductionDirective*)this)->getTypenameList();
            std::vector<const char*>::iterator list_item;
            std::string id = ((OpenMPDeclareReductionDirective*)this)->getIdentifier();
            std::string combiner = ((OpenMPDeclareReductionDirective*)this)->getCombiner();
            int list_index = 0;
            std::string list_name;
            std::string expr_name;
            std::string tkind = "declare_reduction";
            std::string node_id = tkind + "_reduction_identifier";
            current_line = indent + tkind + " -- " + node_id + "\n";
            output << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + id + "\"]\n";
            output << current_line.c_str();
            list_name = tkind + "_typename_list_" + std::to_string(list_index);
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
            node_id = tkind + "_combiner";
            current_line = indent + tkind + " -- " + node_id + "\n";
            output << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + combiner + "\"]\n";
            output << current_line.c_str();
           break;
        }
        case OMPD_declare_mapper: {
            std::string indent = std::string(1, '\t');
            OpenMPDeclareMapperDirectiveIdentifier id = ((OpenMPDeclareMapperDirective*)this)->getIdentifier();
            //std::string id = ((OpenMPDeclareMapperDirective*)this)->getUserDefinedIdentifier();
            std::string type_var = ((OpenMPDeclareMapperDirective*)this)->getTypeVar();
            int list_index = 0;
            std::string tkind = "declare_mapper";
            std::string node_id = tkind + "_mapper_identifier";
            std::string parameter_string;
            switch (id) {
                case OMPD_DECLARE_MAPPER_IDENTIFIER_default:
                {
                    parameter_string = "default";
                    break;
                }
                case OMPD_DECLARE_MAPPER_IDENTIFIER_user: 
                {
                    parameter_string = ((OpenMPDeclareMapperDirective*)this)->getUserDefinedIdentifier();
                    break;
                }
                default:
                    ;
            };
            current_line = indent + tkind + " -- " + node_id + "\n";
            output << current_line.c_str();

            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
            output << current_line.c_str();
            node_id = tkind + "_type_var";
            current_line = indent + tkind + " -- " + node_id + "\n";
            output << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + type_var + "\"]\n";
            output << current_line.c_str();
            break;
        }
        case OMPD_declare_simd: {
            std::string indent = std::string(1, '\t');
            std::string proc_name = ((OpenMPDeclareSimdDirective*)this)->getProcName();
            int list_index = 0;
            std::string tkind = "declare_simd";
            std::string node_id = tkind + "_proc_name";
            if (proc_name != ""){
              current_line = indent + tkind + " -- " + node_id + "\n";
              output << current_line.c_str();
              current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + proc_name + "\"]\n";
              output << current_line.c_str();
            }
           break;
        }
        default: {
            node_id = directive_kind.substr(0, directive_kind.size() - 1);
        }
    };

    std::vector<OpenMPClause*>* clauses = this->getClausesInOriginalOrder();
    if (clauses->size() != 0) {
        std::vector<OpenMPClause*>::iterator iter;
        int clause_index = 0;
        for (iter = clauses->begin(); iter != clauses->end(); iter++) {
            (*iter)->generateDOT(output, 1, clause_index, node_id);
            clause_index += 1;
        };
    };

    output << "}\n";

};

void OpenMPDirective::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node, std::string trait_score) {

    std::string directive_kind;
    OpenMPDirectiveKind kind = this->getKind();
    switch(kind) {
        case OMPD_cancellation_point :
            directive_kind = "cancellation_point";
            break;
        case OMPD_for_simd:
            directive_kind = "for_simd";
            break;
        case OMPD_distribute_simd:
            directive_kind = "distribute_simd";
            break;
        case OMPD_distribute_parallel_for:
            directive_kind = "distribute_parallel_for";
            break; 
       case OMPD_distribute_parallel_do:
            directive_kind = "distribute_parallel_do";
            break;
        case OMPD_distribute_parallel_for_simd:
            directive_kind = "distribute_parallel_for_simd";
            break;
        case OMPD_distribute_parallel_do_simd:
            directive_kind = "distribute_parallel_do_simd";
            break;
        case OMPD_declare_reduction:
            directive_kind = "declare_reduction";
            break;
        case OMPD_declare_mapper:
            directive_kind = "declare_mapper";
            break;
        case OMPD_taskloop_simd:
            directive_kind = "taskloop_simd";
            break;
        case OMPD_target_data:
            directive_kind = "target_data";
            break;
        case OMPD_target_enter_data:
            directive_kind = "target_enter_data";
            break;
        case OMPD_target_exit_data:
            directive_kind = "target_exit_data";
            break;
        case OMPD_target_update:
            directive_kind = "target_update";
            break;
        case OMPD_declare_target:
            directive_kind = "declare_target";
            break;
        case OMPD_end_declare_target:
            directive_kind = "end_declare_target";
            break;
        case OMPD_depobj:
            directive_kind = "depobj ";
            break;
        case OMPD_teams_distribute:
            directive_kind += "teams_distribute ";
            break;
        case OMPD_teams_distribute_simd:
            directive_kind += "teams_distribute_simd ";
            break;
        case OMPD_teams_distribute_parallel_for:
            directive_kind += "teams_distribute_parallel_for ";
            break;
        case OMPD_teams_distribute_parallel_for_simd:
            directive_kind += "teams_distribute_parallel_for_simd ";
            break;
        case OMPD_teams_loop:
            directive_kind += "teams_loop ";
            break;
        case OMPD_target_parallel:
            directive_kind += "target_parallel ";
            break;
        case OMPD_target_parallel_for:
            directive_kind += "target_parallel_for ";
            break;
        case OMPD_target_parallel_for_simd:
            directive_kind += "target_parallel_for_simd ";
            break;
        case OMPD_target_parallel_loop:
            directive_kind += "target_parallel_loop ";
            break;
        case OMPD_target_simd:
            directive_kind += "target_simd ";
            break;
        case OMPD_target_teams:
            directive_kind += "target_teams ";
            break;
        case OMPD_target_teams_distribute:
            directive_kind += "target_teams_distribute ";
            break;
        case OMPD_target_teams_distribute_simd:
            directive_kind += "target_teams_distribute_simd ";
            break;
        case OMPD_target_teams_loop:
            directive_kind += "target_teams_loop ";
            break;
        case OMPD_target_teams_distribute_parallel_for:
            directive_kind += "target_teams_distribute_parallel_for ";
            break;
        case OMPD_target_teams_distribute_parallel_for_simd:
            directive_kind += "target_teams_distribute_parallel_for_simd ";
            break;
        case OMPD_teams_distribute_parallel_do:
            directive_kind += "teams distribute parallel do ";
            break;
        case OMPD_teams_distribute_parallel_do_simd:
            directive_kind += "teams distribute parallel do simd ";
            break;
        case OMPD_target_parallel_do:
            directive_kind += "target parallel do ";
            break;
        case OMPD_target_parallel_do_simd:
            directive_kind += "target parallel do simd ";
            break;
        case OMPD_target_teams_distribute_parallel_do:
            directive_kind += "target teams distribute parallel do ";
            break;
        case OMPD_target_teams_distribute_parallel_do_simd:
            directive_kind += "target teams distribute parallel do simd ";
            break;
        default:
            directive_kind = this->toString().substr(0, this->toString().size()-1);
    }
    std::string current_line;
    std::string indent = std::string(depth, '\t');
    std::string directive_id = parent_node + "_" + directive_kind + "_" + std::to_string(index);
    current_line = indent + parent_node + " -- " + directive_id + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    dot_file << indent << directive_id.c_str() << " [label = \"" + directive_kind + "\"]\n";

    // output score
    if (trait_score != "") {
        current_line = indent + directive_id + " -- " + directive_id + "_score" + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + directive_id + "_score" + " [label = \"score\\n " + trait_score + "\"]\n";
        dot_file << current_line.c_str();
    }; 

    switch (kind) {
        case OMPD_allocate: {
            std::vector<const char *> *list = ((OpenMPAllocateDirective *) this)->getAllocateList();
            std::vector<const char *>::iterator list_item;
            int list_index = 0;
            std::string list_name;
            std::string expr_name;
            std::string tkind = "allocate";
            list_name = tkind + "_directive_list_" + std::to_string(list_index);
            current_line = indent + tkind + " -- " + list_name + "\n";
            dot_file << current_line.c_str();
            for (list_item = list->begin(); list_item != list->end(); list_item++) {
                expr_name = list_name + "_expr" + std::to_string(list_index);
                list_index += 1;
                current_line = indent + indent + list_name + " -- " + expr_name + "\n";
                dot_file << current_line.c_str();
                current_line =
                        indent + indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*list_item) +
                        "\"]\n";
                dot_file << current_line.c_str();
            }
            break;
        }
        case OMPD_threadprivate: {
            std::vector<const char *> *list = ((OpenMPThreadprivateDirective*)this)->getThreadprivateList();
            std::vector<const char *>::iterator list_item;
            int list_index = 0;
            std::string list_name;
            std::string expr_name;
            std::string tkind = "threadprivate";
            list_name = tkind + "_directive_list_" + std::to_string(list_index);
            current_line = indent + tkind + " -- " + list_name + "\n";
            dot_file << current_line.c_str();
            for (list_item = list->begin(); list_item != list->end(); list_item++) {
                expr_name = list_name + "_expr" + std::to_string(list_index);
                list_index += 1;
                current_line = indent + indent + list_name + " -- " + expr_name + "\n";
                dot_file << current_line.c_str();
                current_line =
                        indent + indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*list_item) +
                        "\"]\n";
                dot_file << current_line.c_str();
            }
            break;
        }
        case OMPD_declare_reduction: {
            std::vector<const char*>* list = ((OpenMPDeclareReductionDirective*)this)->getTypenameList();
            std::vector<const char*>::iterator list_item;
            std::string id = ((OpenMPDeclareReductionDirective*)this)->getIdentifier();
            std::string combiner = ((OpenMPDeclareReductionDirective*)this)->getCombiner();
            int list_index = 0;
            std::string list_name;
            std::string expr_name;
            std::string tkind = "declare_reduction";
            std::string node_id = tkind + "_reduction_identifier";
            current_line = indent + tkind + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + id + "\"]\n";
            dot_file << current_line.c_str();
            list_name = tkind + "_typename_list_" + std::to_string(list_index);
            current_line = indent + tkind + " -- " + list_name + "\n";
            dot_file << current_line.c_str();
            for (list_item = list->begin(); list_item != list->end(); list_item++) {
                expr_name = list_name + "_expr" + std::to_string(list_index);
                list_index += 1;
                current_line = indent + indent + list_name + " -- " + expr_name + "\n";
                dot_file << current_line.c_str();
                current_line =
                        indent + indent + "\t" + expr_name + " [label = \"" + expr_name + "\\n " + std::string(*list_item) +
                        "\"]\n";
                dot_file << current_line.c_str();
            }
            node_id = tkind + "_combiner";
            current_line = indent + tkind + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + combiner + "\"]\n";
            dot_file << current_line.c_str();
            break;
        }
        case OMPD_declare_mapper: {
            std::string indent = std::string(1, '\t');
            OpenMPDeclareMapperDirectiveIdentifier id = ((OpenMPDeclareMapperDirective*)this)->getIdentifier();
            std::string type_var = ((OpenMPDeclareMapperDirective*)this)->getTypeVar();
            int list_index = 0;
            std::string tkind = "declare_mapper";
            std::string node_id = tkind + "_mapper_identifier";
            std::string parameter_string;
            switch (id) {
                case OMPD_DECLARE_MAPPER_IDENTIFIER_default:
                {
                    parameter_string = "default";
                    break;
                }
                case OMPD_DECLARE_MAPPER_IDENTIFIER_user: {
                    parameter_string = ((OpenMPDeclareMapperDirective*)this)->getUserDefinedIdentifier();
                    break;
                }
                default:
                    ;
            };
            current_line = indent + tkind + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + parameter_string + "\"]\n";
            dot_file << current_line.c_str();
            node_id = tkind + "_type_var";
            current_line = indent + tkind + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"" + node_id + "\\n " + type_var + "\"]\n";
            dot_file << current_line.c_str();
            break;
        }
        default: {
            ;
        }
    };

    std::vector<OpenMPClause*>* clauses = this->getClausesInOriginalOrder();
    if (clauses->size() != 0) {
        std::vector<OpenMPClause*>::iterator iter;
        int clause_index = 0;
        for (iter = clauses->begin(); iter != clauses->end(); iter++) {
            (*iter)->generateDOT(dot_file, depth+1, clause_index, directive_id);
            clause_index += 1;
        };
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
        case OMPC_initializer:
            clause_kind += "initializer";
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
        case OMPC_do:
            clause_kind += "do";
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
        case OMPC_acq_rel:
            clause_kind += "acq_rel";
            break;
        case OMPC_release:
            clause_kind += "release";
            break;
        case OMPC_acquire:
            clause_kind += "acquire";
            break;
        case OMPC_read:
            clause_kind += "read";
            break;
        case OMPC_write:
            clause_kind += "write";
            break;
        case OMPC_update:
            clause_kind += "update";
            break;
        case OMPC_capture:
            clause_kind += "capture";
            break;
        case OMPC_seq_cst:
            clause_kind += "seq_cst";
            break;
        case OMPC_relaxed:
            clause_kind += "relaxed";
            break;
        case OMPC_hint:
            clause_kind += "hint";
            break;
        case OMPC_destroy:
            clause_kind += "destroy";
            break;
        case OMPC_threads:
            clause_kind += "threads";
            break;
        case OMPC_simd:
            clause_kind += "simd";
            break;
        case OMPC_uses_allocators:
            clause_kind += "uses_allocators";
            break;
        default:
            printf("The clause enum is not supported yet.\n");
    }
    //parent_node = parent_node.substr(0, parent_node.size()-1);
    std::string clause_label = clause_kind;
    clause_kind = parent_node + "_" + clause_kind + "_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + " -- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    current_line = indent + clause_kind + " [label = \"" + clause_label + + "\"]\n";
    dot_file << current_line.c_str();

    if (this->getKind() == OMPC_default) {
        OpenMPDirective* variant_directive = ((OpenMPDefaultClause*)this)->getVariantDirective();
        if (variant_directive != NULL) {
            variant_directive->generateDOT(dot_file, depth+1, 0, clause_kind, "");
        };
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
            current_line = indent + "\t" + expr_name + " [label = \"" + "expr" + "\\n " + std::string(*it) + "\"]\n";
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

void OpenMPVariantClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string parameter_string;
    std::pair<std::string, std::string>* parameter_pair_string;
    std::vector<std::pair<std::string, OpenMPDirective*> >* parameter_pair_directives;
    OpenMPDirective* variant_directive = NULL;

/*

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



    result += clause_string + ") ";

    //return result;*/
    std::string current_line;
    std::string indent = std::string(depth, '\t');
    std::string clause_string;
    std::string clause_type;
    std::vector<OpenMPDirective*>* parameter_directives;
    parent_node = parent_node.substr(0, parent_node.size()-1);
    OpenMPClauseKind clause_kind = this->getKind();
    switch (clause_kind) {
        case OMPC_when:
            clause_type = "when";
            break;
        case OMPC_match:
            clause_type = "match";
            break;
        default:
            std::cout << "The variant clause is not supported.\n";
    };
    clause_string = parent_node + "_" + clause_type + "_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + " -- " + clause_string + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    current_line = indent + clause_string + " [label = \"" + clause_type + "\"]\n";
    dot_file << current_line.c_str();

    std::string node_id = "";
    // check user
    parameter_pair_string = this->getUserCondition();
    if (parameter_pair_string->second.size() > 0) {
        node_id = clause_string + "_user_condition";
        current_line = indent + clause_string + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"user_condition\"]\n";
        dot_file << current_line.c_str();
        // output score
        if (parameter_pair_string->first.size() > 0) {
            current_line = indent + "\t" + node_id + " -- " + node_id + "_score\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t\t" + node_id + "_score [label = \"score\\n " + parameter_pair_string->first + "\"]\n";
            dot_file << current_line.c_str();
        };
        // output condition expression
        current_line = indent + "\t" + node_id + " -- " + node_id + "_expr\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t" + node_id + "_expr [label = \"expr\\n " + parameter_pair_string->second + "\"]\n";
        dot_file << current_line.c_str();
    };

    // check construct
    parameter_pair_directives = this->getConstructDirective();
    if (parameter_pair_directives->size() != 0) {
        node_id = clause_string + "_construct";
        current_line = indent + clause_string + " -- " + node_id + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + node_id + " [label = \"construct\"]\n";
        dot_file << current_line.c_str();
        for (int i = 0; i < parameter_pair_directives->size(); i++) {
            parameter_pair_directives->at(i).second->generateDOT(dot_file, depth+2, i, node_id, parameter_pair_directives->at(i).first);
        };
    };

    // check device_arch
    bool has_device = false;
    parameter_pair_string = this->getArchExpression();
    if (parameter_pair_string->second.size() > 0) {
        if (!has_device) {
            node_id = clause_string + "_device";
            current_line = indent + clause_string + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"device\"]\n";
            dot_file << current_line.c_str();
            has_device = true;
        };
        current_line = indent + "\t" + node_id + " -- " + node_id + "_arch\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t" + node_id + "_arch [label = \"arch\"]\n";
        dot_file << current_line.c_str();
        // output score
        if (parameter_pair_string->first.size() > 0 ) {
            current_line = indent + "\t\t" + node_id + "_arch -- " + node_id + "_arch_score\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t\t\t" + node_id + "_arch_score [label = \"score\\n " + parameter_pair_string->first + "\"]\n";
            dot_file << current_line.c_str();
        };
        // output arch expression
        current_line = indent + "\t\t" + node_id + "_arch -- " + node_id + "_arch_expr\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t\t" + node_id + "_arch_expr [label = \"expr\\n " + parameter_pair_string->second + "\"]\n";
        dot_file << current_line.c_str();
    };

    // check device_isa
    parameter_pair_string = this->getIsaExpression();
    if (parameter_pair_string->second != "") {
        if (!has_device) {
            node_id = clause_string + "_device";
            current_line = indent + clause_string + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"device\"]\n";
            dot_file << current_line.c_str();
            has_device = true;
        };
        current_line = indent + "\t" + node_id + " -- " + node_id + "_isa\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t" + node_id + "_isa [label = \"isa\"]\n";
        dot_file << current_line.c_str();
        // output score
        if (parameter_pair_string->first.size() > 0) {
            current_line = indent + "\t\t" + node_id + "_isa -- " + node_id + "_isa_score\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t\t\t" + node_id + "_isa_score [label = \"score\\n " + parameter_pair_string->first + "\"]\n";
            dot_file << current_line.c_str();
        };
        // output isa expression
        current_line = indent + "\t\t" + node_id + "_isa -- " + node_id + "_isa_expr\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t\t" + node_id + "_isa_expr [label = \"expr\\n " + parameter_pair_string->second + "\"]\n";
        dot_file << current_line.c_str();
    };

    // check device_kind
    parameter_string.clear();
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

    if (parameter_string.size() > 0) {
        if (!has_device) {
            node_id = clause_string + "_device";
            current_line = indent + clause_string + " -- " + node_id + "\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t" + node_id + " [label = \"device\"]\n";
            dot_file << current_line.c_str();
            has_device = true;
        };
        current_line = indent + "\t" + node_id + " -- " + node_id + "_kind\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t" + node_id + "_kind [label = \"kind\"]\n";
        dot_file << current_line.c_str();
        // output score
        if (context_kind->first.size() > 0) {
            current_line = indent + "\t\t" + node_id + "_kind -- " + node_id + "_kind_score\n";
            dot_file << current_line.c_str();
            current_line = indent + "\t\t\t" + node_id + "_kind_score [label = \"score\\n " + context_kind->first + "\"]\n";
            dot_file << current_line.c_str();
        };
        // output kind value
        current_line = indent + "\t\t" + node_id + "_kind -- " + node_id + "_kind_value\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t\t\t" + node_id + "_kind_value [label = \"" + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
    };

    if (clause_kind == OMPC_when) {
        variant_directive = ((OpenMPWhenClause*)this)->getVariantDirective();
        if (variant_directive != NULL) {
            variant_directive->generateDOT(dot_file, depth+1, 0, clause_string, "");
        };
    };
};

void OpenMPDefaultClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string parameter_string;
    OpenMPDirective* variant_directive = NULL;
    std::string current_line;
    std::string indent = std::string(depth, '\t');
    std::string clause_string;
    std::vector<OpenMPDirective*>* parameter_directives;
    parent_node = parent_node.substr(0, parent_node.size()-1);
    OpenMPClauseKind clause_kind = this->getKind();

    clause_string = "default";

    clause_string = parent_node + "_" + clause_string + "_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + " -- " + clause_string + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    variant_directive = ((OpenMPDefaultClause*)this)->getVariantDirective();
    if (variant_directive != NULL) {
        variant_directive->generateDOT(dot_file, depth+1, 0, clause_string, "");
    };
};

void OpenMPOrderClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string parameter_string;
    OpenMPDirective* variant_directive = NULL;
    std::string current_line;
    std::string indent = std::string(depth, '\t');
    std::string clause_string;
    std::vector<OpenMPDirective*>* parameter_directives;
    parent_node = parent_node.substr(0, parent_node.size()-1);
    OpenMPClauseKind clause_kind = this->getKind();

    clause_string = "order";

    clause_string = parent_node + "_" + clause_string + "_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + " -- " + clause_string + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
};

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

void OpenMPAlignedClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "aligned_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    std::string parameter_string;

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
    std:string alignment = this->getUserDefinedAlignment();
    if (alignment != "") {
        parameter_string = alignment;
        std::string alignment_name = clause_kind + "_aligned_alignment";
        current_line = indent + clause_kind + " -- " + alignment_name + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + alignment_name + " [label = \"" + alignment_name + "\\n " + parameter_string + "\"]\n";
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

void OpenMPDistScheduleClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "dist_schedule_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    OpenMPDistScheduleClauseKind kind = this->getKind();
    std::string parameter_string;
    switch (kind) {
        case OMPC_DIST_SCHEDULE_KIND_static:
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

    std:string chunk_size = this->getChunkSize();
    if (chunk_size != "") {
        parameter_string = chunk_size;
        std::string chunk_size_name = clause_kind + "_chunk_size";
        current_line = indent + clause_kind + " -- " + chunk_size_name + "\n";
        dot_file << current_line.c_str();
        current_line = indent + "\t" + chunk_size_name + " [label = \"" + chunk_size_name + "\\n " + parameter_string + "\"]\n";
        dot_file << current_line.c_str();
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

void OpenMPInitializerClause::generateDOT(std::ofstream& dot_file, int depth, int index, std::string parent_node) {

    std::string current_line;
    std::string indent = std::string(depth, '\t');

    std::string clause_kind = "initializer_" + std::to_string(depth) + "_" + std::to_string(index);
    current_line = indent + parent_node + "-- " + clause_kind + "\n";
    dot_file << current_line.c_str();
    indent += "\t";
    std::string priv = this->getUserDefinedPriv();
    std::string parameter_string;
    parameter_string += "omp_priv = " + priv;
    if (parameter_string.size() > 0) {
        std::string node_id = clause_kind + "_priv_list";
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
