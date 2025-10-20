#include <iostream>
#include "rhytm_test.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"
#include "topological_sort.hpp"

int main(int argc, char* argv[]) {

    auto config = get_cmd_path(argc, argv);
    auto xmlData = content(config.xml_path);
    std::istringstream input(xmlData);
    Lexer lexer(input);
    Parser parser(lexer);
    System system = parser.parseSystem();
    CodeGenerator gen(system);
    gen.write(config.out_path);

    return 0;
}


