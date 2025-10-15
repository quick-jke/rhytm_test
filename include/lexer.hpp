#ifndef INCLUDE_LEXER_HPP
#define INCLUDE_LEXER_HPP

#include "token.hpp"
#include <istream>
#include <string>
#include <cctype>
#include <sstream>
#include <queue>

class Lexer {
public:
    explicit Lexer(std::istream& input);

    TOKEN getNextToken();
    const std::string& getText() const { return currentText; }

private:
    std::istream& in;
    char currentChar = 0;
    std::string currentText;
    bool atEnd = false;

    void advance();
    void skipWhitespace();
    TOKEN readTag();
    TOKEN readAttributeName();
    void readAttributeValue(); 
    void readTextContent();
    bool isNameStartChar(char c);
    bool isNameChar(char c);

    std::queue<TOKEN> pendingTokens;
    std::queue<std::string> pendingTexts;

    void readOpeningTag(const std::string& tagName);

};

#endif
