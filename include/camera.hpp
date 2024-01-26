#pragma once
#include <glm/glm.hpp>
#include "ray.hpp"

class Camera {
private:
    const float m_fovH; //radiant
    const float m_rayDistance;
    const float m_rayFieldHeight;
    const int m_height;
    const int m_width;


    glm::vec3 m_position;
    glm::vec3 m_forward;

    Ray* m_rayField;
    
    void recalcRayField();
    void resetRayOrigin();
public:
    Camera(const float& fovH, const int& width, const int& height, const glm::vec3& position, const glm::vec3& forward = {0.0, 0.0, 1.0});
    void move(const glm::vec3& direction);
    Ray* getRayField();

};