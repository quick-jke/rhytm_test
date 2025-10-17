// Parser.cpp
#include "parser.hpp"
#include <stdexcept>
#include <limits>

std::vector<int> parseIntegerArray(const std::string& input) {
    if (input.empty()) {
        throw std::runtime_error("Empty input");
    }

    std::string s = input;

    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        throw std::runtime_error("Only whitespace in input");
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    s = s.substr(start, end - start + 1);

    if (s.size() < 2 || s.front() != '[' || s.back() != ']') {
        throw std::runtime_error("Array must be enclosed in [ ]");
    }

    std::string inner = s.substr(1, s.size() - 2);
    inner.erase(inner.find_last_not_of(" \t\r\n") + 1);
    inner.erase(0, inner.find_first_not_of(" \t\r\n"));

    if (inner.empty()) {
        return {};
    }

    std::vector<int> result;
    size_t i = 0;

    while (i < inner.size()) {
        while (i < inner.size() && std::isspace(static_cast<unsigned char>(inner[i]))) {
            ++i;
        }
        if (i >= inner.size()) break;

        bool negative = false;
        if (inner[i] == '-') {
            negative = true;
            ++i;
        } else if (inner[i] == '+') {
            ++i;
        }

        long long num = 0; 
        size_t digits = 0;
        while (i < inner.size() && std::isdigit(static_cast<unsigned char>(inner[i]))) {
            num = num * 10 + (inner[i] - '0');
            ++digits;
            ++i;
        }

        if (digits == 0) {
            throw std::runtime_error("Expected integer, got non-digit at position " + std::to_string(i));
        }

        if (negative) num = -num;

        if (num > static_cast<long long>(std::numeric_limits<int>::max()) || num < static_cast<long long>(std::numeric_limits<int>::min())) {
            throw std::runtime_error("Integer out of range");
        }

        result.push_back(static_cast<int>(num));

        while (i < inner.size() && std::isspace(static_cast<unsigned char>(inner[i]))) {
            ++i;
        }

        if (i < inner.size()) {
            if (inner[i] == ',') {
                ++i;
            } else {
                throw std::runtime_error("Expected ',' or end of array, got '" + std::string(1, inner[i]) + "'");
            }
        }
    }

    return result;
}

Parser::Parser(Lexer& l) : lexer(l){
    advance();
}


void Parser::advance() {
    currentToken = lexer.getNextToken();
    currentText = lexer.getText();
}

void Parser::expect(TOKEN expected) {
    if (currentToken != expected) {
        throw std::runtime_error(
            "Expected " + std::string(tokenToString(expected)) +
            ", got " + std::string(tokenToString(currentToken))
        );
    }
    advance();
}

P Parser::parseP() {
    advance();

    P p;
    while (true) {
        if (currentToken == tok_attr_name) {
            p.Name = PNameEnumByString(currentText);
            advance();
        } else if (currentToken == tok_text_content) {
            std::string content = currentText;

            if (!content.empty() && content.front() == '[' && content.back() == ']') {
                try {
                    p.value = parseIntegerArray(content); 
                } catch (...) {
                    p.value = content; 
                }
            }
            else if (isDirectionFormat(content)) {
                if (auto dirVal = tryParseDirectionValue(content)) {
                    p.value = *dirVal;
                } else {
                    p.value = content; 
                }
            }
            else if (auto signPair = tryParseSignPair(content)) {
                p.value = *signPair;
            }
            else {
                p.value = content;
            }

            advance();
        } else if (currentToken == tok_p_end) {
            advance();
            break;
        } else {
            advance(); 
        }
    }

    return p;
}

Port Parser::parsePort() {
    Port port;

    advance(); 

    while (currentToken == tok_p_start) {
        port.p.push_back(parseP());
    }

    expect(tok_port_end); 
    return port;
}

Branch Parser::parseBranch() {
    advance(); 

    Branch branch;
    while (currentToken != tok_branch_end && currentToken != tok_eof) {
        if (currentToken == tok_p_start) {
            branch.p.push_back(parseP());
        } else {
            advance();
        }
    }

    if (currentToken == tok_branch_end) {
        advance();
    } else {
        throw std::runtime_error("Expected BRANCH_END in Branch");
    }

    return branch;
}

Block Parser::parseBlock() {
    Block block;

    while (true) {
        if (currentToken == tok_attr_blocktype) {
            if (currentText == "Inport"){
                block.blockType = enums::Inport;
            } 
            else if (currentText == "Sum") {
                block.blockType = enums::Sum;
            }
            else if (currentText == "Gain") {
                block.blockType = enums::Gain;
            }
            else if (currentText == "UnitDelay") {
                block.blockType = enums::UnitDelay;
            }
            else if (currentText == "Outport") {
                block.blockType = enums::Outport;
            }
            else {
                throw std::runtime_error("Unknown BlockType: " + currentText);
            }
            advance();
        } else if (currentToken == tok_attr_name) {
            block.Name = currentText;
            advance();
        } else if (currentToken == tok_attr_sid) {
            try {
                block.SID = std::stoi(currentText);
            } catch (...) {
                throw std::runtime_error("Invalid SID: " + currentText);
            }
            advance();
        } else if (currentToken == tok_p_start) {
            block.p.push_back(parseP());
        } else if (currentToken == tok_port_start) {
            block.ports.push_back(parsePort());
        } else if (currentToken == tok_block_end) {
            advance();
            break;
        } else {
            advance();
        }
    }
    return block;
}

Line Parser::parseLine() {
    Line line;
    while (true) {
        if (currentToken == tok_p_start) {
            line.p.push_back(parseP());
        } else if (currentToken == tok_branch_start) {
            line.branches.push_back(parseBranch());
        } else if (currentToken == tok_line_end) {
            advance();
            break;
        } else {
            advance();
        }
    }
    return line;
}

System Parser::parseSystem() {
    System system;

    while (true) {
        if (currentToken == tok_block_start) {
            system.blocks.push_back(parseBlock());
        } else if (currentToken == tok_line_start) {
            system.lines.push_back(parseLine());
        } else if (currentToken == tok_system_end) {
            advance();
            break;
        } else if (currentToken == tok_eof) {
            break;
        } else {
            advance();
        }
    }

    return system;
}
