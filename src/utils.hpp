#pragma once

#include <numbers>

namespace grid {

constexpr double EARTH_RADIUS = 6371000.0;

constexpr double DegToRad(double degree) {
    return degree * std::numbers::pi / 180.0;
}

constexpr double RadToDeg(double radians) {
    return radians * 180.0 / std::numbers::pi;
}

} // namespace grid
