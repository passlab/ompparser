#include <iostream>
#include <regex>
#include <fstream>

void output(std::vector<std::string>);
int openFile(std::ifstream&, const char*);
std::vector<std::string>* process(std::ifstream&);

void output(std::vector<std::string> *omp_pragmas) {

    if (omp_pragmas != NULL) {
        for (int i = 0; i < omp_pragmas->size(); i++) {
            std::cout << omp_pragmas->at(i) << std::endl;
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

std::vector<std::string>* process(std::ifstream& input_file) {

    std::string input_pragma;
    int total_amount = 0;
    int line_no = 0;
    int current_pragma_line_no = 1;
    std::vector<std::string> *omp_pragmas = new std::vector<std::string>();

    char current_char = input_file.peek();
    std::string current_line;
    std::regex c_regex ("^([[:blank:]]*#pragma)([[:blank:]]+)(omp)[[:blank:]]+(.*)");
    std::regex comment_regex ("[/][*].*[*][/](?!.*[*][/].*)");
    std::regex continue_regex ("([\\\\]+[[:blank:]]*$)");

    while (!input_file.eof()) {
        line_no += 1;
        switch (current_char) {
            case '\n':
                input_file.seekg(1, std::ios_base::cur);
                break;
            default:
                std::getline(input_file, current_line);
                // remove the inline comments
                current_line = std::regex_replace(current_line, comment_regex, "");
                input_pragma = "";
                if (std::regex_match(current_line, c_regex)) {
                    // combine continuous lines if necessary
                    while (std::regex_search(current_line, continue_regex)) {
                        // remove the slash part at the end
                        current_line = std::regex_replace(current_line, continue_regex, "");
                        // add the current line to the pragma string
                        input_pragma += current_line;
                        // get the next line
                        std::getline(input_file, current_line);
                        // remove the inline comments of next line
                        current_line = std::regex_replace(current_line, comment_regex, "");
                    };
                    input_pragma += current_line;
                    total_amount += 1;
                    current_pragma_line_no = line_no;
                    //std::cout << input_pragma << std::endl;
                    omp_pragmas->push_back(input_pragma);
                }
        };
        current_char = input_file.peek();
    };

    return omp_pragmas;
}

int main( int argc, const char* argv[] ) {
    const char* filename = NULL;
    int result;
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

    std::vector<std::string> *omp_pragmas = process(input_file);

    std::cout << "=================== SUMMARY ===================\n";
    std::cout << "TOTAL OPENMP PRAGMAS: " << omp_pragmas->size() << "\n";

    output(omp_pragmas);

    return 0;
}

