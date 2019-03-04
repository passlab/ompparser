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
    OPENMP_DIRECTIVE(for) 
    OPENMP_DIRECTIVE(simd)/*YAYING*/
    OPENMP_DIRECTIVE(metadirective)

    OPENMP_DIRECTIVE(unknown)
    OPENMP_DIRECTIVE(teams)
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
    OPENMP_CLAUSE(num_teams, OMPNumTeamsClause)
    OPENMP_CLAUSE(thread_limit, OMPThreadLimitClause)

    OPENMP_CLAUSE(lastprivate, OMPLastprivateClause)
    OPENMP_CLAUSE(collapse, OMPCollapseClause)
    OPENMP_CLAUSE(ordered, OMPOrderedClause)
    OPENMP_CLAUSE(nowait, OMPNowaitClause)
    OPENMP_CLAUSE(order, OMPOrderClause)
    OPENMP_CLAUSE(linear, OMPLinearClause)
    OPENMP_CLAUSE(schedule, OMPScheduleClause)

    OPENMP_CLAUSE(safelen, OMPSafelenClause)
    OPENMP_CLAUSE(simdlen, OMPSimdlenClause)
    OPENMP_CLAUSE(aligned, OMPAlignedClause)
    OPENMP_CLAUSE(nontemporal, OMPNontemporalClause)

// OpenMP clause for MetaDirective
    OPENMP_CLAUSE(when, OMPWhenClause)

    OPENMP_CLAUSE(unknown, OMPUnknownClause)
#undef OPENMP_CLAUSE
};

/// OpenMP attributes for 'if' clause.
enum OpenMPIfClauseKind {
#define OPENMP_IF_KIND(Name) OMPC_IF_##Name,
    OPENMP_IF_KIND(parallel)
    OPENMP_IF_KIND(simd)
    OPENMP_IF_KIND(task)

    OPENMP_IF_KIND(unspecified)
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
#define OPENMP_ALLOCATE_ALLOCATOR_KIND(Name) OMPC_ALLOCATE_ALLOCATOR_##Name,
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
    OPENMP_REDUCTION_IDENTIFIER(plus)
    OPENMP_REDUCTION_IDENTIFIER(minus)
    OPENMP_REDUCTION_IDENTIFIER(mul)
    OPENMP_REDUCTION_IDENTIFIER(bitand)
    OPENMP_REDUCTION_IDENTIFIER(bitor)
    OPENMP_REDUCTION_IDENTIFIER(bitxor)
    OPENMP_REDUCTION_IDENTIFIER(logand)
    OPENMP_REDUCTION_IDENTIFIER(logor)
    OPENMP_REDUCTION_IDENTIFIER(eqv) /* only for Fortran */
    OPENMP_REDUCTION_IDENTIFIER(neqv) /* only for Fortran */
    OPENMP_REDUCTION_IDENTIFIER(max)
    OPENMP_REDUCTION_IDENTIFIER(min)
    OPENMP_REDUCTION_IDENTIFIER(user)

    OPENMP_REDUCTION_IDENTIFIER(unknown)
#undef OPENMP_REDUCTION_IDENTIFIER
};

/// modifiers for 'lastprivate' clause.
enum OpenMPLastprivateClauseModifier {
#define OPENMP_LASTPRIVATE_MODIFIER(Name) OMPC_LASTPRIVATE_MODIFIER_##Name,

    OPENMP_LASTPRIVATE_MODIFIER(conditional)
    OPENMP_LASTPRIVATE_MODIFIER(user)

    OPENMP_LASTPRIVATE_MODIFIER(unknow)
#undef OPENMP_LASTPRIVATE_MODIFIER
};

/// modifiers for 'linear' clause.
enum OpenMPLinearClauseModifier {
#define OPENMP_LINEAR_MODIFIER(Name) OMPC_LINEAR_MODIFIER_##Name,

    OPENMP_LINEAR_MODIFIER(val)
    OPENMP_LINEAR_MODIFIER(ref)
    OPENMP_LINEAR_MODIFIER(uval)
    OPENMP_LINEAR_MODIFIER(user)

    OPENMP_LINEAR_MODIFIER(unknow)
#undef OPENMP_LINEAR_MODIFIER
};

/// modifiers for 'schedule' clause.
enum OpenMPScheduleClauseModifier {
#define OPENMP_SCHEDULE_MODIFIER(Name) OMPC_SCHEDULE_MODIFIER_##Name,
    OPENMP_SCHEDULE_MODIFIER(monotonic)
    OPENMP_SCHEDULE_MODIFIER(nonmonotonic)
    OPENMP_SCHEDULE_MODIFIER(simd)
    OPENMP_SCHEDULE_MODIFIER(user)

    OPENMP_SCHEDULE_MODIFIER(unknow)
#undef OPENMP_SCHEDULE_MODIFIER
};

/// OpenMP attributes for 'schedule' clause.
enum OpenMPScheduleClauseKind {
#define OPENMP_SCHEDULE_KIND(Name) OMPC_SCHEDULE_KIND_##Name,

    OPENMP_SCHEDULE_KIND(static)
    OPENMP_SCHEDULE_KIND(dynamic)
    OPENMP_SCHEDULE_KIND(guided)
    OPENMP_SCHEDULE_KIND(auto)
    OPENMP_SCHEDULE_KIND(runtime)
    OPENMP_SCHEDULE_KIND(user)

    OPENMP_SCHEDULE_KIND(unknown)
#undef OPENMP_SCHEDULE_KIND
};
#endif

