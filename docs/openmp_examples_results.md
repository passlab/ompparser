Filename | Input | Output | Pass
--- | --- | --- | ---
Example_affinity.1.c | ` #pragma omp parallel proc_bind(spread) num_threads(4) ` | ` #pragma omp parallel num_threads (4) proc_bind (spread) ` | true 
Example_affinity.1.f | ` !$OMP PARALLEL PROC_BIND(SPREAD) NUM_THREADS(4) ` | ` !$omp parallel num_threads (4) proc_bind (spread) ` | true 
Example_affinity.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_affinity.2.c | ` #pragma omp parallel num_threads(16) proc_bind(spread) ` | ` #pragma omp parallel num_threads (16) proc_bind (spread) ` | true 
Example_affinity.2.f90 | ` !$omp parallel num_threads(16) proc_bind(spread) ` | ` !$omp parallel num_threads (16) proc_bind (spread) ` | true 
Example_affinity.2.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_affinity.3.c | ` #pragma omp parallel proc_bind(close) num_threads(4) ` | ` #pragma omp parallel num_threads (4) proc_bind (close) ` | true 
Example_affinity.3.f | ` !$OMP PARALLEL PROC_BIND(CLOSE) NUM_THREADS(4) ` | ` !$omp parallel num_threads (4) proc_bind (close) ` | true 
Example_affinity.3.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_affinity.4.c | ` #pragma omp parallel num_threads(16) proc_bind(close) ` | ` #pragma omp parallel num_threads (16) proc_bind (close) ` | true 
Example_affinity.4.f90 | ` !$omp parallel num_threads(16) proc_bind(close) ` | ` !$omp parallel num_threads (16) proc_bind (close) ` | true 
Example_affinity.4.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_affinity.5.c | ` #pragma omp parallel proc_bind(master) num_threads(4) ` | ` #pragma omp parallel num_threads (4) proc_bind (master) ` | true 
Example_affinity.5.f | ` !$OMP PARALLEL PROC_BIND(MASTER) NUM_THREADS(4) ` | ` !$omp parallel num_threads (4) proc_bind (master) ` | true 
Example_affinity.5.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_affinity.6.c | ` #pragma omp parallel num_threads(n_procs) proc_bind(close) ` | ` #pragma omp parallel num_threads (n_procs) proc_bind (close) ` | true 
Example_affinity.6.c | ` #pragma omp parallel num_threads(n_sockets) private(socket_num) \  ` | ` #pragma omp parallel num_threads (n_sockets) private (socket_num) ` | true 
Example_affinity.6.f90 | ` !$omp parallel num_threads(n_procs) proc_bind(close) ` | ` !$omp parallel num_threads (n_procs) proc_bind (close) ` | true 
Example_affinity.6.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_affinity.6.f90 | ` !$omp parallel num_threads(n_sockets) private(socket_num) & ` | `  ` | true 
Example_affinity.6.f90 | ` !$omp&         proc_bind(spread) ` | `  ` | true 
Example_affinity.6.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_array_sections.1.c | ` #pragma omp target data map( A[0:4] ) ` | `  ` | true 
Example_array_sections.1.c | ` #pragma omp target map( A[7:20] ) ` | `  ` | true 
Example_array_sections.1.f90 | ` !$omp target data map( A(1:4) ) ` | `  ` | true 
Example_array_sections.1.f90 | ` !$omp target map( A(8:27) ) ` | `  ` | true 
Example_array_sections.1.f90 | ` !$omp end target ` | `  ` | true 
Example_array_sections.1.f90 | ` !$omp end target data ` | `  ` | true 
Example_array_sections.2.c | ` #pragma omp target data map( A[0:4] ) ` | `  ` | true 
Example_array_sections.2.c | ` #pragma omp target map( p[3:20] ) ` | `  ` | true 
Example_array_sections.2.f90 | ` !$omp target data map( A(1:4) ) ` | `  ` | true 
Example_array_sections.2.f90 | ` !$omp target map( p(4:23) ) ` | `  ` | true 
Example_array_sections.2.f90 | ` !$omp end target ` | `  ` | true 
Example_array_sections.2.f90 | ` !$omp end target data ` | `  ` | true 
Example_array_sections.3.c | ` #pragma omp target data map( A[0:4] ) ` | `  ` | true 
Example_array_sections.3.c | ` #pragma omp target map( p[7:20] ) ` | `  ` | true 
Example_array_sections.3.f90 | ` !$omp target data map( A(1:4) ) ` | `  ` | true 
Example_array_sections.3.f90 | ` !$omp target map( p(8:27) ) ` | `  ` | true 
Example_array_sections.3.f90 | ` !$omp end target ` | `  ` | true 
Example_array_sections.3.f90 | ` !$omp end target data ` | `  ` | true 
Example_array_sections.4.c | ` #pragma omp target data map( A[0:10] ) ` | `  ` | true 
Example_array_sections.4.c | ` #pragma omp target map( p[3:7] ) ` | `  ` | true 
Example_array_sections.4.f90 | ` !$omp target data map( A(1:10) ) ` | `  ` | true 
Example_array_sections.4.f90 | ` !$omp target map( p(4:10) ) ` | `  ` | true 
Example_array_sections.4.f90 | ` !$omp end target ` | `  ` | true 
Example_array_sections.4.f90 | ` !$omp end target data ` | `  ` | true 
Example_associate.1.f | ` !$omp parallel private(b, c)        ! invalid to privatize b ` | `  ` | true 
Example_associate.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_associate.2.f | ` !$omp parallel private(i) ` | ` !$omp parallel private (i) ` | true 
Example_associate.2.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_associate.3.f90 | ` !$omp parallel private(v) ` | ` !$omp parallel private (v) ` | true 
Example_associate.3.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_async_target.1.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_async_target.1.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_async_target.1.c | ` #pragma omp task shared(Z) ` | ` #pragma omp task shared (Z) ` | true 
Example_async_target.1.c | ` #pragma omp target map(Z[C:CHUNKSZ]) ` | `  ` | true 
Example_async_target.1.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_async_target.1.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_async_target.1.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_async_target.1.f90 | ` !$omp task shared(z) ` | ` !$omp task shared (z) ` | true 
Example_async_target.1.f90 | ` !$omp target map(z(C:C+CHUNKSZ-1)) ` | `  ` | true 
Example_async_target.1.f90 | ` !$omp parallel do ` | `  ` | true 
Example_async_target.1.f90 | ` !$omp end target ` | `  ` | true 
Example_async_target.1.f90 | ` !$omp end task ` | `  ` | true 
Example_async_target.1.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_async_target.2.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_async_target.2.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_async_target.2.c | ` #pragma omp task shared(v1, v2) depend(out: v1, v2) ` | ` #pragma omp task shared (v1, v2) depend (out : v1, v2) ` | true 
Example_async_target.2.c | ` #pragma omp target device(dev) map(v1, v2) ` | `  ` | true 
Example_async_target.2.c | ` #pragma omp task shared(v1, v2, p) depend(in: v1, v2) ` | ` #pragma omp task shared (v1, v2, p) depend (in : v1, v2) ` | true 
Example_async_target.2.c | ` #pragma omp target device(dev) map(to: v1, v2) map(from: p[0:N]) ` | ` #pragma omp target device (dev) map ( to : v1, v2) map ( from : p[0:N]) ` | true 
Example_async_target.2.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_async_target.2.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_async_target.2.f90 | ` !$omp declare target (init) ` | ` !$omp declare target (init)  ` | true 
Example_async_target.2.f90 | ` !$omp target data map(v1,v2) ` | `  ` | true 
Example_async_target.2.f90 | ` !$omp task shared(v1,v2) depend(out: N) ` | ` !$omp task shared (v1, v2) depend (out : n) ` | true 
Example_async_target.2.f90 | ` !$omp target device(idev) ` | ` !$omp target device (idev) ` | true 
Example_async_target.2.f90 | ` !$omp end target ` | `  ` | true 
Example_async_target.2.f90 | ` !$omp end task ` | `  ` | true 
Example_async_target.2.f90 | ` !$omp task shared(v1,v2,p) depend(in: N) ` | ` !$omp task shared (v1, v2, p) depend (in : n) ` | true 
Example_async_target.2.f90 | ` !$omp target device(idev) map(from: p) ` | ` !$omp target device (idev) map ( from : p) ` | true 
Example_async_target.2.f90 | ` !$omp parallel do ` | `  ` | true 
Example_async_target.2.f90 | ` !$omp end target ` | `  ` | true 
Example_async_target.2.f90 | ` !$omp end task ` | `  ` | true 
Example_async_target.2.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_async_target.2.f90 | ` !$omp end target data ` | `  ` | true 
Example_async_target.3.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_async_target.3.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_async_target.3.c | ` #pragma omp target teams distribute parallel for nowait \ ` | ` #pragma omp target teams distribute parallel for nowait ` | true 
Example_async_target.3.c | ` #pragma omp for schedule(dynamic,chunk) ` | ` #pragma omp for schedule (:dynamic, chunk) ` | true 
Example_async_target.3.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_async_target.3.f90 | ` !$omp master ` | ` !$omp master threadprivate  ` | true 
Example_async_target.3.f90 | ` !$omp target teams distribute parallel do nowait & ` | `  ` | true 
Example_async_target.3.f90 | ` !$omp&                    map(to: v1(1:n/2))   & ` | `  ` | true 
Example_async_target.3.f90 | ` !$omp&                    map(to: v2(1:n/2))   &  ` | `  ` | true 
Example_async_target.3.f90 | ` !$omp&                    map(from: vxv(1:n/2))  ` | `  ` | true 
Example_async_target.3.f90 | ` !$omp end master ` | `  ` | true 
Example_async_target.3.f90 | ` !$omp do schedule(dynamic,chunk) ` | `  ` | true 
Example_async_target.3.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_async_target.4.c | ` #pragma omp parallel num_threads(2) ` | ` #pragma omp parallel num_threads (2) ` | true 
Example_async_target.4.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_async_target.4.c | ` #pragma omp task depend(out:v1) ` | ` #pragma omp task depend (out : v1) ` | true 
Example_async_target.4.c | ` #pragma omp task depend(out:v2) ` | ` #pragma omp task depend (out : v2) ` | true 
Example_async_target.4.c | ` #pragma omp target nowait depend(in:v1,v2) depend(out:p) \ ` | ` #pragma omp target nowait depend (in : v1, v2) depend (out : p) ` | true 
Example_async_target.4.c | ` #pragma omp parallel for private(i) ` | ` #pragma omp parallel for private (i) ` | true 
Example_async_target.4.c | ` #pragma omp task depend(in:p) ` | ` #pragma omp task depend (in : p) ` | true 
Example_async_target.4.f90 | ` !$omp parallel num_threads(2) ` | ` !$omp parallel num_threads (2) ` | true 
Example_async_target.4.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_async_target.4.f90 | ` !$omp task depend(out:v1) ` | ` !$omp task depend (out : v1) ` | true 
Example_async_target.4.f90 | ` !$omp end task ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp task depend(out:v2) ` | ` !$omp task depend (out : v2) ` | true 
Example_async_target.4.f90 | ` !$omp end task ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp target nowait depend(in:v1,v2) depend(out:p) & ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp&                 map(to:v1,v2)  map(from: p) ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp parallel do ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp end target ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp task depend(in:p) ` | ` !$omp task depend (in : p) ` | true 
Example_async_target.4.f90 | ` !$omp end task ` | `  ` | true 
Example_async_target.4.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_async_target.4.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_atomic.1.c | ` #pragma omp parallel for shared(x, y, index, n) ` | ` #pragma omp parallel for shared (x, y, index, n) ` | true 
Example_atomic.1.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_atomic.1.f | ` !$OMP   PARALLEL DO SHARED(X, Y, INDEX, N) ` | `  ` | true 
Example_atomic.1.f | ` !$OMP       ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic.2.c | ` #pragma omp atomic read ` | ` #pragma omp atomic read ` | true 
Example_atomic.2.c | ` #pragma omp atomic write ` | ` #pragma omp atomic write ` | true 
Example_atomic.2.f | ` !$omp atomic read ` | ` !$omp atomic read ` | true 
Example_atomic.2.f | ` !$omp atomic write ` | ` !$omp atomic write ` | true 
Example_atomic.3.c | ` #pragma omp atomic capture ` | ` #pragma omp atomic capture ` | true 
Example_atomic.3.c | ` #pragma omp flush ` | ` #pragma omp flush  ` | true 
Example_atomic.3.c | ` #pragma omp flush ` | ` #pragma omp flush  ` | true 
Example_atomic.3.f | ` !$omp atomic capture ` | ` !$omp atomic capture ` | true 
Example_atomic.3.f | ` !$omp end atomic ` | `  ` | true 
Example_atomic.3.f | ` !$omp flush ` | ` !$omp flush  ` | true 
Example_atomic.3.f | ` !$omp flush ` | ` !$omp flush  ` | true 
Example_atomic_restrict.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_atomic_restrict.1.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_atomic_restrict.1.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_atomic_restrict.1.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_atomic_restrict.1.f | ` !$OMP     ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic_restrict.1.f | ` !$OMP     ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic_restrict.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_atomic_restrict.2.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_atomic_restrict.2.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_atomic_restrict.2.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_atomic_restrict.2.f | ` !$OMP   ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic_restrict.2.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_atomic_restrict.2.f | ` !$OMP     ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic_restrict.2.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_atomic_restrict.3.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_atomic_restrict.3.f | ` !$OMP     ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic_restrict.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_atomic_restrict.3.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_atomic_restrict.3.f | ` !$OMP     ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_atomic_restrict.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_barrier_regions.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_barrier_regions.1.c | ` #pragma omp parallel shared(k) ` | ` #pragma omp parallel shared (k) ` | true 
Example_barrier_regions.1.c | ` #pragma omp parallel private(i) shared(n) ` | ` #pragma omp parallel private (i) shared (n) ` | true 
Example_barrier_regions.1.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_barrier_regions.1.f | ` !$OMP   BARRIER ` | ` !$omp barrier  ` | true 
Example_barrier_regions.1.f | ` !$OMP   PARALLEL SHARED(K) ` | ` !$omp parallel shared (k) ` | true 
Example_barrier_regions.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_barrier_regions.1.f | ` !$OMP   PARALLEL PRIVATE(I) SHARED(N) ` | ` !$omp parallel private (i) shared (n) ` | true 
Example_barrier_regions.1.f | ` !$OMP     DO ` | `  ` | true 
Example_barrier_regions.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_cancellation.1.cpp | ` #pragma omp parallel shared(ex) ` | ` #pragma omp parallel shared (ex) ` | true 
Example_cancellation.1.cpp | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_cancellation.1.cpp | ` #pragma omp atomic write ` | ` #pragma omp atomic write ` | true 
Example_cancellation.1.cpp | ` #pragma omp cancel for ` | ` #pragma omp cancel for ` | true 
Example_cancellation.1.cpp | ` #pragma omp cancel parallel ` | ` #pragma omp cancel parallel ` | true 
Example_cancellation.1.cpp | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_cancellation.1.f90 | ` !$omp parallel shared(err) ` | ` !$omp parallel shared (err) ` | true 
Example_cancellation.1.f90 | ` !$omp do private(s, B) ` | `  ` | true 
Example_cancellation.1.f90 | ` !$omp cancellation point do ` | `  ` | true 
Example_cancellation.1.f90 | ` !$omp atomic write ` | ` !$omp atomic write ` | true 
Example_cancellation.1.f90 | ` !$omp cancel do ` | `  ` | true 
Example_cancellation.1.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_cancellation.2.c | ` #pragma omp task shared(found) if(level < 10) ` | ` #pragma omp task if (level<10) shared (found) ` | true 
Example_cancellation.2.c | ` #pragma omp atomic write ` | ` #pragma omp atomic write ` | true 
Example_cancellation.2.c | ` #pragma omp cancel taskgroup ` | ` #pragma omp cancel taskgroup ` | true 
Example_cancellation.2.c | ` #pragma omp task shared(found) if(level < 10) ` | ` #pragma omp task if (level<10) shared (found) ` | true 
Example_cancellation.2.c | ` #pragma omp atomic write ` | ` #pragma omp atomic write ` | true 
Example_cancellation.2.c | ` #pragma omp cancel taskgroup ` | ` #pragma omp cancel taskgroup ` | true 
Example_cancellation.2.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_cancellation.2.c | ` #pragma omp parallel shared(found, tree, value) ` | ` #pragma omp parallel shared (found, tree, value) ` | true 
Example_cancellation.2.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_cancellation.2.c | ` #pragma omp taskgroup ` | ` #pragma omp taskgroup  ` | true 
Example_cancellation.2.f90 | ` !$omp task shared(found) if(level<10) ` | ` !$omp task if (level<10) shared (found) ` | true 
Example_cancellation.2.f90 | ` !$omp critical ` | ` !$omp critical  ` | true 
Example_cancellation.2.f90 | ` !$omp end critical ` | `  ` | true 
Example_cancellation.2.f90 | ` !$omp cancel taskgroup ` | ` !$omp cancel taskgroup ` | true 
Example_cancellation.2.f90 | ` !$omp end task ` | `  ` | true 
Example_cancellation.2.f90 | ` !$omp task shared(found) if(level<10) ` | ` !$omp task if (level<10) shared (found) ` | true 
Example_cancellation.2.f90 | ` !$omp critical ` | ` !$omp critical  ` | true 
Example_cancellation.2.f90 | ` !$omp end critical ` | `  ` | true 
Example_cancellation.2.f90 | ` !$omp cancel taskgroup ` | ` !$omp cancel taskgroup ` | true 
Example_cancellation.2.f90 | ` !$omp end task ` | `  ` | true 
Example_cancellation.2.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_cancellation.2.f90 | ` !$omp parallel shared(found, tree, value) ` | ` !$omp parallel shared (found, tree, value) ` | true 
Example_cancellation.2.f90 | ` !$omp master ` | ` !$omp master threadprivate  ` | true 
Example_cancellation.2.f90 | ` !$omp taskgroup ` | ` !$omp taskgroup  ` | true 
Example_cancellation.2.f90 | ` !$omp end taskgroup ` | `  ` | true 
Example_cancellation.2.f90 | ` !$omp end master ` | `  ` | true 
Example_cancellation.2.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_carrays_fpriv.1.c | ` #pragma omp parallel firstprivate(B, C, D, E) ` | ` #pragma omp parallel firstprivate (B, C, D, E) ` | true 
Example_collapse.1.c | ` #pragma omp for collapse(2) private(i, k, j) ` | ` #pragma omp for private (i, k, j) collapse (2) ` | true 
Example_collapse.1.f | ` !$omp do collapse(2) private(i,j,k) ` | `  ` | true 
Example_collapse.1.f | ` !$omp end do ` | `  ` | true 
Example_collapse.2.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_collapse.2.c | ` #pragma omp for collapse(2) lastprivate(jlast, klast) ` | ` #pragma omp for lastprivate (jlast, klast) collapse (2) ` | true 
Example_collapse.2.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_collapse.2.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_collapse.2.f | ` !$omp do private(j,k) collapse(2) lastprivate(jlast, klast) ` | `  ` | true 
Example_collapse.2.f | ` !$omp end do ` | `  ` | true 
Example_collapse.2.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_collapse.2.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_collapse.2.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_collapse.3.c | ` #pragma omp parallel num_threads(2) ` | ` #pragma omp parallel num_threads (2) ` | true 
Example_collapse.3.c | ` #pragma omp for collapse(2) ordered private(j,k) schedule(static,3) ` | ` #pragma omp for private (j, k) collapse (2) ordered schedule (:static, 3) ` | true 
Example_collapse.3.c | ` #pragma omp ordered ` | ` #pragma omp ordered  ` | true 
Example_collapse.3.f | ` !$omp parallel num_threads(2) ` | ` !$omp parallel num_threads (2) ` | true 
Example_collapse.3.f | ` !$omp do collapse(2) ordered private(j,k) schedule(static,3) ` | `  ` | true 
Example_collapse.3.f | ` !$omp ordered ` | ` !$omp ordered  ` | true 
Example_collapse.3.f | ` !$omp end ordered ` | `  ` | true 
Example_collapse.3.f | ` !$omp end do ` | `  ` | true 
Example_collapse.3.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_copyin.1.c | ` #pragma omp threadprivate(work,size,tol) ` | ` #pragma omp threadprivate (work,size,tol) ` | true 
Example_copyin.1.c | ` #pragma omp parallel copyin(tol,size) ` | `  ` | true 
Example_copyin.1.f | ` !$OMP   THREADPRIVATE(WORK,SIZE,TOL) ` | ` !$omp threadprivate (work,size,tol) ` | true 
Example_copyin.1.f | ` !$OMP   PARALLEL COPYIN(TOL,SIZE) ` | `  ` | true 
Example_copyin.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_copyprivate.1.c | ` #pragma omp threadprivate(x, y) ` | ` #pragma omp threadprivate (x,y) ` | true 
Example_copyprivate.1.c | ` #pragma omp single copyprivate(a,b,x,y) ` | ` #pragma omp single copyprivate (a, b, x, y) ` | true 
Example_copyprivate.1.f | ` !$OMP   THREADPRIVATE (/XY/) ` | `  ` | true 
Example_copyprivate.1.f | ` !$OMP   SINGLE ` | ` !$omp single  ` | true 
Example_copyprivate.1.f | ` !$OMP   END SINGLE COPYPRIVATE (A,B,/XY/) ` | ` !$omp end single copyprivate (a, b, /xy/) ` | true 
Example_copyprivate.2.c | ` #pragma omp single copyprivate(tmp) ` | ` #pragma omp single copyprivate (tmp) ` | true 
Example_copyprivate.2.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_copyprivate.2.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_copyprivate.2.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_copyprivate.2.c | ` #pragma omp single nowait ` | ` #pragma omp single nowait ` | true 
Example_copyprivate.2.f | ` !$OMP   SINGLE ` | ` !$omp single  ` | true 
Example_copyprivate.2.f | ` !$OMP   END SINGLE COPYPRIVATE (TMP)  ! copies the pointer only ` | `  ` | true 
Example_copyprivate.2.f | ` !$OMP   MASTER ` | ` !$omp master threadprivate  ` | true 
Example_copyprivate.2.f | ` !$OMP   END MASTER ` | `  ` | true 
Example_copyprivate.2.f | ` !$OMP   BARRIER ` | ` !$omp barrier  ` | true 
Example_copyprivate.2.f | ` !$OMP   BARRIER ` | ` !$omp barrier  ` | true 
Example_copyprivate.2.f | ` !$OMP   SINGLE ` | ` !$omp single  ` | true 
Example_copyprivate.2.f | ` !$OMP   END SINGLE NOWAIT ` | ` !$omp end single nowait ` | true 
Example_copyprivate.3.c | ` #pragma omp single copyprivate(lock_ptr) ` | ` #pragma omp single copyprivate (lock_ptr) ` | true 
Example_copyprivate.3.f | ` !$OMP   SINGLE ` | ` !$omp single  ` | true 
Example_copyprivate.3.f | ` !$OMP   END SINGLE COPYPRIVATE(NEW_LOCK) ` | ` !$omp end single copyprivate (new_lock) ` | true 
Example_copyprivate.4.f | ` !$OMP   SINGLE ` | ` !$omp single  ` | true 
Example_copyprivate.4.f | ` !$OMP   END SINGLE COPYPRIVATE(A,B) ` | ` !$omp end single copyprivate (a, b) ` | true 
Example_copyprivate.4.f | ` !$OMP   BARRIER ` | ` !$omp barrier  ` | true 
Example_copyprivate.4.f | ` !$OMP   SINGLE ` | ` !$omp single  ` | true 
Example_copyprivate.4.f | ` !$OMP   END SINGLE NOWAIT ` | ` !$omp end single nowait ` | true 
Example_cpp_reference.1.cpp | ` #pragma omp task // x is implicitly determined firstprivate(x) ` | ` #pragma omp task  ` | true 
Example_cpp_reference.1.cpp | ` #pragma omp parallel private(y) ` | ` #pragma omp parallel private (y) ` | true 
Example_critical.1.c | ` #pragma omp parallel shared(x, y) private(ix_next, iy_next) ` | ` #pragma omp parallel private (ix_next, iy_next) shared (x, y) ` | true 
Example_critical.1.c | ` #pragma omp critical (xaxis) ` | ` #pragma omp critical (xaxis)  ` | true 
Example_critical.1.c | ` #pragma omp critical (yaxis) ` | ` #pragma omp critical (yaxis)  ` | true 
Example_critical.1.f | ` !$OMP PARALLEL SHARED(X, Y) PRIVATE(IX_NEXT, IY_NEXT) ` | ` !$omp parallel private (ix_next, iy_next) shared (x, y) ` | true 
Example_critical.1.f | ` !$OMP CRITICAL(XAXIS) ` | ` !$omp critical (xaxis)  ` | true 
Example_critical.1.f | ` !$OMP END CRITICAL(XAXIS) ` | `  ` | true 
Example_critical.1.f | ` !$OMP CRITICAL(YAXIS) ` | ` !$omp critical (yaxis)  ` | true 
Example_critical.1.f | ` !$OMP END CRITICAL(YAXIS) ` | `  ` | true 
Example_critical.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_critical.2.c | ` #pragma omp parallel shared(x, y) private(ix_next, iy_next) ` | ` #pragma omp parallel private (ix_next, iy_next) shared (x, y) ` | true 
Example_critical.2.c | ` #pragma omp critical (xaxis) hint(omp_lock_hint_contended) ` | ` #pragma omp critical (xaxis) hint (_lock_hint_contended) ` | true 
Example_critical.2.c | ` #pragma omp critical (yaxis) hint(omp_lock_hint_contended) ` | ` #pragma omp critical (yaxis) hint (_lock_hint_contended) ` | true 
Example_critical.2.f | ` !$OMP PARALLEL SHARED(X, Y) PRIVATE(IX_NEXT, IY_NEXT) ` | ` !$omp parallel private (ix_next, iy_next) shared (x, y) ` | true 
Example_critical.2.f | ` !$OMP CRITICAL(XAXIS) HINT(OMP_LOCK_HINT_CONTENDED) ` | ` !$omp critical (xaxis) hint (_lock_hint_contended) ` | true 
Example_critical.2.f | ` !$OMP END CRITICAL(XAXIS) ` | `  ` | true 
Example_critical.2.f | ` !$OMP CRITICAL(YAXIS) HINT(OMP_LOCK_HINT_CONTENDED) ` | ` !$omp critical (yaxis) hint (_lock_hint_contended) ` | true 
Example_critical.2.f | ` !$OMP END CRITICAL(YAXIS) ` | `  ` | true 
Example_critical.2.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_declare_target.1.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_declare_target.1.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_declare_target.1.c | ` #pragma omp target if(n > THRESHOLD) ` | ` #pragma omp target if (n>THRESHOLD) ` | true 
Example_declare_target.1.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_declare_target.1.f90 | ` !$omp target if( N > THRESHOLD ) ` | ` !$omp target if (n>threshold) ` | true 
Example_declare_target.1.f90 | ` !$omp end target ` | `  ` | true 
Example_declare_target.2.cpp | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_declare_target.2.cpp | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_declare_target.2.cpp | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_declare_target.2.f90 | ` !$omp declare target(fib) ` | ` !$omp declare target (fib)  ` | true 
Example_declare_target.2.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_declare_target.2.f90 | ` !$omp end target ` | `  ` | true 
Example_declare_target.2.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_declare_target.3.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_declare_target.3.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_declare_target.3.c | ` #pragma omp target update to(v1, v2) ` | ` #pragma omp target update to (v1, v2) ` | true 
Example_declare_target.3.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_declare_target.3.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_declare_target.3.c | ` #pragma omp target update from(p) ` | ` #pragma omp target update from (p) ` | true 
Example_declare_target.3.f90 | ` !$omp declare target (N, p, v1, v2) ` | ` !$omp declare target (n,p,v1,v2)  ` | true 
Example_declare_target.3.f90 | ` !$omp target update to(v1, v2) ` | ` !$omp target update to (v1, v2) ` | true 
Example_declare_target.3.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_declare_target.3.f90 | ` !$omp parallel do ` | `  ` | true 
Example_declare_target.3.f90 | ` !$omp end target ` | `  ` | true 
Example_declare_target.3.f90 | ` !$omp target update from (p) ` | ` !$omp target update from (p) ` | true 
Example_declare_target.4.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_declare_target.4.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_declare_target.4.c | ` #pragma omp target update to(Q) ` | ` #pragma omp target update to (Q) ` | true 
Example_declare_target.4.c | ` #pragma omp target map(tofrom: tmp) ` | ` #pragma omp target map ( tofrom : tmp) ` | true 
Example_declare_target.4.c | ` #pragma omp parallel for reduction(+:tmp) ` | ` #pragma omp parallel for reduction (+ : tmp) ` | true 
Example_declare_target.4.f90 | ` !$omp declare target (N,Q) ` | ` !$omp declare target (n,q)  ` | true 
Example_declare_target.4.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_declare_target.4.f90 | ` !$omp target map(tofrom: tmp) ` | ` !$omp target map ( tofrom : tmp) ` | true 
Example_declare_target.4.f90 | ` !$omp parallel do reduction(+:tmp) ` | `  ` | true 
Example_declare_target.4.f90 | ` !$omp end target ` | `  ` | true 
Example_declare_target.5.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_declare_target.5.c | ` #pragma omp declare simd uniform(i) linear(k) notinbranch ` | ` #pragma omp declare simd linear uniform (i) notinbranch ` | true 
Example_declare_target.5.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_declare_target.5.c | ` #pragma omp target map(tofrom: tmp) ` | ` #pragma omp target map ( tofrom : tmp) ` | true 
Example_declare_target.5.c | ` #pragma omp parallel for reduction(+:tmp) ` | ` #pragma omp parallel for reduction (+ : tmp) ` | true 
Example_declare_target.5.c | ` #pragma omp simd reduction(+:tmp1) ` | ` #pragma omp simd reduction (+ : tmp1) ` | true 
Example_declare_target.5.f90 | ` !$omp declare target (N,Q) ` | ` !$omp declare target (n,q)  ` | true 
Example_declare_target.5.f90 | ` !$omp declare simd uniform(i) linear(k) notinbranch ` | ` !$omp declare simd linear uniform (i) notinbranch ` | true 
Example_declare_target.5.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_declare_target.5.f90 | ` !$omp target map(tofrom: tmp) ` | ` !$omp target map ( tofrom : tmp) ` | true 
Example_declare_target.5.f90 | ` !$omp parallel do private(tmp1) reduction(+:tmp) ` | `  ` | true 
Example_declare_target.5.f90 | ` !$omp simd reduction(+:tmp1) ` | ` !$omp simd reduction (+ : tmp1) ` | true 
Example_declare_target.5.f90 | ` !$omp end target ` | `  ` | true 
Example_declare_target.6.c | ` #pragma omp declare target link(sp,sv1,sv2) \ ` | ` #pragma omp declare target link (sp, sv1, sv2) ` | true 
Example_declare_target.6.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_declare_target.6.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_declare_target.6.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_declare_target.6.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_declare_target.6.c | ` #pragma omp target map(to:sv1,sv2) map(from:sp) ` | ` #pragma omp target map ( to : sv1, sv2) map ( from : sp) ` | true 
Example_declare_target.6.c | ` #pragma omp target map(to:dv1,dv2) map(from:dp) ` | ` #pragma omp target map ( to : dv1, dv2) map ( from : dp) ` | true 
Example_declare_target.6.f90 | ` !$omp declare target link(sp,sv1,sv2) ` | ` !$omp declare target link (sp, sv1, sv2) ` | true 
Example_declare_target.6.f90 | ` !$omp declare target link(dp,dv1,dv2) ` | ` !$omp declare target link (dp, dv1, dv2) ` | true 
Example_declare_target.6.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_declare_target.6.f90 | ` !$omp parallel do ` | `  ` | true 
Example_declare_target.6.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_declare_target.6.f90 | ` !$omp parallel do ` | `  ` | true 
Example_declare_target.6.f90 | ` !$omp target map(to:sv1,sv2) map(from:sp) ` | ` !$omp target map ( to : sv1, sv2) map ( from : sp) ` | true 
Example_declare_target.6.f90 | ` !$omp end target ` | `  ` | true 
Example_declare_target.6.f90 | ` !$omp target map(to:dv1,dv2) map(from:dp) ` | ` !$omp target map ( to : dv1, dv2) map ( from : dp) ` | true 
Example_declare_target.6.f90 | ` !$omp end target ` | `  ` | true 
Example_default_none.1.c | ` #pragma omp threadprivate(x) ` | ` #pragma omp threadprivate (x) ` | true 
Example_default_none.1.c | ` #pragma omp parallel default(none) private(a) shared(z, c) ` | ` #pragma omp parallel default (none) private (a) shared (z, c) ` | true 
Example_default_none.1.c | ` #pragma omp for firstprivate(y) ` | ` #pragma omp for firstprivate (y) ` | true 
Example_default_none.1.f | ` !$OMP THREADPRIVATE(/BLOCKX/) ` | `  ` | true 
Example_default_none.1.f | ` !$OMP   PARALLEL DEFAULT(NONE) PRIVATE(A) SHARED(Z) PRIVATE(J) ` | ` !$omp parallel default (none) private (a, j) shared (z) ` | true 
Example_default_none.1.f | ` !$OMP DO firstprivate(y) ` | `  ` | true 
Example_default_none.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_device.1.c | ` #pragma omp declare target ` | ` #pragma omp declare target  ` | true 
Example_device.1.c | ` #pragma omp end declare target ` | ` #pragma omp end declare target  ` | true 
Example_device.1.c | ` #pragma omp target device(42) map(p[:N], v1[:N], v2[:N]) ` | `  ` | true 
Example_device.1.c | ` #pragma omp parallel for private(i) num_threads(nthreads) ` | ` #pragma omp parallel for num_threads (nthreads) private (i) ` | true 
Example_device.1.f90 | ` !$omp declare target ` | ` !$omp declare target  ` | true 
Example_device.1.f90 | ` !$omp parallel do private(i) num_threads(nthreads) ` | `  ` | true 
Example_device.1.f90 | ` !$omp target device(42) map(p, v1, v2) ` | `  ` | true 
Example_device.1.f90 | ` !$omp end target ` | `  ` | true 
Example_device.2.c | ` #pragma omp target if(do_offload) map(to: v1[0:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target if (do_offload) map ( to : v1[0:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_device.2.c | ` #pragma omp parallel for if(N>1000) private(i) ` | ` #pragma omp parallel for if (N>1000) private (i) ` | true 
Example_device.2.f90 | ` !$omp target if(do_offload) map(to: v1, v2) map(from: p) ` | ` !$omp target if (do_offload) map ( to : v1, v2) map ( from : p) ` | true 
Example_device.2.f90 | ` !$omp parallel do if(N>1000) ` | `  ` | true 
Example_device.2.f90 | ` !$omp end target ` | `  ` | true 
Example_device.4.c | ` #pragma omp target is_device_ptr(mem_dev_cpy) device(t) ` | ` #pragma omp target device (t) is_device_ptr (mem_dev_cpy) ` | true 
Example_device.4.c | ` #pragma omp teams distribute parallel for ` | ` #pragma omp teams distribute parallel for  ` | true 
Example_doacross.1.c | ` #pragma omp for ordered(1) ` | ` #pragma omp for ordered (1) ` | true 
Example_doacross.1.c | ` #pragma omp ordered depend(sink: i-1) ` | ` #pragma omp ordered depend (sink : i-1) ` | true 
Example_doacross.1.c | ` #pragma omp ordered depend(source) ` | ` #pragma omp ordered depend (source) ` | true 
Example_doacross.1.f90 | ` !$omp do ordered(1) ` | `  ` | true 
Example_doacross.1.f90 | ` !$omp ordered depend(sink: i-1) ` | ` !$omp ordered depend (sink : i-1) ` | true 
Example_doacross.1.f90 | ` !$omp ordered depend(source) ` | ` !$omp ordered depend (source) ` | true 
Example_doacross.2.c | ` #pragma omp for ordered(2) ` | ` #pragma omp for ordered (2) ` | true 
Example_doacross.2.c | ` #pragma omp ordered depend(sink: i-1,j) depend(sink: i,j-1) ` | `  ` | true 
Example_doacross.2.c | ` #pragma omp ordered depend(source) ` | ` #pragma omp ordered depend (source) ` | true 
Example_doacross.2.f90 | ` !$omp do ordered(2) ` | `  ` | true 
Example_doacross.2.f90 | ` !$omp ordered depend(sink: j-1,i) depend(sink: j,i-1) ` | `  ` | true 
Example_doacross.2.f90 | ` !$omp ordered depend(source) ` | ` !$omp ordered depend (source) ` | true 
Example_doacross.3.c | ` #pragma omp parallel for ordered(2) private(i,j,k) ` | ` #pragma omp parallel for private (i, j, k) ordered (2) ` | true 
Example_doacross.3.c | ` #pragma omp ordered depend(sink: i-1,j) depend(sink: i+1,j) \ ` | `  ` | true 
Example_doacross.3.f90 | ` !$omp parallel do ordered(2) private(i,j,k,tmp1,tmp2,tmp3) ` | `  ` | true 
Example_doacross.3.f90 | ` !$omp ordered depend(sink: i-1,j) depend(sink: i+1,j) & ` | `  ` | true 
Example_doacross.3.f90 | ` !$omp&        depend(sink: i,j-1) depend(sink: i,j+1) ` | `  ` | true 
Example_doacross.4.c | ` #pragma omp for collapse(2) ordered(2) ` | ` #pragma omp for collapse (2) ordered (2) ` | true 
Example_doacross.4.c | ` #pragma omp ordered depend(source) ` | ` #pragma omp ordered depend (source) ` | true 
Example_doacross.4.c | ` #pragma omp ordered depend(sink: i-1,j) depend(sink: i,j-1) ` | `  ` | true 
Example_doacross.4.f90 | ` !$omp do collapse(2) ordered(2) ` | `  ` | true 
Example_doacross.4.f90 | ` !$omp ordered depend(source) ` | ` !$omp ordered depend (source) ` | true 
Example_doacross.4.f90 | ` !$omp ordered depend(sink: j,i-1) depend(sink: j-1,i) ` | `  ` | true 
Example_doacross.5.c | ` #pragma omp for ordered(1) ` | ` #pragma omp for ordered (1) ` | true 
Example_doacross.5.c | ` #pragma omp ordered depend(sink: i-1) ` | ` #pragma omp ordered depend (sink : i-1) ` | true 
Example_doacross.5.c | ` #pragma omp ordered depend(source) ` | ` #pragma omp ordered depend (source) ` | true 
Example_doacross.5.f90 | ` !$omp do ordered(1) ` | `  ` | true 
Example_doacross.5.f90 | ` !$omp ordered depend(sink: i-1) ` | ` !$omp ordered depend (sink : i-1) ` | true 
Example_doacross.5.f90 | ` !$omp ordered depend(source) ` | ` !$omp ordered depend (source) ` | true 
Example_flush_nolist.1.c | ` #pragma omp flush ` | ` #pragma omp flush  ` | true 
Example_flush_nolist.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_flush_nolist.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_flush_nolist.1.c | ` #pragma omp parallel reduction(+: sum) num_threads(10) ` | ` #pragma omp parallel num_threads (10) reduction (+ : sum) ` | true 
Example_flush_nolist.1.f | ` !$OMP   FLUSH ` | ` !$omp flush  ` | true 
Example_flush_nolist.1.f | ` !$OMP   BARRIER ` | ` !$omp barrier  ` | true 
Example_flush_nolist.1.f | ` !$OMP   BARRIER ` | ` !$omp barrier  ` | true 
Example_flush_nolist.1.f | ` !$OMP   PARALLEL REDUCTION(+: SUM) NUM_THREADS(10) ` | ` !$omp parallel num_threads (10) reduction (+ : sum) ` | true 
Example_flush_nolist.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_do.1.f | ` !$OMP     DO ` | `  ` | true 
Example_fort_do.1.f | ` !$OMP   DO ` | `  ` | true 
Example_fort_do.1.f | ` !$OMP   ENDDO ` | `  ` | true 
Example_fort_do.1.f | ` !$OMP   DO ` | `  ` | true 
Example_fort_do.1.f | ` !$OMP   ENDDO ` | `  ` | true 
Example_fort_do.2.f | ` !$OMP     DO ` | `  ` | true 
Example_fort_do.2.f | ` !$OMP   ENDDO ` | `  ` | true 
Example_fort_loopvar.1.f90 | ` !$OMP PARALLEL PRIVATE(MYOFFSET) ` | ` !$omp parallel private (myoffset) ` | true 
Example_fort_loopvar.1.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP PARALLEL SHARED(A,B,I1,I2) ` | ` !$omp parallel shared (a, b, i1, i2) ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP SECTIONS ` | ` !$omp sections  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP SECTION ` | ` !$omp section  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP SECTION ` | ` !$omp section  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP END SECTIONS ` | ` !$omp end sections  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP SINGLE ` | ` !$omp single  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP END SINGLE ` | ` !$omp end single  ` | true 
Example_fort_loopvar.2.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_race.1.f90 | ` !$OMP PARALLEL SHARED(A) PRIVATE(MYTHREAD) ` | ` !$omp parallel private (mythread) shared (a) ` | true 
Example_fort_race.1.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sa_private.1.f | ` !$OMP    PARALLEL PRIVATE (X) ` | ` !$omp parallel private (x) ` | true 
Example_fort_sa_private.1.f | ` !$OMP    END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sa_private.2.f | ` !$OMP   PARALLEL PRIVATE (X) ` | ` !$omp parallel private (x) ` | true 
Example_fort_sa_private.2.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sa_private.3.f | ` !$OMP   PARALLEL PRIVATE(X) ` | ` !$omp parallel private (x) ` | true 
Example_fort_sa_private.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sa_private.4.f | ` !$OMP PARALLEL DO DEFAULT(PRIVATE) PRIVATE(I,J) LASTPRIVATE(A) ` | `  ` | true 
Example_fort_sa_private.4.f | ` !$OMP END PARALLEL DO       ! The LASTPRIVATE write for A has ` | `  ` | true 
Example_fort_sa_private.5.f | ` !$OMP   PARALLEL PRIVATE(/BLOCK5/) ` | `  ` | true 
Example_fort_sa_private.5.f | ` !$OMP     MASTER ` | ` !$omp master threadprivate  ` | true 
Example_fort_sa_private.5.f | ` !$OMP     END MASTER ` | `  ` | true 
Example_fort_sa_private.5.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.1.f | ` !$OMP   PARALLEL PRIVATE (/C/) ` | `  ` | true 
Example_fort_sp_common.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.1.f | ` !$OMP   PARALLEL SHARED (X,Y) ` | ` !$omp parallel shared (x, y) ` | true 
Example_fort_sp_common.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.2.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_fort_sp_common.2.f | ` !$OMP     DO PRIVATE(/C/) ` | `  ` | true 
Example_fort_sp_common.2.f | ` !$OMP     END DO ` | `  ` | true 
Example_fort_sp_common.2.f | ` !$OMP     DO PRIVATE(X) ` | `  ` | true 
Example_fort_sp_common.2.f | ` !$OMP     END DO ` | `  ` | true 
Example_fort_sp_common.2.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.3.f | ` !$OMP   PARALLEL PRIVATE (/C/) ` | `  ` | true 
Example_fort_sp_common.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.3.f | ` !$OMP   PARALLEL SHARED (/C/) ` | `  ` | true 
Example_fort_sp_common.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.4.f | ` !$OMP   PARALLEL PRIVATE(/C/), SHARED(X) ` | `  ` | true 
Example_fort_sp_common.4.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fort_sp_common.5.f | ` !$OMP   PARALLEL PRIVATE (/C/), SHARED(/C/) ` | `  ` | true 
Example_fort_sp_common.5.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_fpriv_sections.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_fpriv_sections.1.c | ` #pragma omp sections firstprivate( section_count ) ` | `  ` | true 
Example_fpriv_sections.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_fpriv_sections.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_fpriv_sections.1.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_fpriv_sections.1.f90 | ` !$omp sections firstprivate ( section_count ) ` | `  ` | true 
Example_fpriv_sections.1.f90 | ` !$omp section ` | ` !$omp section  ` | true 
Example_fpriv_sections.1.f90 | ` !$omp section ` | ` !$omp section  ` | true 
Example_fpriv_sections.1.f90 | ` !$omp end sections ` | ` !$omp end sections  ` | true 
Example_fpriv_sections.1.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_get_nthrs.1.c | ` #pragma omp parallel for schedule(static) ` | ` #pragma omp parallel for schedule (:static) ` | true 
Example_get_nthrs.1.f | ` !$OMP   PARALLEL DO SCHEDULE(STATIC) ` | `  ` | true 
Example_get_nthrs.1.f | ` !$OMP   END PARALLEL DO ` | `  ` | true 
Example_get_nthrs.2.c | ` #pragma omp parallel private(i) ` | ` #pragma omp parallel private (i) ` | true 
Example_get_nthrs.2.f | ` !$OMP    PARALLEL PRIVATE(I) ` | ` !$omp parallel private (i) ` | true 
Example_get_nthrs.2.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_icv.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_icv.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_icv.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_icv.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_icv.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_icv.1.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_icv.1.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_icv.1.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_icv.1.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_icv.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_icv.1.f | ` !$omp barrier ` | ` !$omp barrier  ` | true 
Example_icv.1.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_icv.1.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_icv.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_init_lock.1.cpp | ` #pragma omp parallel for private(i) ` | ` #pragma omp parallel for private (i) ` | true 
Example_init_lock.1.f | ` !$OMP   PARALLEL DO PRIVATE(I) ` | `  ` | true 
Example_init_lock.1.f | ` !$OMP   END PARALLEL DO ` | `  ` | true 
Example_init_lock_with_hint.1.cpp | ` #pragma omp parallel for private(i) ` | ` #pragma omp parallel for private (i) ` | true 
Example_init_lock_with_hint.1.f | ` !$OMP   PARALLEL DO PRIVATE(I) ` | `  ` | true 
Example_init_lock_with_hint.1.f | ` !$OMP   END PARALLEL DO ` | `  ` | true 
Example_lastprivate.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_lastprivate.1.c | ` #pragma omp for lastprivate(i) ` | ` #pragma omp for lastprivate (i) ` | true 
Example_lastprivate.1.f | ` !$OMP PARALLEL ` | ` !$omp parallel  ` | true 
Example_lastprivate.1.f | ` !$OMP DO LASTPRIVATE(I) ` | `  ` | true 
Example_lastprivate.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_linear_in_loop.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_linear_in_loop.1.c | ` #pragma omp for linear(j:1) ` | ` #pragma omp for linear (j:1) ` | true 
Example_linear_in_loop.1.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_linear_in_loop.1.f90 | ` !$omp do linear(j:1) ` | `  ` | true 
Example_linear_in_loop.1.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_lock_owner.1.c | ` #pragma omp parallel shared (x) ` | ` #pragma omp parallel shared (x) ` | true 
Example_lock_owner.1.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_lock_owner.1.f | ` !$omp parallel shared (x) ` | ` !$omp parallel shared (x) ` | true 
Example_lock_owner.1.f | ` !$omp master ` | ` !$omp master threadprivate  ` | true 
Example_lock_owner.1.f | ` !$omp end master ` | `  ` | true 
Example_lock_owner.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_master.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_master.1.c | ` #pragma omp for private(i) ` | ` #pragma omp for private (i) ` | true 
Example_master.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_master.1.c | ` #pragma omp for private(i,y,error) reduction(+:toobig) ` | ` #pragma omp for private (i, y, error) reduction (+ : toobig) ` | true 
Example_master.1.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_master.1.f | ` !$OMP PARALLEL ` | ` !$omp parallel  ` | true 
Example_master.1.f | ` !$OMP     DO PRIVATE(I) ` | `  ` | true 
Example_master.1.f | ` !$OMP     SINGLE ` | ` !$omp single  ` | true 
Example_master.1.f | ` !$OMP     END SINGLE ` | ` !$omp end single  ` | true 
Example_master.1.f | ` !$OMP     DO PRIVATE(I,Y,ERROR), REDUCTION(+:TOOBIG) ` | `  ` | true 
Example_master.1.f | ` !$OMP     MASTER ` | ` !$omp master threadprivate  ` | true 
Example_master.1.f | ` !$OMP     END MASTER ` | `  ` | true 
Example_master.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_mem_model.1.c | ` #pragma omp parallel num_threads(2) shared(x) ` | ` #pragma omp parallel num_threads (2) shared (x) ` | true 
Example_mem_model.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_mem_model.1.f90 | ` !$OMP PARALLEL NUM_THREADS(2) SHARED(X) ` | ` !$omp parallel num_threads (2) shared (x) ` | true 
Example_mem_model.1.f90 | ` !$OMP BARRIER ` | ` !$omp barrier  ` | true 
Example_mem_model.1.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_mem_model.2.c | ` #pragma omp parallel num_threads(2) ` | ` #pragma omp parallel num_threads (2) ` | true 
Example_mem_model.2.c | ` #pragma omp flush(flag, data) ` | ` #pragma omp flush (flag,data)  ` | true 
Example_mem_model.2.c | ` #pragma omp flush(flag) ` | ` #pragma omp flush (flag)  ` | true 
Example_mem_model.2.c | ` #pragma omp flush(flag, data) ` | ` #pragma omp flush (flag,data)  ` | true 
Example_mem_model.2.c | ` #pragma omp flush(flag, data) ` | ` #pragma omp flush (flag,data)  ` | true 
Example_mem_model.2.c | ` #pragma omp flush(flag, data) ` | ` #pragma omp flush (flag,data)  ` | true 
Example_mem_model.2.f | ` !$OMP PARALLEL NUM_THREADS(2) ` | ` !$omp parallel num_threads (2) ` | true 
Example_mem_model.2.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.2.f | ` !$OMP FLUSH(FLAG) ` | ` !$omp flush (flag)  ` | true 
Example_mem_model.2.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.2.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.2.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.2.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_mem_model.3.c | ` #pragma omp parallel num_threads(3) ` | ` #pragma omp parallel num_threads (3) ` | true 
Example_mem_model.3.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_mem_model.3.c | ` #pragma omp flush(flag) ` | ` #pragma omp flush (flag)  ` | true 
Example_mem_model.3.c | ` #pragma omp flush(flag) ` | ` #pragma omp flush (flag)  ` | true 
Example_mem_model.3.c | ` #pragma omp atomic update ` | ` #pragma omp atomic update ` | true 
Example_mem_model.3.c | ` #pragma omp flush(flag) ` | ` #pragma omp flush (flag)  ` | true 
Example_mem_model.3.c | ` #pragma omp flush(flag) ` | ` #pragma omp flush (flag)  ` | true 
Example_mem_model.3.f | ` !$OMP PARALLEL NUM_THREADS(3) ` | ` !$omp parallel num_threads (3) ` | true 
Example_mem_model.3.f | ` !$OMP ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_mem_model.3.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.3.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.3.f | ` !$OMP ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_mem_model.3.f | ` !$OMP FLUSH(FLAG, DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.3.f | ` !$OMP FLUSH(FLAG,    DATA) ` | ` !$omp flush (flag,data)  ` | true 
Example_mem_model.3.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nestable_lock.1.c | ` #pragma omp parallel sections ` | ` #pragma omp parallel sections  ` | true 
Example_nestable_lock.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_nestable_lock.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_nestable_lock.1.f | ` !$OMP   PARALLEL SECTIONS ` | ` !$omp parallel sections  ` | true 
Example_nestable_lock.1.f | ` !$OMP   SECTION ` | ` !$omp section  ` | true 
Example_nestable_lock.1.f | ` !$OMP   SECTION ` | ` !$omp section  ` | true 
Example_nestable_lock.1.f | ` !$OMP   END PARALLEL SECTIONS ` | `  ` | true 
Example_nested_loop.1.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nested_loop.1.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nested_loop.1.c | ` #pragma omp parallel shared(i, n) ` | ` #pragma omp parallel shared (i, n) ` | true 
Example_nested_loop.1.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nested_loop.1.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nested_loop.1.f | ` !$OMP     DO ` | `  ` | true 
Example_nested_loop.1.f | ` !$OMP       PARALLEL SHARED(I,N) ` | ` !$omp parallel shared (i, n) ` | true 
Example_nested_loop.1.f | ` !$OMP         DO ` | `  ` | true 
Example_nested_loop.1.f | ` !$OMP       END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nested_loop.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nested_loop.2.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nested_loop.2.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nested_loop.2.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nested_loop.2.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nested_loop.2.f | ` !$OMP PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nested_loop.2.f | ` !$OMP DO ` | `  ` | true 
Example_nested_loop.2.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nested_loop.2.f | ` !$OMP PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nested_loop.2.f | ` !$OMP DO ` | `  ` | true 
Example_nested_loop.2.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nesting_restrict.1.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nesting_restrict.1.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nesting_restrict.1.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nesting_restrict.1.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nesting_restrict.1.f | ` !$OMP     DO ` | `  ` | true 
Example_nesting_restrict.1.f | ` !$OMP       DO             ! incorrect nesting of loop regions ` | `  ` | true 
Example_nesting_restrict.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nesting_restrict.2.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nesting_restrict.2.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nesting_restrict.2.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nesting_restrict.2.f | ` !$OMP   DO      ! incorrect nesting of loop regions ` | `  ` | true 
Example_nesting_restrict.2.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nesting_restrict.2.f | ` !$OMP     DO ` | `  ` | true 
Example_nesting_restrict.2.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nesting_restrict.3.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nesting_restrict.3.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nesting_restrict.3.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_nesting_restrict.3.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nesting_restrict.3.f | ` !$OMP     DO ` | `  ` | true 
Example_nesting_restrict.3.f | ` !$OMP       SINGLE            ! incorrect nesting of regions ` | `  ` | true 
Example_nesting_restrict.3.f | ` !$OMP       END SINGLE ` | ` !$omp end single  ` | true 
Example_nesting_restrict.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nesting_restrict.4.c | ` #pragma omp parallel default(shared) ` | ` #pragma omp parallel default (shared) ` | true 
Example_nesting_restrict.4.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_nesting_restrict.4.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_nesting_restrict.4.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nesting_restrict.4.f | ` !$OMP     DO ` | `  ` | true 
Example_nesting_restrict.4.f | ` !$OMP       BARRIER ` | ` !$omp barrier  ` | true 
Example_nesting_restrict.4.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nesting_restrict.5.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nesting_restrict.5.c | ` #pragma omp critical ` | ` #pragma omp critical  ` | true 
Example_nesting_restrict.5.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_nesting_restrict.5.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nesting_restrict.5.f | ` !$OMP     CRITICAL ` | ` !$omp critical  ` | true 
Example_nesting_restrict.5.f | ` !$OMP       BARRIER ` | ` !$omp barrier  ` | true 
Example_nesting_restrict.5.f | ` !$OMP     END CRITICAL ` | `  ` | true 
Example_nesting_restrict.5.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nesting_restrict.6.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nesting_restrict.6.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_nesting_restrict.6.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_nesting_restrict.6.f | ` !$OMP   PARALLEL DEFAULT(SHARED) ` | ` !$omp parallel default (shared) ` | true 
Example_nesting_restrict.6.f | ` !$OMP     SINGLE ` | ` !$omp single  ` | true 
Example_nesting_restrict.6.f | ` !$OMP       BARRIER ` | ` !$omp barrier  ` | true 
Example_nesting_restrict.6.f | ` !$OMP     END SINGLE ` | ` !$omp end single  ` | true 
Example_nesting_restrict.6.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nowait.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nowait.1.c | ` #pragma omp for nowait ` | ` #pragma omp for nowait ` | true 
Example_nowait.1.c | ` #pragma omp for nowait ` | ` #pragma omp for nowait ` | true 
Example_nowait.1.f | ` !$OMP PARALLEL ` | ` !$omp parallel  ` | true 
Example_nowait.1.f | ` !$OMP DO ` | `  ` | true 
Example_nowait.1.f | ` !$OMP END DO NOWAIT ` | `  ` | true 
Example_nowait.1.f | ` !$OMP DO ` | `  ` | true 
Example_nowait.1.f | ` !$OMP END DO NOWAIT ` | `  ` | true 
Example_nowait.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nowait.2.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nowait.2.c | ` #pragma omp for schedule(static) nowait ` | ` #pragma omp for nowait schedule (:static) ` | true 
Example_nowait.2.c | ` #pragma omp for schedule(static) nowait ` | ` #pragma omp for nowait schedule (:static) ` | true 
Example_nowait.2.c | ` #pragma omp for schedule(static) nowait ` | ` #pragma omp for nowait schedule (:static) ` | true 
Example_nowait.2.f90 | ` !$OMP PARALLEL ` | ` !$omp parallel  ` | true 
Example_nowait.2.f90 | ` !$OMP DO SCHEDULE(STATIC) ` | `  ` | true 
Example_nowait.2.f90 | ` !$OMP END DO NOWAIT ` | `  ` | true 
Example_nowait.2.f90 | ` !$OMP DO SCHEDULE(STATIC) ` | `  ` | true 
Example_nowait.2.f90 | ` !$OMP END DO NOWAIT ` | `  ` | true 
Example_nowait.2.f90 | ` !$OMP DO SCHEDULE(STATIC) ` | `  ` | true 
Example_nowait.2.f90 | ` !$OMP END DO NOWAIT ` | `  ` | true 
Example_nowait.2.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nthrs_dynamic.1.c | ` #pragma omp parallel num_threads(10) ` | ` #pragma omp parallel num_threads (10) ` | true 
Example_nthrs_dynamic.1.f | ` !$OMP     PARALLEL NUM_THREADS(10) ` | ` !$omp parallel num_threads (10) ` | true 
Example_nthrs_dynamic.1.f | ` !$OMP     END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nthrs_dynamic.2.c | ` #pragma omp parallel num_threads(10) ` | ` #pragma omp parallel num_threads (10) ` | true 
Example_nthrs_dynamic.2.f | ` !$OMP     PARALLEL NUM_THREADS(10) ` | ` !$omp parallel num_threads (10) ` | true 
Example_nthrs_dynamic.2.f | ` !$OMP     END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_nthrs_nesting.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_nthrs_nesting.1.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_nthrs_nesting.1.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_nthrs_nesting.1.f | ` !$omp barrier ` | ` !$omp barrier  ` | true 
Example_nthrs_nesting.1.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_nthrs_nesting.1.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_nthrs_nesting.1.f | ` !$omp barrier ` | ` !$omp barrier  ` | true 
Example_nthrs_nesting.1.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_nthrs_nesting.1.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_ordered.1.c | ` #pragma omp ordered ` | ` #pragma omp ordered  ` | true 
Example_ordered.1.c | ` #pragma omp parallel for ordered schedule(dynamic) ` | ` #pragma omp parallel for ordered schedule (:dynamic) ` | true 
Example_ordered.1.f | ` !$OMP ORDERED ` | ` !$omp ordered  ` | true 
Example_ordered.1.f | ` !$OMP END ORDERED ` | `  ` | true 
Example_ordered.1.f | ` !$OMP PARALLEL DO ORDERED SCHEDULE(DYNAMIC) ` | `  ` | true 
Example_ordered.1.f | ` !$OMP END PARALLEL DO ` | `  ` | true 
Example_ordered.2.c | ` #pragma omp for ordered ` | ` #pragma omp for ordered ` | true 
Example_ordered.2.c | ` #pragma omp ordered ` | ` #pragma omp ordered  ` | true 
Example_ordered.2.c | ` #pragma omp ordered ` | ` #pragma omp ordered  ` | true 
Example_ordered.2.f | ` !$OMP   DO ORDERED ` | `  ` | true 
Example_ordered.2.f | ` !$OMP     ORDERED ` | ` !$omp ordered  ` | true 
Example_ordered.2.f | ` !$OMP     END ORDERED ` | `  ` | true 
Example_ordered.2.f | ` !$OMP     ORDERED ` | ` !$omp ordered  ` | true 
Example_ordered.2.f | ` !$OMP     END ORDERED ` | `  ` | true 
Example_ordered.3.c | ` #pragma omp for ordered ` | ` #pragma omp for ordered ` | true 
Example_ordered.3.c | ` #pragma omp ordered ` | ` #pragma omp ordered  ` | true 
Example_ordered.3.c | ` #pragma omp ordered ` | ` #pragma omp ordered  ` | true 
Example_ordered.3.f | ` !$OMP   DO ORDERED ` | `  ` | true 
Example_ordered.3.f | ` !$OMP       ORDERED ` | ` !$omp ordered  ` | true 
Example_ordered.3.f | ` !$OMP       END ORDERED ` | `  ` | true 
Example_ordered.3.f | ` !$OMP       ORDERED ` | ` !$omp ordered  ` | true 
Example_ordered.3.f | ` !$OMP       END ORDERED ` | `  ` | true 
Example_parallel.1.c | ` #pragma omp parallel default(shared) private(iam,nt,ipoints,istart) ` | ` #pragma omp parallel default (shared) private (iam, nt, ipoints, istart) ` | true 
Example_parallel.1.f | ` !$OMP PARALLEL DEFAULT(PRIVATE) SHARED(X,NPOINTS) ` | ` !$omp parallel default (private) shared (x, npoints) ` | true 
Example_parallel.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_ploop.1.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_ploop.1.f | ` !$OMP PARALLEL DO  !I is private by default ` | `  ` | true 
Example_ploop.1.f | ` !$OMP END PARALLEL DO ` | `  ` | true 
Example_pra_iterator.1.cpp | ` #pragma omp parallel for default(none) shared(vec) ` | ` #pragma omp parallel for default (none) shared (vec) ` | true 
Example_private.1.c | ` #pragma omp parallel private(i) firstprivate(j) ` | ` #pragma omp parallel private (i) firstprivate (j) ` | true 
Example_private.1.f | ` !$OMP   PARALLEL PRIVATE(I) FIRSTPRIVATE(J) ` | ` !$omp parallel private (i) firstprivate (j) ` | true 
Example_private.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_private.2.c | ` #pragma omp parallel for private(a) ` | ` #pragma omp parallel for private (a) ` | true 
Example_private.2.f | ` !$OMP       PARALLEL DO PRIVATE(A) ` | `  ` | true 
Example_private.2.f | ` !$OMP       END PARALLEL DO ` | `  ` | true 
Example_private.3.c | ` #pragma omp parallel private(a) ` | ` #pragma omp parallel private (a) ` | true 
Example_private.3.c | ` #pragma omp parallel for private(a) ` | ` #pragma omp parallel for private (a) ` | true 
Example_private.3.f | ` !$OMP   PARALLEL PRIVATE(A) ` | ` !$omp parallel private (a) ` | true 
Example_private.3.f | ` !$OMP     PARALLEL DO PRIVATE(A) ` | `  ` | true 
Example_private.3.f | ` !$OMP     END PARALLEL DO ` | `  ` | true 
Example_private.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_psections.1.c | ` #pragma omp parallel sections ` | ` #pragma omp parallel sections  ` | true 
Example_psections.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_psections.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_psections.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_psections.1.f | ` !$OMP PARALLEL SECTIONS ` | ` !$omp parallel sections  ` | true 
Example_psections.1.f | ` !$OMP SECTION ` | ` !$omp section  ` | true 
Example_psections.1.f | ` !$OMP SECTION ` | ` !$omp section  ` | true 
Example_psections.1.f | ` !$OMP SECTION ` | ` !$omp section  ` | true 
Example_psections.1.f | ` !$OMP END PARALLEL SECTIONS ` | `  ` | true 
Example_reduction.1.c | ` #pragma omp parallel for private(i) shared(x, y, n) \ ` | ` #pragma omp parallel for private (i) shared (x, y, n) ` | true 
Example_reduction.1.f90 | ` !$OMP PARALLEL DO PRIVATE(I) SHARED(X, Y, N) REDUCTION(+:A) & ` | `  ` | true 
Example_reduction.1.f90 | ` !$OMP& REDUCTION(IEOR:B) REDUCTION(MIN:C)  REDUCTION(MAX:D) ` | `  ` | true 
Example_reduction.2.c | ` #pragma omp parallel shared(a, b, c, d, x, y, n) \ ` | ` #pragma omp parallel shared (a, b, c, d, x, y, n) ` | true 
Example_reduction.2.c | ` #pragma omp for private(i) ` | ` #pragma omp for private (i) ` | true 
Example_reduction.2.c | ` #pragma omp critical ` | ` #pragma omp critical  ` | true 
Example_reduction.2.f90 | ` !$OMP PARALLEL SHARED(X, Y, A, B, C, D, N) & ` | `  ` | true 
Example_reduction.2.f90 | ` !$OMP&         PRIVATE(A_P, B_P, C_P, D_P) ` | `  ` | true 
Example_reduction.2.f90 | ` !$OMP DO PRIVATE(I) ` | `  ` | true 
Example_reduction.2.f90 | ` !$OMP CRITICAL ` | ` !$omp critical  ` | true 
Example_reduction.2.f90 | ` !$OMP END CRITICAL ` | `  ` | true 
Example_reduction.2.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_reduction.3.c | ` #pragma omp parallel shared(a) private(i) ` | ` #pragma omp parallel private (i) shared (a) ` | true 
Example_reduction.3.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_reduction.3.c | ` #pragma omp for reduction(+:a) ` | ` #pragma omp for reduction (+ : a) ` | true 
Example_reduction.3.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_reduction.3.f90 | ` !$OMP PARALLEL DO REDUCTION(MAX: M) ` | `  ` | true 
Example_reduction.4.f90 | ` !$OMP PARALLEL DO REDUCTION(REN: N)     ! still does MAX ` | `  ` | true 
Example_reduction.5.f90 | ` !$OMP PARALLEL DO REDUCTION(MIN: R)     ! still does MAX ` | `  ` | true 
Example_reduction.6.c | ` #pragma omp parallel shared(a) private(i) ` | ` #pragma omp parallel private (i) shared (a) ` | true 
Example_reduction.6.c | ` #pragma omp master ` | ` #pragma omp master threadprivate  ` | true 
Example_reduction.6.c | ` #pragma omp for reduction(+:a) ` | ` #pragma omp for reduction (+ : a) ` | true 
Example_reduction.6.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_reduction.6.f | ` !$OMP PARALLEL SHARED(A) PRIVATE(I) ` | ` !$omp parallel private (i) shared (a) ` | true 
Example_reduction.6.f | ` !$OMP MASTER ` | ` !$omp master threadprivate  ` | true 
Example_reduction.6.f | ` !$OMP END MASTER ` | `  ` | true 
Example_reduction.6.f | ` !$OMP DO REDUCTION(+:A) ` | `  ` | true 
Example_reduction.6.f | ` !$OMP SINGLE ` | ` !$omp single  ` | true 
Example_reduction.6.f | ` !$OMP END SINGLE ` | ` !$omp end single  ` | true 
Example_reduction.6.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_reduction.7.c | ` #pragma omp parallel for reduction(+:a[0:N]) private(j) ` | ` #pragma omp parallel for private (j) reduction (+ : a[0:N]) ` | true 
Example_reduction.7.f90 | ` !$omp parallel do reduction(+:a) ` | `  ` | true 
Example_set_dynamic_nthrs.1.c | ` #pragma omp parallel shared(x, npoints) private(iam, ipoints) ` | ` #pragma omp parallel private (iam, ipoints) shared (x, npoints) ` | true 
Example_set_dynamic_nthrs.1.f | ` !$OMP   PARALLEL SHARED(X,NPOINTS) PRIVATE(IAM, IPOINTS) ` | ` !$omp parallel private (iam, ipoints) shared (x, npoints) ` | true 
Example_set_dynamic_nthrs.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_SIMD.1.c | ` #pragma omp simd ` | ` #pragma omp simd  ` | true 
Example_SIMD.1.f90 | ` !$omp simd ` | ` !$omp simd  ` | true 
Example_SIMD.2.c | ` #pragma omp declare simd uniform(fact) ` | ` #pragma omp declare simd uniform (fact) ` | true 
Example_SIMD.2.c | ` #pragma omp declare simd uniform(a,b,fact) linear(i:1) ` | ` #pragma omp declare simd linear (i:1) uniform (a, b, fact) ` | true 
Example_SIMD.2.c | ` #pragma omp declare simd uniform(fact) linear(a,b:1) ` | ` #pragma omp declare simd linear (a, b:1) uniform (fact) ` | true 
Example_SIMD.2.c | ` #pragma omp simd private(tmp) ` | ` #pragma omp simd private (tmp) ` | true 
Example_SIMD.2.f90 | ` !$omp declare simd(add1) uniform(fact) ` | `  ` | true 
Example_SIMD.2.f90 | ` !$omp declare simd(add2) uniform(a,b,fact) linear(i:1) ` | `  ` | true 
Example_SIMD.2.f90 | ` !$omp simd private(tmp) ` | ` !$omp simd private (tmp) ` | true 
Example_SIMD.3.c | ` #pragma omp simd private(tmp) reduction(+:sum) ` | ` #pragma omp simd private (tmp) reduction (+ : sum) ` | true 
Example_SIMD.3.f90 | ` !$omp simd private(tmp) reduction(+:sum) ` | ` !$omp simd private (tmp) reduction (+ : sum) ` | true 
Example_SIMD.4.c | ` #pragma omp simd safelen(16) ` | ` #pragma omp simd safelen (16) ` | true 
Example_SIMD.4.f90 | ` !$omp simd safelen(16) ` | ` !$omp simd safelen (16) ` | true 
Example_SIMD.5.c | ` #pragma omp for simd collapse(2) private(tmp) ` | ` #pragma omp for simd private (tmp) collapse (2) ` | true 
Example_SIMD.5.f90 | ` !$omp do simd collapse(2) private(tmp) ` | `  ` | true 
Example_SIMD.6.c | ` #pragma omp declare simd linear(p:1) notinbranch ` | ` #pragma omp declare simd linear (p:1) notinbranch ` | true 
Example_SIMD.6.c | ` #pragma omp simd ` | ` #pragma omp simd  ` | true 
Example_SIMD.6.c | ` #pragma omp declare simd linear(p:1) inbranch ` | ` #pragma omp declare simd linear (p:1) inbranch ` | true 
Example_SIMD.6.c | ` #pragma omp simd ` | ` #pragma omp simd  ` | true 
Example_SIMD.6.f90 | ` !$omp declare simd(foo) notinbranch ` | `  ` | true 
Example_SIMD.6.f90 | ` !$omp simd ` | ` !$omp simd  ` | true 
Example_SIMD.6.f90 | ` !$omp declare simd(goo) inbranch ` | `  ` | true 
Example_SIMD.6.f90 | ` !$omp simd ` | ` !$omp simd  ` | true 
Example_SIMD.7.c | ` #pragma omp declare simd inbranch ` | ` #pragma omp declare simd inbranch ` | true 
Example_SIMD.7.c | ` #pragma omp simd ` | ` #pragma omp simd  ` | true 
Example_SIMD.7.c | ` #pragma omp simd ` | ` #pragma omp simd  ` | true 
Example_SIMD.7.f90 | ` !$omp simd ` | ` !$omp simd  ` | true 
Example_SIMD.7.f90 | ` !$omp simd ` | ` !$omp simd  ` | true 
Example_SIMD.7.f90 | ` !$omp declare simd(fib) inbranch ` | `  ` | true 
Example_SIMD.8.c | ` #pragma omp simd lastprivate(pri) ` | ` #pragma omp simd lastprivate (pri) ` | true 
Example_SIMD.8.f90 | ` !$omp simd private(j) lastprivate(pri) ` | ` !$omp simd private (j) lastprivate (pri) ` | true 
Example_simple_lock.1.c | ` #pragma omp parallel shared(lck) private(id) ` | ` #pragma omp parallel private (id) shared (lck) ` | true 
Example_simple_lock.1.f | ` !$OMP   PARALLEL SHARED(LCK) PRIVATE(ID) ` | ` !$omp parallel private (id) shared (lck) ` | true 
Example_simple_lock.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_single.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_single.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_single.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_single.1.c | ` #pragma omp single nowait ` | ` #pragma omp single nowait ` | true 
Example_single.1.f | ` !$OMP PARALLEL ` | ` !$omp parallel  ` | true 
Example_single.1.f | ` !$OMP SINGLE ` | ` !$omp single  ` | true 
Example_single.1.f | ` !$OMP END SINGLE ` | ` !$omp end single  ` | true 
Example_single.1.f | ` !$OMP SINGLE ` | ` !$omp single  ` | true 
Example_single.1.f | ` !$OMP END SINGLE ` | ` !$omp end single  ` | true 
Example_single.1.f | ` !$OMP SINGLE ` | ` !$omp single  ` | true 
Example_single.1.f | ` !$OMP END SINGLE NOWAIT ` | ` !$omp end single nowait ` | true 
Example_single.1.f | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_standalone.1.c | ` #pragma omp flush(a) ` | ` #pragma omp flush (a)  ` | true 
Example_standalone.1.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_standalone.1.c | ` #pragma omp taskyield ` | ` #pragma omp taskyield  ` | true 
Example_standalone.1.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_standalone.2.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_standalone.2.c | ` #pragma omp flush(a) ` | ` #pragma omp flush (a)  ` | true 
Example_standalone.2.c | ` #pragma omp barrier ` | ` #pragma omp barrier  ` | true 
Example_standalone.2.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_standalone.2.c | ` #pragma omp taskyield ` | ` #pragma omp taskyield  ` | true 
Example_standalone.2.f90 | ` !$OMP FLUSH(A) ` | ` !$omp flush (a)  ` | true 
Example_standalone.2.f90 | ` !$OMP BARRIER ` | ` !$omp barrier  ` | true 
Example_standalone.2.f90 | ` !$OMP TASKWAIT ` | ` !$omp taskwait  ` | true 
Example_standalone.2.f90 | ` !$OMP TASKYIELD ` | ` !$omp taskyield  ` | true 
Example_standalone.2.f90 | ` !$OMP FLUSH(A) ` | ` !$omp flush (a)  ` | true 
Example_standalone.2.f90 | ` !$OMP BARRIER ` | ` !$omp barrier  ` | true 
Example_standalone.2.f90 | ` !$OMP TASKWAIT ` | ` !$omp taskwait  ` | true 
Example_standalone.2.f90 | ` !$OMP TASKYIELD ` | ` !$omp taskyield  ` | true 
Example_target.1.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target.1.c | ` #pragma omp parallel for private(i) ` | ` #pragma omp parallel for private (i) ` | true 
Example_target.1.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target.1.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target.1.f90 | ` !$omp end target ` | `  ` | true 
Example_target.2.c | ` #pragma omp target map(v1, v2, p) ` | `  ` | true 
Example_target.2.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target.2.f90 | ` !$omp target map(v1,v2,p) ` | `  ` | true 
Example_target.2.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target.2.f90 | ` !$omp end target ` | `  ` | true 
Example_target.3.c | ` #pragma omp target map(to: v1, v2) map(from: p) ` | ` #pragma omp target map ( to : v1, v2) map ( from : p) ` | true 
Example_target.3.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target.3.f90 | ` !$omp target map(to: v1,v2) map(from: p) ` | ` !$omp target map ( to : v1, v2) map ( from : p) ` | true 
Example_target.3.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target.3.f90 | ` !$omp end target ` | `  ` | true 
Example_target.4b.f90 | ` !$omp target map(to: v1(1:N), v2(:N)) map(from: p(1:N)) ` | `  ` | true 
Example_target.4b.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target.4b.f90 | ` !$omp end target ` | `  ` | true 
Example_target.4.c | ` #pragma omp target map(to: v1[0:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target map ( to : v1[0:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_target.4.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target.4.f90 | ` !$omp target map(to: v1(1:N), v2(:N)) map(from: p(1:N)) ` | `  ` | true 
Example_target.4.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target.4.f90 | ` !$omp end target ` | `  ` | true 
Example_target.5.c | ` #pragma omp target if(N>THRESHOLD1) map(to: v1[0:N], v2[:N])\ ` | ` #pragma omp target if (N>THRESHOLD1) map ( to : v1[0:N], v2[:N]) ` | true 
Example_target.5.c | ` #pragma omp parallel for if(N>THRESHOLD2) ` | ` #pragma omp parallel for if (N>THRESHOLD2) ` | true 
Example_target.5.f90 | ` !$omp target if(N>THRESHHOLD1) map(to: v1, v2 ) map(from: p) ` | ` !$omp target if (n>threshhold1) map ( to : v1, v2) map ( from : p) ` | true 
Example_target.5.f90 | ` !$omp parallel do if(N>THRESHOLD2) ` | `  ` | true 
Example_target.5.f90 | ` !$omp end target ` | `  ` | true 
Example_target.6.c | ` #pragma omp target parallel for \ ` | ` #pragma omp target parallel for  ` | true 
Example_target.6.f90 | ` !$omp target parallel do  & ` | `  ` | true 
Example_target.6.f90 | ` !$omp&   if(target: N>THRESHHOLD1) if(parallel: N>THRESHOLD2) & ` | `  ` | true 
Example_target.6.f90 | ` !$omp&   map(to: v1, v2 ) map(from: p) ` | `  ` | true 
Example_target.6.f90 | ` !$omp end target parallel do ` | `  ` | true 
Example_target_data.1.c | ` #pragma omp target data map(to: v1[0:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target data map ( to : v1[0:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_target_data.1.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target_data.1.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.1.f90 | ` !$omp target data map(to: v1, v2) map(from: p) ` | ` !$omp target data map ( to : v1, v2) map ( from : p) ` | true 
Example_target_data.1.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target_data.1.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.1.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.1.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_data.2.c | ` #pragma omp target data map(from: p[0:N]) ` | ` #pragma omp target data map ( from : p[0:N]) ` | true 
Example_target_data.2.c | ` #pragma omp target map(to: v1[:N], v2[:N]) ` | ` #pragma omp target map ( to : v1[:N], v2[:N]) ` | true 
Example_target_data.2.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.2.c | ` #pragma omp target map(to: v1[:N], v2[:N]) ` | ` #pragma omp target map ( to : v1[:N], v2[:N]) ` | true 
Example_target_data.2.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.2.f90 | ` !$omp target data map(from: p) ` | ` !$omp target data map ( from : p) ` | true 
Example_target_data.2.f90 | ` !$omp target map(to: v1, v2 ) ` | ` !$omp target map ( to : v1, v2) ` | true 
Example_target_data.2.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.2.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.2.f90 | ` !$omp target map(to: v1, v2 ) ` | ` !$omp target map ( to : v1, v2) ` | true 
Example_target_data.2.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.2.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.2.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_data.3.c | ` #pragma omp target data map(Q[0:rows][0:cols]) ` | `  ` | true 
Example_target_data.3.c | ` #pragma omp target map(tofrom: tmp) ` | ` #pragma omp target map ( tofrom : tmp) ` | true 
Example_target_data.3.c | ` #pragma omp parallel for reduction(+:tmp) ` | ` #pragma omp parallel for reduction (+ : tmp) ` | true 
Example_target_data.3.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target_data.3.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.3.f90 | ` !$omp target data map(Q) ` | `  ` | true 
Example_target_data.3.f90 | ` !$omp target map(tofrom: tmp) ` | ` !$omp target map ( tofrom : tmp) ` | true 
Example_target_data.3.f90 | ` !$omp parallel do reduction(+:tmp) ` | `  ` | true 
Example_target_data.3.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.3.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target_data.3.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.3.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.3.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_data.4.c | ` #pragma omp target data map(to: v1[0:N], v2[:N]) map(from: p0[0:N]) ` | ` #pragma omp target data map ( to : v1[0:N], v2[:N]) map ( from : p0[0:N]) ` | true 
Example_target_data.4.c | ` #pragma omp target map(to: v3[0:N], v4[:N]) map(from: p1[0:N]) ` | ` #pragma omp target map ( to : v3[0:N], v4[:N]) map ( from : p1[0:N]) ` | true 
Example_target_data.4.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.4.f90 | ` !$omp target data map(to: v1, v2) map(from: p0) ` | ` !$omp target data map ( to : v1, v2) map ( from : p0) ` | true 
Example_target_data.4.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_data.4.f90 | ` !$omp target map(to: v3, v4) map(from: p1) ` | ` !$omp target map ( to : v3, v4) map ( from : p1) ` | true 
Example_target_data.4.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.4.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.5.cpp | ` #pragma omp target data map(to: v1[0:N], v2[:N]) map(from: p0[0:N]) ` | ` #pragma omp target data map ( to : v1[0:N], v2[:N]) map ( from : p0[0:N]) ` | true 
Example_target_data.5.cpp | ` #pragma omp target map(to: v3[0:N], v4[:N]) map(from: p1[0:N]) ` | ` #pragma omp target map ( to : v3[0:N], v4[:N]) map ( from : p1[0:N]) ` | true 
Example_target_data.5.cpp | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.5.f90 | ` !$omp target data map(to: v1, v2) map(from: p0) ` | ` !$omp target data map ( to : v1, v2) map ( from : p0) ` | true 
Example_target_data.5.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_data.5.f90 | ` !$omp target map(to: v3, v4) map(from: p1) ` | ` !$omp target map ( to : v3, v4) map ( from : p1) ` | true 
Example_target_data.5.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.5.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.6.c | ` #pragma omp target data if(N>THRESHOLD) map(from: p[0:N]) ` | ` #pragma omp target data if (N>THRESHOLD) map ( from : p[0:N]) ` | true 
Example_target_data.6.c | ` #pragma omp target if (N>THRESHOLD) map(to: v1[:N], v2[:N]) ` | ` #pragma omp target if (N>THRESHOLD) map ( to : v1[:N], v2[:N]) ` | true 
Example_target_data.6.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.6.c | ` #pragma omp target if (N>THRESHOLD) map(to: v1[:N], v2[:N]) ` | ` #pragma omp target if (N>THRESHOLD) map ( to : v1[:N], v2[:N]) ` | true 
Example_target_data.6.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.6.f90 | ` !$omp target data if(N>THRESHOLD) map(from: p) ` | ` !$omp target data if (n>threshold) map ( from : p) ` | true 
Example_target_data.6.f90 | ` !$omp target if(N>THRESHOLD) map(to: v1, v2) ` | ` !$omp target if (n>threshold) map ( to : v1, v2) ` | true 
Example_target_data.6.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.6.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.6.f90 | ` !$omp target if(N>THRESHOLD) map(to: v1, v2) ` | ` !$omp target if (n>threshold) map ( to : v1, v2) ` | true 
Example_target_data.6.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.6.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.6.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_data.7.c | ` #pragma omp target data map(from: p[0:N]) ` | ` #pragma omp target data map ( from : p[0:N]) ` | true 
Example_target_data.7.c | ` #pragma omp target if (N>THRESHOLD) map(to: v1[:N], v2[:N]) ` | ` #pragma omp target if (N>THRESHOLD) map ( to : v1[:N], v2[:N]) ` | true 
Example_target_data.7.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_data.7.f90 | ` !$omp target data map(from: p) ` | ` !$omp target data map ( from : p) ` | true 
Example_target_data.7.f90 | ` !$omp target if(N>THRESHOLD) map(to: v1, v2) ` | ` !$omp target if (n>threshold) map ( to : v1, v2) ` | true 
Example_target_data.7.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_data.7.f90 | ` !$omp end target ` | `  ` | true 
Example_target_data.7.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_unstructured_data.1.c | ` #pragma omp target enter data map(alloc:mat->A[:n]) ` | ` #pragma omp target enter data map ( alloc : mat->A[:n]) ` | true 
Example_target_unstructured_data.1.c | ` #pragma omp target exit data map(delete:mat->A[:mat->N]) ` | ` #pragma omp target exit data map ( delete : mat->A[:mat->N]) ` | true 
Example_target_unstructured_data.1.cpp | ` #pragma omp target enter data map(alloc:v[0:len]) ` | ` #pragma omp target enter data map ( alloc : v[0:len]) ` | true 
Example_target_unstructured_data.1.cpp | ` #pragma omp target exit data map(delete:v[0:len]) ` | ` #pragma omp target exit data map ( delete : v[0:len]) ` | true 
Example_target_unstructured_data.1.f90 | ` !$omp target enter data map(alloc:A) ` | ` !$omp target enter data map ( alloc : a) ` | true 
Example_target_unstructured_data.1.f90 | ` !$omp target exit data map(delete:A) ` | ` !$omp target exit data map ( delete : a) ` | true 
Example_target_update.1.c | ` #pragma omp target data map(to: v1[:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target data map ( to : v1[:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_target_update.1.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target_update.1.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_update.1.c | ` #pragma omp target update to(v1[:N], v2[:N]) ` | ` #pragma omp target update to (v1[:N], v2[:N]) ` | true 
Example_target_update.1.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target_update.1.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_update.1.f90 | ` !$omp target data map(to: v1, v2) map(from: p) ` | ` !$omp target data map ( to : v1, v2) map ( from : p) ` | true 
Example_target_update.1.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target_update.1.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_update.1.f90 | ` !$omp end target ` | `  ` | true 
Example_target_update.1.f90 | ` !$omp target update to(v1, v2) ` | ` !$omp target update to (v1, v2) ` | true 
Example_target_update.1.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target_update.1.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_update.1.f90 | ` !$omp end target ` | `  ` | true 
Example_target_update.1.f90 | ` !$omp end target data ` | `  ` | true 
Example_target_update.2.c | ` #pragma omp target data map(to: v1[:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target data map ( to : v1[:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_target_update.2.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target_update.2.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_update.2.c | ` #pragma omp target update if (changed) to(v1[:N]) ` | ` #pragma omp target update if (changed) to (v1[:N]) ` | true 
Example_target_update.2.c | ` #pragma omp target update if (changed) to(v2[:N]) ` | ` #pragma omp target update if (changed) to (v2[:N]) ` | true 
Example_target_update.2.c | ` #pragma omp target ` | ` #pragma omp target  ` | true 
Example_target_update.2.c | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_target_update.2.f90 | ` !$omp target data map(to: v1, v2) map(from: p) ` | ` !$omp target data map ( to : v1, v2) map ( from : p) ` | true 
Example_target_update.2.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target_update.2.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_update.2.f90 | ` !$omp end target ` | `  ` | true 
Example_target_update.2.f90 | ` !$omp target update if(changed) to(v1(:N)) ` | `  ` | true 
Example_target_update.2.f90 | ` !$omp target update if(changed) to(v2(:N)) ` | `  ` | true 
Example_target_update.2.f90 | ` !$omp target ` | ` !$omp target  ` | true 
Example_target_update.2.f90 | ` !$omp parallel do ` | `  ` | true 
Example_target_update.2.f90 | ` !$omp end target ` | `  ` | true 
Example_target_update.2.f90 | ` !$omp end target data ` | `  ` | true 
Example_task_dep.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_task_dep.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_task_dep.1.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_task_dep.1.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_task_dep.1.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_task_dep.1.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_task_dep.1.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_task_dep.1.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.1.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_task_dep.1.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.1.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_task_dep.1.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_task_dep.2.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_task_dep.2.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_task_dep.2.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_task_dep.2.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_task_dep.2.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_task_dep.2.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_task_dep.2.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_task_dep.2.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.2.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_task_dep.2.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.2.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_task_dep.2.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_task_dep.3.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_task_dep.3.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_task_dep.3.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_task_dep.3.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_task_dep.3.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_task_dep.3.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_task_dep.3.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_task_dep.3.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_task_dep.3.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.3.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_task_dep.3.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.3.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_task_dep.3.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_task_dep.3.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_task_dep.4.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_task_dep.4.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_task_dep.4.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_task_dep.4.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_task_dep.4.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_task_dep.4.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_task_dep.4.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_task_dep.4.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_task_dep.4.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.4.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_task_dep.4.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.4.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_task_dep.4.f90 | ` !$omp end task ` | `  ` | true 
Example_task_dep.4.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_task_dep.4.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_task_dep.5.c | ` #pragma omp task private(ii, jj, kk) \ ` | ` #pragma omp task private (ii, jj, kk) ` | true 
Example_task_dep.5.f90 | ` !$omp task shared(A,B,C) private(ii,jj,kk) & ! I,J,K are firstprivate by default ` | `  ` | true 
Example_task_dep.5.f90 | ` !$omp depend ( in: A(i:i+BM, k:k+BM), B(k:k+BM, j:j+BM) ) & ` | `  ` | true 
Example_task_dep.5.f90 | ` !$omp depend ( inout: C(i:i+BM, j:j+BM) ) ` | `  ` | true 
Example_task_dep.5.f90 | ` !$omp end task ` | `  ` | true 
Example_taskgroup.1.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_taskgroup.1.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_taskgroup.1.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_taskgroup.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_taskgroup.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_taskgroup.1.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_taskgroup.1.c | ` #pragma omp taskgroup ` | ` #pragma omp taskgroup  ` | true 
Example_taskgroup.1.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_taskgroup.1.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_taskgroup.1.f90 | ` !$omp end task ` | `  ` | true 
Example_taskgroup.1.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_taskgroup.1.f90 | ` !$omp end task ` | `  ` | true 
Example_taskgroup.1.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_taskgroup.1.f90 | ` !$omp end task ` | `  ` | true 
Example_taskgroup.1.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_taskgroup.1.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_taskgroup.1.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_taskgroup.1.f90 | ` !$omp end task ` | `  ` | true 
Example_taskgroup.1.f90 | ` !$omp taskgroup ` | ` !$omp taskgroup  ` | true 
Example_taskgroup.1.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_taskgroup.1.f90 | ` !$omp end task ` | `  ` | true 
Example_taskgroup.1.f90 | ` !$omp end taskgroup ! wait on tree traversal in this step ` | `  ` | true 
Example_taskgroup.1.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_taskgroup.1.f90 | ` !$omp end parallel    ! only now is background work required to be complete ` | `  ` | true 
Example_tasking.10.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.10.c | ` #pragma omp for ` | ` #pragma omp for  ` | true 
Example_tasking.10.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.10.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.10.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.10.f90 | ` !$omp do ` | `  ` | true 
Example_tasking.10.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.10.f90 | ` !$omp task     ! Task Scheduling Point 1 ` | `  ` | true 
Example_tasking.10.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.10.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.10.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.11.c | ` #pragma omp task shared(x) mergeable ` | ` #pragma omp task shared (x) mergeable ` | true 
Example_tasking.11.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_tasking.11.f90 | ` !$omp task shared(x) mergeable ` | ` !$omp task shared (x) mergeable ` | true 
Example_tasking.11.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.11.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_tasking.12.c | ` #pragma omp task mergeable ` | ` #pragma omp task mergeable ` | true 
Example_tasking.12.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_tasking.12.f90 | ` !$omp task mergeable ` | ` !$omp task mergeable ` | true 
Example_tasking.12.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.12.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_tasking.13.c | ` #pragma omp task final( pos > LIMIT ) mergeable ` | ` #pragma omp task final (pos>LIMIT) mergeable ` | true 
Example_tasking.13.c | ` #pragma omp task final( pos > LIMIT ) mergeable ` | ` #pragma omp task final (pos>LIMIT) mergeable ` | true 
Example_tasking.13.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_tasking.13.f90 | ` !$omp task final(pos > LIMIT) mergeable ` | ` !$omp task final (pos>limit) mergeable ` | true 
Example_tasking.13.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.13.f90 | ` !$omp task final(pos > LIMIT) mergeable ` | ` !$omp task final (pos>limit) mergeable ` | true 
Example_tasking.13.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.13.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_tasking.14.c | ` #pragma omp task if(0)  // This task is undeferred ` | ` #pragma omp task if (0) ` | true 
Example_tasking.14.c | ` #pragma omp task     // This task is a regular task ` | ` #pragma omp task  ` | true 
Example_tasking.14.c | ` #pragma omp task     // This task is a regular task ` | ` #pragma omp task  ` | true 
Example_tasking.14.c | ` #pragma omp task final(1) // This task is a regular task ` | ` #pragma omp task final (1) ` | true 
Example_tasking.14.c | ` #pragma omp task  // This task is included ` | ` #pragma omp task  ` | true 
Example_tasking.14.c | ` #pragma omp task     // This task is also included ` | ` #pragma omp task  ` | true 
Example_tasking.14.f90 | ` !$omp task if(.FALSE.) ! This task is undeferred ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp task             ! This task is a regular task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp task             ! This task is a regular task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp task final(.TRUE.) ! This task is a regular task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp task               ! This task is included ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp task               ! This task is also included ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.14.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.15.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.15.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.15.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_tasking.15.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_tasking.15.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.15.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_tasking.15.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_tasking.15.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.15.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_tasking.15.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.15.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_tasking.15.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.16.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.16.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.16.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_tasking.16.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_tasking.16.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.16.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_tasking.16.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_tasking.16.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.16.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_tasking.16.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.16.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_tasking.16.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.17.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.17.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.17.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_tasking.17.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_tasking.17.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_tasking.17.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.17.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_tasking.17.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_tasking.17.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.17.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_tasking.17.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.17.f90 | ` !$omp taskwait ` | ` !$omp taskwait  ` | true 
Example_tasking.17.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_tasking.17.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.18.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.18.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.18.c | ` #pragma omp task shared(x) depend(out: x) ` | ` #pragma omp task shared (x) depend (out : x) ` | true 
Example_tasking.18.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_tasking.18.c | ` #pragma omp task shared(x) depend(in: x) ` | ` #pragma omp task shared (x) depend (in : x) ` | true 
Example_tasking.18.f90 | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.18.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_tasking.18.f90 | ` !$omp task shared(x) depend(out: x) ` | ` !$omp task shared (x) depend (out : x) ` | true 
Example_tasking.18.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.18.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_tasking.18.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.18.f90 | ` !$omp task shared(x) depend(in: x) ` | ` !$omp task shared (x) depend (in : x) ` | true 
Example_tasking.18.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.18.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_tasking.18.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.19.c | ` #pragma omp task depend ( in: A[i:BS][k:BS], B[k:BS][j:BS] ) \ ` | ` #pragma omp task depend (in : A[i:BS][k:BS], B[k:BS][j:BS]) ` | true 
Example_tasking.19.f90 | ` !$omp task depend ( in: A(i:i+BM, k:k+BM), B(k:k+BM, j:j+BM) ) & ` | `  ` | true 
Example_tasking.19.f90 | ` !$omp depend ( inout: C(i:i+BM, j:j+BM) ) ` | `  ` | true 
Example_tasking.19.f90 | ` !$omp end task ` | `  ` | true 
Example_tasking.1.c | ` #pragma omp task   // p is firstprivate by default ` | ` #pragma omp task  ` | true 
Example_tasking.1.c | ` #pragma omp task    // p is firstprivate by default ` | ` #pragma omp task  ` | true 
Example_tasking.1.f90 | ` !$OMP TASK     ! P is firstprivate by default ` | `  ` | true 
Example_tasking.1.f90 | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.1.f90 | ` !$OMP TASK     ! P is firstprivate by default ` | `  ` | true 
Example_tasking.1.f90 | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.2.c | ` #pragma omp task    // p is firstprivate by default ` | ` #pragma omp task  ` | true 
Example_tasking.2.c | ` #pragma omp task   // p is firstprivate by default ` | ` #pragma omp task  ` | true 
Example_tasking.2.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_tasking.2.f90 | ` !$OMP TASK    ! P is firstprivate by default ` | `  ` | true 
Example_tasking.2.f90 | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.2.f90 | ` !$OMP TASK    ! P is firstprivate by default ` | `  ` | true 
Example_tasking.2.f90 | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.2.f90 | ` !$OMP TASKWAIT ` | ` !$omp taskwait  ` | true 
Example_tasking.3.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.3.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.3.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.3.f90 | ` !$OMP PARALLEL PRIVATE(P) ` | ` !$omp parallel private (p) ` | true 
Example_tasking.3.f90 | ` !$OMP SINGLE ` | ` !$omp single  ` | true 
Example_tasking.3.f90 | ` !$OMP TASK ` | ` !$omp task  ` | true 
Example_tasking.3.f90 | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.3.f90 | ` !$OMP END SINGLE ` | ` !$omp end single  ` | true 
Example_tasking.3.f90 | ` !$OMP END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_tasking.4.c | ` #pragma omp task shared(i) ` | ` #pragma omp task shared (i) ` | true 
Example_tasking.4.c | ` #pragma omp task shared(j) ` | ` #pragma omp task shared (j) ` | true 
Example_tasking.4.c | ` #pragma omp taskwait ` | ` #pragma omp taskwait  ` | true 
Example_tasking.4.f | ` !$OMP TASK SHARED(i) ` | ` !$omp task shared (i) ` | true 
Example_tasking.4.f | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.4.f | ` !$OMP TASK SHARED(j) ` | ` !$omp task shared (j) ` | true 
Example_tasking.4.f | ` !$OMP END TASK ` | `  ` | true 
Example_tasking.4.f | ` !$OMP TASKWAIT ` | ` !$omp taskwait  ` | true 
Example_tasking.5.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.5.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.5.c | ` #pragma omp task    // i is firstprivate, item is shared ` | ` #pragma omp task  ` | true 
Example_tasking.5.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.5.f | ` !$omp single ! loop iteration variable i is private ` | `  ` | true 
Example_tasking.5.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.5.f | ` !$omp end task ` | `  ` | true 
Example_tasking.5.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_tasking.5.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.6.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.6.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_tasking.6.c | ` #pragma omp task untied ` | ` #pragma omp task untied ` | true 
Example_tasking.6.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.6.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.6.f | ` !$omp single ` | ` !$omp single  ` | true 
Example_tasking.6.f | ` !$omp task untied ` | ` !$omp task untied ` | true 
Example_tasking.6.f | ` !$omp task ! i is firstprivate, item is shared ` | `  ` | true 
Example_tasking.6.f | ` !$omp end task ` | `  ` | true 
Example_tasking.6.f | ` !$omp end task ` | `  ` | true 
Example_tasking.6.f | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_tasking.6.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.7.c | ` #pragma omp threadprivate(tp) ` | ` #pragma omp threadprivate (tp) ` | true 
Example_tasking.7.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.7.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.7.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.7.f | ` !$omp threadprivate(tp) ` | ` !$omp threadprivate (tp) ` | true 
Example_tasking.7.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.7.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.7.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.7.f | ` !$omp end task ` | `  ` | true 
Example_tasking.7.f | ` !$omp end task ` | `  ` | true 
Example_tasking.7.f | ` !$omp end task ` | `  ` | true 
Example_tasking.8.c | ` #pragma omp threadprivate(tp) ` | ` #pragma omp threadprivate (tp) ` | true 
Example_tasking.8.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_tasking.8.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.8.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.8.f | ` !$omp threadprivate(tp) ` | ` !$omp threadprivate (tp) ` | true 
Example_tasking.8.f | ` !$omp parallel ` | ` !$omp parallel  ` | true 
Example_tasking.8.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.8.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.8.f | ` !$omp end task ` | `  ` | true 
Example_tasking.8.f | ` !$omp end task ` | `  ` | true 
Example_tasking.8.f | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_tasking.9.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.9.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.9.c | ` #pragma omp critical //Critical region 1 ` | ` #pragma omp critical  ` | true 
Example_tasking.9.c | ` #pragma omp critical //Critical Region 2 ` | ` #pragma omp critical  ` | true 
Example_tasking.9.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_tasking.9.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.9.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.9.f | ` !$omp critical ` | ` !$omp critical  ` | true 
Example_tasking.9.f | ` !$omp end critical ` | `  ` | true 
Example_tasking.9.f | ` !$omp end task ` | `  ` | true 
Example_tasking.9.f | ` !$omp critical ` | ` !$omp critical  ` | true 
Example_tasking.9.f | ` !$omp task ` | ` !$omp task  ` | true 
Example_tasking.9.f | ` !$omp end task ` | `  ` | true 
Example_tasking.9.f | ` !$omp end critical ` | `  ` | true 
Example_tasking.9.f | ` !$omp end task ` | `  ` | true 
Example_taskloop.1.c | ` #pragma omp taskgroup ` | `  ` | true 
Example_taskloop.1.c | ` #pragma omp task ` | `  ` | true 
Example_taskloop.1.c | ` #pragma omp taskloop private(j) grainsize(500) nogroup  ` | `  ` | true 
Example_taskloop.1.f90 | ` !$omp taskgroup ` | `  ` | true 
Example_taskloop.1.f90 | ` !$omp task ` | `  ` | true 
Example_taskloop.1.f90 | ` !$omp end task ` | `  ` | true 
Example_taskloop.1.f90 | ` !$omp taskloop private(j) grainsize(500) nogroup ` | `  ` | true 
Example_taskloop.1.f90 | ` !$omp end taskloop ` | `  ` | true 
Example_taskloop.1.f90 | ` !$omp end taskgroup ` | `  ` | true 
Example_task_priority.1.c | ` #pragma omp parallel private(i) ` | ` #pragma omp parallel private (i) ` | true 
Example_task_priority.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_task_priority.1.c | ` #pragma omp task priority(i) ` | ` #pragma omp task priority (i) ` | true 
Example_task_priority.1.f90 | ` !$omp parallel private(i) ` | ` !$omp parallel private (i) ` | true 
Example_task_priority.1.f90 | ` !$omp single ` | ` !$omp single  ` | true 
Example_task_priority.1.f90 | ` !$omp task priority(i) ` | ` !$omp task priority (i) ` | true 
Example_task_priority.1.f90 | ` !$omp end task ` | `  ` | true 
Example_task_priority.1.f90 | ` !$omp end single ` | ` !$omp end single  ` | true 
Example_task_priority.1.f90 | ` !$omp end parallel ` | ` !$omp end parallel  ` | true 
Example_taskyield.1.c | ` #pragma omp task ` | ` #pragma omp task  ` | true 
Example_taskyield.1.c | ` #pragma omp taskyield ` | ` #pragma omp taskyield  ` | true 
Example_taskyield.1.f90 | ` !$omp task ` | ` !$omp task  ` | true 
Example_taskyield.1.f90 | ` !$omp taskyield ` | ` !$omp taskyield  ` | true 
Example_taskyield.1.f90 | ` !$omp end task ` | `  ` | true 
Example_teams.1.c | ` #pragma omp target map(to: B[:N], C[:N]) map(tofrom: sum0, sum1)  ` | ` #pragma omp target map ( to : B[:N], C[:N]) map ( tofrom : sum0, sum1) ` | true 
Example_teams.1.c | ` #pragma omp teams num_teams(2) ` | ` #pragma omp teams num_teams (2) ` | true 
Example_teams.1.c | ` #pragma omp parallel for reduction(+:sum0) ` | ` #pragma omp parallel for reduction (+ : sum0) ` | true 
Example_teams.1.c | ` #pragma omp parallel for reduction(+:sum1) ` | ` #pragma omp parallel for reduction (+ : sum1) ` | true 
Example_teams.1.f90 | ` !$omp target map(to: B, C) map(tofrom: sum0, sum1) ` | ` !$omp target map ( to : b, c) map ( tofrom : sum0, sum1) ` | true 
Example_teams.1.f90 | ` !$omp teams num_teams(2) ` | ` !$omp teams num_teams (2) ` | true 
Example_teams.1.f90 | ` !$omp parallel do reduction(+:sum0) ` | `  ` | true 
Example_teams.1.f90 | ` !$omp parallel do reduction(+:sum1) ` | `  ` | true 
Example_teams.1.f90 | ` !$omp end teams ` | ` !$omp end teams  ` | true 
Example_teams.1.f90 | ` !$omp end target ` | `  ` | true 
Example_teams.2.c | ` #pragma omp target map(to: B[0:N], C[0:N]) map(tofrom: sum) ` | ` #pragma omp target map ( to : B[0:N], C[0:N]) map ( tofrom : sum) ` | true 
Example_teams.2.c | ` #pragma omp teams num_teams(num_teams) thread_limit(block_threads) \ ` | `  ` | true 
Example_teams.2.c | ` #pragma omp distribute ` | ` #pragma omp distribute  ` | true 
Example_teams.2.c | ` #pragma omp parallel for reduction(+:sum) ` | ` #pragma omp parallel for reduction (+ : sum) ` | true 
Example_teams.2.f90 | ` !$omp target map(to: B, C) map(tofrom: sum) ` | ` !$omp target map ( to : b, c) map ( tofrom : sum) ` | true 
Example_teams.2.f90 | ` !$omp teams num_teams(num_teams) thread_limit(block_threads) & ` | `  ` | true 
Example_teams.2.f90 | ` !$omp&  reduction(+:sum) ` | `  ` | true 
Example_teams.2.f90 | ` !$omp distribute ` | ` !$omp distribute  ` | true 
Example_teams.2.f90 | ` !$omp parallel do reduction(+:sum) ` | `  ` | true 
Example_teams.2.f90 | ` !$omp end teams ` | ` !$omp end teams  ` | true 
Example_teams.2.f90 | ` !$omp end target ` | `  ` | true 
Example_teams.3.c | ` #pragma omp target teams map(to: B[0:N], C[0:N]) \ ` | ` #pragma omp target teams map ( to : B[0:N], C[0:N]) ` | true 
Example_teams.3.c | ` #pragma omp distribute parallel for reduction(+:sum) ` | ` #pragma omp distribute parallel for reduction (+ : sum) ` | true 
Example_teams.3.f90 | ` !$omp target teams map(to: B, C)  & ` | `  ` | true 
Example_teams.3.f90 | ` !$omp&             defaultmap(tofrom:scalar) reduction(+:sum) ` | `  ` | true 
Example_teams.3.f90 | ` !$omp distribute parallel do reduction(+:sum) ` | `  ` | true 
Example_teams.3.f90 | ` !$omp end target teams ` | `  ` | true 
Example_teams.4.c | ` #pragma omp target map(to: B[0:N], C[0:N]) map(tofrom: sum) ` | ` #pragma omp target map ( to : B[0:N], C[0:N]) map ( tofrom : sum) ` | true 
Example_teams.4.c | ` #pragma omp teams num_teams(8) thread_limit(16) reduction(+:sum) ` | ` #pragma omp teams reduction (+ : sum) num_teams (8) thread_limit (16) ` | true 
Example_teams.4.c | ` #pragma omp distribute parallel for reduction(+:sum) \ ` | ` #pragma omp distribute parallel for reduction (+ : sum) ` | true 
Example_teams.4.f90 | ` !$omp target map(to: B, C) map(tofrom: sum) ` | ` !$omp target map ( to : b, c) map ( tofrom : sum) ` | true 
Example_teams.4.f90 | ` !$omp teams num_teams(8) thread_limit(16) reduction(+:sum) ` | ` !$omp teams reduction (+ : sum) num_teams (8) thread_limit (16) ` | true 
Example_teams.4.f90 | ` !$omp distribute parallel do reduction(+:sum) & ` | `  ` | true 
Example_teams.4.f90 | ` !$omp&  dist_schedule(static, 1024) schedule(static, 64) ` | `  ` | true 
Example_teams.4.f90 | ` !$omp end teams ` | ` !$omp end teams  ` | true 
Example_teams.4.f90 | ` !$omp end target ` | `  ` | true 
Example_teams.5.c | ` #pragma omp target teams map(to: v1[0:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target teams map ( to : v1[0:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_teams.5.c | ` #pragma omp distribute simd ` | ` #pragma omp distribute simd  ` | true 
Example_teams.5.f90 | ` !$omp target teams map(to: v1, v2) map(from: p) ` | ` !$omp target teams map ( to : v1, v2) map ( from : p) ` | true 
Example_teams.5.f90 | ` !$omp distribute simd ` | ` !$omp distribute simd  ` | true 
Example_teams.5.f90 | ` !$omp end target teams ` | `  ` | true 
Example_teams.6.c | ` #pragma omp target teams map(to: v1[0:N], v2[:N]) map(from: p[0:N]) ` | ` #pragma omp target teams map ( to : v1[0:N], v2[:N]) map ( from : p[0:N]) ` | true 
Example_teams.6.c | ` #pragma omp distribute parallel for simd ` | ` #pragma omp distribute parallel for simd  ` | true 
Example_teams.6.f90 | ` !$omp target teams map(to: v1, v2) map(from: p) ` | ` !$omp target teams map ( to : v1, v2) map ( from : p) ` | true 
Example_teams.6.f90 | ` !$omp distribute parallel do simd ` | `  ` | true 
Example_teams.6.f90 | ` !$omp end target teams ` | `  ` | true 
Example_threadprivate.1.c | ` #pragma omp threadprivate(counter) ` | ` #pragma omp threadprivate (counter) ` | true 
Example_threadprivate.1.f | ` !$OMP   THREADPRIVATE(/INC_COMMON/) ` | `  ` | true 
Example_threadprivate.2.c | ` #pragma omp threadprivate(counter) ` | ` #pragma omp threadprivate (counter) ` | true 
Example_threadprivate.2.f | ` !$OMP   THREADPRIVATE(/T/) ` | `  ` | true 
Example_threadprivate.3.cpp | ` #pragma omp threadprivate(a, b) ` | ` #pragma omp threadprivate (a,b) ` | true 
Example_threadprivate.3.cpp | ` #pragma omp parallel for ` | ` #pragma omp parallel for  ` | true 
Example_threadprivate.3.f | ` !$OMP   THREADPRIVATE(/T/) ` | `  ` | true 
Example_threadprivate.3.f | ` !$OMP       PARALLEL COPYIN(/T/) ` | `  ` | true 
Example_threadprivate.3.f | ` !$OMP       END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_threadprivate.4.cpp | ` #pragma omp threadprivate(t1) ` | ` #pragma omp threadprivate (t1) ` | true 
Example_threadprivate.4.cpp | ` #pragma omp threadprivate(t2) ` | ` #pragma omp threadprivate (t2) ` | true 
Example_threadprivate.4.cpp | ` #pragma omp threadprivate(t3) ` | ` #pragma omp threadprivate (t3) ` | true 
Example_threadprivate.4.f | ` !$OMP   THREADPRIVATE(/T/) ` | `  ` | true 
Example_threadprivate.4.f | ` !$OMP       THREADPRIVATE(/T/) ` | `  ` | true 
Example_threadprivate.4.f | ` !$OMP       PARALLEL COPYIN(/T/) ` | `  ` | true 
Example_threadprivate.4.f | ` !$OMP       END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_threadprivate.5.cpp | ` #pragma omp threadprivate(i) ` | ` #pragma omp threadprivate (i) ` | true 
Example_threadprivate.5.f | ` !$OMP   THREADPRIVATE(A, I, PTR) ` | ` !$omp threadprivate (a,i,ptr) ` | true 
Example_threadprivate.5.f | ` !$OMP   PARALLEL COPYIN(I, PTR) ` | ` !$omp parallel copyin (i, ptr) ` | true 
Example_threadprivate.5.f | ` !$OMP     CRITICAL ` | ` !$omp critical  ` | true 
Example_threadprivate.5.f | ` !$OMP     END CRITICAL ` | `  ` | true 
Example_threadprivate.5.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_threadprivate.6.f | ` !$OMP   THREADPRIVATE(WORK) ` | ` !$omp threadprivate (work) ` | true 
Example_threadprivate.6.f | ` !$OMP   PARALLEL PRIVATE(THE_SUM) ` | ` !$omp parallel private (the_sum) ` | true 
Example_threadprivate.6.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.1.f | ` !$OMP    PARALLEL ` | ` !$omp parallel  ` | true 
Example_workshare.1.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.1.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.1.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.2.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_workshare.2.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.2.f | ` !$OMP     END WORKSHARE NOWAIT ` | `  ` | true 
Example_workshare.2.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.2.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.2.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.3.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_workshare.3.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.3.f | ` !$OMP       ATOMIC UPDATE ` | ` !$omp atomic update ` | true 
Example_workshare.3.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.3.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.4.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_workshare.4.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.4.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.4.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.5.f | ` !$OMP   PARALLEL SHARED(SHR) ` | ` !$omp parallel shared (shr) ` | true 
Example_workshare.5.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.5.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.5.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.6.f | ` !$OMP   PARALLEL PRIVATE(PRI) ` | ` !$omp parallel private (pri) ` | true 
Example_workshare.6.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.6.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.6.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_workshare.7.f | ` !$OMP   PARALLEL ` | ` !$omp parallel  ` | true 
Example_workshare.7.f | ` !$OMP     WORKSHARE ` | `  ` | true 
Example_workshare.7.f | ` !$OMP     END WORKSHARE ` | `  ` | true 
Example_workshare.7.f | ` !$OMP   END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_worksharing_critical.1.c | ` #pragma omp parallel sections ` | ` #pragma omp parallel sections  ` | true 
Example_worksharing_critical.1.c | ` #pragma omp section ` | ` #pragma omp section  ` | true 
Example_worksharing_critical.1.c | ` #pragma omp critical (name) ` | ` #pragma omp critical (name)  ` | true 
Example_worksharing_critical.1.c | ` #pragma omp parallel ` | ` #pragma omp parallel  ` | true 
Example_worksharing_critical.1.c | ` #pragma omp single ` | ` #pragma omp single  ` | true 
Example_worksharing_critical.1.f | ` !$OMP   PARALLEL SECTIONS ` | ` !$omp parallel sections  ` | true 
Example_worksharing_critical.1.f | ` !$OMP     SECTION ` | ` !$omp section  ` | true 
Example_worksharing_critical.1.f | ` !$OMP       CRITICAL (NAME) ` | ` !$omp critical (name)  ` | true 
Example_worksharing_critical.1.f | ` !$OMP         PARALLEL ` | ` !$omp parallel  ` | true 
Example_worksharing_critical.1.f | ` !$OMP           SINGLE ` | ` !$omp single  ` | true 
Example_worksharing_critical.1.f | ` !$OMP           END SINGLE ` | ` !$omp end single  ` | true 
Example_worksharing_critical.1.f | ` !$OMP         END PARALLEL ` | ` !$omp end parallel  ` | true 
Example_worksharing_critical.1.f | ` !$OMP       END CRITICAL (NAME) ` | `  ` | true 
Example_worksharing_critical.1.f | ` !$OMP   END PARALLEL SECTIONS ` | `  ` | true 
