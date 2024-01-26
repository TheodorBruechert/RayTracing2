#include "camera.hpp"
#include <iostream>
Camera::Camera(const float& fovH, const int& width, const int& height, const glm::vec3& position, const glm::vec3& forward) :
    m_fovH(fovH),
    m_rayDistance(1.0/(glm::tan(m_fovH/2.0))),
    m_rayFieldHeight(),
    m_width(width), m_height(height),
    m_position(position),
    m_forward(glm::normalize(forward)) {
        recalcRayField();
    }


void Camera::recalcRayField() {
    delete[] m_rayField; //delete old field
    m_rayField = new Ray[m_width*m_height];

    for(int y = 0; y < m_height; y++) {
        for( int x = 0; x < m_width; x++) {
            // x from -1 to 1 with the calculation of m_rayDistance to satisfiying the vertical field of view
            m_rayField[x + y*m_width] = Ray(m_position, glm::vec3((x/(float)m_width)*2.0f - 1.0f, ((y/(float)m_height)*2.0f - 1.0f)*m_height/m_width, m_rayDistance));
        }
    }
}

void Camera::resetRayOrigin() {
    for(int y = 0; y < m_height; y++) {
        for( int x = 0; x < m_width; x++) {
            m_rayField[x + y*m_width].origin = m_position;
        }
    }
}

void Camera::move(const glm::vec3& direction) {
    m_position += direction;
    resetRayOrigin();
};

Ray* Camera::getRayField() {
    return m_rayField;
}