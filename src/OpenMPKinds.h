//===--- OpenMPKinds.h - OpenMP enums ---------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Defines some OpenMP-specific enums and functions.
///
//===----------------------------------------------------------------------===//

#ifndef __OPENMPKINDS_H__
#define __OPENMPKINDS_H__

/// OpenMP directives.
enum OpenMPDirectiveKind {
#define OPENMP_DIRECTIVE(Name) OMPD_##Name,
#define OPENMP_DIRECTIVE_EXT(Name, Str) OMPD_##Name,
    OPENMP_DIRECTIVE(parallel)
    OPENMP_DIRECTIVE(unknown)
#undef OPENMP_DIRECTIVE
#undef OPENMP_DIRECTIVE_EXT
};

/// OpenMP clauses.
enum OpenMPClauseKind {
#define OPENMP_CLAUSE(Name, Class) OMPC_##Name,
// OpenMP clauses.
    OPENMP_CLAUSE(if, OMPIfClause)
    OPENMP_CLAUSE(num_threads, OMPNumThreadsClause)
    OPENMP_CLAUSE(default, OMPDefaultClause)
    OPENMP_CLAUSE(private, OMPPrivateClause)
    OPENMP_CLAUSE(firstprivate, OMPFirstprivateClause)
    OPENMP_CLAUSE(shared,  OMPSharedClause)
    OPENMP_CLAUSE(copyin,  OMPCopyinClause)
    OPENMP_CLAUSE(reduction,  OMPReductionClause)
    OPENMP_CLAUSE(proc_bind, OMPProcBindClause)
    OPENMP_CLAUSE(allocate, OMPAllocateClause)

    OPENMP_CLAUSE(unknown, OMPUnknownClause)
#undef OPENMP_CLAUSE
};

/// OpenMP attributes for 'if' clause.
enum OpenMPIfClauseKind {
#define OPENMP_IF_KIND(Name) OMPC_IF_##Name,
    OPENMP_IF_KIND(parallel)
    OPENMP_IF_KIND(simd)
    OPENMP_IF_KIND(task)

    OPENMP_IF_KIND(unknown)
#undef OPENMP_IF_KIND
};

/// OpenMP attributes for 'default' clause.
enum OpenMPDefaultClauseKind {
#define OPENMP_DEFAULT_KIND(Name) OMPC_DEFAULT_##Name,
    /* private and firstprivate are only for fortran */
    OPENMP_DEFAULT_KIND(private)
    OPENMP_DEFAULT_KIND(firstprivate)
    OPENMP_DEFAULT_KIND(shared)
    OPENMP_DEFAULT_KIND(none)

    OPENMP_DEFAULT_KIND(unknown)
#undef OPENMP_DEFAULT_KIND
};

/// OpenMP attributes for 'proc_bind' clause.
enum OpenMPProcBindClauseKind {
#define OPENMP_PROC_BIND_KIND(Name) OMPC_PROC_BIND_##Name,
    OPENMP_PROC_BIND_KIND(master)
    OPENMP_PROC_BIND_KIND(close)
    OPENMP_PROC_BIND_KIND(spread)

    OPENMP_PROC_BIND_KIND(unknown)
#undef OPENMP_PROC_BIND_KIND
};

/// OpenMP attributes for 'Allocate' clause.
enum OpenMPAllocateClauseAllocator {
#define OPENMP_ALLOCATE_ALLOCATOR(Name) OMPC_ALLOCATE_ALLOCATOR_##Name,
    OPENMP_ALLOCATE_ALLOCATOR_KIND(default)	  	    // omp_default_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(large_cap)		// omp_large_cap_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(cons_mem)		// omp_const_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(high_bw) 		// omp_high_bw_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(low_lat)			// omp_low_lat_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(cgroup)			// omp_cgroup_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(pteam)			// omp_pteam_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(thread)			// omp_thread_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(user)			// user-defined allocator
    OPENMP_ALLOCATE_ALLOCATOR_KIND(unknown)
#undef OPENMP_ALLOCATE_ALLOCATOR_KIND
};

/// modifiers for 'reduction' clause.
enum OpenMPReductionClauseModifier {
#define OPENMP_REDUCTION_MODIFIER(Name) OMPC_REDUCTION_MODIFIER_##Name,
    OPENMP_REDUCTION_MODIFIER(inscan)
    OPENMP_REDUCTION_MODIFIER(task)
    OPENMP_REDUCTION_MODIFIER(default)
#undef OPENMP_REDUCTION_MODIFIER
};

/// identifiers for 'reduction' clause.
enum OpenMPReductionClauseIdentifier {
#define OPENMP_REDUCTION_IDENTIFIER(Name) OMPC_REDUCTION_IDENTIFIER_##Name,
    OPENMP_REDUCTION_IDENTIFIER(reduction_plus)
    OPENMP_REDUCTION_IDENTIFIER(reduction_minus)
    OPENMP_REDUCTION_IDENTIFIER(reduction_mul)
    OPENMP_REDUCTION_IDENTIFIER(reduction_bitand)
    OPENMP_REDUCTION_IDENTIFIER(reduction_bitor)
    OPENMP_REDUCTION_IDENTIFIER(reduction_bitxor)
    OPENMP_REDUCTION_IDENTIFIER(reduction_logand)
    OPENMP_REDUCTION_IDENTIFIER(reduction_logor)
    OPENMP_REDUCTION_IDENTIFIER(reduction_eqv) /* only for Fortran */
    OPENMP_REDUCTION_IDENTIFIER(reduction_neqv) /* only for Fortran */
    OPENMP_REDUCTION_IDENTIFIER(reduction_max)
    OPENMP_REDUCTION_IDENTIFIER(reduction_min)
    OPENMP_REDUCTION_IDENTIFIER(user)
#undef OPENMP_REDUCTION_IDENTIFIER
};

#endif

