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

enum OpenMPBaseLang {
    Lang_C,
    Lang_Cplusplus,
    Lang_Fortran,
};

class SourceLocation {
    int line;
    int column;
    int getLine ( ) { return line; };
    int getColumn ( ) { return column; };

    SourceLocation* parent_construct;

    public:
    SourceLocation(int _line = 0, int _col = 0, SourceLocation* _parent_construct = NULL) : line(_line), column(_col), parent_construct(_parent_construct) { } ;
    void setParentConstruct(SourceLocation* _parent_construct) { parent_construct = _parent_construct; };
    SourceLocation* getParentConstruct() { return parent_construct; };

};

/**
 * The class or baseclass for all the clause classes. For all the clauses that only take 0 to multiple expression or
 * variables, we use this class to create objects. For all other clauses, which requires at least one parameters,
 * we will have an inherit class from this one, and the superclass contains fields for the parameters
 */
class OpenMPClause : public SourceLocation {
protected:
    OpenMPClauseKind kind;

    /* consider this is a struct of array, i.e.
     * the expression/localtionLine/locationColumn are the same index are one record for an expression and its location
     */
    std::vector<const char *> expressions;
    std::vector<const void *> expressionNodes;

    std::vector<SourceLocation> locations;

public:
    OpenMPClause(OpenMPClauseKind k, int _line = 0, int _col = 0) : SourceLocation(_line, _col), kind(k) {};

    OpenMPClauseKind getKind() { return kind; };

    // a list of expressions or variables that are language-specific for the clause, ompparser does not parse them,
    // instead, it only stores them as strings
    void addLangExpr(const char *expression, int line = 0, int col = 0) {
        //TODO: Here we need to do certain normlization, if an expression already exists, we ignore
	    expressions.push_back(expression);
        locations.push_back(SourceLocation(line, col));
    };
    
std::vector<const char *>* getExpressions() { return &expressions; };

    virtual std::string toString();
    std::string expressionToString();
    virtual void generateDOT(std::ofstream&, int, int, std::string);
/*
    std::vector<const char *> &getExpressions() { return expressions; };

    std::string toString();*/
};

/**
 * The class for all the OpenMP directives
 */
class OpenMPDirective : public SourceLocation  {
protected:
    OpenMPDirectiveKind kind;
    OpenMPBaseLang lang;

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
     * @param parameters clause parameters
     * @return
     */
    std::vector<OpenMPClause *> searchOpenMPClause(OpenMPClauseKind kind, int num, int * parameters);

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
     * @param parameters clause parameters, number of parameters should be determined by the kind
     * @return
     */
    OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, int * parameters);

    /**
     * normalize all the clause of a specific kind
     * @param kind
     * @return
     */
    void * normalizeClause(OpenMPClauseKind kind);

public:
    OpenMPDirective(OpenMPDirectiveKind k, OpenMPBaseLang _lang = Lang_C, int _line = 0, int _col = 0) :
            SourceLocation(_line, _col), kind(k), lang(_lang) {};

    OpenMPDirectiveKind getKind() { return kind; };

    map<OpenMPClauseKind, std::vector<OpenMPClause *> *> *getAllClauses() { return &clauses; };

    std::vector<OpenMPClause *> *getClauses(OpenMPClauseKind kind) { return clauses[kind]; };

    std::string toString();

    /* generate DOT representation of the directive */
    void generateDOT(std::ofstream&, int, int, std::string);
    void generateDOT();
    std::string generatePragmaString();
    // To call this method directly to add new clause, it can't be protected.
    OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, ...);
};

// reduction clause
class OpenMPReductionClause : public OpenMPClause {

protected:
    OpenMPReductionClauseModifier modifier;     // modifier
    OpenMPReductionClauseIdentifier identifier; // identifier
    std::string user_defined_identifier;                // user defined identifier if it is used

public:
    OpenMPReductionClause( ) : OpenMPClause(OMPC_reduction) { }

