#pragma once

#include <glm/vec3.hpp>
#include "helperFunctions.hpp"

class Scene {
private:
    glm::vec3 m_camera;
    glm::vec3 m_lightDirection;
public:
    Scene(const glm::vec3& camera, const glm::vec3& lightDirection) : m_camera(camera), m_lightDirection(normalize(lightDirection)) {}
    
    glm::vec3 getCamera() const { return m_camera; }
    glm::vec3 getLightDirection() const { return m_lightDirection; }

    void moveCamera(const glm::vec3& direction) {m_camera += direction;}
};