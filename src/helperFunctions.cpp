#include "helperFunctions.hpp"

void normalize(glm::vec3& p) {
    p = glm::normalize(p);
}

float clip(float a, float threshhold) {
    return (a < threshhold) ? threshhold : a;
}