    OpenMPReductionClause(OpenMPReductionClauseModifier _modifier,
                          OpenMPReductionClauseIdentifier _identifier) : OpenMPClause(OMPC_reduction),
                                         modifier(_modifier), identifier(_identifier), user_defined_identifier ("") { };

    OpenMPReductionClauseModifier getModifier() { return modifier; };

    OpenMPReductionClauseIdentifier getIdentifier() { return identifier; };

    void setUserDefinedIdentifier(char *identifier) { user_defined_identifier = std::string(identifier); };

    std::string getUserDefinedIdentifier() { return user_defined_identifier; };
	
    static OpenMPReductionClause * addReductionClause(OpenMPDirective *directive,  OpenMPReductionClauseModifier modifier, 
			      OpenMPReductionClauseIdentifier identifier, char * user_defined_identifier=NULL);

    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// allocate
class OpenMPAllocateClause : public OpenMPClause {
protected:
    OpenMPAllocateClauseAllocator allocator; // Allocate allocator
    std::string user_defined_allocator;                         /* user defined value if it is used */

public:
    OpenMPAllocateClause(OpenMPAllocateClauseAllocator _allocator) :
            OpenMPClause(OMPC_allocate), allocator(_allocator), user_defined_allocator ("") { };

    OpenMPAllocateClauseAllocator getAllocator() { return allocator; };

    void setUserDefinedAllocator(char *_allocator) { user_defined_allocator = std::string(_allocator); }

    std::string getUserDefinedAllocator() { return user_defined_allocator; };
	
    static OpenMPAllocateClause * addAllocateClause(OpenMPDirective *directive, OpenMPAllocateClauseAllocator allocator);
};


// lastprivate Clause
class OpenMPLastprivateClause : public OpenMPClause {
protected:
    OpenMPLastprivateClauseModifier modifier; // lastprivate modifier
    char *user_defined_modifier;                         /* user defined value if it is used */

public:
    OpenMPLastprivateClause(OpenMPLastprivateClauseModifier _modifier) :
            OpenMPClause(OMPC_lastprivate), modifier(_modifier), user_defined_modifier (NULL) { };

    OpenMPLastprivateClauseModifier getModifier() { return modifier; };

    void setUserDefinedModifier(char *_modifier) { user_defined_modifier = _modifier; }

    char *getUserDefinedModifier() { return user_defined_modifier; };

    static OpenMPLastprivateClause* addLastprivateClause(OpenMPDirective *directive, OpenMPLastprivateClauseModifier modifier);

};

// linear Clause
class OpenMPLinearClause : public OpenMPClause {
protected:
    OpenMPLinearClauseModifier modifier; // linear modifier
    char *user_defined_modifier;                         /* user defined value if it is used */

public:
    OpenMPLinearClause(OpenMPLinearClauseModifier _modifier) :
            OpenMPClause(OMPC_linear), modifier(_modifier), user_defined_modifier (NULL) { };

    OpenMPLinearClauseModifier getModifier() { return modifier; };

    void setUserDefinedModifier(char *_modifier) { user_defined_modifier = _modifier; }

    char *getUserDefinedModifier() { return user_defined_modifier; };
};

// dist_schedule Clause
class OpenMPDistscheduleClause : public OpenMPClause {
	
protected:
    OpenMPDistscheduleClauseKind dist_schedule_kind;     // kind
    char *user_defined_kind;                // user defined identifier if it is used

public:
    OpenMPDistscheduleClause( ) : OpenMPClause(OMPC_dist_schedule) { }

    OpenMPDistscheduleClause(OpenMPDistscheduleClauseKind _dist_schedule_kind) : OpenMPClause(OMPC_dist_schedule), dist_schedule_kind(_dist_schedule_kind), user_defined_kind (NULL) { };

    OpenMPDistscheduleClauseKind getKind() { return dist_schedule_kind; };

    void setUserDefinedKind(char *dist_schedule_kind) { user_defined_kind = dist_schedule_kind; };

