#include "renderer/vulkan/device.hpp"

#include "renderer/vulkan/global.hpp"
#include "utils/type.hpp"

#include <vulkan/vulkan.h>

#include <optional>
#include <stdexcept>

namespace sunset
{

struct QueueFamilyIndices
{
    bool isComplete() { return graphicsFamily.has_value(); }

    std::optional<uint32> graphicsFamily;
};

auto findQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        i++;
    }

    return indices;
}

auto isDeviceSuitable(VkPhysicalDevice device) -> bool
{
    auto indices = findQueueFamilies(device);
    return indices.isComplete();
}

auto createPhysicalDevice() -> void
{
    uint32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

auto createLogicalDevice() -> void
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount       = 1;

    float queuePriority              = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos     = &queueCreateInfo;
    createInfo.queueCreateInfoCount  = 1;
    createInfo.pEnabledFeatures      = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create vulkan logical device.");

        vkGetDeviceQueue(
            device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }
}

auto createDevice() -> void
{
    createPhysicalDevice();
    createLogicalDevice();
}

auto destroyDevice() -> void { vkDestroyDevice(device, nullptr); }
}