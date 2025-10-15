#include "lexer.hpp"
#include <iostream>
#include <cctype>
#include <stdexcept>

Lexer::Lexer(std::istream& input) : in(input) {
    advance();
}

void Lexer::advance() {
    if (in.get(currentChar)) {
        atEnd = false;
    } else {
        atEnd = true;
        currentChar = 0;
    }
}

void Lexer::skipWhitespace() {
    while (!atEnd && std::isspace(static_cast<unsigned char>(currentChar))) {
        advance();
    }
}

bool Lexer::isNameStartChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isNameChar(char c) {
    return isNameStartChar(c) || (c >= '0' && c <= '9') || c == '-' || c == '.';
}

TOKEN Lexer::readTag() {
    if (atEnd || currentChar != '<') return tok_eof;
    advance();

    if (!atEnd && currentChar == '/') {
        advance();
        currentText.clear();
        while (!atEnd && currentChar != '>') {
            currentText += currentChar;
            advance();
        }
        advance(); 

        if (currentText == "System") return tok_system_end;
        if (currentText == "Block") return tok_block_end;
        if (currentText == "Line") return tok_line_end;
        if (currentText == "P") return tok_p_end;
        if (currentText == "Port") return tok_port_end;
        if (currentText == "Branch") return tok_branch_end;
        throw std::runtime_error("Unknown closing tag: " + currentText);
    }

    if (!atEnd && currentChar == '?') {
        advance();
        while (!atEnd) {
            if (currentChar == '?' && in.peek() == '>') {
                advance(); advance();
                break;
            }
            advance();
        }
        return getNextToken();
    }

    currentText.clear();
    while (!atEnd && currentChar != '>' && !std::isspace(static_cast<unsigned char>(currentChar))) {
        currentText += currentChar;
        advance();
    }

    TOKEN startTok;
    if (currentText == "System") startTok = tok_system_start;
    else if (currentText == "Block") startTok = tok_block_start;
    else if (currentText == "Line") startTok = tok_line_start;
    else if (currentText == "P") startTok = tok_p_start;
    else if (currentText == "Port") startTok = tok_port_start;
    else if (currentText == "Branch") startTok = tok_branch_start;
    else throw std::runtime_error("Unknown opening tag: " + currentText);

    readOpeningTag(currentText);

    return startTok;
}



TOKEN Lexer::readAttributeName() {
    currentText.clear();
    while (!atEnd && currentChar != '=' && !std::isspace(static_cast<unsigned char>(currentChar))) {
        currentText += currentChar;
        advance();
    }

    skipWhitespace();
    if (currentChar == '=') advance();
    skipWhitespace();

    if (currentText == "BlockType") return tok_attr_blocktype;
    if (currentText == "Name") return tok_attr_name;
    if (currentText == "SID") return tok_attr_sid;

    return tok_string;
}

void Lexer::readAttributeValue() {
    currentText.clear();
    if (currentChar != '"') {
        throw std::runtime_error("Expected '\"' for attribute value");
    }
    advance();

    while (!atEnd && currentChar != '"') {
        currentText += currentChar;
        advance();
    }
    if (!atEnd) advance();
}

void Lexer::readTextContent() {
    currentText.clear();
    while (!atEnd && currentChar != '<') {
        if (!std::isspace(static_cast<unsigned char>(currentChar))) {
            currentText += currentChar;
        }
        advance();
    }
    while (!currentText.empty() && std::isspace(static_cast<unsigned char>(currentText.back()))) {
        currentText.pop_back();
    }
}


TOKEN Lexer::getNextToken() {
    if (!pendingTokens.empty()) {
        TOKEN tok = pendingTokens.front();
        pendingTokens.pop();
        currentText = pendingTexts.front();
        pendingTexts.pop();
        return tok;
    }

    skipWhitespace();

    if (atEnd) return tok_eof;

    if (currentChar == '<') {
        return readTag();
    }

    if (currentChar != '<' && currentChar != '>') {
        readTextContent();
        std::string trimmed = currentText;
        trimmed.erase(trimmed.find_last_not_of(" \t\n\r")+1);
        trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
        if (trimmed.empty()) {
            return getNextToken();
        }
        return tok_text_content;
    }

    advance();
    return getNextToken();
}

void Lexer::readOpeningTag(const std::string& tagName) {
    skipWhitespace();

    while (!atEnd && currentChar != '>') {
        if (std::isspace(static_cast<unsigned char>(currentChar))) {
            skipWhitespace();
            continue;
        }

        std::string attrName;
        while (!atEnd && currentChar != '=' && !std::isspace(static_cast<unsigned char>(currentChar))) {
            attrName += currentChar;
            advance();
        }

        skipWhitespace();
        if (currentChar == '=') advance();
        skipWhitespace();

        if (currentChar != '"') {
            throw std::runtime_error("Expected '\"' in attribute value");
        }
        advance(); 

        std::string attrValue;
        while (!atEnd && currentChar != '"') {
            attrValue += currentChar;
            advance();
        }
        if (!atEnd) advance();

        TOKEN attrTok;
        if (attrName == "BlockType") attrTok = tok_attr_blocktype;
        else if (attrName == "Name") attrTok = tok_attr_name;
        else if (attrName == "SID") attrTok = tok_attr_sid;
        else attrTok = tok_string;

        pendingTokens.push(attrTok);
        pendingTexts.push(attrValue);

        skipWhitespace();
    }

    if (!atEnd && currentChar == '>') {
        advance();
    }
}
