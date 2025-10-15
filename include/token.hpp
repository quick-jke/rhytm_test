#ifndef INCLUDE_TOKEN_HPP
#define INCLUDE_TOKEN_HPP

#include <string>


enum TOKEN {
    tok_eof = -1,
    tok_system_start = -2,
    tok_system_end   = -3,
    tok_block_start  = -4,
    tok_block_end    = -5,
    tok_line_start   = -6,
    tok_line_end     = -7,
    tok_p_start      = -8,
    tok_p_end        = -9,
    tok_port_start   = -10,
    tok_port_end     = -11,
    tok_branch_start = -12,
    tok_branch_end   = -13,
    tok_attr_blocktype = -14,
    tok_attr_name      = -15,
    tok_attr_sid       = -16,
    tok_string         = -17,  
    tok_lbracket       = -18,  
    tok_rbracket       = -19,  
    tok_comma          = -20,  
    tok_semicolon      = -21,  
    tok_integer        = -22,
    tok_text_content   = -23  
};

inline const std::string tokenToString(TOKEN tok) {
    switch (tok) {
        case tok_eof: return "EOF";
        case tok_system_start: return "SYSTEM_START";
        case tok_system_end: return "SYSTEM_END";
        case tok_block_start: return "BLOCK_START";
        case tok_block_end: return "BLOCK_END";
        case tok_line_start: return "LINE_START";
        case tok_line_end: return "LINE_END";
        case tok_p_start: return "P_START";
        case tok_p_end: return "P_END";
        case tok_port_start: return "PORT_START";
        case tok_port_end: return "PORT_END";
        case tok_branch_start: return "BRANCH_START";
        case tok_branch_end: return "BRANCH_END";
        case tok_attr_blocktype: return "ATTR_BLOCKTYPE";
        case tok_attr_name: return "ATTR_NAME";
        case tok_attr_sid: return "ATTR_SID";
        case tok_string: return "STRING";
        case tok_lbracket: return "LBRACKET";
        case tok_rbracket: return "RBRACKET";
        case tok_comma: return "COMMA";
        case tok_semicolon: return "SEMICOLON";
        case tok_integer: return "INTEGER";
        case tok_text_content: return "TEXT_CONTENT";
        default: return "UNKNOWN";
    }
}


#endif 
