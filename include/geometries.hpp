#pragma once 
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>


class Object {
private: 
    glm::vec4 m_color;
public:
    virtual bool CheckCollision(const glm::vec3& cameraPosition,const glm::vec3& ray, float& t0, float& t1) const = 0;
    glm::vec4 getColor() { return m_color;}
};


class Sphere : Object{
private:
    glm::vec3 m_center;
    float m_radius;

public:
    Sphere(float radius)
        : m_center(0.0f), m_radius(radius) {}

    Sphere(glm::vec3 center, float radius)
        : m_center(center), m_radius(radius) {}

    bool CheckCollision(const glm::vec3& cameraPosition,const glm::vec3& ray, float& t0, float& t1) const;
    glm::vec3 GetCenter() const { return m_center; }
    float GetRadius() const { return m_radius; }
};

