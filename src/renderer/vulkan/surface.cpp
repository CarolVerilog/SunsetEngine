#include "renderer/vulkan/surface.hpp"

#include "renderer/vulkan/global.hpp"

namespace sunset
{
auto createSurface() -> void {}

auto createGLFWSurface(GLFWwindow* window) -> void
{
    glfwCreateWindowSurface(instance, window, nullptr, &surface);
}

auto destroySurface() -> void
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
}