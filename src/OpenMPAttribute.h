//
// Created by Yonghong Yan on 6/15/18.
// Modified by Chrisogonas
//

#ifndef OMPPARSER_OPENMPATTRIBUTE_H_H
#define OMPPARSER_OPENMPATTRIBUTE_H_H

#include <fstream>
#include <iostream>

#include <OpenMPKinds.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

class OpenMPClause {
    
    protected:
		OpenMPClauseKind kind;
		std::vector<const char*> lang_expr;
		std::string label; // clause name/label
		
		// user-defined parameters
		std::string customFirstParameter;
		std::string customSecondParameter;

		// first and second parameters e.g. modifiers and identifiers in reduction clause
		// most of openmp clauses have 2 paramaters
		//
		union firstParameter
		{			
			OpenMPReductionClauseModifier reductionModifier; 		// reduction
			OpenMPAllocateClauseKind allocator; 					// Allocate allocator
			OpenMPLastprivateClauseKind lastPrivate;				// Allocate allocator
			OpenMPProcBindClauseKind proc_bindKind;					// proc_bind
			OpenMPDefaultClauseKind defaultKind;					// default
			OpenMPIfClauseKind ifKind;								// if
			OpenMPOrderClauseKind orderKind;						// order
			OpenMPScheduleClauseModifier scheduleModifier;			// schedule 1st modifier e.g. in case of 'for' directive
		};
		
		union secondParameter
		{
			OpenMPReductionClauseIdentifier reductionIdentifier;	// reduction
			OpenMPScheduleClauseModifier scheduleModifier;			// schedule 2nd modifier e.g. in case of 'for' directive
		};
		
		union thirdParameter
		{
			OpenMPScheduleClauseEnumKind scheduleKind;					// schedule 3rd paramater e.g. in case of 'for' directive
		};
		
		firstParameter first_parameter;
		secondParameter second_parameter;
		thirdParameter third_parameter;
	
    public:
		OpenMPClause(OpenMPClauseKind k) : kind(k) {};
		OpenMPClauseKind const getClauseKind() { return kind; }

		/* a list of language expressions, variables, etc that are not parsed by the ompparser */
		void addLangExpr(const char * expr) { lang_expr.push_back(expr); }
		
		std::vector<const char*>* getExpr() {
		   return &lang_expr;
		} 

		OpenMPClauseKind getKind() { return kind; }
		
		// clause name/label
		void setLabel(std::string name) { label = name; }
		std::string getLabel() { return label; }
		
		// handling first and second parameters
		//
		// user-defined paramaters
		// first paramater		
		void setCustomFirstParameter(std::string value) { customFirstParameter = value; }
		std::string getCustomFirstParameter() { return customFirstParameter; }

		// second paramater
		void setCustomSecondParameter(std::string value) { customSecondParameter = value; }
		std::string getCustomSecondParameter() { return customSecondParameter; }
		
		// REDUCTION
		// set the value for reduction modifier
		void setReductionClauseModifier(OpenMPReductionClauseModifier v) {
			first_parameter.reductionModifier = v;
		}
		
		// get reduction modifier value
		OpenMPReductionClauseModifier getReductionClauseModifier() {
			return first_parameter.reductionModifier;
		}
		
		// set the value for reduction identifier
		void setReductionClauseIdentifier(OpenMPReductionClauseIdentifier v) {
			second_parameter.reductionIdentifier = v;
		}
		
		// get reduction identifier value
		OpenMPReductionClauseIdentifier getReductionClauseIdentifier() {
			return second_parameter.reductionIdentifier;
		}
		
		
		// Handling clauses predefined attributes
		//
		// PROC_BIND
		// set the enum value proc_bind e.g. master or close or spread
		void setProcBindClauseValue(OpenMPProcBindClauseKind v) {
			first_parameter.proc_bindKind = v;
		}
		
		// get proc_bind value
		OpenMPProcBindClauseKind getProcBindClauseValue() {
			return first_parameter.proc_bindKind;
		}

		// ORDER
		// set the value for order
		void setOrderClauseValue(OpenMPOrderClauseKind v) {
			first_parameter.orderKind = v;
		}

		// get order value
		OpenMPOrderClauseKind getOrderClauseValue() {
			return first_parameter.orderKind;
		}
		
		// DEFAULT
		// set the value for default
		void setDefaultClauseValue(OpenMPDefaultClauseKind v) {
			first_parameter.defaultKind = v;
		}

		// get default value
		OpenMPDefaultClauseKind getDefaultClauseValue() {
			return first_parameter.defaultKind;
		}
		
		// 	IF
		// set the value 
		void setIfClauseValue(OpenMPIfClauseKind v) {
			first_parameter.ifKind = v;
		}

		// get value
		OpenMPIfClauseKind getIfClauseValue() {
			return first_parameter.ifKind;
		}
		
		// 	ALLOCATE ALLOCATORS
		// set the value
		void setAllocatorValue(OpenMPAllocateClauseKind v) {
			first_parameter.allocator = v;
		}

		// get value
		OpenMPAllocateClauseKind getAllocateClauseValue() {
			return first_parameter.allocator;
		}
		
