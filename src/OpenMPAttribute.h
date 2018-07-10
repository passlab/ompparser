//
// Created by Yonghong Yan on 6/15/18.
//

#ifndef OMPPARSER_OPENMPATTRIBUTE_H_H
#define OMPPARSER_OPENMPATTRIBUTE_H_H

#include <OpenMPKinds.h>
#include <vector>
#include <string>

class OpenMPClause {
    OpenMPClauseKind kind;

    OpenMPClause(OpenMPClauseKind k) : kind(k) {};

    OpenMPClauseKind const getClauseKind() { return kind; }

    /* a list of language expressions, variables, etc that are not parsed by the ompparser */
    std::vector<const char*> lang_expr;

    void addLangExpr(const char * expr) { lang_expr.push_back(expr); }
    
    OpenMPClauseKind getKind() { return kind; }
};

class OpenMPDirective {
    OpenMPDirectiveKind kind;

    std::vector<OpenMPClause*> clauses;

    OpenMPDirective(OpenMPDirectiveKind k) : kind(k) {};

    OpenMPDirectiveKind const getDirectiveKind() { return kind; }

    void addClause(OpenMPClause * clause) { clauses.push_back(clause);}

    OpenMPDirectiveKind getKind() { return kind; }

    /* generate DOT representation of the directive */
    void generateDOT(const std::string * filename);
};

#endif //OMPPARSER_OPENMPATTRIBUTE_H_H
