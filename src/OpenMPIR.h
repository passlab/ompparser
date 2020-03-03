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
    Lang_unknown
};

class SourceLocation {
    int line;
    int column;

    SourceLocation* parent_construct;

    public:
    SourceLocation(int _line = 0, int _col = 0, SourceLocation* _parent_construct = NULL) : line(_line), column(_col), parent_construct(_parent_construct) { } ;
    void setParentConstruct(SourceLocation* _parent_construct) { parent_construct = _parent_construct; };
    SourceLocation* getParentConstruct() { return parent_construct; };
    int getLine() { return line; };
    void setLine(int _line) { line = _line; };
    int getColumn() { return column; };
    void setColumn(int _column) { column = _column; };

};

/**
 * The class or baseclass for all the clause classes. For all the clauses that only take 0 to multiple expression or
 * variables, we use this class to create objects. For all other clauses, which requires at least one parameters,
 * we will have an inherit class from this one, and the superclass contains fields for the parameters
 */
class OpenMPClause : public SourceLocation {
protected:
    OpenMPClauseKind kind;
    // the clause position in the vector of clauses in original order
    int clause_position = -1;

    /* consider this is a struct of array, i.e.
     * the expression/localtionLine/locationColumn are the same index are one record for an expression and its location
     */
    std::vector<const char *> expressions;
    std::vector<const void *> expressionNodes;

    std::vector<SourceLocation> locations;

public:
    OpenMPClause(OpenMPClauseKind k, int _line = 0, int _col = 0) : SourceLocation(_line, _col), kind(k) {};

    OpenMPClauseKind getKind() { return kind; };
    int getClausePosition() { return clause_position; };
    void setClausePosition(int _clause_position) { clause_position = _clause_position; };

    // a list of expressions or variables that are language-specific for the clause, ompparser does not parse them,
    // instead, it only stores them as strings
    void addLangExpr(const char *expression, int line = 0, int col = 0);
    
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

    /* The vector is used to store the pointers of clauses in original order.
     * While unparsing, the generated pragma keeps the clauses in the same order as the input.
     * For example, #pragma omp parallel shared(a) private(b) is the input.
     * The unparsing won't switch the order of share and private clause. Share clause is always the first.
     *
     * For the clauses that could be normalized, we always merge the second one to the first one.
     * Then the second one will be eliminated and not stored anywhere.
     */
    std::vector<OpenMPClause *>* clauses_in_original_order = new std::vector<OpenMPClause *>();

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
    OpenMPDirective(OpenMPDirectiveKind k, OpenMPBaseLang _lang = Lang_unknown, int _line = 0, int _col = 0) :
            SourceLocation(_line, _col), kind(k), lang(_lang) {};

    OpenMPDirectiveKind getKind() { return kind; };

    map<OpenMPClauseKind, std::vector<OpenMPClause *> *> *getAllClauses() { return &clauses; };

    std::vector<OpenMPClause *> * getClauses(OpenMPClauseKind kind) { return clauses[kind]; };
    std::vector<OpenMPClause *> * getClausesInOriginalOrder() { return clauses_in_original_order; };

    std::string toString();

    /* generate DOT representation of the directive */
    void generateDOT(std::ofstream&, int, int, std::string, std::string);
    void generateDOT();
    std::string generatePragmaString(std::string _prefix = "#pragma omp ", std::string _beginning_symbol = "", std::string _ending_symbol = "");
    // To call this method directly to add new clause, it can't be protected.
    OpenMPClause * addOpenMPClause(int, ...);
    void setBaseLang(OpenMPBaseLang _lang) { lang = _lang; };
    OpenMPBaseLang getBaseLang() { return lang; };
};


//atomic directive
class OpenMPAtomicDirective : public OpenMPDirective {
protected:
    map<OpenMPClauseKind, vector<OpenMPClause *> *> clauses_atomic_after;
    map<OpenMPClauseKind, vector<OpenMPClause *> *> clauses_atomic_clauses;
public:
    OpenMPAtomicDirective () : OpenMPDirective(OMPD_atomic) {};
    std::vector<OpenMPClause *> *getClausesAtomicAfter(OpenMPClauseKind kind) { return clauses_atomic_after[kind]; };
    std::vector<OpenMPClause *> *getAtomicClauses(OpenMPClauseKind kind) { return clauses_atomic_clauses[kind]; };
    map<OpenMPClauseKind, std::vector<OpenMPClause *> *> *getAllClausesAtomicAfter() { return &clauses_atomic_after; };
    map<OpenMPClauseKind, std::vector<OpenMPClause *> *> *getAllAtomicClauses() { return &clauses_atomic_clauses; };
};

class OpenMPEndDirective : public OpenMPDirective {
protected:
    OpenMPDirective* paired_directive;
public:
    OpenMPEndDirective () : OpenMPDirective(OMPD_end) {};
    void setPairedDirective (OpenMPDirective* _paired_directive) { paired_directive = _paired_directive; };
    OpenMPDirective* getPairedDirective() { return paired_directive; };
};

