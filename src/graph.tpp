#pragma once

#include "graph.hpp"
#include <limits>
#include <queue>
#include <vector>

namespace grid {

template <typename THeuristic>
std::pair<std::vector<unsigned int>, double> Graph::AStar(unsigned int start, unsigned int target, THeuristic heuristic) const {
    std::vector<double> gScores(m_AdjList.size(), std::numeric_limits<double>::max());
    std::vector<unsigned int> predecessors(m_AdjList.size(), -1u);
    std::priority_queue<AStarQueueElement> pq;

    gScores[start] = 0;
    pq.emplace(start, heuristic(start, target), 0);
    while (!pq.empty()) {
        auto [currNode, fScore, gScore] = pq.top();
        pq.pop();
        if (currNode == target)
            break;
        if (gScore > gScores[currNode])
            continue;
        for (const Edge &edge : m_AdjList[currNode]) {
            double tentativeG = gScores[currNode] + edge.Weight;
            if (tentativeG < gScores[edge.To]) {
                gScores[edge.To] = tentativeG;
                predecessors[edge.To] = currNode;
                pq.emplace(edge.To, tentativeG + heuristic(edge.To, target), tentativeG);
            }
        }
    }
    return {ReconstructPath(start, target, predecessors), gScores[target]};
}

} // namespace grid
