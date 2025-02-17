#include "constellation.hpp"
#include "types.hpp"  // For grid::Ecef
#include "utils.hpp"  // For grid::RadToDeg
#include <functional> // For std::greater
#include <queue>      // For std::priority_queue
#include <utility>    // For std::pair
#include <vector>     // For std::vector

extern "C" {
#include "astro.h"
}

namespace grid {

Constellation::Constellation(unsigned int numOrbits, unsigned int numSatellitesPerOrbit, double inclination, double meanMotion, double timeSec)
    : NumOrbits(numOrbits), NumSatellitesPerOrbit(numSatellitesPerOrbit), Inclination(inclination), MeanMotion(meanMotion),
      SatellitesEcef(NumOrbits * NumSatellitesPerOrbit), SatellitesLla(NumOrbits * NumSatellitesPerOrbit) {
    // Calculate satellite positions
    Now now;
    now.n_mjd = J2000 + timeSec / 86400.0;
    now.n_lat = now.n_lng = now.n_tz = now.n_elev = now.n_dip = now.n_pressure = 0.0;
    now.n_temp = 15.0;
    now.n_epoch = J2000;
    Obj obj;
    obj.o_name[0] = '\0';
    obj.o_type = EARTHSAT;
    obj.o_flags = FUSER0;
    obj.es_epoch = J2000;
    obj.es_n = MeanMotion;
    obj.es_e = 0.001;
    obj.es_startok = obj.es_endok = obj.es_ap = obj.es_decay = obj.es_drag = 0.0f;
    obj.es_inc = Inclination;
    obj.es_orbit = 0;
    for (unsigned int orbitIdx = 0; orbitIdx < NumOrbits; ++orbitIdx)
        for (unsigned int satIdxInOrbit = 0; satIdxInOrbit < NumSatellitesPerOrbit; ++satIdxInOrbit) {
            obj.es_M = satIdxInOrbit * 360.0 / NumSatellitesPerOrbit;
            obj.es_raan = orbitIdx * 360.0 / NumOrbits;
            obj_earthsat(&now, &obj);
            auto satIdx = orbitIdx * NumSatellitesPerOrbit + satIdxInOrbit;
            SatellitesLla[satIdx] = {
                .Longitude = RadToDeg(obj.s_sublng),
                .Latitude = RadToDeg(obj.s_sublat),
                .Altitude = obj.s_elev,
            };
            SatellitesEcef[satIdx] = SatellitesLla[satIdx].ToEcef();
        }
}

std::vector<unsigned int> Constellation::GetConnectedSatellites(Ecef position, unsigned int maxConnectionCount, double maxConnectionDistance) const {
    using Distance = std::pair<double, unsigned int>;
    std::priority_queue<Distance, std::vector<Distance>, std::greater<>> distances;
    for (unsigned int satIdx = 0; satIdx < SatellitesEcef.size(); ++satIdx) {
        auto distance = Ecef::Distance(position, SatellitesEcef[satIdx]);
        if (distance > maxConnectionDistance)
            continue;
        distances.emplace(distance, satIdx);
        if (distances.size() > maxConnectionCount)
            distances.pop();
    }
    std::vector<unsigned int> result;
    while (!distances.empty()) {
        result.push_back(distances.top().second);
        distances.pop();
    }
    return result;
}

} // namespace grid
