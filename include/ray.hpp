#pragma once
#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}
    Ray() : origin(glm::vec3(0.0)), direction(glm::vec3(0.0)) {}
};


