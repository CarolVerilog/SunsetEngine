#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

namespace sunset
{
auto createSurface() -> void;
auto createGLFWSurface(GLFWwindow* window) -> void;
auto destroySurface() -> void;
}