class OpenMPRequiresDirective : public OpenMPDirective {
protected:
    std::vector<std::string> user_defined_implementation;
public:
    OpenMPRequiresDirective () : OpenMPDirective(OMPD_requires) {};
    void addUserDefinedImplementation (const char* _user_defined_implementation) { user_defined_implementation.push_back(std::string(_user_defined_implementation)); };
    std::vector<std::string>* getUserDefinedImplementation() { return &user_defined_implementation; };
};

//declare variant directive
class OpenMPDeclareVariantDirective : public OpenMPDirective {
protected:
    std::string variant_func_id;
public:
    OpenMPDeclareVariantDirective () : OpenMPDirective(OMPD_declare_variant) {};
    void setVariantFuncID (const char* _variant_func_id) { variant_func_id = std::string(_variant_func_id); };
    std::string getVariantFuncID () { return variant_func_id; };
};

//allocate directive
class OpenMPAllocateDirective : public OpenMPDirective {
protected:
    std::vector<const char*> allocate_list;
public:
    OpenMPAllocateDirective () : OpenMPDirective(OMPD_allocate) {};
    void addAllocateList (const char* _allocate_list) { allocate_list.push_back(_allocate_list); };
    std::vector<const char*>* getAllocateList () { return &allocate_list; };
};

//threadprivate directive
class OpenMPThreadprivateDirective : public OpenMPDirective {
protected:
    std::vector<const char*> threadprivate_list;
public:
    OpenMPThreadprivateDirective () : OpenMPDirective(OMPD_threadprivate) {};
    void addThreadprivateList (const char* _threadprivate_list) { threadprivate_list.push_back(_threadprivate_list); };
    std::vector<const char*>* getThreadprivateList () { return &threadprivate_list; };
};

//declare simd directive
class OpenMPDeclareSimdDirective : public OpenMPDirective {
protected:
    std::string proc_name;

public:
    OpenMPDeclareSimdDirective () : OpenMPDirective(OMPD_declare_simd) {};
    void addProcName (std::string _proc_name) { proc_name = _proc_name;}
    std::string getProcName() { return proc_name;}
};

//declare reduction directive
class OpenMPDeclareReductionDirective : public OpenMPDirective {
protected:
    std::vector<const char*> typename_list;
    std::string identifier;
    std::string combiner;
public:
    OpenMPDeclareReductionDirective () : OpenMPDirective(OMPD_declare_reduction) {};
    void addTypenameList (const char* _typename_list) { typename_list.push_back(_typename_list); };
    std::vector<const char*>* getTypenameList () { return &typename_list; };
    void setIdentifier (std::string _identifier) { identifier = _identifier;}
    std::string getIdentifier() { return identifier;}
    void setCombiner (const char *_combiner) { combiner = std::string(_combiner);}
    std::string getCombiner() {return combiner;}
};

//declare mapper directive
class OpenMPDeclareMapperDirective : public OpenMPDirective {
protected:
    std::string identifier;
    std::string type_var;

public:
    OpenMPDeclareMapperDirective () : OpenMPDirective(OMPD_declare_mapper) {};
    void setIdentifier (std::string _identifier) { identifier = _identifier;}
    std::string getIdentifier() { return identifier;}
    void setTypeVar (std::string _type_var) { type_var = _type_var;}
    std::string getTypeVar() { return type_var;}
};

// reduction clause
class OpenMPReductionClause : public OpenMPClause {

protected:
    OpenMPReductionClauseModifier modifier = OMPC_REDUCTION_MODIFIER_unknown;  // modifier
    OpenMPReductionClauseIdentifier identifier; // identifier
    std::string user_defined_identifier;  // user defined identifier if it is used

public:
    OpenMPReductionClause() : OpenMPClause(OMPC_reduction) { }

    OpenMPReductionClause(OpenMPReductionClauseModifier _modifier,
                          OpenMPReductionClauseIdentifier _identifier) : OpenMPClause(OMPC_reduction),
                                         modifier(_modifier), identifier(_identifier), user_defined_identifier ("") { };

    OpenMPReductionClauseModifier getModifier() { return modifier; };

    OpenMPReductionClauseIdentifier getIdentifier() { return identifier; };

    void setUserDefinedIdentifier(char* _identifier) { user_defined_identifier = std::string(_identifier); };

    std::string getUserDefinedIdentifier() { return user_defined_identifier; };

    static OpenMPClause * addReductionClause(OpenMPDirective *,  OpenMPReductionClauseModifier, 
                  OpenMPReductionClauseIdentifier, char * );

    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// initializer clause
class OpenMPInitializerClause : public OpenMPClause {
protected:
    OpenMPInitializerClausePriv priv; // initializer priv
    std::string user_defined_priv;                         /* user defined value if it is used */
public:
    OpenMPInitializerClause(OpenMPInitializerClausePriv _priv) :
            OpenMPClause(OMPC_initializer), priv(_priv), user_defined_priv ("") { };
    
    OpenMPInitializerClausePriv getPriv() { return priv; };

    void setUserDefinedPriv(char *_priv) { user_defined_priv = std::string(_priv); }

