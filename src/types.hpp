#pragma once

#include "utils.hpp"
#include <cmath>

namespace grid {

struct Ecef {
    double X, Y, Z;

    static double Distance(const Ecef &pos1, const Ecef &pos2) {
        return std::sqrt((pos1.X - pos2.X) * (pos1.X - pos2.X) + (pos1.Y - pos2.Y) * (pos1.Y - pos2.Y) + (pos1.Z - pos2.Z) * (pos1.Z - pos2.Z));
    }
};

struct Lla {
    double Longitude, Latitude, Altitude = 0.0;

    Ecef ToEcef() const {
        return Ecef{
            .X = (EARTH_RADIUS + Altitude) * std::cos(DegToRad(Latitude)) * std::cos(DegToRad(Longitude)),
            .Y = (EARTH_RADIUS + Altitude) * std::cos(DegToRad(Latitude)) * std::sin(DegToRad(Longitude)),
            .Z = (EARTH_RADIUS + Altitude) * std::sin(DegToRad(Latitude)),
        };
    }
};

struct ConstellationConfig {
    unsigned int NumOrbits;
    unsigned int NumSatellitesPerOrbit;
    double Inclination;
    double MeanMotion;
    unsigned int MaxConnectionCount;
    double MaxConnectionDistance;
};

} // namespace grid
