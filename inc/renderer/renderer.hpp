#pragma once

#include "utils/singleton.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace sunset
{
struct Renderer : Singleton<Renderer>
{
    friend Singleton;
    auto run() -> void;

private:
    Renderer() = default;

    auto initWindow() -> void;
    auto initVulkan() -> void;

    auto createSyncObjs() -> void;

    auto mainLoop() -> void;
    auto drawFrame() -> void;

    auto cleanUp() -> void;

    uint32_t     width  = 800u;
    uint32_t     height = 600u;
    GLFWwindow*  window;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    VkSubmitInfo submitInfo;
    VkPresentInfoKHR presentInfo;
};
}
