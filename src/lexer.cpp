#include "lexer.hpp"


namespace {
    std::string trim(const std::string& s) {
        if (s.empty()) return s;
        const auto first = s.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return {};
        const auto last = s.find_last_not_of(" \t\n\r");
        return s.substr(first, (last - first + 1));
    }
}

Lexer::Lexer(std::istream& input) : in(input) {
    if (!in.good()) {
        throw std::runtime_error("Invalid input stream");
    }
    advance();
}

void Lexer::advance() {
    if (!in) {
        atEnd = true;
        currentChar = 0;
        return;
    }

    in.get(currentChar);
    if (in.eof()) {
        atEnd = true;
        currentChar = 0;
    } else if (in.fail()) {
        throw std::runtime_error("Stream read error");
    } else {
        atEnd = false;
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
    if (atEnd || currentChar != '<') {
        return tok_eof;
    }
    advance();

    if (!atEnd && currentChar == '/') {
        advance();
        currentText.clear();
        while (!atEnd && currentChar != '>') {
            currentText += currentChar;
            advance();
        }
        if (atEnd) {
            throw std::runtime_error("Unclosed closing tag");
        }
        advance(); 

        const std::string& tag = currentText;
        if (tag == "System") return tok_system_end;
        if (tag == "Block") return tok_block_end;
        if (tag == "Line") return tok_line_end;
        if (tag == "P") return tok_p_end;
        if (tag == "Port") return tok_port_end;
        if (tag == "Branch") return tok_branch_end;
        throw std::runtime_error("Unknown closing tag: " + tag);
    }

    if (!atEnd && currentChar == '?') {
        advance();
        while (!atEnd) {
            if (currentChar == '?' && in.peek() == '>') {
                advance();
                advance(); 
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

    const std::string tagName = currentText;
    TOKEN startTok;
    if (tagName == "System") startTok = tok_system_start;
    else if (tagName == "Block") startTok = tok_block_start;
    else if (tagName == "Line") startTok = tok_line_start;
    else if (tagName == "P") startTok = tok_p_start;
    else if (tagName == "Port") startTok = tok_port_start;
    else if (tagName == "Branch") startTok = tok_branch_start;
    else throw std::runtime_error("Unknown opening tag: " + tagName);

    readOpeningTag(tagName);
    return startTok;
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
        if (currentChar != '=') {
            throw std::runtime_error("Expected '=' after attribute name in tag '" + tagName + "'");
        }
        advance(); 
        skipWhitespace();

        if (currentChar != '"') {
            throw std::runtime_error("Expected '\"' for attribute value in tag '" + tagName + "'");
        }
        advance(); 

        std::string attrValue;
        while (!atEnd && currentChar != '"') {
            attrValue += currentChar;
            advance();
        }
        if (atEnd) {
            throw std::runtime_error("Unterminated attribute value in tag '" + tagName + "'");
        }
        advance(); 

        TOKEN attrTok;
        if (attrName == "BlockType") attrTok = tok_attr_blocktype;
        else if (attrName == "Name") attrTok = tok_attr_name;
        else if (attrName == "SID") attrTok = tok_attr_sid;
        else attrTok = tok_string;

        pendingTokens.push(attrTok);
        pendingTexts.push(attrValue);

        skipWhitespace();
    }

    if (atEnd) {
        throw std::runtime_error("Unclosed opening tag: <" + tagName);
    }
    advance();
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

    if (atEnd) {
        return tok_eof;
    }

    if (currentChar == '<') {
        return readTag();
    }

    readTextContent();
    if (!currentText.empty()) {
        return tok_text_content;
    }

    return getNextToken();
}

void Lexer::readTextContent() {
    currentText.clear();
    while (!atEnd && currentChar != '<') {
        currentText += currentChar;
        advance();
    }

    currentText = trim(currentText);
}
