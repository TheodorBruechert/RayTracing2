#include "render.hpp"
#include "helperFunctions.hpp"
#include "geometries.hpp"
#include <cstdint>
#include <png.h>


Renderer::Renderer(std::string imgPath, std::size_t width, std::size_t height)
    : m_imgPath(imgPath), m_imgData(new uint32_t[width*height]), m_aspectRatio(width / static_cast<float>(height)), m_width(width), m_height(height), m_scene(glm::vec3(0, 0, -2), glm::vec3(1.0, 2.0, 0.0)), m_sphere(0.5) {}

Renderer::~Renderer() {
    delete[] m_imgData;
}

py::array_t<uint32_t> Renderer::Render() {
    //Render
    auto beginTime = std::chrono::high_resolution_clock::now();

    glm::vec3 lightSource(10.0f, -15.0f, 15.0f);
    glm::vec3 cameraPosition(0.0f, 0.0f, 2.0f);
    Scene scene( cameraPosition, lightSource);
    //sphere
    Sphere sphere(0.5f);
    
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
           
            m_imgData[y*m_width + x] = PerPixel(y, x, scene, sphere);
        }
    }
    //check elapsed Time
    milliseconds duration = std::chrono::duration_cast<milliseconds>(std::chrono::high_resolution_clock::now() - beginTime);
    std::cout << "raw render took " << duration.count() << "ms" << std::endl;
    
    return py::array_t<uint32_t>(m_height*m_width, m_imgData);
}

uint32_t Renderer::PerPixel(const std::size_t& x, const std::size_t& y, const Scene& scene, const Sphere& sphere) {
//default color:
    glm::vec4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 ray = glm::vec3(m_aspectRatio * ((x / (float)m_width) * 2.0f - 1.0f), (y / (float)m_height) * 2.0f - 1.0f, -1.0f);
    //t0 < t1 therefore, the hit point of the ray is at cameraPosition + t0 * ray
    float t0, t1;
    if (sphere.CheckCollision(m_scene.getCamera(), ray, t0, t1)) {
        pixelColor = { 1.0f, 0.0, 0.0f, 1.0f };
        glm::vec3 visualPointOnSphere = m_scene.getCamera() + t0 * ray;
        glm::vec3 visualPointToLightSource = scene.getLightSource() - visualPointOnSphere;
        normalize(visualPointToLightSource);
        float lightFactor = clip(glm::dot(visualPointToLightSource, visualPointOnSphere - sphere.GetCenter()) / (sphere.GetRadius()), 0.0f);
        for(int i = 0; i < 3; i++) {
            pixelColor[i] *= lightFactor;
        }
        pixelColor = glm::clamp(pixelColor, glm::vec4{0.0f}, glm::vec4{1.0f});
    }

    return ConvertFloatRGBAToARGB(pixelColor);
}


//should be clamped color between 0 and 1
uint32_t Renderer::ConvertFloatRGBAToARGB(const glm::vec4& color) {
    return  (((uint8_t)(color.a*255.0f) << 24) | ((uint8_t)(color.r*255.0f) << 16) | ((uint8_t)(color.g*255.0f) << 8) | ((uint8_t)(color.b*255.0f)));
}

void Renderer::SaveImg() {
    FILE* file = fopen(m_imgPath.c_str(), "wb");
    if (!file) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!pngPtr) {
        fclose(file);
        std::cerr << "Error: Unable to create PNG write structure." << std::endl;
        return;
    }

    png_infop infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) {
        fclose(file);
        png_destroy_write_struct(&pngPtr, nullptr);
        std::cerr << "Error: Unable to create PNG info structure." << std::endl;
        return;
    }

    png_set_IHDR(pngPtr, infoPtr, m_width, m_height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_bytep* rowPointers = new png_bytep[m_height];
    for (int y = 0; y < m_height; ++y) {
        rowPointers[y] = reinterpret_cast<png_bytep>(&m_imgData[y * m_width]);
    }

    png_init_io(pngPtr, file);
    png_set_rows(pngPtr, infoPtr, rowPointers);
    png_write_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, nullptr);

    delete[] rowPointers;

    png_destroy_write_struct(&pngPtr, &infoPtr);
    fclose(file);
}


