#include "gateway_scorer.hpp"
#include "types.hpp"
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(grid, m) {
    py::class_<grid::Ecef>(m, "Ecef")
        .def(py::init<double, double, double>(), py::arg("x"), py::arg("y"), py::arg("z"))
        .def_readwrite("x", &grid::Ecef::X)
        .def_readwrite("y", &grid::Ecef::Y)
        .def_readwrite("z", &grid::Ecef::Z)
        .def("distance", &grid::Ecef::Distance);

    py::class_<grid::Lla>(m, "Lla")
        .def(py::init<double, double, double>(), py::arg("longitude"), py::arg("latitude"), py::arg("altitude") = 0.0)
        .def_readwrite("longitude", &grid::Lla::Longitude)
        .def_readwrite("latitude", &grid::Lla::Latitude)
        .def_readwrite("altitude", &grid::Lla::Altitude)
        .def("to_ecef", &grid::Lla::ToEcef);

    py::class_<grid::ConstellationConfig>(m, "ConstellationConfig")
        .def(py::init<unsigned int, unsigned int, double, double, unsigned int, double>(), py::arg("num_orbits"), py::arg("num_satellites_per_orbit"),
             py::arg("inclination"), py::arg("mean_motion"), py::arg("max_connection_count"), py::arg("max_connection_distance"))
        .def_readwrite("num_orbits", &grid::ConstellationConfig::NumOrbits)
        .def_readwrite("num_satellites_per_orbit", &grid::ConstellationConfig::NumSatellitesPerOrbit)
        .def_readwrite("inclination", &grid::ConstellationConfig::Inclination)
        .def_readwrite("mean_motion", &grid::ConstellationConfig::MeanMotion)
        .def_readwrite("max_connection_count", &grid::ConstellationConfig::MaxConnectionCount)
        .def_readwrite("max_connection_distance", &grid::ConstellationConfig::MaxConnectionDistance);

    py::class_<grid::GatewayScorerConfig>(m, "GatewayScorerConfig")
        .def(py::init<grid::ConstellationConfig, std::vector<double>, std::vector<std::pair<grid::Lla, grid::Lla>>, unsigned int, double>(),
             py::arg("cst_config"), py::arg("time_slices"), py::arg("comm_pairs"), py::arg("max_path_count"), py::arg("alpha"))
        .def_readwrite("cst_config", &grid::GatewayScorerConfig::CstConfig)
        .def_readwrite("time_slices", &grid::GatewayScorerConfig::TimeSlices)
        .def_readwrite("comm_pairs", &grid::GatewayScorerConfig::CommPairs)
        .def_readwrite("max_path_count", &grid::GatewayScorerConfig::MaxPathCount)
        .def_readwrite("alpha", &grid::GatewayScorerConfig::Alpha);

    py::class_<grid::GatewayScorer>(m, "GatewayScorer")
        .def(py::init<grid::GatewayScorerConfig>(), py::arg("config"))
        .def("calc_score", &grid::GatewayScorer::CalcScore);
}