    std::string getUserDefinedPriv() { return user_defined_priv; };
    static OpenMPClause * addInitializerClause(OpenMPDirective *, OpenMPInitializerClausePriv, char *);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// allocate clause
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

    static OpenMPClause * addAllocateClause(OpenMPDirective *, OpenMPAllocateClauseAllocator, char*);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
// allocator
class OpenMPAllocatorClause : public OpenMPClause {
protected:
    OpenMPAllocatorClauseAllocator allocator; // Allocate allocator
    std::string user_defined_allocator;                         /* user defined value if it is used */

public:
    OpenMPAllocatorClause(OpenMPAllocatorClauseAllocator _allocator) :
            OpenMPClause(OMPC_allocator), allocator(_allocator), user_defined_allocator ("") { };

    OpenMPAllocatorClauseAllocator getAllocator() { return allocator; };

    void setUserDefinedAllocator(char *_allocator) { user_defined_allocator = std::string(_allocator); }

    std::string getUserDefinedAllocator() { return user_defined_allocator; };

    static OpenMPClause * addAllocatorClause(OpenMPDirective *, OpenMPAllocatorClauseAllocator, char*);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// lastprivate Clause
class OpenMPLastprivateClause : public OpenMPClause {
protected:
    OpenMPLastprivateClauseModifier modifier; // lastprivate modifier

public:
    OpenMPLastprivateClause() : OpenMPClause(OMPC_lastprivate) { }

    OpenMPLastprivateClause(OpenMPLastprivateClauseModifier _modifier) :
            OpenMPClause(OMPC_lastprivate), modifier(_modifier) { };

    OpenMPLastprivateClauseModifier getModifier() { return modifier; };

    static OpenMPClause * addLastprivateClause(OpenMPDirective*, OpenMPLastprivateClauseModifier);

    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// linear Clause
class OpenMPLinearClause : public OpenMPClause {
protected:
    OpenMPLinearClauseModifier modifier; // linear modifier

    std::string user_defined_step = "";

public:
    OpenMPLinearClause(OpenMPLinearClauseModifier _modifier) :
            OpenMPClause(OMPC_linear), modifier(_modifier) { };

    OpenMPLinearClauseModifier getModifier() { return modifier; };

    void setUserDefinedStep(const char *_step) { user_defined_step = _step; };

    std::string getUserDefinedStep() { return user_defined_step; };
    
    static OpenMPClause* addLinearClause(OpenMPDirective*, OpenMPLinearClauseModifier);

    void mergeLinear(OpenMPDirective*, OpenMPClause*);

    std::string toString();
    //std::string expressionToString(bool);
    void generateDOT(std::ofstream&, int, int, std::string);

};

// aligned Clause
class OpenMPAlignedClause : public OpenMPClause {
protected:

    std::string user_defined_alignment;

public:
    OpenMPAlignedClause( ) : OpenMPClause(OMPC_aligned) { };

    void setUserDefinedAlignment(const char *_alignment) { user_defined_alignment = _alignment; };

    std::string getUserDefinedAlignment() { return user_defined_alignment; };

    std::string toString();
    static OpenMPClause* addAlignedClause(OpenMPDirective*);
    //std::string expressionToString(bool);
    void generateDOT(std::ofstream&, int, int, std::string);

};
// dist_schedule Clause
class OpenMPDistscheduleClause : public OpenMPClause {

protected:
    OpenMPDistscheduleClauseKind dist_schedule_kind;     // kind
    std::string user_defined_kind;                // user defined identifier if it is used

public:
    OpenMPDistscheduleClause( ) : OpenMPClause(OMPC_dist_schedule) { }

    OpenMPDistscheduleClause(OpenMPDistscheduleClauseKind _dist_schedule_kind) : OpenMPClause(OMPC_dist_schedule), dist_schedule_kind(_dist_schedule_kind), user_defined_kind ("") { };

    OpenMPDistscheduleClauseKind getKind() { return dist_schedule_kind; };

    void setUserDefinedKind(char *dist_schedule_kind) { user_defined_kind = dist_schedule_kind; };

    std::string getUserDefinedKind() { return user_defined_kind; };

    static OpenMPClause* addDistscheduleClause(OpenMPDirective *, OpenMPDistscheduleClauseKind, char *);

    std::string toString();

    void generateDOT(std::ofstream&, int, int, std::string);
};


// schedule Clause
class OpenMPScheduleClause : public OpenMPClause {

protected:
    OpenMPScheduleClauseModifier modifier1;     // modifier1
    OpenMPScheduleClauseModifier modifier2;     // modifier2
    OpenMPScheduleClauseKind schedulekind; // identifier
    std::string user_defined_kind;                // user defined identifier if it is used
    std::string chunk_size;

public:
    OpenMPScheduleClause( ) : OpenMPClause(OMPC_schedule) { }

    OpenMPScheduleClause(OpenMPScheduleClauseModifier _modifier1, OpenMPScheduleClauseModifier _modifier2,
                          OpenMPScheduleClauseKind _schedulekind) : OpenMPClause(OMPC_schedule),
                                         modifier1(_modifier1),modifier2(_modifier2), schedulekind(_schedulekind), user_defined_kind ("") { };

