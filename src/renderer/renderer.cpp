#include "renderer/renderer.hpp"

#include "renderer/vulkan/command.hpp"
#include "renderer/vulkan/device.hpp"
#include "renderer/vulkan/global.hpp"
#include "renderer/vulkan/pipeline.hpp"
#include "renderer/vulkan/instance.hpp"
#include "renderer/vulkan/render_pass.hpp"
#include "renderer/vulkan/surface.hpp"
#include "renderer/vulkan/swapchain.hpp"

#include <GLFW/glfw3.h>

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
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

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

    std::vector<std::string> deviceEnabledExtensions;
    deviceEnabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    std::vector<std::string> deviceEnabledLayers;
#ifdef DEBUG
    deviceEnabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    createInstance(instanceEnabledExtensions, instanceEnabledLayers);
    createGLFWSurface(window);
    createDevice(deviceEnabledExtensions, deviceEnabledLayers);
    createSwapchain(width, height);
    createGraphicsPipeline();
    createCommandPool();
    createCommandBuffer();
    createSyncObjs();
}

auto Renderer::createSyncObjs() -> void
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (
    vkCreateSemaphore(
    device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
    vkCreateSemaphore(
    device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
    vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan semaphores!");
    }
}


auto Renderer::mainLoop() -> void
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

auto Renderer::drawFrame() -> void
{
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(
    device, swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
    &imageIndex);

    vkResetCommandBuffer(commandBuffer, 0);
    recordCommandBuffer(commandBuffer, imageIndex);

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[]     = {
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores    = waitSemaphores;
    submitInfo.pWaitDstStageMask  = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    VkSemaphore signalSemaphores[]  = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error(
        "Failed to submit Vulkan draw command buffer.");
    }

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapChains;
    presentInfo.pImageIndices   = &imageIndex;
    presentInfo.pResults        = nullptr;

    vkQueuePresentKHR(presentQueue, &presentInfo);
}

auto Renderer::cleanUp() -> void
{
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);

    destroyCommandBuffer();
    destroyCommandPool();
    destroyGraphicsPipeline();
    destroySwapchain();
    destroyDevice();
    destroySurface();
    destroyInstance();
    glfwDestroyWindow(window);
    glfwTerminate();
}
}
