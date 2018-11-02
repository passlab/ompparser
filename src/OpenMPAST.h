//
// Created by Yonghong Yan on 6/15/18.
// Modified by Chrisogonas
//

#ifndef OMPPARSER_OPENMPATTRIBUTE_H_H
#define OMPPARSER_OPENMPATTRIBUTE_H_H

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
/**
 * The superclass for all the clauses.
 */
class OpenMPClause
{
  protected:
	OpenMPClauseKind kind;
	std::vector<const char *> expressions;
	std::string label;

  public:
	OpenMPClause(OpenMPClauseKind k) : kind(k){};
	OpenMPClauseKind getKind();

	// a list of clause language expressions, variables, etc that are not parsed by the ompparser
	void addExpression(const char *expression);
	std::vector<const char *> getExpressions();

	// clause name/label
	std::string getLabel();
};

/**
 * The class for all the OpenMP directives
 */
class OpenMPDirective
{
  protected:
	OpenMPDirectiveKind kind;

	/* the map to store all the clauses of the directive */
	map<OpenMPClauseKind, vector<OpenMPClause *> *> clauses;

	/**
	 * This method searches the clauses map to see whether an OpenMPClause object exists in the map and whether it
	 * has the exact same kind and parameters as the arguments of the call to this method.
	 */
	//		OpenMPClause *searchOpenMPClause(OpenMPClauseKind kind, ...); // initial proposed/discussed template
	OpenMPClause *searchOpenMPClause(OpenMPClause * clause);

	/**
	 * Get vector<OpenMPClause> list of all clauses of a certain OpenMPClauseKind
	 * 
	 */	
	vector<OpenMPClause *> * getOpenMPClausesList(OpenMPClauseKind kind);

	/** add a clause of specific kind and has several parameters (first, second, third, etc).
	 * This method should first search the clauses map to see whether an OpenMPClause object exists in the map and whether it
	 * has exactly the same kind and parameters as the arguments of the call to this method. If so, it should only return 
	 * that OpenMPClause object, otherwise, it should create a new OpenMPClause object and insert in the map 
	 * */
	// std::vector<OpenMPClause *> findOpenMPClause(OpenMPClauseKind kind, ...); // discussed template
		std::vector<OpenMPClause *> * findOpenMPClause(OpenMPClause * clause);

  public:
	OpenMPDirective(OpenMPDirectiveKind k) : kind(k){};
	OpenMPDirectiveKind getKind();
	map<OpenMPClauseKind, std::vector<OpenMPClause *> *> getClauses();

	// merge expressions
	OpenMPClause * mergeExpressions(OpenMPClause * vector_clause, OpenMPClause * new_clause);

	// clause name/label
	std::string getLabel();

	/* generate DOT representation of the directive */
	void generateDOT();
};

// reduction clause
class OpenMPReductionClause : public OpenMPClause
{

  protected:
	OpenMPReductionClauseModifier reductionModifier;	 // modifier
	OpenMPReductionClauseIdentifier reductionIdentifier; // identifier
	char *userDefinedIdentifier;						 /* user defined identifier if it is used */

  public:
	OpenMPReductionClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// REDUCTION - modifier
	void setReductionClauseModifier(OpenMPReductionClauseModifier v);
	OpenMPReductionClauseModifier getReductionClauseModifier();

	// REDUCTION - identifier
	void setReductionClauseIdentifier(OpenMPReductionClauseIdentifier v);
	OpenMPReductionClauseIdentifier getReductionClauseIdentifier();

	void setUserDefinedIdentifier(char *identifier) { userDefinedIdentifier = identifier; };
	char *getUserDefinedIdentifier();
};

// in_reduction clause
class OpenMPInReductionClause : public OpenMPClause
{
  protected:
	OpenMPReductionClauseIdentifier reductionIdentifier; // identifier
	char *userDefinedIdentifier;						 /* user defined identifier if it is used */
  public:
	OpenMPInReductionClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// REDUCTION - identifier
	void setReductionClauseIdentifier(OpenMPReductionClauseIdentifier v);
	OpenMPReductionClauseIdentifier getReductionClauseIdentifier();

	void setUserDefinedIdentifier(char *identifier) { userDefinedIdentifier = identifier; }
	char *getUserDefinedIdentifier();
};

// allocate
class OpenMPAllocateClause : public OpenMPClause
{
  protected:
	OpenMPAllocateClauseKind allocator; // Allocate allocator
	char *userDefinedAllocator;						 /* user defined value if it is used */

  public:
	OpenMPAllocateClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// 	ALLOCATE ALLOCATORS
	void setAllocatorValue(OpenMPAllocateClauseKind v);
	OpenMPAllocateClauseKind getAllocateClauseValue();

	void setUserDefinedAllocator(char *udt_value) { userDefinedAllocator = udt_value; }
	char *getUserDefinedAllocator();	
};

// Lastprivate Clause
class OpenMPLastprivateClause : public OpenMPClause
{

  protected:
	OpenMPLastprivateClauseKind lastPrivate; // lastPrivate

  public:
	OpenMPLastprivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// 	LASTPRIVATE
	void setLastprivateValue(OpenMPLastprivateClauseKind v);
	OpenMPLastprivateClauseKind getLastprivateClauseValue();
};

// ProcBind Clause
class OpenMPProcBindClause : public OpenMPClause
{

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
class OpenMPDefaultClause : public OpenMPClause
{

  protected:
	OpenMPDefaultClauseKind defaultKind; // default

