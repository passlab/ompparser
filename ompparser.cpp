#include <iostream>
#include <regex>
#include <fstream>
#include <OpenMPIR.h>

void output(std::vector<std::string>);
int openFile(std::ifstream&, const char*);
extern std::vector<std::string>* preProcessC(std::ifstream&);
extern OpenMPDirective* parseOpenMP(const char*, void *_exprParse(const char*));
extern void setLang(OpenMPBaseLang);

void output(std::vector<OpenMPDirective*> *omp_ast_list) {

    if (omp_ast_list != NULL) {
        for (int i = 0; i < omp_ast_list->size(); i++) {
            if (omp_ast_list->at(i) != NULL) {
                std::cout << omp_ast_list->at(i)->generatePragmaString() << std::endl;
            }
            else {
                std::cout << "NULL" << std::endl;
            };
        };
    };
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
    int result;
    int i;
    std::vector<OpenMPDirective*> *omp_ast_list = new std::vector<OpenMPDirective*>();
    OpenMPDirective* omp_ast = NULL;
    std::vector<std::string> *omp_directive_list = new std::vector<std::string>();
    if (argc > 1) {
        filename = argv[1];
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

    std::vector<std::string> *omp_pragmas = preProcessC(input_file);

    // parse the preprocessed inputs
    for (i = 0; i < omp_pragmas->size(); i++) {
        omp_ast = parseOpenMP(omp_pragmas->at(i).c_str(), NULL);
        omp_ast_list->push_back(omp_ast);
        if (omp_ast != NULL) {
            omp_directive_list->push_back(omp_ast->toString());
        }
        else {
            omp_directive_list->push_back("");
        };
    };

    std::cout << "=================== SUMMARY ===================\n";
    std::cout << "TOTAL OPENMP PRAGMAS: " << omp_pragmas->size() << "\n";

    output(omp_ast_list);

    return 0;
}

