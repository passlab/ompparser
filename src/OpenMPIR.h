#ifndef OMPPARSER_OPENMPAST_H
#define OMPPARSER_OPENMPAST_H

#include <fstream>
#include <iostream>

#include "OpenMPKinds.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include <cassert>

using namespace std;

class SourceLocation {
    int line;
    int column;
    int getLine ( ) { return line; };
    int getColumn ( ) { return column; };

    protected:
    SourceLocation(int _line = 0, int _col = 0) : line(_line), column(_col) { } ;
};

/**
 * The baseclass for all the clause classes.
 */
class OpenMPClause : public SourceLocation {
protected:
    OpenMPClauseKind kind;

    /* consider this is a struct of array, i.e.
     * the expression/localtionLine/locationColumn are the same index are one record for an expression and its location
     */
    std::vector<const char *> expressions;
    std::vector<SourceLocation> locations;

public:
    OpenMPClause(OpenMPClauseKind k, int _line = 0, int _col = 0) : SourceLocation(_line, _col), kind(k) {};

    OpenMPClauseKind getKind() { return kind; };

    // a list of expressions or variables that are language-specific for the clause, ompparser does not parse them,
    // instead, it only stores them as strings
    void addLangExpr(const char *expression, int line = 0, int col = 0) {
    //TODO: Here we need to do certain normlization, if an expression already exists, we ignore
	expressions.push_back(expression);
    //locations.push_back(SourceLocation(line, col));
    };

    std::vector<const char *> &getExpressions() { return expressions; };

    std::string toString();
};

/**
 * The class for all the OpenMP directives
 */
class OpenMPDirective : public SourceLocation  {
protected:
    OpenMPDirectiveKind kind;

    /* the map to store clauses of the directive, for each clause, we store a vector of OpenMPClause objects
     * since there could be multiple clause objects for those clauses that take parameters, e.g. reduction clause
     *
     * for those clauses just take no parameters, but may take some variables or expressions, we only need to have
     * one OpenMPClause object, e.g. shared, private.
     *
     * The design and use of this map should make sure that for any clause, we should only have one OpenMPClause object
     * for each instance of kind and full parameters
     */
    map<OpenMPClauseKind, vector<OpenMPClause *> *> clauses;

    /**
     *
     * This method searches the clauses map to see whether one or more OpenMPClause objects of the specified kind
     * parameters exist in the directive, if so it returns the objects that match.
     * @param kind clause kind
     * @param ... clause parameters
     * @return
     */
    std::vector<OpenMPClause *> searchOpenMPClause(OpenMPClauseKind kind, ...);

    /**
     * Search and add a clause of kind and parameters specified by the variadic parameters.
     * This should be the only call used to add an OpenMPClause object.
     *
     * The method may simply create an OpenMPClause-subclassed object and return it. In this way, normalization will be
     * needed later on.
     *
     * Or the method may do the normalization while adding a clause.
     * it first searches the clauses map to see whether an OpenMPClause object
     * of the specified kind and parameters exists in the map. If so, it only return
     * that OpenMPClause object, otherwise, it should create a new OpenMPClause object and insert in the map
     *
     * NOTE: if only partial parameters are provided as keys to search for a clause, the function will only
     * return the first one that matches. Thus, the method should NOT be called with partial parameters of a specific clause
     * @param kind
     * @param ...
     * @return
     */
    //OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, ...);

    /**
     * normalize all the clause of a specific kind
     * @param kind
     * @return
     */
    void * normalizeClause(OpenMPClauseKind kind);

public:
    OpenMPDirective(OpenMPDirectiveKind k , int _line = 0, int _col = 0) : SourceLocation(_line, _col), kind(k) {};

    OpenMPDirectiveKind getKind() { return kind; };

    map<OpenMPClauseKind, std::vector<OpenMPClause *> *> &getClauses();

    std::vector<OpenMPClause *> &getClauses(OpenMPClauseKind kind);

    std::string toString();

    /* generate DOT representation of the directive */
    void generateDOT();

    // To call this method directly to add new clause, it can't be protected.
    OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, ...);
};

// reduction clause
class OpenMPReductionClause : public OpenMPClause {

protected:
    OpenMPReductionClauseModifier modifier;     // modifier
    OpenMPReductionClauseIdentifier identifier; // identifier
    char *userDefinedIdentifier;                // user defined identifier if it is used

public:
    OpenMPReductionClause(OpenMPClauseKind k) : OpenMPClause(k) { }

