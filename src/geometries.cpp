#include "geometries.hpp"

bool Sphere::CheckCollision(const glm::vec3& rayOrigin, const glm::vec3& ray, float& t0, float& t1) const {
    //geometric approach explained here https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    glm::vec3 originToCenter = m_center - rayOrigin;
    float tca = glm::dot(originToCenter, ray);
    float centerToMiddleOnRay = glm::sqrt(glm::dot(originToCenter, originToCenter) - tca*tca);
    if( centerToMiddleOnRay > m_radius) return false;
    float thc = glm::sqrt(m_radius*m_radius - centerToMiddleOnRay*centerToMiddleOnRay);
    t0 = tca - thc;
    t1 = tca + thc;
    return true;

    // float a = glm::dot(ray, ray);
    // float b = 2 * (glm::dot(ray, rayOrigin) - glm::dot(ray, m_center));
    // float c = glm::dot(rayOrigin, rayOrigin) - 2 * glm::dot(rayOrigin, m_center) + glm::dot(m_center, m_center) - m_radius * m_radius;
    // float discriminant = b * b - 4 * a * c;
    // if (discriminant < 0) {
    //     return false;
    // }
    // else {
    //     t0 = (-b - glm::sqrt(discriminant)) / (2 * a);
    //     t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
    //     if(t0 < 0 ) return false;
    //     else return true;
    // }
}
