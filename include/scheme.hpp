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

inline std::string blockTypeToString(BlockType bt) {
    switch (bt) {
        case Inport:    return "Inport";
        case Sum:       return "Sum";
        case Gain:      return "Gain";
        case UnitDelay: return "UnitDelay";
        case Outport:   return "Outport";
        default:        return "Unknown";
    }
}

struct P{
    std::string Name;
    std::variant<std::string, std::vector<int>> value;

    std::string to_string() const {
        std::ostringstream oss;
        oss << "P{Name=\"" << Name << "\", value=";
        if (std::holds_alternative<std::string>(value)) {
            oss << std::get<std::string>(value);
        } else {
            const auto& vec = std::get<std::vector<int>>(value);
            oss << "[";
            for (size_t i = 0; i < vec.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << vec[i];
            }
            oss << "]";
        }
        oss << "}";
        return oss.str();
    }
};

struct Port{
    std::vector<P> p;
    std::string to_string() const {
        std::ostringstream oss;
        oss << "Port{p=[";
        for (size_t i = 0; i < p.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << p[i].to_string();
        }
        oss << "]}";
        return oss.str();
    }
};

struct Branch{
    std::vector<P> p;

    std::string to_string() const {
        std::ostringstream oss;
        oss << "Branch{p=[";
        for (size_t i = 0; i < p.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << p[i].to_string();
        }
        oss << "]}";
        return oss.str();
    }
};

struct Block{
    BlockType blockType;
    std::string Name;
    int SID;
    std::vector<P> p;
    std::vector<Port> ports;

    std::string to_string() const {
        std::ostringstream oss;
        oss << "Block{blockType=" << blockTypeToString(blockType)
            << ", Name=\"" << Name << "\", SID=" << SID
            << ", p=[";
        for (size_t i = 0; i < p.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << p[i].to_string();
        }
        oss << "], ports=[";
        for (size_t i = 0; i < ports.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << ports[i].to_string();
        }
        oss << "]}";
        return oss.str();
    }
};

struct Line{
    std::vector<P> p;
    std::vector<Branch> branches;

    std::string to_string() const {
        std::ostringstream oss;
        oss << "Line{p=[";
        for (size_t i = 0; i < p.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << p[i].to_string();
        }
        oss << "], branches=[";
        for (size_t i = 0; i < branches.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << branches[i].to_string();
        }
        oss << "]}";
        return oss.str();
    }
};

struct System{
    std::vector<Line> lines;
    std::vector<Block> blocks;

    std::string to_string() const {
        std::ostringstream oss;
        oss << "System{blocks=[\n";
        for (size_t i = 0; i < blocks.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << "  " << blocks[i].to_string();
        }
        oss << "\n], lines=[\n";
        for (size_t i = 0; i < lines.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << "  " << lines[i].to_string();
        }
        oss << "\n]}";
        return oss.str();
    }
};



#endif
