#ifndef INCLUDE_SCHEME_HPP
#define INCLUDE_SCHEME_HPP
#include <string>
#include <variant>
#include <vector>
enum BlockType{
    Inport,
    Sum,
    Gain,
    UnitDelay,
    Outport
};
struct P{
    std::string Name;
    std::variant<std::string, std::vector<int>> value;
};

struct Port{
    std::vector<P> p;
};

struct Branch{
    std::vector<P> p;
};

struct Block{
    BlockType blockType;
    std::string Name;
    int SID;
    std::vector<P> p;
    std::vector<Port> ports;
};

struct Line{
    std::vector<P> p;
    std::vector<Branch> branches;
};

struct System{
    std::vector<Line> lines;
    std::vector<Block> blocks;
};



#endif
