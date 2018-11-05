#include "OpenMPAST.h"
#include <algorithm>

using namespace std;

/**
 * a list of language expressions, variables, etc that are not parsed by the ompparser
 */
void addExpression(const char *expression, int line = 0, int col = 0) {
    //TODO: Here we need to do certain normlization, if an expression already exists, we ignore
	expressions.push_back(expression);
    locations.push_back(SourceLocation(line, col));
}

std::string OpenMPClause::toString( ) {
    /* TODO maybe we have a better way to associate an enum with a string, so we
     * can just search from a array indexed by OpenMPClauseKind to return the string here
     */
	if (this.kind == OMPC_num_threads) return "num_threads";
	return "none";
}

// REDUCTION
// set the value for reduction modifier
void OpenMPReductionClause::setReductionClauseModifier(OpenMPReductionClauseModifier v)
{
	reductionModifier = v;
	//	param_one_reduction.push_back(&v);
}

// get reduction modifier value
OpenMPReductionClauseModifier OpenMPReductionClause::getReductionClauseModifier()
{
	return reductionModifier;
}

// set the value for reduction identifier
void OpenMPReductionClause::setReductionClauseIdentifier(OpenMPReductionClauseIdentifier v)
{
	reductionIdentifier = v;
}

// get reduction identifier value
OpenMPReductionClauseIdentifier OpenMPReductionClause::getReductionClauseIdentifier()
{
	return reductionIdentifier;
}

// Handling clauses predefined attributes
//
// PROC_BIND
// set the enum value proc_bind e.g. master or close or spread
void OpenMPProcBindClause::setProcBindClauseValue(OpenMPProcBindClauseKind v)
{
	proc_bindKind = v;
}

// get proc_bind value
OpenMPProcBindClauseKind OpenMPProcBindClause::getProcBindClauseValue()
{
	return proc_bindKind;
}

// ORDER
// set the value for order
void OpenMPOrderClause::setOrderClauseValue(OpenMPOrderClauseKind v)
{
	orderKind = v;
}

// get order value
OpenMPOrderClauseKind OpenMPOrderClause::getOrderClauseValue()
{
	return orderKind;
}

// DEFAULT
// set the value for default
void OpenMPDefaultClause::setDefaultClauseValue(OpenMPDefaultClauseKind v)
{
	defaultKind = v;
}

// get default value
OpenMPDefaultClauseKind OpenMPDefaultClause::getDefaultClauseValue()
{
	return defaultKind;
}

// 	IF
// set the value
void OpenMPIfClause::setIfClauseValue(OpenMPIfClauseKind v)
{
	ifKind = v;
}

// get value
OpenMPIfClauseKind OpenMPIfClause::getIfClauseValue()
{
	return ifKind;
}

// BIND
// set the value
void OpenMPBindClause::setBindClauseValue(OpenMPBindClauseKind v)
{
	bindKind = v;
}

// get value
OpenMPBindClauseKind OpenMPBindClause::getBindClauseValue()
{
	return bindKind;
}

// 	ALLOCATE ALLOCATORS
// set the value
void OpenMPAllocateClause::setAllocatorValue(OpenMPAllocateClauseKind v)
{
	allocator = v;
}

// get value
OpenMPAllocateClauseKind OpenMPAllocateClause::getAllocateClauseValue()
{
	return allocator;
}

// 	LASTPRIVATE
// set the value
void OpenMPLastprivateClause::setLastprivateValue(OpenMPLastprivateClauseKind v)
{
	lastPrivate = v;
}

// get value
OpenMPLastprivateClauseKind OpenMPLastprivateClause::getLastprivateClauseValue()
{
	return lastPrivate;
}

// 	SCHEDULE - 1st modifier paramater
// set the value
void OpenMPScheduleClause::setScheduleFirstModifier(OpenMPScheduleClauseModifier v)
{
	scheduleModifier1 = v;
}

// get value
OpenMPScheduleClauseModifier OpenMPScheduleClause::getScheduleFirstModifier()
{
	return scheduleModifier1;
}

// 	SCHEDULE - 2nd modifier paramater
// set the value
void OpenMPScheduleClause::setScheduleSecondModifier(OpenMPScheduleClauseModifier v)
{
	scheduleModifier2 = v;
}

// get valu
OpenMPScheduleClauseModifier OpenMPScheduleClause::getScheduleSecondModifier()
{
	return scheduleModifier2;
}

// 	SCHEDULE - 3rd paramater (schedule kind)
// set the value
void OpenMPScheduleClause::setScheduleKindValue(OpenMPScheduleClauseEnumKind v)
{
	scheduleKind = v;
}

// get value
OpenMPScheduleClauseEnumKind OpenMPScheduleClause::getScheduleKindValue()
{
	return scheduleKind;
}

// DEPEND
// set the value
void OpenMPDependClause::setDependDependenceType(OpenMPDependClauseKind v)
{
	dependType = v;
}

// get value
OpenMPDependClauseKind OpenMPDependClause::getDependDependenceType()
{
	return dependType;
}

