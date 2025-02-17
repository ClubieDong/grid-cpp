#pragma once

#include "types.hpp" // grid::Ecef, grid::Lla
#include <vector>    // For std::vector

namespace grid {

class Constellation {
public:
    unsigned int NumOrbits;
    unsigned int NumSatellitesPerOrbit;
    double Inclination;
    double MeanMotion;
    std::vector<Ecef> SatellitesEcef;
    std::vector<Lla> SatellitesLla;

    explicit Constellation(unsigned int numOrbits, unsigned int numSatellitesPerOrbit, double inclination, double meanMotion, double timeSec);

    std::vector<unsigned int> GetConnectedSatellites(Ecef position, unsigned int maxConnectionCount, double maxConnectionDistance) const;
};

} // namespace grid
