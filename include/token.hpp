#ifndef INCLUDE_TOKEN_HPP
#define INCLUDE_TOKEN_HPP

#include <string>


enum TOKEN {
    tok_eof             = -1,
    tok_system_start    = -2,
    tok_system_end      = -3,
    tok_block_start     = -4,
    tok_block_end       = -5,
    tok_line_start      = -6,
    tok_line_end        = -7,
    tok_p_start         = -8,
    tok_p_end           = -9,
    tok_port_start      = -10,
    tok_port_end        = -11,
    tok_branch_start    = -12,
    tok_branch_end      = -13,
    tok_attr_blocktype  = -14,
    tok_attr_name       = -15,
    tok_attr_sid        = -16,
    tok_string          = -17,  
    tok_lbracket        = -18,  
    tok_rbracket        = -19,  
    tok_comma           = -20,  
    tok_semicolon       = -21,  
    tok_integer         = -22,
    tok_text_content    = -23  
};




#endif 
