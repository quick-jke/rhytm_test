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

    // std::cout << "=== Token Stream ===\n";
    // while (true) {
    //     TOKEN tok = lexer.getNextToken();
    //     std::cout << "Token: " << tokenToString(tok);
    //     if (tok != tok_eof) {
    //         std::string text = lexer.getText();
    //         if (!text.empty()) {
    //             std::cout << " | Value: \"" << text << "\"";
    //         }
    //     }
    //     std::cout << "\n";
    //     if (tok == tok_eof) break;
    // }

    Parser parser(lexer);
    // std::cout << parser.parseSystem().to_string() << std::endl;
    System system = parser.parseSystem();


    CodeGenerator gen(system);
    gen.write(config.out_path);

    return 0;
}


