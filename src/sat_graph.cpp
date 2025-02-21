#include "sat_graph.hpp"
#include "types.hpp"

namespace grid {

void SatGraph::AddGroundNode(Ecef pos, unsigned int nodeIdx) {
    auto connectedSats = m_Constellation.GetConnectedSatellites(pos);
    for (auto connectedSatIdx : connectedSats) {
        auto satNodeIdx = GetSatNodeIdx(connectedSatIdx);
        AddEdge(satNodeIdx, nodeIdx, Ecef::Distance(m_Constellation.GetSatelliteEcef(connectedSatIdx), pos));
    }
}

SatGraph::SatGraph(const ConstellationConfig &config, double timeSec, const std::vector<Lla> &gateways)
    : Graph(config.NumOrbits * config.NumSatellitesPerOrbit + gateways.size() + 2), m_Constellation(config, timeSec) {
    // ISLs
    for (unsigned int orbitIdx = 0; orbitIdx < config.NumOrbits; ++orbitIdx)
        for (unsigned int satIdxInOrbit = 0; satIdxInOrbit < config.NumSatellitesPerOrbit; ++satIdxInOrbit) {
            // Calculate satellite index
            auto currSatNodeIdx = GetSatNodeIdx(orbitIdx, satIdxInOrbit);
            auto nextOrbitSatNodeIdx = GetSatNodeIdx((orbitIdx + 1) % config.NumOrbits, satIdxInOrbit);
            auto nextSatInOrbitNodeIdx = GetSatNodeIdx(orbitIdx, (satIdxInOrbit + 1) % config.NumSatellitesPerOrbit);
            // Get satellite positions
            auto currSatPos = m_Constellation.GetSatelliteEcef(orbitIdx, satIdxInOrbit);
            auto nextOrbitSatPos = m_Constellation.GetSatelliteEcef((orbitIdx + 1) % config.NumOrbits, satIdxInOrbit);
            auto nextSatInOrbitPos = m_Constellation.GetSatelliteEcef(orbitIdx, (satIdxInOrbit + 1) % config.NumSatellitesPerOrbit);
            // Create edges
            AddEdge(currSatNodeIdx, nextOrbitSatNodeIdx, Ecef::Distance(currSatPos, nextOrbitSatPos));
            AddEdge(currSatNodeIdx, nextSatInOrbitNodeIdx, Ecef::Distance(currSatPos, nextSatInOrbitPos));
        }
    // Gateways
    for (unsigned int gwIdx = 0; gwIdx < gateways.size(); ++gwIdx)
        AddGroundNode(gateways[gwIdx].ToEcef(), GetGatewayNodeIdx(gwIdx));
}

void SatGraph::SetCommPair(Lla src, Lla dst) {
    ClearEdgesOfNode(GetSrcNodeIdx());
    ClearEdgesOfNode(GetDstNodeIdx());
    AddGroundNode(src.ToEcef(), GetSrcNodeIdx());
    AddGroundNode(dst.ToEcef(), GetDstNodeIdx());
}

std::pair<std::vector<unsigned int>, double> SatGraph::FindShortestPath() const {
    return Dijkstra(GetSrcNodeIdx(), GetDstNodeIdx());
}

unsigned int SatGraph::GetSatNodeIdx(unsigned int satIdx) const {
    return satIdx;
}

unsigned int SatGraph::GetSatNodeIdx(unsigned int orbitIdx, unsigned int satIdxInOrbit) const {
    return orbitIdx * m_Constellation.GetSatelliteCount() + satIdxInOrbit;
}

unsigned int SatGraph::GetGatewayNodeIdx(unsigned int gatewayIdx) const {
    return m_Constellation.GetSatelliteCount() + gatewayIdx;
}

unsigned int SatGraph::GetSrcNodeIdx() const {
    return GetNodeCount() - 2;
}

unsigned int SatGraph::GetDstNodeIdx() const {
    return GetNodeCount() - 1;
}

bool SatGraph::IsGatewayNode(unsigned int nodeIdx) const {
    return m_Constellation.GetSatelliteCount() < nodeIdx && nodeIdx < GetNodeCount() - 2;
}

} // namespace grid
