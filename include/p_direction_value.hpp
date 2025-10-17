#ifndef INCLUDE_P_DIRECTION_VALUE_HPP
#define INCLUDE_P_DIRECTION_VALUE_HPP
#include "p_direction.hpp"
#include <string>
struct PDirectionValue{
    int SID;
    enums::PDirection dir;
    int port;

    std::string to_string() const;
};
#endif
