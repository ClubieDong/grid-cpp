#pragma once

#include "constellation.hpp"
#include "graph.hpp"
#include "types.hpp"
#include <vector>

namespace grid {

class SatGraph : public Graph {
private:
    Constellation m_Constellation;

    void AddGroundNode(Ecef pos, unsigned int nodeIdx);

public:
    explicit SatGraph(const ConstellationConfig &config, double timeSec, const std::vector<Lla> &gateways);

    void SetCommPair(Lla src, Lla dst);
    std::pair<std::vector<unsigned int>, double> FindShortestPath() const;

    unsigned int GetSatNodeIdx(unsigned int satIdx) const;
    unsigned int GetSatNodeIdx(unsigned int orbitIdx, unsigned int satIdxInOrbit) const;
    unsigned int GetGatewayNodeIdx(unsigned int gatewayIdx) const;
    unsigned int GetSrcNodeIdx() const;
    unsigned int GetDstNodeIdx() const;
    bool IsSatNode(unsigned int nodeIdx) const;
};

} // namespace grid
