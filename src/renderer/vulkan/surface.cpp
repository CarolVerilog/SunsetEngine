#include "renderer/vulkan/surface.hpp"

#include "renderer/vulkan/global.hpp"

#include <stdexcept>

namespace sunset
{
auto createSurface() -> void {}

auto createGLFWSurface(GLFWwindow* window) -> void
{
    if(glfwCreateWindowSurface(instance, window, nullptr, &surface)!=VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan GLFW surface.");
    }
}

auto destroySurface() -> void
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
}
