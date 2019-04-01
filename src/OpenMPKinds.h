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
    OPENMP_DIRECTIVE(simd)
    OPENMP_DIRECTIVE(for_simd)
    OPENMP_DIRECTIVE(declare)
    OPENMP_DIRECTIVE(distribute)
    OPENMP_DIRECTIVE(distribute_simd)
    OPENMP_DIRECTIVE(distribute_parallel_for)
    OPENMP_DIRECTIVE(distribute_parallel_for_simd)
    OPENMP_DIRECTIVE(loop)
    OPENMP_DIRECTIVE(scan)
    OPENMP_DIRECTIVE(sections)
    OPENMP_DIRECTIVE(section)
    OPENMP_DIRECTIVE(single) 
    OPENMP_DIRECTIVE(cancel)
    OPENMP_DIRECTIVE(cancellation_point)
    OPENMP_DIRECTIVE(allocate)      
    OPENMP_DIRECTIVE(threadprivate)      /*YAYING*/
    OPENMP_DIRECTIVE(metadirective)
    OPENMP_DIRECTIVE(declare_variant)
    OPENMP_DIRECTIVE(task)
    OPENMP_DIRECTIVE(taskloop)
    OPENMP_DIRECTIVE(taskloop_simd)
    OPENMP_DIRECTIVE(taskyield)
    OPENMP_DIRECTIVE(requires)
    OPENMP_DIRECTIVE(target_data)
    OPENMP_DIRECTIVE(target_enter_data)
    OPENMP_DIRECTIVE(target_update)
    OPENMP_DIRECTIVE(target_exit_data)
    OPENMP_DIRECTIVE(target)

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

    OPENMP_CLAUSE(uniform, OMPUniformClause)
    OPENMP_CLAUSE(inbranch, OMPInbranchClause)
    OPENMP_CLAUSE(notinbranch, OMPNotinbranchClause)

    OPENMP_CLAUSE(dist_schedule, OMPDistscheduleClause)

    OPENMP_CLAUSE(bind, OMPBindClause)

    OPENMP_CLAUSE(inclusive, OMPInclusiveClause)
    OPENMP_CLAUSE(exclusive, OMPExclusiveClause)

    OPENMP_CLAUSE(copyprivate, OMPCopyprivateClause)
    OPENMP_CLAUSE(parallel, OMPParallelClause)
    OPENMP_CLAUSE(sections, OMPSectionsClause)
    OPENMP_CLAUSE(for, OMPForClause)
    OPENMP_CLAUSE(taskgroup, OMPTaskgroupClause)

    OPENMP_CLAUSE(allocator, OMPAllocatorClause)

    OPENMP_CLAUSE(final, OMPFinalClause)
    OPENMP_CLAUSE(untied, OMPUntiedClause)
    OPENMP_CLAUSE(requires, OMPRequiresClause)
    OPENMP_CLAUSE(mergeable, OMPMergeableClause)
    OPENMP_CLAUSE(in_reduction, OMPInReductionClause)
    OPENMP_CLAUSE(depend, OMPDependClause)
    OPENMP_CLAUSE(priority, OMPPriorityClause)
    OPENMP_CLAUSE(affinity, OMPAffinityClause)
    OPENMP_CLAUSE(detach, OMPDetachClause)

    OPENMP_CLAUSE(grainsize, OMPGrainsizeClause)
    OPENMP_CLAUSE(num_tasks, OMPNumTasksClause)
    OPENMP_CLAUSE(nogroup, OMPNogroupClause)

    OPENMP_CLAUSE(reverse_offload, OMPReverse_OffloadClause)
    OPENMP_CLAUSE(unified_address, OMPUnifiedAddressClause)
    OPENMP_CLAUSE(unified_shared_memory, OMPUnifiedShared_memoryClause)
    OPENMP_CLAUSE(atomic_default_mem_order, OMPAtomicDefaultMemOrderClause)
    OPENMP_CLAUSE(dynamic_allocators, OMPDynamicAllocatorsClause)

    OPENMP_CLAUSE(device, OMPDeviceClause)
    OPENMP_CLAUSE(use_device_ptr, OMPUseDevicePtrClause)
    OPENMP_CLAUSE(use_device_addr, OMPUseDeviceAddrClause)
    OPENMP_CLAUSE(is_device_ptr, OMPIsDevicePtrClause)

    OPENMP_CLAUSE(defaultmap, OMPDefaultmapClause)
    OPENMP_CLAUSE(to, OMPToClause)
    OPENMP_CLAUSE(from, OMPFromClause)
    OPENMP_CLAUSE(uses_allocators, OMPUsesAllocatorsClause)
