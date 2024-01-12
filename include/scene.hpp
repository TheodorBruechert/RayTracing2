#pragma once

#include <glm/vec3.hpp>

class Scene {
private:
    glm::vec3 m_camera;
    glm::vec3 m_lightSource;
public:
    glm::vec3 getCamera() const { return m_camera; }
    glm::vec3 getLightSource() const { return m_lightSource; }
    Scene(const glm::vec3 camera, const glm::vec3 lightSource) : m_camera(camera), m_lightSource(lightSource) {}
};