#include "renderer/renderer.hpp"

#include "renderer/vulkan/device.hpp"
#include "renderer/vulkan/instance.hpp"
#include "renderer/vulkan/surface.hpp"
#include "renderer/vulkan/swapchain.hpp"

#include <GLFW/glfw3.h>

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

auto getGLFWRequiredInstanceExtensions() -> std::vector<std::string>
{
    uint32_t     glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<std::string> extensions(
    glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

auto Renderer::initVulkan() -> void
{
    std::vector<std::string> instanceEnabledExtensions;
    auto glfwRequiredInstanceExtensions = getGLFWRequiredInstanceExtensions();
    instanceEnabledExtensions.insert(
    instanceEnabledExtensions.end(), glfwRequiredInstanceExtensions.begin(),
    glfwRequiredInstanceExtensions.end());

#ifdef DEBUG
    instanceEnabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    std::vector<std::string> instanceEnabledLayers;

#ifdef DEBUG
    instanceEnabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    createInstance(instanceEnabledExtensions, instanceEnabledLayers);

    createGLFWSurface(window);

    std::vector<std::string> deviceEnabledExtensions;
    deviceEnabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    std::vector<std::string> deviceEnabledLayers;

#ifdef DEBUG
    deviceEnabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    createDevice(deviceEnabledExtensions, deviceEnabledLayers);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    createSwapchain(width, height);
}

auto Renderer::mainLoop() -> void
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

auto Renderer::cleanUp() -> void
{
    destroySwapchain();
    destroyDevice();
    destroySurface();
    destroyInstance();
    glfwDestroyWindow(window);
    glfwTerminate();
}
}
