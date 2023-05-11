#include "renderer/vulkan/instance.hpp"

#include "renderer/vulkan/func_loader.hpp"
#include "renderer/vulkan/global.hpp"
#include "utils/string.hpp"
#include "utils/type.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace sunset
{

auto checkInstanceExtensionSupport(
const std::vector<std::string>& extensionNames) -> void
{
    uint32 extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(
    nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensionNames(
    extensionNames.begin(), extensionNames.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensionNames.erase(extension.extensionName);
    }

    if (!requiredExtensionNames.empty()) {
        throw std::runtime_error(
        "Vulkan required instance extensions not available.");
    }
}

auto checkInstanceLayerSupport(const std::vector<std::string>& layerNames)
-> void
{
    uint32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    std::set<std::string> requiredLayerNames(
    layerNames.begin(), layerNames.end());

    for (const auto& availableLayer : availableLayers) {
        requiredLayerNames.erase(availableLayer.layerName);
    }

    if (!requiredLayerNames.empty()) {
        throw std::runtime_error(
        "Vulkan required instance layers not available.");
    }
}

VKAPI_ATTR auto VKAPI_CALL debugCallback(
VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
VkDebugUtilsMessageTypeFlagsEXT             messageType,
const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
-> VkBool32
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

auto populateDebugMessengerCreateInfo(
VkDebugUtilsMessengerCreateInfoEXT* createInfo) -> void
{
    *createInfo       = {};
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
    createInfo->pUserData       = nullptr;
}

#ifdef DEBUG
auto createDebugMessenger() -> void
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    if (
    LOAD_VULKAN_FUNC(
    vkCreateDebugUtilsMessengerEXT, instance, &createInfo, nullptr,
    &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan debug messenger.");
    }
}
#endif

auto createInstance(
const std::vector<std::string>& extensionNames,
const std::vector<std::string>& layerNames) -> void
{
    checkInstanceExtensionSupport(extensionNames);
    checkInstanceLayerSupport(layerNames);
    auto extensionNamesC = toCStringArray(extensionNames);
    auto layerNamesC     = toCStringArray(layerNames);

    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Sunset Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "Sunset Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo        = &appInfo;
    createInfo.ppEnabledExtensionNames = extensionNamesC.data();
    createInfo.enabledExtensionCount   = extensionNames.size();
    createInfo.ppEnabledLayerNames     = layerNamesC.data();
    createInfo.enabledLayerCount       = layerNames.size();

#ifdef DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugInfo;
    populateDebugMessengerCreateInfo(&debugInfo);
    createInfo.pNext = &debugInfo;
#else
    createInfo.enabledLayerCount = 0;
    createInfo.pNext             = nullptr;
#endif

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance.");
    }

#ifdef DEBUG
    createDebugMessenger();
#endif
}

auto destroyInstance() -> void
{
#ifdef DEBUG
    LOAD_VULKAN_FUNC(
    vkDestroyDebugUtilsMessengerEXT, instance, debugMessenger, nullptr);
#endif
    vkDestroyInstance(instance, nullptr);
}
}
