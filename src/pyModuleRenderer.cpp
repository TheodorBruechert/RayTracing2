#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "render.hpp"
#include <glm/glm.hpp>


namespace py = pybind11;


PYBIND11_MODULE(rendererModule, m) {
    m.doc() = "Python bindings for Renderer";

    py::class_<Renderer>(m, "Renderer")
        .def(py::init< std::size_t, std::size_t>())
        .def("render", &Renderer::Render)
        .def("moveCamera", &Renderer::py_moveCamera);

}
