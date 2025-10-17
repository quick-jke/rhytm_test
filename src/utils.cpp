#include "utils.hpp"
std::string removeSpaces(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                 [](unsigned char ch) { return !std::isspace(ch); });
    return result;
}


enums::PName PNameEnumByString(std::string_view str) {
    if (str == "PortNumber") return enums::PortNumber;
    if (str == "SampleTime") return enums::SampleTime;
    if (str == "IconShape") return enums::IconShape;
    if (str == "Position")  return enums::Position;
    if (str == "Points")    return enums::Points;
    if (str == "Inputs")    return enums::Inputs;
    if (str == "Ports")     return enums::Ports;
    if (str == "Port")     return enums::Port;
    if (str == "Gain")     return enums::PGain;
    if (str == "Name")      return enums::Name;
    if (str == "Dst")       return enums::Dst;
    if (str == "Src")       return enums::Src;
    return enums::Invalid;
}

std::string PDirectionToString(enums::PDirection dir){
    switch (dir) {
        case enums::In: return "In";
        case enums::Out: return "Out";
        default: return "Unknown";
    }
}

std::string PNameToString(enums::PName name){
    switch (name) {
        case enums::PortNumber: return "PortNumber";
        case enums::SampleTime: return "SampleTime";
        case enums::IconShape: return "IconShape";
        case enums::Position: return "Position";
        case enums::Inputs: return "Inputs";
        case enums::Points: return "Points";
        case enums::Ports: return "Ports";
        case enums::Port: return "Port";
        case enums::PGain: return "PGain";
        case enums::Name: return "Name";
        case enums::Dst: return "Dst";
        case enums::Src: return "Src";
        default: return "Invalid";
    }
}


std::string tokenToString(TOKEN tok) {
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


bool isIntegerArray(std::string_view s) {
    if (s.size() < 2 || s.front() != '[' || s.back() != ']') return false;
    return true;
}

bool isDirectionFormat(std::string_view s) {
    static const std::regex dir_regex(R"(^\d+#(in|out):\d+$)", std::regex_constants::icase);
    return std::regex_match(s.begin(), s.end(), dir_regex);
}

std::optional<PDirectionValue> tryParseDirectionValue(std::string_view s) {
    if (!isDirectionFormat(s)) return std::nullopt;

    size_t hash = s.find('#');
    size_t colon = s.find(':', hash);

    std::string_view sid_str = s.substr(0, hash);
    std::string_view dir_str = s.substr(hash + 1, colon - hash - 1);
    std::string_view port_str = s.substr(colon + 1);

    int sid, port;
    auto [_, ec1] = std::from_chars(sid_str.data(), sid_str.data() + sid_str.size(), sid);
    auto [__, ec2] = std::from_chars(port_str.data(), port_str.data() + port_str.size(), port);
    if (ec1 != std::errc{} || ec2 != std::errc{}) return std::nullopt;

    enums::PDirection dir = (dir_str == "in" || dir_str == "IN") ? enums::In : enums::Out;

    return PDirectionValue{sid, dir, port};
}



std::string blockTypeToString(enums::BlockType bt){
    switch (bt) {
        case enums::Inport:    return "Inport";
        case enums::Sum:       return "Sum";
        case enums::Gain:      return "Gain";
        case enums::UnitDelay: return "UnitDelay";
        case enums::Outport:   return "Outport";
        default:        return "Unknown";
    }
}




std::optional<SignPair> tryParseSignPair(std::string_view s) {
    if (s.size() != 2) return std::nullopt;
    char c1 = s[0], c2 = s[1];
    if ((c1 != '+' && c1 != '-') || (c2 != '+' && c2 != '-'))
        return std::nullopt;
    return SignPair{c1, c2};
}
