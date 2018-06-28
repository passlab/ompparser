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
};

class OpenMPDirective {
    OpenMPDirectiveKind kind;

    std::vector<OpenMPClause*> clauses;

    OpenMPDirective(OpenMPDirectiveKind k) : kind(k) {};

    OpenMPDirectiveKind const getDirectiveKind() { return kind; }

    void addClause(OpenMPClause * clause) { clauses.push_back(clause);}

    /* generate DOT representation of the directive */
    void generateDOT(const std::string * filename);

};

// standalone omp_parser
class openMPNode {
    
    const char* type;
    const char* id;
    openMPNode* parent;
    const char* val;
    std::vector<openMPNode*> children;
    
    public:
        
        //openMPNode (char*);

        std::vector<openMPNode*>* getChildren() {
            return &children;
        }

        openMPNode* getLast() {
            return children.back();
        }

        const char* getType() {
            return type;
        }

        void setType (const char* nodeType) {
            type = nodeType;
        }

        openMPNode* getParent() {
            return parent;
        }

        void setParent (openMPNode* node) {
            parent = node;
        }

        void addChild (openMPNode* node) {
            //openMPNode* child = new openMPNode(childType);
            //openMPNode* child = new openMPNode;
            //child->setType(childType);
            children.push_back(node);
            node->setParent(this);
            //return child;
        }

        const char* getVal() {
            return val;
        }

        void setVal (const char* value) {
            val = value;
        }
};

/*
openMPNode::openMPNode (char* nodeType) {
    type = nodeType;
};
*/


#endif //OMPPARSER_OPENMPATTRIBUTE_H_H
