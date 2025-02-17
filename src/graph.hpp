#pragma once

#include <vector> // For std::vector

namespace grid {

class Graph {
private:
    struct Edge {
        unsigned int To;
        double Weight;
    };

    std::vector<std::vector<Edge>> m_AdjList;

    struct DijkstraQueueElement {
        unsigned int Node;
        double Distance;

        bool operator<(const DijkstraQueueElement &other) const { return Distance > other.Distance; }
    };

    struct AStarQueueElement {
        unsigned int Node;
        double FScore, GScore;

        bool operator<(const AStarQueueElement &other) const { return FScore > other.FScore; }
    };

    std::vector<unsigned int> ReconstructPath(unsigned int start, unsigned int target, const std::vector<unsigned int> &predecessors) const;

public:
    explicit Graph(unsigned int nodeCount) : m_AdjList(nodeCount) {}

    void AddEdge(unsigned int node1, unsigned int node2, double weight);
    void ChangeEdgeWeight(unsigned int node1, unsigned int node2, double newWeight);

    std::vector<unsigned int> Dijkstra(unsigned int start, unsigned int target) const;

    template <typename THeuristic>
    std::vector<unsigned int> AStar(unsigned int start, unsigned int target, THeuristic heuristic) const;
};

} // namespace grid

#include "graph.tpp"