// OpenMP clause for MetaDirective
    OPENMP_CLAUSE(when, OMPWhenClause)

    OPENMP_CLAUSE(match, OMPMatchClause)

    OPENMP_CLAUSE(unknown, OMPUnknownClause)
#undef OPENMP_CLAUSE
};

// context selector set for 'when' clause.
enum OpenMPWhenClauseSelectorSet {
#define OPENMP_WHEN_SELECTOR_SET(Name) OMPC_WHEN_SELECTOR_SET_##Name,
    OPENMP_WHEN_SELECTOR_SET(user)
    OPENMP_WHEN_SELECTOR_SET(construct)
    OPENMP_WHEN_SELECTOR_SET(device)
    OPENMP_WHEN_SELECTOR_SET(implementation)
    OPENMP_WHEN_SELECTOR_SET(unknown)
#undef OPENMP_WHEN_SELECTOR
};

// context selector for 'when' clause.
enum OpenMPWhenClauseSelectorParameter {
#define OPENMP_WHEN_SELECTOR_PARAMETER(Name) OMPC_WHEN_SELECTOR_PARAMETER_##Name,
    OPENMP_WHEN_SELECTOR_PARAMETER(condition)
    OPENMP_WHEN_SELECTOR_PARAMETER(score)
    OPENMP_WHEN_SELECTOR_PARAMETER(unknown)
#undef OPENMP_WHEN_SELECTOR_PARAMETER
};

// context selector for 'when' clause.
enum OpenMPClauseContextKind {
#define OPENMP_CONTEXT_KIND(Name) OMPC_CONTEXT_KIND_##Name,
    OPENMP_CONTEXT_KIND(host)
    OPENMP_CONTEXT_KIND(nohost)
    OPENMP_CONTEXT_KIND(any)
    OPENMP_CONTEXT_KIND(cpu)
    OPENMP_CONTEXT_KIND(gpu)
    OPENMP_CONTEXT_KIND(fpga)
    OPENMP_CONTEXT_KIND(unknown)
#undef OPENMP_CONTEXT_KIND
};

// context selector for 'when' clause.
enum OpenMPClauseContextVendor {
#define OPENMP_CONTEXT_VENDOR(Name) OMPC_CONTEXT_VENDOR_##Name,
    OPENMP_CONTEXT_VENDOR(amd)
    OPENMP_CONTEXT_VENDOR(arm)
    OPENMP_CONTEXT_VENDOR(bsc)
    OPENMP_CONTEXT_VENDOR(cray)
    OPENMP_CONTEXT_VENDOR(fujitsu)
    OPENMP_CONTEXT_VENDOR(gnu)
    OPENMP_CONTEXT_VENDOR(ibm)
    OPENMP_CONTEXT_VENDOR(intel)
    OPENMP_CONTEXT_VENDOR(llvm)
    OPENMP_CONTEXT_VENDOR(pgi)
    OPENMP_CONTEXT_VENDOR(ti)
    OPENMP_CONTEXT_VENDOR(user)
    OPENMP_CONTEXT_VENDOR(unknown)
    OPENMP_CONTEXT_VENDOR(unspecified) // default enum for vendor list
#undef OPENMP_CONTEXT_VENDOR
};


// OpenMP attributes for 'if' clause.
enum OpenMPIfClauseModifier {
#define OPENMP_IF_MODIFIER(Name) OMPC_IF_MODIFIER_##Name,
    OPENMP_IF_MODIFIER(parallel)
    OPENMP_IF_MODIFIER(simd)
    OPENMP_IF_MODIFIER(task)
    OPENMP_IF_MODIFIER(cancel)
    OPENMP_IF_MODIFIER(target_data)
    OPENMP_IF_MODIFIER(target_enter_data)
    OPENMP_IF_MODIFIER(target_exit_data)
    OPENMP_IF_MODIFIER(target)
    OPENMP_IF_MODIFIER(target_update)

    OPENMP_IF_MODIFIER(unspecified)
    OPENMP_IF_MODIFIER(unknown)
    OPENMP_IF_MODIFIER(user)
#undef OPENMP_IF_MODIFIER
};

