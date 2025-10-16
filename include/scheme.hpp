#ifndef INCLUDE_SCHEME_HPP
#define INCLUDE_SCHEME_HPP
#include <string>
#include <variant>
#include <vector>
#include <sstream>
#include "utils.hpp"
enum BlockType{
    Inport,
    Sum,
    Gain,
    UnitDelay,
    Outport
};

inline std::string blockTypeToString(BlockType bt);

struct P{
    std::string Name;
    std::variant<std::string, std::vector<int>> value;

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
    BlockType blockType;
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
