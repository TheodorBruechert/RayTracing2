#include "render.hpp"


int main(int argc, char* argv[]) {
    std::string img_path = (argc > 1) ? argv[1] : "";
    std::cout << "IMAGE PATH:" << img_path << std::endl;

    unsigned width = 800;
    unsigned height = (unsigned)((9/(float)16)*width);
    //Renderer
    Renderer renderer(img_path, width, height);
    auto renderTime = renderer.Render();

    return 0;
}
