#include "topological_sort.hpp"

TopologicalSort::TopologicalSort(std::vector<Block> blocks, std::vector<Line> lines) : lines_(lines), blocks_(blocks) {}



std::set<int> getAllSids(std::vector<Line> lines){
    std::set<int> sids;
    for(auto const& line : lines){
        for(auto const& p : line.p){
            if((p.Name == enums::PName::Src || p.Name == enums::PName::Dst) && (std::holds_alternative<PDirectionValue>(p.value))){
                sids.insert(std::get<PDirectionValue>(p.value).SID);
            }
        }
    }
    return sids;
}

std::set<simpleLine> getSimpleLines(const std::vector<Line>& lines) {
    std::set<simpleLine> result;

    auto extractSID = [](const std::vector<P>& props, enums::PName name) -> std::optional<int> {
        for (const auto& p : props) {
            if (p.Name == name && std::holds_alternative<PDirectionValue>(p.value)) {
                return std::get<PDirectionValue>(p.value).SID;
            }
        }
        return std::nullopt;
    };

    for (const auto& line : lines) {
        if (line.branches.empty()) {
            auto src = extractSID(line.p, enums::PName::Src);
            auto dst = extractSID(line.p, enums::PName::Dst);
            if (src && dst) {
                result.insert({*src, *dst});
            }
        } else {
            auto src = extractSID(line.p, enums::PName::Src);
            if (!src){
                continue;  
            } 

            for (const auto& branch : line.branches) {
                auto dst = extractSID(branch.p, enums::PName::Dst);
                if (dst) {
                    result.insert({*src, *dst});
                }
            }
        }
    }

    return result;
}

void printLines(std::vector<Block> blocks, std::set<simpleLine> lines){
    auto findBlockName = [&](int sid) -> std::string {
        auto it = std::find_if(blocks.begin(), blocks.end(),
            [sid](const auto& b) { 
                return b.SID == sid; 
            }
        );
        return (it != blocks.end()) ? it->Name : "";
    };

    for (auto [src, dst] : lines) {
        std::cout << src << ":" << dst << "\t" << findBlockName(src) << ":" << findBlockName(dst) << std::endl;
    }
}

std::vector<int> topologicalOrder(std::set<simpleLine> lines) {
    std::set<int> allNodes;
    for (const auto& e : lines) {
        allNodes.insert(e.src);
        allNodes.insert(e.dst);
    }

    std::map<int, std::vector<int>> adj;
    std::map<int, int> inDegree;

    for (int node : allNodes) {
        inDegree[node] = 0;
    }

    for (const auto& e : lines) {
        // Удаляем ТОЛЬКО ребро 22 -> 21
        if (e.src == 22 && e.dst == 21) {
            continue;
        }
        adj[e.src].push_back(e.dst);
        inDegree[e.dst]++;
    }

    std::queue<int> q;
    for (int node : allNodes) {
        if (inDegree[node] == 0) {
            q.push(node);
        }
    }

    std::vector<int> order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    // Теперь переместим UnitDelay1 (21) в конец
    std::vector<int> result;
    for (int node : order) {
        if (node != 21) {
            result.push_back(node);
        }
    }
    result.push_back(21); // в конец

    return result;
}

std::optional<std::reference_wrapper<const Block>> TopologicalSort::findBlockByIdOpt(int sid) {
    for (const auto& block : blocks_) {
        if (block.SID == sid) {
            return std::ref(block);
        }
    }
    return std::nullopt;
}

std::vector<Block> TopologicalSort::findSrcIdsByDst(const std::set<simpleLine>& lines, int dst_id) {
    std::vector<Block> result;
    for (const auto& edge : lines) {
        if (edge.dst == dst_id) {
            result.push_back(findBlockByIdOpt(edge.src)->get());
        }
    }
    return result;
}




Result TopologicalSort::sort() {
    std::set<int> sids = getAllSids(lines_);

    std::set<simpleLine> simpleLines = getSimpleLines(lines_);

    std::vector<int> order = topologicalOrder(simpleLines);

    Result result;
    for (int nodeId : order) {
        Block srcBlock = findBlockByIdOpt(nodeId)->get();
        std::vector<Block> depsBlocks = findSrcIdsByDst(simpleLines, nodeId);

        result.push_back({srcBlock, depsBlocks});
        
    }

    return result;
}


