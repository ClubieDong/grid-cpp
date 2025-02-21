#pragma once

#include "types.hpp"
#include <vector>

namespace grid {

class Constellation {
private:
    std::vector<Ecef> m_Satellites;

public:
    ConstellationConfig Config;

    explicit Constellation(const ConstellationConfig &config, double timeSec);

    std::vector<unsigned int> GetConnectedSatellites(Ecef position) const;

    const Ecef &GetSatelliteEcef(unsigned int idx) const;
    const Ecef &GetSatelliteEcef(unsigned int orbitIdx, unsigned int satIdxInOrbit) const;
    unsigned int GetSatelliteCount() const;
};

} // namespace grid
