#include "renderer/renderer.hpp"

#include "renderer/vulkan/device.hpp"
#include "renderer/vulkan/func_loader.hpp"
#include "renderer/vulkan/global.hpp"
#include "renderer/vulkan/instance.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace sunset
{
auto Renderer::run() -> void
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}

auto Renderer::initWindow() -> void
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Sunset Engine", nullptr, nullptr);
}

auto Renderer::initVulkan() -> void
{
    createInstance();
    createDevice();
}

auto Renderer::mainLoop() -> void
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

auto Renderer::cleanUp() -> void
{
    destroyDevice();
    destroyInstance();
    glfwDestroyWindow(window);
    glfwTerminate();
}
}