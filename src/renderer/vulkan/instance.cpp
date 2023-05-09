#include "renderer/vulkan/instance.hpp"

#include "renderer/vulkan/func_loader.hpp"
#include "renderer/vulkan/global.hpp"
#include "utils/type.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <cstring>
#include <iostream>
#include <vector>

namespace sunset
{

auto createValidationLayers() -> void
{
    uint32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            throw std::runtime_error("Vulkan validation layers not available.");
        }
    }
}

auto getExtensions() -> std::vector<const char*>
{
    uint32_t     glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(
        glfwExtensions, glfwExtensions + glfwExtensionCount);
#ifdef DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensions;
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

auto createDebugMessenger() -> void
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    LOAD_VULKAN_FUNC(
        vkCreateDebugUtilsMessengerEXT, instance, &createInfo, nullptr,
        &debugMessenger);
}

auto createInstance() -> void
{
#ifdef DEBUG
    createValidationLayers();
#endif

    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Sunset Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "Sunset Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions                    = getExtensions();
    createInfo.enabledExtensionCount   = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef DEBUG
    createInfo.enabledLayerCount   = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();

    VkDebugUtilsMessengerCreateInfoEXT debugInfo;
    populateDebugMessengerCreateInfo(&debugInfo);
    createInfo.pNext = &debugInfo;
#else
    createInfo.enabledLayerCount = 0;
    creatInfo.pNext              = nullptr;
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