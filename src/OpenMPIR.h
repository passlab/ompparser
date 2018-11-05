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
    void addExpression(const char *expression, int line = 0, int col = 0);

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
    OpenMPClause * addOpenMPClause(OpenMPClauseKind kind, ...);

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

    std::string toString();

    /* generate DOT representation of the directive */
    void generateDOT();
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
                                         modifer(_modifer), identifier(_identifier) { };

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
    OpenMPAllocateClauseKind allocator; // Allocate allocator
    char *userDefinedAllocator;                         /* user defined value if it is used */

public:
    OpenMPAllocateClause(OpenMPClauseKind k, OpenMPAllocateClauseKind _allocator) :
            OpenMPClause(k), allocator(_allocator) { };

    OpenMPAllocateClauseKind getAllocator() { return allocator; };

    void setUserDefinedAllocator(char *_allocator) { userDefinedAllocator = _allocator; }

    char *getUserDefinedAllocator() { return userDefinedAllocator; };
};

// Lastprivate Clause
class OpenMPLastprivateClause : public OpenMPClause {

protected:
    OpenMPLastprivateClauseKind lastPrivate; // lastPrivate

public:
    OpenMPLastprivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // 	LASTPRIVATE
    void setLastprivateValue(OpenMPLastprivateClauseKind v);

    OpenMPLastprivateClauseKind getLastprivateClauseValue();
};

// ProcBind Clause
class OpenMPProcBindClause : public OpenMPClause {

protected:
    OpenMPProcBindClauseKind proc_bindKind; // proc_bind

public:
    OpenMPProcBindClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // PROC_BIND
    void setProcBindClauseValue(OpenMPProcBindClauseKind v);

    OpenMPProcBindClauseKind getProcBindClauseValue();
    //void addProcBindClauseKind(OpenMPProcBindClauseKind v);
};

// Default Clause
class OpenMPDefaultClause : public OpenMPClause {

protected:
    OpenMPDefaultClauseKind defaultKind; // default

public:
    OpenMPDefaultClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // DEFAULT
    void setDefaultClauseValue(OpenMPDefaultClauseKind v);

    OpenMPDefaultClauseKind getDefaultClauseValue();
};

// if Clause
class OpenMPIfClause : public OpenMPClause {

protected:
    OpenMPIfClauseKind ifKind; // if

public:
    OpenMPIfClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // 	IF
    void setIfClauseValue(OpenMPIfClauseKind v);

    OpenMPIfClauseKind getIfClauseValue();
};

// Bind Clause
class OpenMPBindClause : public OpenMPClause {

protected:
    OpenMPBindClauseKind bindKind; // bind

public:
    OpenMPBindClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // 	BIND
    void setBindClauseValue(OpenMPBindClauseKind v);

    OpenMPBindClauseKind getBindClauseValue();
};

// Order Clause
class OpenMPOrderClause : public OpenMPClause {

protected:
    OpenMPOrderClauseKind orderKind; // order

public:
    OpenMPOrderClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // ORDER
    void setOrderClauseValue(OpenMPOrderClauseKind v);

    OpenMPOrderClauseKind getOrderClauseValue();
};

// Schedule Clause
class OpenMPScheduleClause : public OpenMPClause {

protected:
    OpenMPScheduleClauseModifier scheduleModifier1; // schedule 1st modifier e.g. in case of 'for' directive
    OpenMPScheduleClauseModifier scheduleModifier2; // schedule 2nd modifier e.g. in case of 'for' directive
    OpenMPScheduleClauseEnumKind scheduleKind;        // schedule 3rd parameter e.g. in case of 'for' directive

public:
    OpenMPScheduleClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // 	SCHEDULE - 1st modifier paramater
    void setScheduleFirstModifier(OpenMPScheduleClauseModifier v);

    OpenMPScheduleClauseModifier getScheduleFirstModifier();

    // 	SCHEDULE - 2nd modifier paramater
    void setScheduleSecondModifier(OpenMPScheduleClauseModifier v);

    OpenMPScheduleClauseModifier getScheduleSecondModifier();

    // 	SCHEDULE - 3rd paramater (schedule kind)
    void setScheduleKindValue(OpenMPScheduleClauseEnumKind v);

    OpenMPScheduleClauseEnumKind getScheduleKindValue();
};

// Dist_Schedule Clause
class OpenMPDistScheduleClause : public OpenMPClause {

protected:
    OpenMPScheduleClauseEnumKind scheduleKind; // schedule 3rd parameter e.g. in case of 'for' directive
    int schedule_chunk_size;

public:
    OpenMPDistScheduleClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Depend Clause
class OpenMPDependClause : public OpenMPClause {

protected:
    OpenMPDependClauseKind dependType; // depend type value

public:
    OpenMPDependClause(OpenMPClauseKind k) : OpenMPClause(k) {}

    // DEPEND
    void setDependDependenceType(OpenMPDependClauseKind v);

    OpenMPDependClauseKind getDependDependenceType();
};

// Copyin Clause
class OpenMPCopyinClause : public OpenMPClause {

protected:
public:
    OpenMPCopyinClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Ordered Clause
class OpenMPOrderedClause : public OpenMPClause {

protected:
public:
    OpenMPOrderedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Collapse Clause
class OpenMPCollapseClause : public OpenMPClause {

protected:
public:
    OpenMPCollapseClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Nowait Clause
class OpenMPNowaitClause : public OpenMPClause {

public:
    OpenMPNowaitClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Copyprivate Clause
class OpenMPCopyprivateClause : public OpenMPClause {

protected:
public:
    OpenMPCopyprivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Detach Clause
class OpenMPDetachClause : public OpenMPClause {

protected:
    const char *user_defined_event_handler;

public:
    OpenMPDetachClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Affinity Clause
class OpenMPAffinityClause : public OpenMPClause {

protected:
    // TODO - Define aff_modifier and its getters and setters

public:
    OpenMPAffinityClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Inclusive Clause
class OpenMPInclusiveClause : public OpenMPClause {

protected:
public:
    OpenMPInclusiveClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Exclusive Clause
class OpenMPExclusiveClause : public OpenMPClause {

protected:
public:
    OpenMPExclusiveClause(OpenMPClauseKind k) : OpenMPClause(k) {}
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

// Final Clause
class OpenMPFinalClause : public OpenMPClause {

protected:
public:
    OpenMPFinalClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Untied Clause
class OpenMPUntiedClause : public OpenMPClause {

public:
    OpenMPUntiedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Mergeable Clause
class OpenMPMergeableClause : public OpenMPClause {

public:
    OpenMPMergeableClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Numthreads Clause
class OpenMPNumthreadsClause : public OpenMPClause {

protected:
public:
    OpenMPNumthreadsClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Priority Clause
class OpenMPPriorityClause : public OpenMPClause {

protected:
public:
    OpenMPPriorityClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Safelen Clause
class OpenMPSafelenClause : public OpenMPClause {

protected:
public:
    OpenMPSafelenClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Simdlen Clause
class OpenMPSimdlenClause : public OpenMPClause {

protected:
public:
    OpenMPSimdlenClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Aligned Clause
class OpenMPAlignedClause : public OpenMPClause {

protected:
    // TODO - separate var list from expressions

public:
    OpenMPAlignedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Nontemporal Clause
class OpenMPNontemporalClause : public OpenMPClause {

protected:
public:
    OpenMPNontemporalClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Aligned Clause
class OpenMPLinearClause : public OpenMPClause {

protected:
    // TODO - separate var list from expressions

public:
    OpenMPLinearClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

#endif //OMPPARSER_OPENMPAST_H
