//
// Created by Yonghong Yan on 6/15/18.
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
    std::string label;
	
    public:
    OpenMPClause(OpenMPClauseKind k) : kind(k) {};
    OpenMPClauseKind const getClauseKind() { return kind; }

    /* a list of language expressions, variables, etc that are not parsed by the ompparser */

    void addLangExpr(const char * expr) { lang_expr.push_back(expr); }
    
    std::vector<const char*>* getExpr() {
       return &lang_expr;
    } 

		OpenMPClauseKind getKind() { return kind; }
		
		void setLabel(std::string name) { label = name; }
		std::string getLabel() { return label; }
};

// clauses that accept modifier, identifier
// e.g. reduction clause of parallel directive
class OpenMPIdentifierModifierClause : OpenMPClause {

	protected:
		OpenMPReductionClauseModifier reductionModifier;	
		OpenMPReductionClauseIdentifier reductionIdentifier;
		
	public:
	
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
class OpenMPEnumClause : OpenMPClause {

	protected:
		union clauseOptions
		{	
			OpenMPProcBindClauseKind proc_bindKind;
			OpenMPDefaultClauseKind defaultKind;
		};
		
		clauseOptions clauseItem;
		
    public:

		// set the value for proc_bind e.g. master or close or spread
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