    OpenMPScheduleClauseModifier getModifier1() { return modifier1; };
    OpenMPScheduleClauseModifier getModifier2() { return modifier2; };
    OpenMPScheduleClauseKind getKind() { return schedulekind; };

    void setUserDefinedKind(char *schedulekind) { user_defined_kind = schedulekind; };

    std::string getUserDefinedKind() { return user_defined_kind; };

    static OpenMPClause* addScheduleClause(OpenMPDirective *, OpenMPScheduleClauseModifier, OpenMPScheduleClauseModifier, OpenMPScheduleClauseKind, char *);

    void setChunkSize(const char *_step) { chunk_size = _step; };

    std::string getChunkSize() { return chunk_size; };
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// OpenMP clauses with variant directives, such as WHEN and MATCH clauses.
class OpenMPVariantClause : public OpenMPClause {
protected:
    std::pair<std::string, std::string> user_condition_expression;
    std::vector<std::pair<std::string, OpenMPDirective*> > construct_directives;
    std::pair<std::string, std::string> arch_expression;
    std::pair<std::string, std::string> isa_expression;
    std::pair<std::string, OpenMPClauseContextKind> context_kind_name = std::make_pair("", OMPC_CONTEXT_KIND_unknown);
    std::pair<std::string, std::string> extension_expression;
    std::pair<std::string, OpenMPClauseContextVendor> context_vendor_name = std::make_pair("", OMPC_CONTEXT_VENDOR_unspecified);
    std::pair<std::string, std::string> implementation_user_defined_expression;

public:
    OpenMPVariantClause(OpenMPClauseKind _kind) : OpenMPClause(_kind) { };

    void setUserCondition(const char* _score, const char* _user_condition_expression) { user_condition_expression = std::make_pair(std::string(_score), std::string(_user_condition_expression)); };
    std::pair<std::string, std::string>* getUserCondition() { return &user_condition_expression; };
    void addConstructDirective(const char* _score, OpenMPDirective* _construct_directive) { construct_directives.push_back(std::make_pair(std::string(_score), _construct_directive)); };
    std::vector<std::pair<std::string, OpenMPDirective*> >* getConstructDirective() { return &construct_directives; };
    void setArchExpression(const char* _score, const char* _arch_expression) { arch_expression = std::make_pair(std::string(_score), std::string(_arch_expression)); };
    std::pair<std::string, std::string>* getArchExpression() { return &arch_expression; };
    void setIsaExpression(const char* _score, const char* _isa_expression) { isa_expression = std::make_pair(std::string(_score), std::string(_isa_expression)); };
    std::pair<std::string, std::string>* getIsaExpression() { return &isa_expression; };
    void setContextKind(const char* _score, OpenMPClauseContextKind _context_kind_name) { context_kind_name = std::make_pair(std::string(_score), _context_kind_name); };
    std::pair<std::string, OpenMPClauseContextKind>* getContextKind() { return &context_kind_name; };
    void setExtensionExpression(const char* _score, const char* _extension_expression) { extension_expression = std::make_pair(std::string(_score), std::string(_extension_expression)); };
    std::pair<std::string, std::string>* getExtensionExpression() { return &extension_expression; };
    void setImplementationKind(const char* _score, OpenMPClauseContextVendor _context_vendor_name) { context_vendor_name = std::make_pair(std::string(_score), _context_vendor_name); };
    std::pair<std::string, OpenMPClauseContextVendor>* getImplementationKind() { return &context_vendor_name; };
    void setImplementationExpression(const char* _score, const char* _implementation_user_defined_expression) { implementation_user_defined_expression = std::make_pair(std::string(_score), std::string(_implementation_user_defined_expression)); };
    std::pair<std::string, std::string>* getImplementationExpression() { return &implementation_user_defined_expression; };
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// When Clause
class OpenMPWhenClause : public OpenMPVariantClause {
protected:
    OpenMPDirective* variant_directive = NULL; // variant directive inside the WHEN clause


public:
    OpenMPWhenClause() : OpenMPVariantClause(OMPC_when) { };
    OpenMPDirective* getVariantDirective() { return variant_directive; };
    void setVariantDirective(OpenMPDirective* _variant_directive) { variant_directive = _variant_directive; };

    static OpenMPClause * addWhenClause(OpenMPDirective* directive);
    //void generateDOT(std::ofstream&, int, int, std::string);
};

// Match Clause
class OpenMPMatchClause : public OpenMPVariantClause {
protected:

public:
    OpenMPMatchClause( ) : OpenMPVariantClause(OMPC_match) { };

    static OpenMPClause * addMatchClause(OpenMPDirective* directive);
    //void generateDOT(std::ofstream&, int, int, std::string);
};


// ProcBind Clause
class OpenMPProcBindClause : public OpenMPClause {

protected:
    OpenMPProcBindClauseKind proc_bind_kind; // proc_bind

public:
    OpenMPProcBindClause(OpenMPProcBindClauseKind _proc_bind_kind) :
            OpenMPClause(OMPC_proc_bind), proc_bind_kind(_proc_bind_kind) { };

