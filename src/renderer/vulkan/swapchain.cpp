#include "renderer/vulkan/swapchain.hpp"

#include "renderer/vulkan/global.hpp"
#include "renderer/vulkan/queue.hpp"
#include "utils/type.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace sunset
{
auto getSwapchainSupportDetails(VkPhysicalDevice device)
-> SwapchainSupportDetails
{
    SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
    device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
    device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

auto chooseSwapSurfaceFormat(
const std::vector<VkSurfaceFormatKHR>& availableFormats) -> VkSurfaceFormatKHR
{
    for (const auto& availableFormat : availableFormats) {
        if (
        availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

auto chooseSwapPresentMode(
const std::vector<VkPresentModeKHR>& availablePresentModes) -> VkPresentModeKHR
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(
uint32 width, uint32 height, const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (
    capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = {width, height};

        actualExtent.width = std::clamp(
        actualExtent.width, capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(
        actualExtent.height, capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

auto createSwapchain(uint32 width, uint32 height) -> void
{
    SwapchainSupportDetails swapchainSupport =
    getSwapchainSupportDetails(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat =
    chooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode =
    chooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent =
    chooseSwapExtent(width, height, swapchainSupport.capabilities);

    uint32 imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (
    swapchainSupport.capabilities.maxImageCount > 0 &&
    imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = surface;
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    QueueFamilyIndices indices              = findQueueFamilies(physicalDevice);
    uint32             queueFamilyIndices[] = {
    indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices   = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices   = nullptr;
    }

    createInfo.preTransform   = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;
    createInfo.oldSwapchain   = VK_NULL_HANDLE;

    if (
    vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) !=
    VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat=surfaceFormat.format;
    swapchainExtent=extent;
}

auto destroySwapchain() -> void
{
    vkDestroySwapchainKHR(device, swapchain, nullptr);
}
}
