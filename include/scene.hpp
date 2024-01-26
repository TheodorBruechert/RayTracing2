#pragma once

#include <glm/vec3.hpp>
#include "helperFunctions.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include <math.h>

class Scene {
private:
    std::unique_ptr<Camera> m_camera;
    glm::vec3 m_lightDirection;
public:
    Scene(const glm::vec3& lightDirection, Camera& camera) : m_lightDirection(normalize(lightDirection)), m_camera(new Camera(camera)) {}
    
    Ray* getRayField() const { return m_camera->getRayField(); }
    glm::vec3 getLightDirection() const { return m_lightDirection; }

    void moveCamera(const glm::vec3& direction) {m_camera->move(direction);}
};