#ifndef INCLUDE_SCHEME_HPP
#define INCLUDE_SCHEME_HPP

#include <string>
#include <variant>
#include <vector>
#include <sstream>
#include "utils.hpp"

#include "block_type.hpp"
#include "p_direction.hpp"
#include "p_name.hpp"
#include "p_direction_value.hpp"
#include "sign_pair.hpp"




struct P{
    enums::PName Name;
    std::variant<PDirectionValue, SignPair, std::vector<int>, std::string> value;

    std::string to_string() const;
};

struct Port{
    std::vector<P> p;
    std::string to_string() const;
};

struct Branch{
    std::vector<P> p;

    std::string to_string() const;
};

struct Block{
    enums::BlockType blockType;
    std::string Name;
    int SID;
    std::vector<P> p;
    std::vector<Port> ports;

    std::string to_string() const;
};

struct Line{
    std::vector<P> p;
    std::vector<Branch> branches;

    std::string to_string() const;
};

struct System{
    std::vector<Line> lines;
    std::vector<Block> blocks;

    std::string to_string() const;

    std::string content();
};



#endif