    OpenMPReductionClause(OpenMPClauseKind k, OpenMPReductionClauseModifier _modifier,
                          OpenMPReductionClauseIdentifier _identifier) : OpenMPClause(k),
                                         modifier(_modifier), identifier(_identifier) { };

    OpenMPReductionClauseModifier getModifier() { return modifier; };

    OpenMPReductionClauseIdentifier getIdentifier() { return identifier; };

    void setUserDefinedIdentifier(char *identifier) { userDefinedIdentifier = identifier; };

    char *getUserDefinedIdentifier() { return userDefinedIdentifier; };
};

// in_reduction clause
class OpenMPInReductionClause : public OpenMPClause {
protected:
    OpenMPReductionClauseIdentifier identifier; // identifier
    char *userDefinedIdentifier;                         /* user defined identifier if it is used */
public:
    OpenMPInReductionClause(OpenMPClauseKind k) : OpenMPClause(k) {}
    OpenMPInReductionClause(OpenMPClauseKind k,
                            OpenMPReductionClauseIdentifier _identifier) : OpenMPClause(k), identifier(_identifier) { };

    OpenMPReductionClauseIdentifier getIdentifier() { return identifier; };

    void setUserDefinedIdentifier(char *identifier) { userDefinedIdentifier = identifier; }

    char *getUserDefinedIdentifier() { return userDefinedIdentifier; };
};

// allocate
class OpenMPAllocateClause : public OpenMPClause {
protected:
    OpenMPAllocateClauseAllocator allocator; // Allocate allocator
    char *userDefinedAllocator;                         /* user defined value if it is used */

public:
    OpenMPAllocateClause(OpenMPClauseKind k, OpenMPAllocateClauseAllocator _allocator) :
            OpenMPClause(k), allocator(_allocator) { };

    OpenMPAllocateClauseAllocator getAllocator() { return allocator; };

    void setUserDefinedAllocator(char *_allocator) { userDefinedAllocator = _allocator; }

    char *getUserDefinedAllocator() { return userDefinedAllocator; };
};


// ProcBind Clause
class OpenMPProcBindClause : public OpenMPClause {

protected:
    OpenMPProcBindClauseKind proc_bindKind; // proc_bind

public:
    OpenMPProcBindClause(OpenMPClauseKind k, OpenMPProcBindClauseKind pbkind) :
            OpenMPClause(k), proc_bindKind(pbkind) { };

    OpenMPProcBindClauseKind getProcBindClauseKind() { return proc_bindKind; };
    //void addProcBindClauseKind(OpenMPProcBindClauseKind v);
};

// Default Clause
class OpenMPDefaultClause : public OpenMPClause {

protected:
    OpenMPDefaultClauseKind defaultKind; // default

public:
    OpenMPDefaultClause(OpenMPClauseKind k, OpenMPDefaultClauseKind _defaultKind) :
            OpenMPClause(k), defaultKind(_defaultKind) { };

    OpenMPDefaultClauseKind getDefaultClauseKind() {return defaultKind; };
};

// if Clause
class OpenMPIfClause : public OpenMPClause {

protected:
    OpenMPIfClauseKind ifKind; // if

public:
    OpenMPIfClause(OpenMPClauseKind k, OpenMPIfClauseKind _ifKind) :
            OpenMPClause(k), ifKind(_ifKind) { };

    OpenMPIfClauseKind getIfClauseKind() { return ifKind; };
    void setIfClauseKind(OpenMPIfClauseKind ifKind) { this->ifKind = ifKind; };
};

// Copyin Clause
class OpenMPCopyinClause : public OpenMPClause {

protected:
public:
    OpenMPCopyinClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Private Clause
class OpenMPPrivateClause : public OpenMPClause {

protected:
public:
    OpenMPPrivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// FirstPrivate Clause
class OpenMPFirstprivateClause : public OpenMPClause {

protected:
public:
    OpenMPFirstprivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Shared Clause
class OpenMPSharedClause : public OpenMPClause {

protected:
public:
    OpenMPSharedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Numthreads Clause
class OpenMPNumthreadsClause : public OpenMPClause {

protected:
public:
    OpenMPNumthreadsClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

#endif //OMPPARSER_OPENMPAST_H
