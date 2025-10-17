#ifndef INCLUDE_UTILS_HPP
#define INCLUDE_UTILS_HPP
#include <algorithm>
#include <cctype>
#include <string>
#include <charconv>
#include <regex>
#include <optional>

#include "p_name.hpp"
#include "token.hpp"
#include "p_direction.hpp"
#include "scheme.hpp"
#include "p_direction_value.hpp"
#include "block_type.hpp"
#include "sign_pair.hpp"


std::string removeSpaces(const std::string& str);
enums::PName PNameEnumByString(std::string_view str);
std::string tokenToString(TOKEN tok);
std::string PDirectionToString(enums::PDirection dir);
std::string PNameToString(enums::PName name);
std::string blockTypeToString(enums::BlockType bt);
bool isIntegerArray(std::string_view s);
bool isDirectionFormat(std::string_view s);
std::optional<PDirectionValue> tryParseDirectionValue(std::string_view s);
std::optional<SignPair> tryParseSignPair(std::string_view s);

#endif
