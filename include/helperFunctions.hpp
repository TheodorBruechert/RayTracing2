#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#define COLOR(r, g, b, ...) glm::vec4((r) / 255.0f, (g) / 255.0f, (b) / 255.0f, ##__VA_ARGS__)

// Predefined colors using the macro
#define BLACK(a) COLOR(0, 0, 0, a)
#define RED(a) COLOR(255, 0, 0, a)
#define GREEN(a) COLOR(0, 255, 0, a)
#define BLUE(a) COLOR(0, 0, 255, a)
#define YELLOW(a) COLOR(255, 255, 0, a)
#define PURPLE(a) COLOR(128, 0, 128, a) // Purple
#define ORANGE(a) COLOR(255, 165, 0, a) // Orange
#define CYAN(a) COLOR(0, 255, 255, a) // Cyan

float clip(float a, float threshhold = 0.0f);
void normalize(glm::vec3& p);

template<class T>
float magnitude(T vec) {
    return glm::sqrt(glm::dot(vec, vec));
}