/// OpenMP attributes for 'default' clause.
enum OpenMPDefaultClauseKind {
#define OPENMP_DEFAULT_KIND(Name) OMPC_DEFAULT_##Name,
    /* private and firstprivate are only for fortran */
    OPENMP_DEFAULT_KIND(private)
    OPENMP_DEFAULT_KIND(firstprivate)
    OPENMP_DEFAULT_KIND(shared)
    OPENMP_DEFAULT_KIND(none)
    OPENMP_DEFAULT_KIND(variant)

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
    OPENMP_ALLOCATE_ALLOCATOR_KIND(default)      // omp_default_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(large_cap)// omp_large_cap_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(cons_mem)// omp_const_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(high_bw) // omp_high_bw_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(low_lat)// omp_low_lat_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(cgroup)// omp_cgroup_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(pteam)// omp_pteam_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(thread)// omp_thread_mem_alloc
    OPENMP_ALLOCATE_ALLOCATOR_KIND(user)// user-defined allocator
    OPENMP_ALLOCATE_ALLOCATOR_KIND(unknown)
#undef OPENMP_ALLOCATE_ALLOCATOR_KIND
};

/// OpenMP attributes for 'Allocator' clause.
enum OpenMPAllocatorClauseAllocator {
#define OPENMP_ALLOCATOR_ALLOCATOR_KIND(Name) OMPC_ALLOCATOR_ALLOCATOR_##Name,
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(default)      // omp_default_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(large_cap)// omp_large_cap_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(cons_mem)// omp_const_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(high_bw) // omp_high_bw_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(low_lat)// omp_low_lat_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(cgroup)// omp_cgroup_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(pteam)// omp_pteam_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(thread)// omp_thread_mem_alloc
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(user)// user-defined allocator
    OPENMP_ALLOCATOR_ALLOCATOR_KIND(unknown)
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

/// step for 'linear' clause.
enum OpenMPLinearClauseStep {
#define OPENMP_LINEAR_STEP(Name) OMPC_LINEAR_Step_##Name,

    OPENMP_LINEAR_STEP(user)

    OPENMP_LINEAR_STEP(unknow)
#undef OPENMP_LINEAR_STEP
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

/// OpenMP attributes for 'dist_schedule' clause.
enum OpenMPDistscheduleClauseKind {
#define OPENMP_DISTSCHEDULE_KIND(Name) OMPC_DISTSCHEDULE_KIND_##Name,

    OPENMP_DISTSCHEDULE_KIND(static)
    OPENMP_DISTSCHEDULE_KIND(user)

    OPENMP_DISTSCHEDULE_KIND(unknown)
#undef OPENMP_DISTSCHEDULE_KIND
};

/// OpenMP attributes for 'bind' clause.
enum OpenMPBindClauseKind {
#define OPENMP_BIND_KIND(Name) OMPC_BIND_##Name,
    OPENMP_BIND_KIND(teams)
    OPENMP_BIND_KIND(parallel)
    OPENMP_BIND_KIND(thread)
    OPENMP_BIND_KIND(user)

    OPENMP_BIND_KIND(unknown)
#undef OPENMP_BIND_KIND
};

/// OpenMP attributes for 'atomic_default_mem_order' clause.
enum OpenMPAtomicDefaultMemOrderClauseKind {
#define OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(Name) OMPC_ATOMIC_DEFAULT_MEM_ORDER_##Name,
    OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(seq_cst)
    OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(acq_rel)
    OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(relaxed)

    OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(unknown)
#undef OPENMP_DEFAULT_KIND
};

/// OpenMP attributes for 'UsesAllocators' clause.
enum OpenMPUsesAllocatorsClauseAllocator {
#define OPENMP_USESALLOCATORS_ALLOCATOR_KIND(Name) OMPC_USESALLOCATORS_ALLOCATOR_##Name,
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(default)      // omp_default_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(large_cap)// omp_large_cap_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(cons_mem)// omp_const_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(high_bw) // omp_high_bw_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(low_lat)// omp_low_lat_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(cgroup)// omp_cgroup_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(pteam)// omp_pteam_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(thread)// omp_thread_mem_alloc
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(user)// user-defined allocator
    OPENMP_USESALLOCATORS_ALLOCATOR_KIND(unknown)
#undef OPENMP_USESALLOCATORS_ALLOCATOR_KIND
};

/// modifiers for 'device' clause.
enum OpenMPDeviceClauseModifier {
#define OPENMP_DEVICE_MODIFIER(Name) OMPC_DEVICE_MODIFIER_##Name,
    OPENMP_DEVICE_MODIFIER(ancestor)
    OPENMP_DEVICE_MODIFIER(device_num)
#undef OPENMP_DEVICE_MODIFIER
};

enum OpenMPInReductionClauseIdentifier {
#define OPENMP_IN_REDUCTION_IDENTIFIER(Name) OMPC_IN_REDUCTION_IDENTIFIER_##Name,
    OPENMP_IN_REDUCTION_IDENTIFIER(plus)
    OPENMP_IN_REDUCTION_IDENTIFIER(minus)
    OPENMP_IN_REDUCTION_IDENTIFIER(mul)
    OPENMP_IN_REDUCTION_IDENTIFIER(bitand)
    OPENMP_IN_REDUCTION_IDENTIFIER(bitor)
    OPENMP_IN_REDUCTION_IDENTIFIER(bitxor)
    OPENMP_IN_REDUCTION_IDENTIFIER(logand)
    OPENMP_IN_REDUCTION_IDENTIFIER(logor)
    OPENMP_IN_REDUCTION_IDENTIFIER(eqv) /* only for Fortran */
    OPENMP_IN_REDUCTION_IDENTIFIER(neqv) /* only for Fortran */
    OPENMP_IN_REDUCTION_IDENTIFIER(max)
    OPENMP_IN_REDUCTION_IDENTIFIER(min)
    OPENMP_IN_REDUCTION_IDENTIFIER(user)

