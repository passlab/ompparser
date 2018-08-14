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

// handling allocators
//

// The memspace argument must be one of the predefined memory spaces below:
typedef enum {
	omp_default_mem_space, // Represents the system default storage.
	omp_large_cap_mem_space, // Represents storage with large capacity.
	omp_const_mem_space, // Represents storage optimized for variables with constant values. The result of writing to this storage is unspecified.
	omp_high_bw_mem_space, // Represents storage with high bandwidth.
	omp_low_lat_mem_space // Represents storage with low latency.
} omp_memspace_t;

// Allocator traits
typedef struct {
	enum sync_hint {
		contended, // default
		uncontended,
		serialized, 
		sequential		
	};
	int alignment; // A positive integer value which is a power of 2, default is 1 byte
	enum access {
		all, // default
		cgroup,
		pteam,
		thread
	};
	int pool_size; // Positive integer value; it is implementation-defined
	enum fallback {
		default_mem_fb, // default
		null_fb,
		abort_fb, 
		allocator_fb
	};
	int fb_data; // an allocator handle - OpenMPCustomAllocator
	bool pinned = false; // default is false
	enum partition {
		environment, // default
		nearest,
		blocked,
		interleaved
	};
} traits;

typedef enum {
	OMP_ATK_THREADMODEL = 1,
	OMP_ATK_ALIGNMENT = 2,
	OMP_ATK_ACCESS = 3,
	OMP_ATK_POOL_SIZE = 4,
	OMP_ATK_FALLBACK = 5,
	OMP_ATK_FB_DATA = 6,
	OMP_ATK_PINNED = 7,
	OMP_ATK_PARTITION = 8
} omp_alloctrait_key_t;

typedef enum {
	OMP_ATV_FALSE = 0,
	OMP_ATV_TRUE = 1,
	OMP_ATV_DEFAULT = 2,
	OMP_ATV_CONTENDED = 3,
	OMP_ATV_UNCONTENDED = 4,
	OMP_ATV_SEQUENTIAL = 5,
	OMP_ATV_PRIVATE = 6,
	OMP_ATV_ALL = 7,
	OMP_ATV_THREAD = 8,
	OMP_ATV_PTEAM = 9,
	OMP_ATV_CGROUP = 10,
	OMP_ATV_DEFAULT_MEM_FB = 11,
	OMP_ATV_NULL_FB = 12,
	OMP_ATV_ABORT_FB = 13,
	OMP_ATV_ALLOCATOR_FB = 14,
	OMP_ATV_ENVIRONMENT = 15,
	OMP_ATV_NEAREST = 16,
	OMP_ATV_BLOCKED = 17,
	OMP_ATV_INTERLEAVED = 18
} omp_alloctrait_value_t;

typedef struct {
	omp_alloctrait_key_t key;
	uint omp_uintptr_t; // omp_uintptr_t is an unsigned integer type capable of holding a pointer.
} omp_alloctrait_t;

enum { OMP_NULL_ALLOCATOR = NULL };

typedef struct {
	omp_memspace_t memspace_t;
	int ntraits;
	omp_alloctrait_t traits_params;
} omp_allocator_t;

/*
// Predefined Allocators
// ---------------------
omp_default_mem_alloc
omp_large_cap_mem_alloc
omp_const_mem_alloc
omp_high_bw_mem_alloc
omp_low_lat_mem_alloc
omp_cgroup_mem_alloc
omp_pteam_mem_alloc
omp_thread_mem_alloc
*/

// allocators can take default values or they can be specifically implemented as custom allocators 
// memspace are predefined values, ntraits specifies number of traits, traits is a list of allocator attributes
// This class is for user defined allocators
// TODO - Fully implement this class
class OpenMPCustomAllocator {
	
	protected:
		omp_allocator_t *allocator;
		
	public:
		OpenMPCustomAllocator() {}
		OpenMPCustomAllocator (const omp_memspace_t *memspace, const int ntraits, const omp_alloctrait_t traits[]) {
			allocator->memspace_t = omp_large_cap_mem_space; //memspace;
			allocator->ntraits = ntraits; // number of traits
			// TODO - add traits here
			// 
		}
   
};

#endif //OMPPARSER_OPENMPATTRIBUTE_H_H
