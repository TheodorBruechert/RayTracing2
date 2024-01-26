#include "render.hpp"
#include "helperFunctions.hpp"
#include "geometries.hpp"
#include "pybindCasts.hpp"
#include "camera.hpp"
#include <cstdint>
#include <png.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>

Renderer::Renderer( std::size_t width, std::size_t height)
    : m_imgData(new uint32_t[width*height]), m_aspectRatio(width / static_cast<float>(height)), m_width(width), m_height(height), m_sphere(1.5) {
        Camera camera(
            M_PI_4, //field of view 45°
            m_width,
            m_height,
            glm::vec3(0.0, 0.0, -8.0) //position
        );
        m_scene = std::unique_ptr<Scene>( new Scene(
            glm::vec3(0, 0, 1), //lightdirection
            camera
        ));
    }

Renderer::~Renderer() {
    delete[] m_imgData;
}

py::array_t<uint32_t> Renderer::Render() {

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            m_imgData[y*m_width + x] = PerPixel(x, y);
        }
    }
    //check elapsed Time
    
    return py::array_t<uint32_t>(m_height*m_width, m_imgData);
}

uint32_t Renderer::PerPixel(const std::size_t& x, const std::size_t& y) {
//default color:
   

    glm::vec4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);
    Ray ray = m_scene->getRayField()[x + y*m_width];
    //t0 < t1 therefore, the hit point of the ray is at cameraPosition + t0 * ray

    
    float t0, t1;
    if (m_sphere.CheckCollision(ray.origin, ray.direction, t0, t1)) {
        pixelColor = { 1.0f, 0.0, 0.0f, 1.0f };
        glm::vec3 visualPointOnSphere = ray.origin + t0 * ray.direction;

        glm::vec3 normal = visualPointOnSphere - m_sphere.GetCenter();
        glm::vec3 lightDirection = m_scene->getLightDirection();
        normalize(normal);
        float lightFactor = clip(glm::dot(-lightDirection, normal), 0.0f);
        for(int i = 0; i < 3; i++) {
            pixelColor[i] *= lightFactor;
        }
        pixelColor = glm::clamp(pixelColor, glm::vec4{0.0f}, glm::vec4{1.0f});
       
    }

    return ConvertFloatRGBAToARGB(pixelColor);
}


//should be clamped color between 0 and 1
uint32_t Renderer::ConvertFloatRGBAToARGB(const glm::vec4& color) {
    return  (((uint8_t)(color.a*255.0f) << 24) | ((uint8_t)(color.r*255.0f) << 16) | ((uint8_t)(color.g*255.0f) << 8) | ((uint8_t)(color.b*255.0f)));
}

void Renderer::py_moveCamera(const py::array_t<float>& direction) {
    moveCamera(cast::numpyArrayToVec3(direction));
}
void Renderer::moveCamera(const glm::vec3& direction){
    m_scene->moveCamera(direction);
}
