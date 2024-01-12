#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "scene.hpp"
#include "geometries.hpp"
#include <pybind11/numpy.h>

namespace  py = pybind11;

using milliseconds = std::chrono::milliseconds;


class Renderer {
private:
    const std::string m_imgPath;
    uint32_t* m_imgData;
    float m_aspectRatio;
    std::size_t m_width;
    std::size_t m_height;

    Scene m_scene;
    Sphere m_sphere;

    //returns a color in RGBA
    uint32_t PerPixel(const std::size_t& x, const std::size_t& y, const Scene& scene, const Sphere& sphere);
public:
    Renderer(std::string imgPath, std::size_t width, std::size_t height);
    ~Renderer();

    py::array_t<uint32_t> Render();
    uint32_t ConvertFloatRGBAToARGB(const glm::vec4& color);
    void SaveImg();
};