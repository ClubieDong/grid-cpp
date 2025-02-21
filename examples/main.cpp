#include "gateway_scorer.hpp"
#include <iostream>

int main() {
    grid::GatewayScorer scorer(grid::GatewayScorerConfig{
        .CstConfig = grid::ConstellationConfig{
            .NumOrbits = 72,
            .NumSatellitesPerOrbit = 22,
            .Inclination = 53.0,
            .MeanMotion = 15.09,
            .MaxConnectionCount = 4,
            .MaxConnectionDistance = 1123000.0,
        },
        .TimeSlices = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0},
        .CommPairs = {
            {grid::Lla{.Longitude = 0.0, .Latitude = 0.0}, grid::Lla{.Longitude = 0.0, .Latitude = 50.0}},
            {grid::Lla{.Longitude = 0.0, .Latitude = 0.0}, grid::Lla{.Longitude = 50.0, .Latitude = 0.0}},
        },
        .MaxPathCount = 5,
        .Alpha = 0.5,
    });

    auto score = scorer.CalcScore({
        grid::Lla{.Longitude = 0.0, .Latitude = 0.0},
        grid::Lla{.Longitude = 0.0, .Latitude = 50.0},
        grid::Lla{.Longitude = 50.0, .Latitude = 0.0},
    });

    std::cout << score << '\n';

    return 0;
}
