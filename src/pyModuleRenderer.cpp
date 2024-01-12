#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "render.hpp"
#include <glm/glm.hpp>


namespace py = pybind11;


PYBIND11_MODULE(rendererModule, m) {
    m.doc() = "Python bindings for Renderer";

    py::class_<Renderer>(m, "Renderer")
        .def(py::init<std::string, std::size_t, std::size_t>())
        .def("Render", &Renderer::Render);

}