  public:
	OpenMPDefaultClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// DEFAULT
	void setDefaultClauseValue(OpenMPDefaultClauseKind v);
	OpenMPDefaultClauseKind getDefaultClauseValue();
};

// if Clause
class OpenMPIfClause : public OpenMPClause
{

  protected:
	OpenMPIfClauseKind ifKind; // if

  public:
	OpenMPIfClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// 	IF
	void setIfClauseValue(OpenMPIfClauseKind v);
	OpenMPIfClauseKind getIfClauseValue();
};

// Bind Clause
class OpenMPBindClause : public OpenMPClause
{

  protected:
	OpenMPBindClauseKind bindKind; // bind

  public:
	OpenMPBindClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// 	BIND
	void setBindClauseValue(OpenMPBindClauseKind v);
	OpenMPBindClauseKind getBindClauseValue();
};

// Order Clause
class OpenMPOrderClause : public OpenMPClause
{

  protected:
	OpenMPOrderClauseKind orderKind; // order

  public:
	OpenMPOrderClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// ORDER
	void setOrderClauseValue(OpenMPOrderClauseKind v);
	OpenMPOrderClauseKind getOrderClauseValue();
};

// Schedule Clause
class OpenMPScheduleClause : public OpenMPClause
{

  protected:
	OpenMPScheduleClauseModifier scheduleModifier1; // schedule 1st modifier e.g. in case of 'for' directive
	OpenMPScheduleClauseModifier scheduleModifier2; // schedule 2nd modifier e.g. in case of 'for' directive
	OpenMPScheduleClauseEnumKind scheduleKind;		// schedule 3rd parameter e.g. in case of 'for' directive

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
class OpenMPDistScheduleClause : public OpenMPClause
{

  protected:
	OpenMPScheduleClauseEnumKind scheduleKind; // schedule 3rd parameter e.g. in case of 'for' directive
	int schedule_chunk_size;

  public:
	OpenMPDistScheduleClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Depend Clause
class OpenMPDependClause : public OpenMPClause
{

  protected:
	OpenMPDependClauseKind dependType; // depend type value

  public:
	OpenMPDependClause(OpenMPClauseKind k) : OpenMPClause(k) {}

	// DEPEND
	void setDependDependenceType(OpenMPDependClauseKind v);
	OpenMPDependClauseKind getDependDependenceType();
};

// Copyin Clause
class OpenMPCopyinClause : public OpenMPClause
{

  protected:
  public:
	OpenMPCopyinClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Ordered Clause
class OpenMPOrderedClause : public OpenMPClause
{

  protected:
  public:
	OpenMPOrderedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Collapse Clause
class OpenMPCollapseClause : public OpenMPClause
{

  protected:
  public:
	OpenMPCollapseClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Nowait Clause
class OpenMPNowaitClause : public OpenMPClause
{

  public:
	OpenMPNowaitClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Copyprivate Clause
class OpenMPCopyprivateClause : public OpenMPClause
{

  protected:
  public:
	OpenMPCopyprivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Detach Clause
class OpenMPDetachClause : public OpenMPClause
{

  protected:
	const char *user_defined_event_handler;

  public:
	OpenMPDetachClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Affinity Clause
class OpenMPAffinityClause : public OpenMPClause
{

  protected:
	// TODO - Define aff_modifier and its getters and setters

  public:
	OpenMPAffinityClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Inclusive Clause
class OpenMPInclusiveClause : public OpenMPClause
{

  protected:
  public:
	OpenMPInclusiveClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Exclusive Clause
class OpenMPExclusiveClause : public OpenMPClause
{

  protected:
  public:
	OpenMPExclusiveClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Private Clause
class OpenMPPrivateClause : public OpenMPClause
{

  protected:
  public:
	OpenMPPrivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// FirstPrivate Clause
class OpenMPFirstprivateClause : public OpenMPClause
{

  protected:
  public:
	OpenMPFirstprivateClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Shared Clause
class OpenMPSharedClause : public OpenMPClause
{

  protected:
  public:
	OpenMPSharedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Final Clause
class OpenMPFinalClause : public OpenMPClause
{

  protected:
  public:
	OpenMPFinalClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Untied Clause
class OpenMPUntiedClause : public OpenMPClause
{

  public:
	OpenMPUntiedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Mergeable Clause
class OpenMPMergeableClause : public OpenMPClause
{

  public:
	OpenMPMergeableClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Numthreads Clause
class OpenMPNumthreadsClause : public OpenMPClause
{

  protected:
  public:
	OpenMPNumthreadsClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Priority Clause
class OpenMPPriorityClause : public OpenMPClause
{

  protected:
  public:
	OpenMPPriorityClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Safelen Clause
class OpenMPSafelenClause : public OpenMPClause
{

  protected:
  public:
	OpenMPSafelenClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Simdlen Clause
class OpenMPSimdlenClause : public OpenMPClause
{

  protected:
  public:
	OpenMPSimdlenClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Aligned Clause
class OpenMPAlignedClause : public OpenMPClause
{

  protected:
	// TODO - separate var list from expressions

  public:
	OpenMPAlignedClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Nontemporal Clause
class OpenMPNontemporalClause : public OpenMPClause
{

  protected:
  public:
	OpenMPNontemporalClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

// Aligned Clause
class OpenMPLinearClause : public OpenMPClause
{

  protected:
	// TODO - separate var list from expressions

  public:
	OpenMPLinearClause(OpenMPClauseKind k) : OpenMPClause(k) {}
};

#endif //OMPPARSER_OPENMPATTRIBUTE_H_H
