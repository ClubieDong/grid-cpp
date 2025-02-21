#include "constellation.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <cassert>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

extern "C" {
#include "astro.h"
}

namespace grid {

Constellation::Constellation(const ConstellationConfig &config, double timeSec)
    : m_Satellites(config.NumOrbits * config.NumSatellitesPerOrbit), Config(config) {
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
    obj.es_n = Config.MeanMotion;
    obj.es_e = 0.001;
    obj.es_startok = obj.es_endok = obj.es_ap = obj.es_decay = obj.es_drag = 0.0f;
    obj.es_inc = Config.Inclination;
    obj.es_orbit = 0;
    for (unsigned int orbitIdx = 0; orbitIdx < Config.NumOrbits; ++orbitIdx)
        for (unsigned int satIdxInOrbit = 0; satIdxInOrbit < Config.NumSatellitesPerOrbit; ++satIdxInOrbit) {
            obj.es_M = satIdxInOrbit * 360.0 / Config.NumSatellitesPerOrbit;
            obj.es_raan = orbitIdx * 360.0 / Config.NumOrbits;
            obj_earthsat(&now, &obj);
            Lla lla = {
                .Longitude = RadToDeg(obj.s_sublng),
                .Latitude = RadToDeg(obj.s_sublat),
                .Altitude = obj.s_elev,
            };
            m_Satellites[orbitIdx * Config.NumSatellitesPerOrbit + satIdxInOrbit] = lla.ToEcef();
        }
}

std::vector<unsigned int> Constellation::GetConnectedSatellites(Ecef position) const {
    using Distance = std::pair<double, unsigned int>;
    std::priority_queue<Distance, std::vector<Distance>, std::greater<>> distances;
    for (unsigned int satIdx = 0; satIdx < m_Satellites.size(); ++satIdx) {
        auto distance = Ecef::Distance(position, m_Satellites[satIdx]);
        if (distance > Config.MaxConnectionDistance)
            continue;
        distances.emplace(distance, satIdx);
        if (distances.size() > Config.MaxConnectionCount)
            distances.pop();
    }
    std::vector<unsigned int> result;
    while (!distances.empty()) {
        result.push_back(distances.top().second);
        distances.pop();
    }
    return result;
}

const Ecef &Constellation::GetSatelliteEcef(unsigned int idx) const {
    assert(idx < GetSatelliteCount());
    return m_Satellites[idx];
}

const Ecef &Constellation::GetSatelliteEcef(unsigned int orbitIdx, unsigned int satIdxInOrbit) const {
    assert(orbitIdx < Config.NumOrbits);
    assert(satIdxInOrbit < Config.NumSatellitesPerOrbit);
    return GetSatelliteEcef(orbitIdx * Config.NumSatellitesPerOrbit + satIdxInOrbit);
}

unsigned int Constellation::GetSatelliteCount() const {
    return m_Satellites.size();
}

} // namespace grid
