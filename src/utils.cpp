#include "utils.hpp"
std::string removeSpaces(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                 [](unsigned char ch) { return !std::isspace(ch); });
    return result;
}