    char *getUserDefinedKind() { return user_defined_kind; };
};


// schedule Clause
class OpenMPScheduleClause : public OpenMPClause {

protected:
    OpenMPScheduleClauseModifier modifier;     // modifier
    OpenMPScheduleClauseKind schedulekind; // identifier
    char *user_defined_kind;                // user defined identifier if it is used

public:
    OpenMPScheduleClause( ) : OpenMPClause(OMPC_schedule) { }

    OpenMPScheduleClause(OpenMPScheduleClauseModifier _modifier,
                          OpenMPScheduleClauseKind _schedulekind) : OpenMPClause(OMPC_schedule),
                                         modifier(_modifier), schedulekind(_schedulekind), user_defined_kind (NULL) { };

    OpenMPScheduleClauseModifier getModifier() { return modifier; };

    OpenMPScheduleClauseKind getKind() { return schedulekind; };

    void setUserDefinedKind(char *schedulekind) { user_defined_kind = schedulekind; };

    char *getUserDefinedKind() { return user_defined_kind; };
};

// When Clause
class OpenMPWhenClause : public OpenMPClause {
protected:
    OpenMPDirective* sub_directive; // sub directive inside the WHEN clause
    std::string context_selector; // the context selector inside the WHEN clause

public:
    OpenMPWhenClause( ) : OpenMPClause(OMPC_when) { };

    OpenMPWhenClause(OpenMPDirective* _sub_directive) :
            OpenMPClause(OMPC_when), sub_directive(_sub_directive), context_selector("") { };

    OpenMPDirective* getSubDirective() { return sub_directive; };
    void setSubDirective(OpenMPDirective* _sub_directive) { sub_directive = _sub_directive; };
    void setContextSelector(const char* _context_selector) { context_selector = std::string(_context_selector); }

    std::string getContextSelector() { return context_selector; };
};

// ProcBind Clause
class OpenMPProcBindClause : public OpenMPClause {

protected:
    OpenMPProcBindClauseKind proc_bind_kind; // proc_bind

public:
    OpenMPProcBindClause(OpenMPProcBindClauseKind _proc_bind_kind) :
            OpenMPClause(OMPC_proc_bind), proc_bind_kind(_proc_bind_kind) { };

    OpenMPProcBindClauseKind getProcBindClauseKind() { return proc_bind_kind; };
    //void addProcBindClauseKind(OpenMPProcBindClauseKind v);
};

// Bind Clause
class OpenMPBindClause : public OpenMPClause {

protected:
    OpenMPBindClauseKind bind_kind; // proc_bind

public:
    OpenMPBindClause(OpenMPBindClauseKind _bind_kind) :
            OpenMPClause(OMPC_bind), bind_kind(_bind_kind) { };

    OpenMPBindClauseKind getBindClauseKind() { return bind_kind; };
    //void addProcBindClauseKind(OpenMPProcBindClauseKind v);
};

// Default Clause
class OpenMPDefaultClause : public OpenMPClause {

protected:
    OpenMPDefaultClauseKind default_kind; // default

public:
    OpenMPDefaultClause(OpenMPDefaultClauseKind _default_kind) :
            OpenMPClause(OMPC_default), default_kind(_default_kind) { };

    OpenMPDefaultClauseKind getDefaultClauseKind() {return default_kind; };
};

// if Clause
class OpenMPIfClause : public OpenMPClause {

protected:
    OpenMPIfClauseKind if_kind; // if

public:
    OpenMPIfClause(OpenMPIfClauseKind _if_kind) :
            OpenMPClause(OMPC_if), if_kind(_if_kind) { };

    OpenMPIfClauseKind getIfClauseKind() { return if_kind; };
    void setIfClauseKind(OpenMPIfClauseKind if_kind) { this->if_kind = if_kind; };
};

#ifdef __cplusplus
extern "C" {
#endif
    extern  OpenMPDirective * parseOpenMP(const char *, void * exprParse(const char * expr));
#ifdef __cplusplus
}
#endif

#endif //OMPPARSER_OPENMPAST_H
