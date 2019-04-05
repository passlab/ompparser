#include <stdio.h>
#include <OpenMPIR.h>
#include <string.h>
#include <iostream>
#include <regex>

extern OpenMPDirective* parseOpenMP(const char*, void *_exprParse(const char*));

void output(OpenMPDirective*);
std::string test(OpenMPDirective*);
int openFile(std::ifstream&, const char*);

void output(OpenMPDirective* node) {

    std::string unparsing_string = node->generatePragmaString();
    std::cout << unparsing_string << "\n";
    node->generateDOT();

}

std::string test(OpenMPDirective* node) {

    std::string unparsing_string;
    if (node) {
        unparsing_string = node->generatePragmaString();
    }
    return unparsing_string;
}

int openFile(std::ifstream& file, const char* filename) {
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Exception caused by opening the given file\n";
        return -1;
    }

    return 0;
}



int main( int argc, const char* argv[] ) {
    const char* filename = NULL;
    const char* mode = "string";
    int result;
    if (argc > 1) {
        filename = argv[1];
    };
    if (argc > 2) {
        mode = argv[2];
    };
    std::ifstream input_file;

    if (filename != NULL) {
        result = openFile(input_file, filename);
    }
    else {
        std::cout << "No specific testing file is provided, use the default PARALLEL testing instead.\n";
        result = openFile(input_file, "../tests/parallel.txt");
    };
    if (result) {
        std::cout << "No testing file is available.\n";
        return -1;
    };
    std::string input_pragma;
    std::string output_pragma;
    std::string validation_string;
    int total_amount = 0;
    int passed_amount = 0;
    int failed_amount = 0;
    int invalid_amount = 0;
    int line_no = 0;
    int current_pragma_line_no = 1;

    char current_char = input_file.peek();
    std::string current_line;
    OpenMPBaseLang base_lang = Lang_C;
    std::regex fortran_regex ("[!][$][Oo][Mm][Pp]");
    bool is_fortran = false;

    while (!input_file.eof()) {
        line_no += 1;
        switch (current_char) {
            case '\n':
                input_file.seekg(1, ios_base::cur);
                break;
            default:
                std::getline(input_file, current_line);
                if (std::regex_match(current_line.substr(0, 5), fortran_regex)) {
                    is_fortran = true;
                    base_lang = Lang_Fortran;
                }
                else {
                    base_lang = Lang_C;
                };
                if (current_line.substr(0, 7) == "#pragma" || is_fortran) {
                    total_amount += 1;
                    if (input_pragma.size()) {
                        std::cout << "======================================\n";
                        std::cout << "Line: " << current_pragma_line_no << "\n";
                        std::cout << "GIVEN INPUT: " << input_pragma << "\n";
                        std::cout << "GENERATED OUTPUT: " << output_pragma << "\n";
                        std::cout << "Missing the output for validation.\n";
                        std::cout << "======================================\n";
                        invalid_amount += 1;
                    };
                    current_pragma_line_no = line_no;
                    input_pragma = current_line;
                    /*
                    if (is_fortran) {
                        std::transform(current_line.begin(), current_line.end(), current_line.begin(), ::tolower);
                    };
                    */
                    OpenMPDirective* openMPAST = parseOpenMP(current_line.c_str(), NULL);
                    output_pragma = test(openMPAST);
                    is_fortran = false;
                }
                else if (current_line.substr(0, 6) == "PASS: ") {
                    validation_string = current_line.substr(6);
                    if (input_pragma.size() == 0) {
                        validation_string.clear();
                    }
                    else if (output_pragma != validation_string) {
                        std::cout << "======================================\n";
                        std::cout << "Line: " << current_pragma_line_no << "\n";
                        std::cout << "FAILED INPUT: " << input_pragma << "\n";
                        std::cout << "WRONG OUTPUT: " << output_pragma << "\n";
                        std::cout << "EXPECTED OUTPUT: " << validation_string << "\n";
                        std::cout << "======================================\n";
                        failed_amount += 1;
                    }
                    else {
                        passed_amount += 1;
                    }
                    input_pragma.clear();
                    validation_string.clear();
                }
        };
        current_char = input_file.peek();
    };

    if (input_pragma.size()) {
        std::cout << "======================================\n";
        std::cout << "Line: " << current_pragma_line_no << "\n";
        std::cout << "GIVEN INPUT: " << input_pragma << "\n";
        std::cout << "GENERATED OUTPUT: " << output_pragma << "\n";
        std::cout << "Missing the output for validation.\n";
        std::cout << "======================================\n";
        invalid_amount += 1;
    };
    input_pragma.clear();

    std::cout << "=================== SUMMARY ===================\n";
    std::cout << "TOTAL TESTS  : " << total_amount << "\n";
    std::cout << "PASSED TESTS : " << passed_amount << "\n";
    std::cout << "FAILED TESTS : " << failed_amount << "\n";
    std::cout << "INVALID TESTS: " << invalid_amount << "\n";

    //const char* input = "omp teams num_teams (4)";
     //const char* input = "omp teams num_teams (4) thread_limit (4+5) private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y) shared (a, b, c[1:10]) allocate (user_defined_test : m, n[1:5]) reduction (tasktest : x11, y, z) default (none)";

    //const char* input = "omp metadirective when ( user = { condition (b < 14) } , device = {arch(x64), kind(gpu)}, implementation = {extension(riscv), vendor(pgi)} : ) when ( user = { condition (a < 4) } device = { kind (cpu) isa(avx512f) } implementation = {vendor(gnu)} : parallel private (a, bb)) when ( construct = {parallel (score(4) : private (e) )} : parallel private (a, bb)) default (parallel shared (c, dd))";

        //const char* input = "omp declare variant (...) match ( user = { condition (a < 4) } device = { kind (fpga) isa(avx512f) } implementation = {vendor(llvm)})";
    //const char* input = "omp for collapse(a) order(dasfe) nowait ordered(sd) allocate (no, allo, cator) lastprivate(conditional:i, last, private) linear(var(s,f,e):2) linear(s,f,e) schedule(static)";
   
    //const char* input = "omp simd collapse(a) order(dasfe)  safelen(sd) simdlen(4) nontemporal(non, temporal) lastprivate(conditional:i, last, private) linear(var(s,f,e):2) linear(s,f,e)  aligned(s,f,e)";

//const char* input = "omp for schedule(monotonic:static,x) linear(var(s,f,e):3) linear(val(s,f,e):s)";


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
        //const char* input = "omp allocate(a,b,c) allocator(omp_default_mem_alloc    )";

//const char* input = "omp target if(target:3456) device(ancestor:5) private (x, n[1:5]) firstprivate (foo(x), y) in_reduction (test_identifier : x11, y, z) is_device_ptr(m,n,j) defaultmap(alloc:pointer) nowait depend(iterator(int bba=4:120:2), in:m, n) allocate (omp_default_mem_alloc : m, n[1:5]) uses_allocators(omp_default_mem_alloc(1234567),omp_const_mem_alloc(234)) ";
//const char* input = "omp task if(task: 5858*5) final (5) affinity(iterator(int bba=4:120:2, b=1:220:3, int c=2:57:9):b, c) in_reduction (test_identifier : x11, y, z) untied mergeable priority(5) detach(abc) depend(iterator(int bba=4:120:2, b=1:220:3, int c=2:57:9),in:m, n) private (a[foo(x, goo(x, y)):100], b[1:30], c) firstprivate (foo(x), y) shared (a, b, c[1:10]) allocate (user_defined_test : m, n[1:5]) default (none)";
//const char* input = "omp requires ext_user_test reverse_offload unified_address unified_shared_memory,atomic_default_mem_order(acq_rel) dynamic_allocators";
//const char* input = "omp target data if(target data:3456) device(5) use_device_ptr(*p,*pt,*ptr) use_device_addr(5)";
//const char* input = "omp target enter data if(target enter data:3456) device(5) depend(iterator(int bba=4:120:2), in:m, n) nowait ";
//const char* input = "omp target update if(target update:3456) device(5) depend(iterator(int bba=4:120:2), in:m, n) nowait ";
//const char* input = "omp target update to(mapper(default):m,i,o) from(m,i,o)";
//const char* input = "omp target exit data if(target exit data:3456) device(5) depend(iterator(int bba=4:120:2), in:m, n) nowait ";
//const char* input = "omp end declare target";
 //const char* input = "omp target update if(target update:3456) device(5) depend(iterator(int bba=4:120:2), in:m, n) nowait ";
 //const char* input = "omp target update to(mapper(default):m,i,o) from(mapper(default):m,i,o)";
// const char* input = "omp declare target to(x,y,z) link(b,n,m) device_type(nohost)";
 // const char* input ="#pragma omp parallel reduction (inscan, + : a, foo(x)) reduction (abc : x, y, z) reduction (task, user_defined_value : x, y, z) reduction (inscan, max : a, foo(x))";
//const char* input = "omp taskloop simd collapse(a) order(dasfe)  safelen(sd) simdlen(4) nontemporal(non, temporal) lastprivate(conditional:i, last, private) linear(var(s,f,e):2) linear(s,f,e)  aligned(s,f,e) if(taskloop:3456) shared(x,y,z) private (x, n[1:5]) firstprivate (foo(x), y) lastprivate(rt,e,tre) reduction (default, + : a, foo(x)) in_reduction (abc : x, y, z) default(shared) grainsize(8) num_tasks(45) collapse(34) final(890) priority(4) untied mergeable nogroup allocate (user_defined_test : m, n[1:5])";
//const char* input = "omp taskyield";
//const char* input = "omp barrier";
//const char* input = "omp taskwait depend(iterator(int bba=4:120:2, b=1:220:3, int c=2:57:9),in:m, n) depend(iterator(int bba=4:120:2, b=1:220:3, int c=2:57:9),out:v, b) depend(iterator(int bba=4:120:2, b=1:220:3, int c=2:57:9),inout:x, c)";
const char* input = "omp taskloop if(taskloop:3456) shared(x,y,z) private (x, n[1:5]) firstprivate (foo(x), y) lastprivate(rt,e,tre) reduction (default, + : a, foo(x)) in_reduction (abc : x, y, z) default(shared) grainsize(8) num_tasks(45) collapse(34) final(890) priority(4) untied mergeable nogroup allocate (user_defined_test : m, n[1:5])";


        
        
   // OpenMPDirective* openMPAST = parseOpenMP(input, NULL);
   // output(openMPAST);
    return 0;
}