    OpenMPProcBindClauseKind getProcBindClauseKind() { return proc_bind_kind; };
    static OpenMPClause * addProcBindClause(OpenMPDirective*, OpenMPProcBindClauseKind);
    std::string toString();
    //void addProcBindClauseKind(OpenMPProcBindClauseKind v);
};

// Bind Clause
class OpenMPBindClause : public OpenMPClause {

protected:
    OpenMPBindClauseKind bind_kind;

public:
    OpenMPBindClause(OpenMPBindClauseKind _bind_kind) :
            OpenMPClause(OMPC_bind), bind_kind(_bind_kind) { };

    OpenMPBindClauseKind getBindClauseKind() { return bind_kind; };
    static OpenMPClause * addBindClause(OpenMPDirective*, OpenMPBindClauseKind);
    std::string toString();
    //void addProcBindClauseKind(OpenMPProcBindClauseKind v);
};

// Default Clause
class OpenMPDefaultClause : public OpenMPClause {

protected:
    OpenMPDefaultClauseKind default_kind = OMPC_DEFAULT_unknown;
    OpenMPDirective* variant_directive = NULL; // variant directive inside the DEFAULT clause

public:
    OpenMPDefaultClause(OpenMPDefaultClauseKind _default_kind) : OpenMPClause(OMPC_default), default_kind(_default_kind) { };

    OpenMPDefaultClauseKind getDefaultClauseKind() {return default_kind; };

    OpenMPDirective* getVariantDirective() { return variant_directive; };
    void setVariantDirective(OpenMPDirective* _variant_directive) { variant_directive = _variant_directive; };

    static OpenMPClause * addDefaultClause(OpenMPDirective*, OpenMPDefaultClauseKind);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// Order Clause
class OpenMPOrderClause : public OpenMPClause {

protected:
    OpenMPOrderClauseKind order_kind = OMPC_ORDER_unspecified;

public:
    OpenMPOrderClause(OpenMPOrderClauseKind _order_kind) : OpenMPClause(OMPC_order), order_kind(_order_kind) { };

    OpenMPOrderClauseKind getOrderClauseKind() {return order_kind; };

    static OpenMPClause * addOrderClause(OpenMPDirective*, OpenMPOrderClauseKind);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// if Clause
class OpenMPIfClause : public OpenMPClause {

protected:
    OpenMPIfClauseModifier modifier; // linear modifier
    std::string user_defined_modifier;        /* user defined value if it is used */

public:
    OpenMPIfClause(OpenMPIfClauseModifier _modifier) :
            OpenMPClause(OMPC_if), modifier(_modifier), user_defined_modifier ("") { };

    OpenMPIfClauseModifier getModifier() { return modifier; };

    void setUserDefinedModifier(char *_modifier) { user_defined_modifier = _modifier; };


    std::string getUserDefinedModifier() { return user_defined_modifier; };
    
    static OpenMPClause* addIfClause(OpenMPDirective *directive, OpenMPIfClauseModifier modifier, char * user_defined_modifier);

    std::string toString();

    void generateDOT(std::ofstream&, int, int, std::string);
};
// hint Clause
class OpenMPHintClause : public OpenMPClause {
public:
    OpenMPHintClause() :
            OpenMPClause(OMPC_hint) { };

    static OpenMPClause* addHintClause(OpenMPDirective *directive, int before_or_after);
};
// acq_rel Clause
class OpenMPAcqRelClause : public OpenMPClause {
public:
    OpenMPAcqRelClause() :
            OpenMPClause(OMPC_acq_rel) { };

    static OpenMPClause* addAcqRelClause(OpenMPDirective *directive, int before_or_after);
};
// seq_cst Clause
class OpenMPSeqCstClause : public OpenMPClause {
public:
    OpenMPSeqCstClause() :
            OpenMPClause(OMPC_seq_cst) { };

    static OpenMPClause* addSeqCstClause(OpenMPDirective *directive, int before_or_after);
};
// release Clause
class OpenMPReleaseClause : public OpenMPClause {
public:
    OpenMPReleaseClause() :
            OpenMPClause(OMPC_release) { };

    static OpenMPClause* addReleaseClause(OpenMPDirective *directive, int before_or_after);
};
// acquire Clause
class OpenMPAcquireClause : public OpenMPClause {
public:
    OpenMPAcquireClause() :
            OpenMPClause(OMPC_acquire) { };

    static OpenMPClause* addAcquireClause(OpenMPDirective *directive, int before_or_after);
};
// relaxed Clause
class OpenMPRelaxedClause : public OpenMPClause {
public:
    OpenMPRelaxedClause() :
            OpenMPClause(OMPC_relaxed) { };

    static OpenMPClause* addRelaxedClause(OpenMPDirective *directive, int before_or_after);
};
// read Clause
class OpenMPReadClause : public OpenMPClause {
public:
    OpenMPReadClause() :
            OpenMPClause(OMPC_read) { };

