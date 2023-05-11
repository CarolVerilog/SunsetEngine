#include "renderer/vulkan/device.hpp"

#include "renderer/vulkan/global.hpp"
#include "utils/string.hpp"
#include "utils/type.hpp"

#include <vulkan/vulkan.h>

#include <optional>
#include <set>
#include <stdexcept>

namespace sunset
{

struct QueueFamilyIndices
{
    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    std::optional<uint32> graphicsFamily;
    std::optional<uint32> presentFamily;
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

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(
        device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        i++;
    }

    return indices;
}

auto checkDeviceExtensionSupport(
VkPhysicalDevice device, const std::vector<std::string>& extensionNames) -> bool
{
    uint32 extensionCount;
    vkEnumerateDeviceExtensionProperties(
    device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
    device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensionNames(
    extensionNames.begin(), extensionNames.end());

    for (const auto& availableExtension : availableExtensions) {
        requiredExtensionNames.erase(availableExtension.extensionName);
    }

    return requiredExtensionNames.empty();
}

auto checkDeviceLayerSupport(
VkPhysicalDevice device, const std::vector<std::string>& layerNames) -> bool
{
    uint32 layerCount;
    vkEnumerateDeviceLayerProperties(device, &layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateDeviceLayerProperties(
    device, &layerCount, availableLayers.data());

    std::set<std::string> requiredLayerNames(
    layerNames.begin(), layerNames.end());

    for (const auto& availableLayer : availableLayers) {
        requiredLayerNames.erase(availableLayer.layerName);
    }

    return requiredLayerNames.empty();
}

auto isDeviceSuitable(
VkPhysicalDevice device, const std::vector<std::string>& extensionNames,
const std::vector<std::string>& layerNames) -> bool
{
    auto indices = findQueueFamilies(device);
    return indices.isComplete() &&
           checkDeviceExtensionSupport(device, extensionNames) &&
           checkDeviceLayerSupport(device, layerNames);
}

auto createPhysicalDevice(
const std::vector<std::string>& extensionNames,
const std::vector<std::string>& layerNames) -> void
{
    uint32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, extensionNames, layerNames)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

auto createLogicalDevice(
const std::vector<std::string>& extensionNames,
const std::vector<std::string>& layerNames) -> void
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<uint32>                  queueFamilyIndices = {
    indices.graphicsFamily.value(), indices.presentFamily.value()};

    for (auto queueFamilyIndex : queueFamilyIndices) {
        VkDeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount       = 1;

        float queuePriority              = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    auto extensionNamesC = toCStringArray(extensionNames);
    auto layerNamesC     = toCStringArray(layerNames);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos       = queueCreateInfos.data();
    createInfo.queueCreateInfoCount    = queueCreateInfos.size();
    createInfo.ppEnabledExtensionNames = extensionNamesC.data();
    createInfo.enabledExtensionCount   = extensionNames.size();
    createInfo.ppEnabledLayerNames     = layerNamesC.data();
    createInfo.enabledLayerCount       = layerNames.size();

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);
    createInfo.pEnabledFeatures = &features;

    if (
    vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) !=
    VK_SUCCESS) {
        throw std::runtime_error("Failed to create vulkan logical device.");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

auto createDevice(
const std::vector<std::string>& extensionNames,
const std::vector<std::string>& layerNames) -> void
{
    createPhysicalDevice(extensionNames, layerNames);
    createLogicalDevice(extensionNames, layerNames);
}

auto destroyDevice() -> void { vkDestroyDevice(device, nullptr); }
}