		// 	LASTPRIVATE
		// set the value
		void setLastprivateValue(OpenMPLastprivateClauseKind v) {
			first_parameter.lastPrivate = v;
		}

		// get value
		OpenMPLastprivateClauseKind getLastprivateClauseValue() {
			return first_parameter.lastPrivate;
		}
		
		// 	SCHEDULE - 1st modifier paramater
		// set the value
		void setScheduleFirstModifier(OpenMPScheduleClauseModifier v) {
			first_parameter.scheduleModifier = v;
		}

		// get value
		OpenMPScheduleClauseModifier getScheduleFirstModifier() {
			return first_parameter.scheduleModifier;
		}
		
		// 	SCHEDULE - 2nd modifier paramater
		// set the value
		void setScheduleSecondModifier(OpenMPScheduleClauseModifier v) {
			second_parameter.scheduleModifier = v;
		}

		// get value
		OpenMPScheduleClauseModifier getScheduleSecondModifier() {
			return second_parameter.scheduleModifier;
		}
		
		// 	SCHEDULE - 3rd paramater (schedule kind)
		// set the value
		void setScheduleKindValue(OpenMPScheduleClauseEnumKind v) {
			third_parameter.scheduleKind = v;
		}

		// get value
		OpenMPScheduleClauseEnumKind getScheduleKindValue() {
			return third_parameter.scheduleKind;
		}
};

class OpenMPDirective {

    protected:
		OpenMPDirectiveKind kind;
		std::vector<OpenMPClause*> clauses;
		std::string label;
	
    public:
		OpenMPDirective(OpenMPDirectiveKind k) : kind(k) {};

		OpenMPDirectiveKind const getDirectiveKind() { return kind; }

		void setLabel(std::string name) { label = name; }
		std::string getLabel() { return label; }
		
		void addClause(OpenMPClause * clause) { clauses.push_back(clause);}

		OpenMPDirectiveKind getKind() { return kind; }

		std::vector<OpenMPClause*>* getClauses() {
			return &clauses;
		}

		/* generate DOT representation of the directive */
		void generateDOT() {

			std::string filename;
			filename.append(this->getLabel()).append(".dot");
			std::cout << "Generating dot file " << filename << std::endl;
			int i = 0; // for auto-incrementing clauses graph IDs

			// open a file in write mode.
			ofstream outfile;
			outfile.open(filename);
		   
			// start line to the file 
			outfile << "digraph G {" << endl;	   
			
			if (clauses.size() > 0) {
				for(auto const& value: clauses) {
					//outfile << "\t" << kind << " -> " << value->getKind() << ";" << endl;
					outfile << "\t" << this->label << " -> " << value->getLabel() << "_" << i++ << ";" << endl;
				}
			}
			// close line to the file 
			outfile << "}" << endl;			
		
			// close the opened file.
			outfile.close();		
			std::cout << "Generated dot file!\n";
		}
};

#endif //OMPPARSER_OPENMPATTRIBUTE_H_H

/*
// clauses that accept modifier, identifier
// e.g. reduction clause of parallel directive
class OpenMPIdentifierModifierClause : public OpenMPClause {

	protected:
		OpenMPReductionClauseModifier reductionModifier;	
		OpenMPReductionClauseIdentifier reductionIdentifier;
		
	public:
	
		OpenMPIdentifierModifierClause(OpenMPClauseKind kind): OpenMPClause(kind) {};
		
		// set the value for reduction modifier
		void setReductionClauseModifier(OpenMPReductionClauseModifier v) {
			reductionModifier = v;
		}
		
		// get reduction modifier value
		OpenMPReductionClauseModifier getReductionClauseModifier() {
			return reductionModifier;
		}
		
		// set the value for reduction identifier
		void setReductionClauseIdentifier(OpenMPReductionClauseIdentifier v) {
			reductionIdentifier = v;
		}
		
		// get reduction identifier value
		OpenMPReductionClauseIdentifier getReductionClauseIdentifier() {
			return reductionIdentifier;
		}
	
};

// clauses that accept predefined values
// e.g. proc_bind and shared clauses of parallel directive
class OpenMPEnumClause : public OpenMPClause {

	protected:
		union clauseOptions
		{	
			OpenMPProcBindClauseKind proc_bindKind;
			OpenMPDefaultClauseKind defaultKind;			
		};
		
		string allocator; 
		clauseOptions clauseItem;
		
    public:

		OpenMPEnumClause(OpenMPClauseKind kind): OpenMPClause(kind) {};
		
		// set the enum value proc_bind e.g. master or close or spread
		void setProcBindClauseValue(OpenMPProcBindClauseKind v) {
			clauseItem.proc_bindKind = v;
		}
		
		// get proc_bind value
		OpenMPProcBindClauseKind getProcBindClauseValue() {
			return clauseItem.proc_bindKind;
		}
		
		// set the value for default
		void setDefaultClauseValue(OpenMPDefaultClauseKind v) {
			clauseItem.defaultKind = v;
		}

		// get default value
		OpenMPDefaultClauseKind getDefaultClauseValue() {
			return clauseItem.defaultKind;
		}
	
};
*/