    static OpenMPClause* addReadClause(OpenMPDirective *directive);
};

// capture Clause
class OpenMPCaptureClause : public OpenMPClause {
public:
    OpenMPCaptureClause() :
            OpenMPClause(OMPC_capture) { };

    static OpenMPClause* addCaptureClause(OpenMPDirective *directive);
};

// update Clause
class OpenMPUpdateClause : public OpenMPClause {
public:
    OpenMPUpdateClause() :
            OpenMPClause(OMPC_update) { };

    static OpenMPClause* addUpdateClause(OpenMPDirective *directive);
};

// write Clause
class OpenMPWriteClause : public OpenMPClause {
public:
    OpenMPWriteClause() :
            OpenMPClause(OMPC_write) { };

    static OpenMPClause* addWriteClause(OpenMPDirective *directive);
};
//in_reduction clause
class OpenMPInReductionClause : public OpenMPClause {

protected:
    OpenMPInReductionClauseIdentifier identifier; // identifier
    std::string user_defined_identifier;                // user defined identifier if it is used

public:
    OpenMPInReductionClause( ) : OpenMPClause(OMPC_in_reduction) { }

    OpenMPInReductionClause(OpenMPInReductionClauseIdentifier _identifier) : OpenMPClause(OMPC_in_reduction),identifier(_identifier), user_defined_identifier ("") { };

    OpenMPInReductionClauseIdentifier getIdentifier() { return identifier; };

    void setUserDefinedIdentifier(char* _identifier) { user_defined_identifier = std::string(_identifier); };

    std::string getUserDefinedIdentifier() { return user_defined_identifier; };

    static OpenMPClause * addInReductionClause(OpenMPDirective *, OpenMPInReductionClauseIdentifier, char *);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
// depend clause
class OpenMPDependClause : public OpenMPClause {

protected:
    OpenMPDependClauseModifier modifier;     // modifier
    OpenMPDependClauseType type; //type
    std::string dependence_vector;      
    std::vector<std::vector<const char*>* > depend_iterators_definition_class;

public:
    OpenMPDependClause( ) : OpenMPClause(OMPC_depend) { }

    OpenMPDependClause(OpenMPDependClauseModifier _modifier, OpenMPDependClauseType _type) : OpenMPClause(OMPC_depend),modifier(_modifier), type(_type) { };
    OpenMPDependClauseModifier getModifier() { return modifier; };
    OpenMPDependClauseType getType() { return type; };
    void addDependenceVector(const char* _dependence_vector) { dependence_vector = std::string(_dependence_vector); };
    std::string getDependenceVector() { return dependence_vector; };
    void setDependIteratorsDefinitionClass(std::vector<std::vector<const char*>* >* _depend_iterators_definition_class) { depend_iterators_definition_class = *_depend_iterators_definition_class; };
    std::vector<vector<const char*>* >* getDependIteratorsDefinitionClass() { return &depend_iterators_definition_class; };
    static OpenMPClause * addDependClause(OpenMPDirective *, OpenMPDependClauseModifier, OpenMPDependClauseType);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
    void mergeDepend(OpenMPDirective*, OpenMPClause*);

};

// affinity clause
class OpenMPAffinityClause : public OpenMPClause {

protected:
    OpenMPAffinityClauseModifier modifier;     // modifier
    std::vector<std::vector<const char*>* > iterators_definition_class;
public:
    OpenMPAffinityClause( ) : OpenMPClause(OMPC_affinity) { }

    OpenMPAffinityClause(OpenMPAffinityClauseModifier _modifier): OpenMPClause(OMPC_affinity),
                                         modifier(_modifier){ };
    void addIteratorsDefinitionClass (std::vector<const char*>* _iterator_definition) { iterators_definition_class.push_back(_iterator_definition);};
    std::vector<vector<const char*>* >* getIteratorsDefinitionClass  () { return &iterators_definition_class; };
    OpenMPAffinityClauseModifier getModifier() { return modifier; };
    static OpenMPClause * addAffinityClause(OpenMPDirective *, OpenMPAffinityClauseModifier);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
//atomic_default_mem_order clause
class OpenMPAtomicDefaultMemOrderClause : public OpenMPClause {

protected:
    OpenMPAtomicDefaultMemOrderClauseKind kind; 

public:
    OpenMPAtomicDefaultMemOrderClause(OpenMPAtomicDefaultMemOrderClauseKind _kind) :
            OpenMPClause(OMPC_atomic_default_mem_order), kind(_kind) { };

    OpenMPAtomicDefaultMemOrderClauseKind getKind() {return kind; };

    static OpenMPClause * addAtomicDefaultMemOrderClause(OpenMPDirective* directive, OpenMPAtomicDefaultMemOrderClauseKind kind);

    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// device clause
class OpenMPDeviceClause : public OpenMPClause {

protected:
    OpenMPDeviceClauseModifier modifier;     // modifier

public:
    OpenMPDeviceClause( ) : OpenMPClause(OMPC_device) { }

    OpenMPDeviceClause(OpenMPDeviceClauseModifier _modifier) : OpenMPClause(OMPC_device),
                                         modifier(_modifier) { };

