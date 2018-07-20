//
// Created by Yonghong Yan on 6/15/18.
//

#ifndef OMPPARSER_OPENMPATTRIBUTE_H_H
#define OMPPARSER_OPENMPATTRIBUTE_H_H

#include <OpenMPKinds.h>
#include <vector>
#include <string>

class OpenMPClause {
    
    protected:
    OpenMPClauseKind kind;
    std::vector<const char*> lang_expr;
    
    public:
    OpenMPClause(OpenMPClauseKind k) : kind(k) {};
    OpenMPClauseKind const getClauseKind() { return kind; }

    /* a list of language expressions, variables, etc that are not parsed by the ompparser */

    void addLangExpr(const char * expr) { lang_expr.push_back(expr); }
    
    std::vector<const char*>* getExpr() {
       return &lang_expr;
    } 

    OpenMPClauseKind getKind() { return kind; }
};

class OpenMPReductionClause : OpenMPClause {
    // <properType for reduction_modifier> reduction_modifier;
    // <properType for reduction_identifier> reduction_identifier;
    // def file
    
    
};

class OpenMPDirective {

    protected:
    OpenMPDirectiveKind kind;
    std::vector<OpenMPClause*> clauses;

    public:
    OpenMPDirective(OpenMPDirectiveKind k) : kind(k) {};

    OpenMPDirectiveKind const getDirectiveKind() { return kind; }

    void addClause(OpenMPClause * clause) { clauses.push_back(clause);}

    OpenMPDirectiveKind getKind() { return kind; }

    std::vector<OpenMPClause*>* getClauses() {
        return &clauses;
    }

    /* generate DOT representation of the directive */
    void generateDOT(const std::string * filename);
};

#endif //OMPPARSER_OPENMPATTRIBUTE_H_H
