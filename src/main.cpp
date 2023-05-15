#include "renderer/renderer.hpp"

#include <exception>
#include <iostream>

int main()
{
    try {
        sunset::Renderer::get().run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
