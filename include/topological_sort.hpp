#ifndef INCLUDE_TOPOLOGICAL_SORT_HPP
#define INCLUDE_TOPOLOGICAL_SORT_HPP

#include "scheme.hpp"
#include <vector>
#include <set>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>

using Result = std::vector<std::pair<Block, std::vector<Block>>>;

struct simpleLine{
    int src;
    int dst;

    bool operator<(const simpleLine& other) const {
        if (src != other.src) {
            return src < other.src;
        }
        return dst < other.dst;
    }
};


class TopologicalSort {
public:
    TopologicalSort(std::vector<Block> blocks, std::vector<Line> lines);

    Result sort();


private:

    std::vector<Block> blocks_;
    std::vector<Line> lines_;

    std::optional<std::reference_wrapper<const Block>> findBlockByIdOpt(int sid);
    std::vector<Block> findSrcIdsByDst(const std::set<simpleLine>& lines, int dst_id);
    std::vector<int> topologicalOrder(std::set<simpleLine> lines);
};

#endif 
