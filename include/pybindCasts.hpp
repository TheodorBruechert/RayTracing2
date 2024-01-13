#include <glm/vec3.hpp>
#include <pybind11/numpy.h>

namespace cast { 
glm::vec3 numpyArrayToVec3(py::array_t<float> input) {
    auto buffer = input.request();
    float* ptr = static_cast<float*>(buffer.ptr);

    if (buffer.size != 3) {
        throw std::runtime_error("NumPy array must have exactly 3 elements for conversion to glm::vec3.");
    }

    return glm::vec3(ptr[0], ptr[1], ptr[2]);
}   

}
