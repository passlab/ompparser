#include <stdio.h>
#include <OpenMPIR.h>
#include <string.h>
#include <iostream>
#include <regex>

extern OpenMPDirective* parseOpenMP(const char*, void *_exprParse(const char*));
extern void setLang(OpenMPBaseLang);
void output(OpenMPDirective*);
std::string test(OpenMPDirective*);
int openFile(std::ifstream&, const char*);

void output(OpenMPDirective* node) {
    if (node) {
        std::string unparsing_string = node->generatePragmaString();
        std::cout << unparsing_string << "\n";
        node->generateDOT();
    };
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
    std::string directives;

    while (!input_file.eof()) {
        line_no += 1;
        switch (current_char) {
            case '\n':
                input_file.seekg(1, ios_base::cur);
                break;
            default:
                std::getline(input_file, current_line);
                current_line = std::regex_replace(current_line, std::regex("^\\s+"), std::string(""));
                if (std::regex_match(current_line.substr(0, 5), fortran_regex)) {
                    is_fortran = true;
                    base_lang = Lang_Fortran;
                }
                else {
                    base_lang = Lang_C;
                };
                if (current_line.substr(0, 7) == "#pragma" || is_fortran) {
                    total_amount += 1;
                    current_pragma_line_no = line_no;
                    input_pragma = current_line;
                    if (is_fortran) {
                        std::transform(current_line.begin(), current_line.end(), current_line.begin(), ::tolower);
                    };
                    OpenMPDirective* openMPAST = parseOpenMP(current_line.c_str(), NULL);
                    //output_pragma = test(openMPAST);
                    if (openMPAST) {
                        directives += openMPAST->toString();
                        directives += ", ";
                    }
                    is_fortran = false;
                }
                input_pragma.clear();
                validation_string.clear();
        };
        current_char = input_file.peek();
    };

    input_pragma.clear();
    std::size_t loc = std::string(filename).find_last_of("/\\");
    printf("%s: %s\n", std::string(filename).substr(loc+1).c_str(), directives.c_str());


    return 0;
}