    OpenMPDeviceClauseModifier getModifier() { return modifier; };
    
    static OpenMPClause * addDeviceClause(OpenMPDirective *directive, OpenMPDeviceClauseModifier modifier);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);

};

// to clause
class OpenMPToClause : public OpenMPClause {

protected:
    OpenMPToClauseKind to_kind;  
    std::string mapper_identifier;       
public:

    OpenMPToClause( ) : OpenMPClause(OMPC_to) { }
    OpenMPToClause(OpenMPToClauseKind _to_kind): OpenMPClause(OMPC_to),
                                         to_kind(_to_kind){ };
    OpenMPToClauseKind getKind() { return to_kind; };
    void setMapperIdentifier(const char* _identifier) { mapper_identifier = std::string(_identifier); };
    std::string getMapperIdentifier() { return mapper_identifier; };
    static OpenMPClause * addToClause(OpenMPDirective *, OpenMPToClauseKind);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
// from clause
class OpenMPFromClause : public OpenMPClause {

protected:
    OpenMPFromClauseKind from_kind;
    std::string mapper_identifier;     
public:

    OpenMPFromClause( ) : OpenMPClause(OMPC_from) { }
    OpenMPFromClause(OpenMPFromClauseKind _from_kind): OpenMPClause(OMPC_from),
                                         from_kind(_from_kind){ };
    OpenMPFromClauseKind getKind() { return from_kind; };

    void setMapperIdentifier(const char* _identifier) { mapper_identifier = std::string(_identifier); };
    std::string getMapperIdentifier() { return mapper_identifier; };
    static OpenMPClause * addFromClause(OpenMPDirective *, OpenMPFromClauseKind);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
// defaultmap Clause
class OpenMPDefaultmapClause : public OpenMPClause {
protected:
    OpenMPDefaultmapClauseBehavior behavior; // defaultmap behavior
    OpenMPDefaultmapClauseCategory category; // defaultmap category

public:
    OpenMPDefaultmapClause(OpenMPDefaultmapClauseBehavior _behavior,OpenMPDefaultmapClauseCategory _category) :
            OpenMPClause(OMPC_defaultmap), behavior(_behavior), category(_category)  { };
 
    OpenMPDefaultmapClauseBehavior getBehavior() { return behavior; };
    OpenMPDefaultmapClauseCategory  getCategory () { return category ; };

    static OpenMPClause * addDefaultmapClause(OpenMPDirective *, OpenMPDefaultmapClauseBehavior, OpenMPDefaultmapClauseCategory);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
// device type Clause
class OpenMPDeviceTypeClause : public OpenMPClause {

protected:
    OpenMPDeviceTypeClauseKind device_type_kind = OMPC_DEVICE_TYPE_unknown;
public:
    OpenMPDeviceTypeClause(OpenMPDeviceTypeClauseKind _device_type_kind) : OpenMPClause(OMPC_default), device_type_kind(_device_type_kind) { };

    OpenMPDeviceTypeClauseKind getDeviceTypeClauseKind() {return device_type_kind; };

    static OpenMPClause * addDeviceTypeClause(OpenMPDirective* directive, OpenMPDeviceTypeClauseKind devicetypeKind);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
class OpenMPTaskReductionClause : public OpenMPClause {
//task reduction clause
protected:
    OpenMPTaskReductionClauseIdentifier identifier; // identifier
    std::string user_defined_identifier;                // user defined identifier if it is used

public:
    OpenMPTaskReductionClause( ) : OpenMPClause(OMPC_task_reduction) { }

    OpenMPTaskReductionClause(OpenMPTaskReductionClauseIdentifier _identifier) : OpenMPClause(OMPC_task_reduction), identifier(_identifier),  user_defined_identifier ("") { };

    OpenMPTaskReductionClauseIdentifier getIdentifier() { return identifier; };

    void setUserDefinedIdentifier(char* _identifier) { user_defined_identifier = std::string(_identifier); };

    std::string getUserDefinedIdentifier() { return user_defined_identifier; };

    static OpenMPClause * addTaskReductionClause(OpenMPDirective *, OpenMPTaskReductionClauseIdentifier, char *);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

// map clause
class OpenMPMapClause : public OpenMPClause {
protected:
    OpenMPMapClauseModifier modifier1; 
    OpenMPMapClauseModifier modifier2;
    OpenMPMapClauseModifier modifier3;
    OpenMPMapClauseType type = OMPC_MAP_TYPE_unknown; 
    std::string mapper_identifier;

public:
    OpenMPMapClause() : OpenMPClause(OMPC_map) { }

    OpenMPMapClause(OpenMPMapClauseModifier _modifier1, OpenMPMapClauseModifier _modifier2, OpenMPMapClauseModifier _modifier3, OpenMPMapClauseType _type, std::string _mapper_identifier) : OpenMPClause(OMPC_map), modifier1(_modifier1), modifier2(_modifier2), modifier3(_modifier3),  type(_type), mapper_identifier (_mapper_identifier) { };
                          