    OPENMP_IN_REDUCTION_IDENTIFIER(unknown)
#undef OPENMP_IN_REDUCTION_IDENTIFIER
};

enum OpenMPDependClauseModifier {
#define OPENMP_DEPEND_MODIFIER(Name) OMPC_DEPEND_MODIFIER_##Name,
    OPENMP_DEPEND_MODIFIER(iterator)
    OPENMP_DEPEND_MODIFIER(unknown)
#undef OPENMP_DEPEND_MODIFIER
};

enum OpenMPDependClauseType {
#define OPENMP_DEPEND_TYPE(Name) OMPC_DEPEND_TYPE_##Name,
    OPENMP_DEPEND_TYPE(in)
    OPENMP_DEPEND_TYPE(out)
    OPENMP_DEPEND_TYPE(inout)
    OPENMP_DEPEND_TYPE(mutexinoutset)
    OPENMP_DEPEND_TYPE(depobj)
    OPENMP_DEPEND_TYPE(unknown)
#undef OPENMP_DEPEND_TYPE
};

enum OpenMPAffinityClauseModifier {
#define OPENMP_AFFINITY_MODIFIER(Name) OMPC_AFFINITY_MODIFIER_##Name,
    OPENMP_AFFINITY_MODIFIER(iterator)
    OPENMP_AFFINITY_MODIFIER(unknown)
#undef OPENMP_AFFINITY_MODIFIER
};

enum OpenMPToClauseKind {
#define OPENMP_TO_KIND(Name) OMPC_TO_##Name,
    OPENMP_TO_KIND(mapper)
    OPENMP_TO_KIND(unknown)
#undef OPENMP_TO_KIND
};

enum OpenMPFromClauseKind {
#define OPENMP_FROM_KIND(Name) OMPC_FROM_##Name,
    OPENMP_FROM_KIND(mapper)
    OPENMP_FROM_KIND(unknown)
#undef OPENMP_FROM_KIND
};

enum OpenMPDefaultmapClauseBehavior {
#define OPENMP_DEFAULTMAP_BEHAVIOR(Name) OMPC_DEFAULTMAP_BEHAVIOR_##Name,

    OPENMP_DEFAULTMAP_BEHAVIOR(alloc)
    OPENMP_DEFAULTMAP_BEHAVIOR(to)
    OPENMP_DEFAULTMAP_BEHAVIOR(from)
    OPENMP_DEFAULTMAP_BEHAVIOR(tofrom)
    OPENMP_DEFAULTMAP_BEHAVIOR(firstprivate)
    OPENMP_DEFAULTMAP_BEHAVIOR(none)
    OPENMP_DEFAULTMAP_BEHAVIOR(default)
    OPENMP_DEFAULTMAP_BEHAVIOR(unknown)
#undef OPENMP_DEFAULTMAP_BEHAVIOR
};

enum OpenMPDefaultmapClauseCategory {
#define OPENMP_DEFAULTMAP_CATEGORY(Name) OMPC_DEFAULTMAP_CATEGORY_##Name,

    OPENMP_DEFAULTMAP_CATEGORY(scalar)
    OPENMP_DEFAULTMAP_CATEGORY(aggregate)
    OPENMP_DEFAULTMAP_CATEGORY(pointer)
    OPENMP_DEFAULTMAP_CATEGORY(unknown)
#undef OPENMP_DEFAULTMAP_CATEGORY
};

#endif

