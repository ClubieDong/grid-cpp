#include "graph.hpp"
#include <algorithm> // For std::reverse
#include <limits>    // For std::numeric_limits
#include <queue>     // For std::priority_queue
#include <vector>    // For std::vector

namespace grid {

std::vector<unsigned int> Graph::ReconstructPath(unsigned int start, unsigned int target, const std::vector<unsigned int> &predecessors) const {
    if (predecessors[target] == -1u && start != target)
        return {};
    std::vector<unsigned int> path;
    unsigned int current = target;
    while (current != start) {
        path.push_back(current);
        current = predecessors[current];
        if (current == -1u)
            return {};
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

void Graph::AddEdge(unsigned int node1, unsigned int node2, double weight) {
    m_AdjList[node1].emplace_back(node2, weight);
    m_AdjList[node2].emplace_back(node1, weight);
}

void Graph::ChangeEdgeWeight(unsigned int node1, unsigned int node2, double newWeight) {
    for (Edge &edge : m_AdjList[node1])
        if (edge.To == node2) {
            edge.Weight = newWeight;
            break;
        }
    for (Edge &edge : m_AdjList[node2])
        if (edge.To == node1) {
            edge.Weight = newWeight;
            break;
        }
}

std::vector<unsigned int> Graph::Dijkstra(unsigned int start, unsigned int target) const {
    std::vector<double> distances(m_AdjList.size(), std::numeric_limits<double>::max());
    std::vector<unsigned int> predecessors(m_AdjList.size(), -1u);
    std::priority_queue<DijkstraQueueElement> pq;

    distances[start] = 0;
    pq.emplace(start, 0);
    while (!pq.empty()) {
        auto [currNode, currDist] = pq.top();
        pq.pop();
        if (currNode == target)
            break;
        if (currDist > distances[currNode])
            continue;
        for (const Edge &edge : m_AdjList[currNode]) {
            double newDist = currDist + edge.Weight;
            if (newDist < distances[edge.To]) {
                distances[edge.To] = newDist;
                predecessors[edge.To] = currNode;
                pq.emplace(edge.To, newDist);
            }
        }
    }
    return ReconstructPath(start, target, predecessors);
}

} // namespace grid
