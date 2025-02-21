#pragma once

#include "sat_graph.hpp"
#include "types.hpp"
#include <vector>

namespace grid {

class GatewayScorer {
public:
    GatewayScorerConfig Config;

    explicit GatewayScorer(const GatewayScorerConfig &config) : Config(config) {}

    double CalcScorePerTimeSlicePerCommPair(SatGraph &graph) const;
    double CalcScorePerTimeSlice(const std::vector<Lla> &gateways, double timeSec) const;
    double CalcScore(const std::vector<Lla> &gateways) const;
};

} // namespace grid
