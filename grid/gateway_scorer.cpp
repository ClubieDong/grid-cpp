#include "gateway_scorer.hpp"
#include "sat_graph.hpp"
#include <cassert>
#include <limits>
#include <thread>
#include <future>

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
    auto diffRatio = (maxLength - minLength) / minLength;
    auto avgLatency = totalLength / LIGHT_SPEED / foundPathCount;
    return Config.Alpha * diffRatio + (1.0 - Config.Alpha) * avgLatency;
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
    // Concurrently calculate the score for each time slice
    auto numThreads = std::thread::hardware_concurrency();
    auto slicesPerThread = (Config.TimeSlices.size() + numThreads - 1) / numThreads;
    std::vector<std::future<double>> futures;
    for (unsigned int startIdx = 0; startIdx < Config.TimeSlices.size(); startIdx += slicesPerThread) {
        auto endIdx = std::min<unsigned int>(startIdx + slicesPerThread, Config.TimeSlices.size());
        futures.emplace_back(std::async(std::launch::async, [this, &gateways, startIdx, endIdx] {
            double partialResult = 0.0;
            for (unsigned int idx = startIdx; idx < endIdx; ++idx)
                partialResult += CalcScorePerTimeSlice(gateways, Config.TimeSlices[idx]);
            return partialResult;
        }));
    }
    // Reduce the results
    double result = 0.0;
    for (auto &future : futures)
        result += future.get();
    return result / Config.TimeSlices.size();
}

} // namespace grid
