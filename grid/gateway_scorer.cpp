#include "gateway_scorer.hpp"
#include "sat_graph.hpp"
#include <cassert>
#include <limits>

namespace grid {

double GatewayScorer::CalcScorePerTimeSlicePerCommPair(SatGraph &graph) const {
    unsigned int foundPathCount = 0;
    double minLength = std::numeric_limits<double>::max(), maxLength = 0.0, totalLength = 0.0;
    std::vector<std::tuple<unsigned int, unsigned int, double>> removedEdges; // (node1, node2, weight)
    while (foundPathCount < Config.MaxPathCount) {
        // Find a path
        auto [path, length] = graph.FindShortestPath();
        if (path.empty())
            break;
        ++foundPathCount;
        minLength = std::min(minLength, length);
        maxLength = std::max(maxLength, length);
        totalLength += length;
        // Remove a GSL
        double maxPathLengthAfterRemoval = -1.0, removedEdgeWeight;
        unsigned int edgeIdxToRemove;
        for (unsigned int idx = 1; idx < path.size(); ++idx) {
            if (graph.IsSatNode(path[idx - 1]) && graph.IsSatNode(path[idx]))
                continue;
            auto originalWeight = graph.GetEdgeWeight(path[idx - 1], path[idx]);
            graph.ChangeEdgeWeight(path[idx - 1], path[idx], std::numeric_limits<double>::max());
            auto [newPath, newLength] = graph.FindShortestPath();
            graph.ChangeEdgeWeight(path[idx - 1], path[idx], originalWeight);
            if (newLength > maxPathLengthAfterRemoval) {
                maxPathLengthAfterRemoval = newLength;
                removedEdgeWeight = originalWeight;
                edgeIdxToRemove = idx;
            }
        }
        assert(maxPathLengthAfterRemoval >= 0.0);
        graph.ChangeEdgeWeight(path[edgeIdxToRemove - 1], path[edgeIdxToRemove], std::numeric_limits<double>::max());
        removedEdges.emplace_back(path[edgeIdxToRemove - 1], path[edgeIdxToRemove], removedEdgeWeight);
    }
    for (const auto &[node1, node2, weight] : removedEdges)
        graph.ChangeEdgeWeight(node1, node2, weight);
    assert(foundPathCount > 0);
    auto diff = (maxLength - minLength) / minLength;
    auto avg = totalLength / foundPathCount;
    return (Config.Alpha * diff + (1.0 - Config.Alpha) * avg) / LIGHT_SPEED;
}

double GatewayScorer::CalcScorePerTimeSlice(const std::vector<Lla> &gateways, double timeSec) const {
    double result = 0.0;
    SatGraph graph(Config.CstConfig, timeSec, gateways);
    for (const auto &[src, dst] : Config.CommPairs) {
        graph.SetCommPair(src, dst);
        result += CalcScorePerTimeSlicePerCommPair(graph);
    }
    return result / Config.CommPairs.size();
}

double GatewayScorer::CalcScore(const std::vector<Lla> &gateways) const {
    double result = 0.0;
    for (auto timeSlice : Config.TimeSlices)
        result += CalcScorePerTimeSlice(gateways, timeSlice);
    return result / Config.TimeSlices.size();
}

} // namespace grid
