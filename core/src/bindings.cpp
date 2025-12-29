//
// Created by Anastasiia Zabolotna on 15.11.2025.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Navigation.h"
#include "Vector.h"

namespace py = pybind11;

PYBIND11_MODULE(navigator_module, m) {
    m.doc() = "IMU Navigation Module";

    py::class_<Vector>(m, "Vector")
        .def(py::init<float, float, float>())
        .def_property_readonly("x", &Vector::getX)
        .def_property_readonly("y", &Vector::getY)
        .def_property_readonly("z", &Vector::getZ);

    py::class_<Navigation>(m, "Navigation")
        .def(py::init<int, float, float, float, float, float>(),
             py::arg("window_size")=25,
             py::arg("acc_thresh")=0.3f,

             py::arg("acc_p_noise")=0.1f,
             py::arg("acc_m_noise")=0.5f,
             py::arg("gyro_p_noise")=0.01f,
             py::arg("gyro_m_noise")=0.1f)
        .def("process_reading", &Navigation::process_reading)
        .def("get_trajectory", &Navigation::get_trajectory);
}