#include <stdio.h>
#include <OpenMPIR.h>
#include <string.h>
#include <iostream>

extern OpenMPDirective* parseOpenMP(const char*, void * _exprParse(const char*));

void output(OpenMPDirective*);

void output(OpenMPDirective* node) {

        std::string unparsing_string = node->generatePragmaString();
        std::cout << unparsing_string << "\n";
	    //node->generateDOT();

}

int main( int argc, const char* argv[] ) {

        //const char* input = "omp teams num_teams (4)";
        //const char* input = "omp teams num_teams (4) thread_limit (4+5) private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y) shared (a, b, c[1:10]) allocate (user_defined_test : m, n[1:5]) reduction (tasktest : x11, y, z) default (none)";
	//const char* input = "omp parallel  private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y), shared (a, b, c[1:10]) num_threads (4) ";


	//const char* input = "omp parallel reduction (tasktest : x11, y, z) allocate (user_defined_test : m, n[1:5]) allocate (omp_high_bw_mem_alloc : m, n[1:5]) reduction (inscan, max : a, foo(x))";

	//const char* input = "omp parallel reduction (tasktest : x11, y, z) private (x, n[1:5]) private (m, n[1:5]) reduction (inscan, max : a, foo(x))";


	//const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) num_threads (3*5+4/(7+10)) allocate (omp_user_defined_mem_alloc : m, n[1:5]) allocate (no, allo, cator)";

	//const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y), shared (a, b, c[1:10]) ";

	//const char* input = "OMP parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y), shared (a, b, c[1:10]) ";


	//const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) num_threads (3*5+4/(7+10)) allocate (no, allo, cator)";

	//const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y), shared (a, b, c[1:10]) ";

	//const char* input = "omp parallel private (a[foo(x, goo(x, y)):100], b[1:30], c) allocate (xx) allocate (no, allo, cator)";
	const char* input = "omp metadirective when ( user = { condition (b < 14) } : ) when ( user = { condition (a < 4) } device = { kind (cpu) isa(avx512f) } implementation = {vendor(gnu)} : parallel private (a, bb)) when ( construct = {parallel (score(4) : private (e) )} : parallel private (a, bb)) default (parallel shared (c, dd))";

    //const char* input = "omp declare variant (...) match ( user = { condition (a < 4) } device = { kind (fpga) isa(avx512f) } implementation = {vendor(llvm)})";
	//const char* input = "omp for collapse(a) order(dasfe) nowait ordered(sd) allocate (no, allo, cator) lastprivate(conditional:i, last, private) linear(var(s,f,e):2) linear(s,f,e) schedule(static)";
   
	//const char* input = "omp simd collapse(a) order(dasfe)  safelen(sd) simdlen(4) nontemporal(non, temporal) lastprivate(conditional:i, last, private) linear(var(s,f,e):2) linear(s,f,e)  aligned(s,f,e)";
 
	//const char* input = "omp for lastprivate(conditional:i, last, private)";
        //const char* input = "omp for simd collapse(a) safelen(sd) simdlen(4) nontemporal(non, temporal) lastprivate(conditional:i, last, private)  linear(s,f,e)  aligned(s,f,e:2) nowait ordered(sd) order(dasfe)";
        //const char* input = "omp declare simdlen(4) linear(val(s,f,e))  aligned(s,f,e:2) inbranch notinbranch uniform(c,b,a) ";
        //const char* input = "omp distribute dist_schedule(static,3) collapse(a) allocate (no, allo, cator) lastprivate(conditional:i, last, private) ";
        //const char* input = "omp distribute simd dist_schedule(static,3) collapse(a) allocate (no, allo, cator) lastprivate(conditional:i, last, private) safelen(sd) simdlen(4) nontemporal(non, temporal)";
        //const char* input = "omp distribute parallel for dist_schedule(static,3) collapse(a) order(dasfe) nowait ordered(sd) allocate (no, allo, cator) lastprivate(conditional:i, last, private)";
        //const char* input = "omp distribute parallel for simd dist_schedule(static,3) safelen(sd) simdlen(4) collapse(a) order(dasfe) nowait ordered(sd) allocate (no, allo, cator) lastprivate(conditional:i, last, private)";
        //const char* input = "omp loop bind(teams) order(sdfa)";
        //const char* input = "omp scan inclusive(a,b,c)";
        //const char* input = "omp scan exclusive(a,b,c)";
        //const char* input = "omp sections reduction (tasktest : x11, y, z) allocate (user_defined_test : m, n[1:5]) private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y)" ;
        //const char* input = "omp section";
        //const char* input = "omp single nowait copyprivate(co,py) allocate (user_defined_test : m, n[1:5]) private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y)" ;
        //const char* input = "omp cancel parallel if(cancel:af)" ;
        //const char* input = "omp cancellation   point sections" ;
        //const char* input = "omp parallel if(cancel:af)" ;
        
        
        OpenMPDirective* openMPAST = parseOpenMP(input, NULL);
        output(openMPAST);
        printf("\n");
        return 0;
}

