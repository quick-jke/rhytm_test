#include <iostream>
#include "rhytm_test.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {

    auto config = get_cmd_path(argc, argv);
    auto xmlData = content(config.xml_path);


    std::istringstream input(xmlData);
    Lexer lexer(input);
    Parser parser(lexer);
    
    std::cout << parser.parseSystem().to_string();

    return 0;
}