    OpenMPMapClauseModifier getModifier1() { return modifier1; };
    OpenMPMapClauseModifier getModifier2() { return modifier2; };
    OpenMPMapClauseModifier getModifier3() { return modifier3; };
    OpenMPMapClauseType getType() { return type; };
    std::string getMapperIdentifier() {return mapper_identifier;};
    static OpenMPClause * addMapClause(OpenMPDirective *, OpenMPMapClauseModifier, OpenMPMapClauseModifier, OpenMPMapClauseModifier, OpenMPMapClauseType, std::string);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};

//declare target directive
class OpenMPDeclareTargetDirective : public OpenMPDirective {
protected:
    std::vector<std::string> extended_list;
public:
    OpenMPDeclareTargetDirective () : OpenMPDirective(OMPD_declare_target) {};
    void addExtendedList (const char* _extended_list) { extended_list.push_back(std::string(_extended_list)); };
    std::vector<std::string>* getExtendedList () { return &extended_list; };
};

//flush directive
class OpenMPFlushDirective : public OpenMPDirective {
protected:
    std::vector<std::string> flush_list;
public:
    OpenMPFlushDirective () : OpenMPDirective(OMPD_flush) {};
    void addFlushList (const char* _flush_list) { flush_list.push_back(std::string(_flush_list)); };
    std::vector<std::string>* getFlushList () { return &flush_list; };
};

// critical directive
class OpenMPCriticalDirective : public OpenMPDirective {

protected:  
    std::string critical_name; 
public:
    OpenMPCriticalDirective( ) : OpenMPDirective(OMPD_critical) { }  
    void setCriticalName(const char* _name) { critical_name = std::string(_name); };
    std::string getCriticalName() { return critical_name; };
};
//DepobjUpdate clause
class OpenMPDepobjUpdateClause : public OpenMPClause {

protected:
    OpenMPDepobjUpdateClauseDependeceType type; 
public:
    OpenMPDepobjUpdateClause() : OpenMPClause(OMPC_depobj_update) { }

    OpenMPDepobjUpdateClause(OpenMPDepobjUpdateClauseDependeceType _type) : OpenMPClause(OMPC_depobj_update),type(_type){};

    OpenMPDepobjUpdateClauseDependeceType getType() { return type; };
    static OpenMPClause * addDepobjUpdateClause(OpenMPDirective *, OpenMPDepobjUpdateClauseDependeceType);
    std::string toString();
    void generateDOT(std::ofstream&, int, int, std::string);
};
// depobj directive
class OpenMPDepobjDirective : public OpenMPDirective {

protected:  
    std::string depobj; 
public:
    OpenMPDepobjDirective() : OpenMPDirective(OMPD_depobj) { }  
    void addDepobj(const char* _depobj) { depobj = std::string(_depobj); };
    std::string getDepobj() { return depobj; };
    void generateDOT(std::ofstream&, int, int, std::string);
};
//ordered directive
class OpenMPOrderedDirective : public OpenMPDirective {

protected:  
public:
    OpenMPOrderedDirective() : OpenMPDirective(OMPD_ordered) { }  
    void generateDOT(std::ofstream&, int, int, std::string);
};
// uses_allocators clause_parameters
class usesAllocatorParameter {
protected:
    OpenMPUsesAllocatorsClauseAllocator allocator;
    std::string allocator_traits_array;
    std::string allocator_user;
public:
    usesAllocatorParameter(OpenMPUsesAllocatorsClauseAllocator _allocator, std::string _allocator_traits_array, std::string _allocator_user):
    allocator(_allocator), allocator_traits_array(_allocator_traits_array), allocator_user(_allocator_user){};
    OpenMPUsesAllocatorsClauseAllocator getUsesAllocatorsAllocator() { return allocator;};
    std::string getAllocatorTraitsArray(){ return allocator_traits_array;};
    std::string getAllocatorUser(){ return allocator_user;};
};
// uses_allocators clause
class OpenMPUsesAllocatorsClause : public OpenMPClause {
protected:
    std::vector<usesAllocatorParameter*> usesAllocatorsAllocatorSequence;
public:
    OpenMPUsesAllocatorsClause() : OpenMPClause(OMPC_uses_allocators) { };
    void addUsesAllocatorsAllocatorSequence(OpenMPUsesAllocatorsClauseAllocator _allocator, std::string _allocator_traits_array, std::string _allocator_user) {
        usesAllocatorParameter* usesAllocatorsAllocator = new usesAllocatorParameter(_allocator, _allocator_traits_array, _allocator_user);
        usesAllocatorsAllocatorSequence.push_back(usesAllocatorsAllocator);
    };
    std::vector<usesAllocatorParameter*>* getUsesAllocatorsAllocatorSequence() {
        return &usesAllocatorsAllocatorSequence;
    };
    static OpenMPClause * addUsesAllocatorsClause(OpenMPDirective* directive);
    std::string toString();
};

#ifdef __cplusplus
extern "C" {
#endif
    extern  OpenMPDirective * parseOpenMP(const char *, void * exprParse(const char * expr));
#ifdef __cplusplus
}
#endif

#endif //OMPPARSER_OPENMPAST_H
