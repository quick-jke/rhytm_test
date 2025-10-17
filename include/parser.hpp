#ifndef INCLUDE_PARSER_HPP
#define INCLUDE_PARSER_HPP

#include "lexer.hpp"
#include "scheme.hpp"
#include "utils.hpp"
#include <memory>
#include <iostream>

class Parser {
public:
    explicit Parser(Lexer& lexer);
    System parseSystem();

private:
    Lexer& lexer;
    TOKEN currentToken;
    std::string currentText;

    void advance();
    void expect(TOKEN expected);
    
    P parseP();
    Port parsePort();
    Branch parseBranch();
    Block parseBlock();
    Line parseLine();
};

#endif