// checks if a specific clause configuration exists or not
// if not, it creates it
std::vector<OpenMPClause *> *OpenMPDirective::findOpenMPClause(OpenMPClause *clause)
{
	// void OpenMPDirective::addOpenMPClause(OpenMPClause *clause)
	OpenMPClauseKind k = clause->getKind();
	std::vector<OpenMPClause *> *v_clauses;

	OpenMPClause *op_clause = OpenMPDirective::searchOpenMPClause(clause);

	auto it = clauses.find(k);
	if (op_clause == nullptr) // if absent, create
	{
		v_clauses->push_back(clause);
		clauses.insert(pair<OpenMPClauseKind, std::vector<OpenMPClause *> *>(k, v_clauses)); // insert fresh if does not exist
	}
	else
	{
		v_clauses = OpenMPDirective::getOpenMPClausesList(k);
		OpenMPClause * oop_clause = OpenMPDirective::mergeExpressions(op_clause, clause);
		v_clauses->push_back(oop_clause);
		clauses.insert(pair<OpenMPClauseKind, std::vector<OpenMPClause *> *>(k, v_clauses)); // insert into existing vector
	}

	return OpenMPDirective::getOpenMPClausesList(k);
}

// locate a specific instance of OpenMPClause item in vector and return it
// or return nothing if not found
OpenMPClause *OpenMPDirective::searchOpenMPClause(OpenMPClause *clause)
{
	OpenMPClause *clauseFound = nullptr;
	OpenMPClauseKind kind = clause->getKind();
	auto it = OpenMPDirective::getClauses().find(clause->getKind());

	switch (kind)
	{
	case OMPC_num_threads:
	{
		// TODO - add other clauses that require no parameters
		for (auto const &value : *it->second)
		{
			clauseFound = value; // return the only item in the vector
		}
	}
	break;
	case OMPC_schedule:
	{
		OpenMPScheduleClause *sched_clause = reinterpret_cast<OpenMPScheduleClause *>(clause);

		for (auto const &value : *it->second)
		{
			OpenMPScheduleClause *vec_clause = reinterpret_cast<OpenMPScheduleClause *>(value);
			if ((vec_clause->getKind() == sched_clause->getKind()) &&
				(vec_clause->getScheduleFirstModifier() == sched_clause->getScheduleFirstModifier()) &&
				(vec_clause->getScheduleSecondModifier() == sched_clause->getScheduleSecondModifier()) &&
				(vec_clause->getScheduleKindValue() == sched_clause->getScheduleKindValue()))
			{
				clauseFound = value;
				break;
			}
		}
	}
	break;
	case OMPC_reduction:
	{
		OpenMPReductionClause *red_clause = reinterpret_cast<OpenMPReductionClause *>(clause);

		for (auto const &value : *it->second)
		{
			OpenMPReductionClause *vec_clause = reinterpret_cast<OpenMPReductionClause *>(value);
			if ((vec_clause->getKind() == red_clause->getKind()) &&
				(vec_clause->getReductionClauseModifier() == red_clause->getReductionClauseModifier()) &&
				(vec_clause->getReductionClauseIdentifier() == red_clause->getReductionClauseIdentifier()) &&
				(vec_clause->getUserDefinedIdentifier() == red_clause->getUserDefinedIdentifier()))
			{
				clauseFound = value;
				break;
			}
		}
	}
	break;
	case OMPC_allocate:
	{
		OpenMPAllocateClause *alloc_clause = reinterpret_cast<OpenMPAllocateClause *>(clause);

		for (auto const &value : *it->second)
		{
			OpenMPAllocateClause *vec_clause = reinterpret_cast<OpenMPAllocateClause *>(value);
			if ((vec_clause->getKind() == alloc_clause->getKind()) &&
				(vec_clause->getAllocateClauseValue() == alloc_clause->getAllocateClauseValue()) &&
				(vec_clause->getUserDefinedAllocator() == alloc_clause->getUserDefinedAllocator()))
			{
				clauseFound = value;
				break;
			}
		}
	}
	break;

	default:
		break;
	}
	return clauseFound;
}

// get vector<OpenMPClause> list of all clauses of a certain OpenMPClauseKind
vector<OpenMPClause *> *OpenMPDirective::getOpenMPClausesList(OpenMPClauseKind kind)
{
	OpenMPClause *clause = nullptr;
	auto it = OpenMPDirective::getClauses().find(kind);

	return it->second;
}

// =========================================================================

OpenMPDirectiveKind OpenMPDirective::getKind() { return kind; }

// get all clauses for the directive
map<OpenMPClauseKind, std::vector<OpenMPClause *> *> OpenMPDirective::getClauses()
{
	return clauses;
}

// merge expressions
OpenMPClause * OpenMPDirective::mergeExpressions(OpenMPClause * vector_clause, OpenMPClause * new_clause) 
{ 

	// append expressions and variables, if any
	std::vector<const char*> expr = (*new_clause).getExpressions();
	if (expr.size() > 0) 
	{				
		for(auto const& value: expr) 
		{
			vector_clause->addExpression(value);
		}			
	}

	return vector_clause;
}

/* generate DOT representation of the directive */
void OpenMPDirective::generateDOT()
{

	std::string filename;
	filename.append(this->getLabel()).append(".dot");
	std::cout << "Generating dot file " << filename << std::endl;
	int i = 0; // for auto-incrementing clauses graph IDs

	// open a file in write mode.
	ofstream outfile;
	outfile.open(filename);

	// start line to the file
	outfile << "digraph G {" << endl;

	if (clauses.size() > 0)
	{
		for (auto const &value : clauses)
		{
			//			outfile << "\t" << this->label << " -> " << value.second->getLabel() << "_" << i++ << ";" << endl;
		}
	}
	// close line to the file
	outfile << "}" << endl;

	// close the opened file.
	outfile.close();
	std::cout << "Generated dot file!\n";
}
