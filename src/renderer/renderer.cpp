#include "renderer/renderer.hpp"

#include "renderer/utils/vulkan_func_loader.hpp"

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
    window = glfwCreateWindow(width, height, "sunset engine", nullptr, nullptr);
}

auto initValidationLayers(const std::vector<const char*>& validationLayers)
    -> void
{
    uint32_t layerCount;
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
            throw std::runtime_error("validation layers not available.");
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

auto initDebugMessenger(
    VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger) -> void
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    LOAD_VULKAN_FUNC(
        vkCreateDebugUtilsMessengerEXT, instance, &createInfo, nullptr,
        debugMessenger);
}

auto initInstance(
    VkInstance* instance, const std::vector<const char*>* validationLayers)
    -> void
{
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions                    = getExtensions();
    createInfo.enabledExtensionCount   = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();


#ifdef DEBUG
    createInfo.enabledLayerCount   = validationLayers->size();
    createInfo.ppEnabledLayerNames = validationLayers->data();

    VkDebugUtilsMessengerCreateInfoEXT debugInfo;
    populateDebugMessengerCreateInfo(&debugInfo);
    createInfo.pNext = &debugInfo;
#else
    createInfo.enabledLayerCount = 0;
    creatInfo.pNext              = nullptr;
#endif

    if (vkCreateInstance(&createInfo, nullptr, instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vulkan instance.");
    }
}

auto Renderer::initVulkan() -> void
{
#ifdef DEBUG
    initValidationLayers(validationLayers);
#endif
    initInstance(&instance, &validationLayers);
}


auto Renderer::mainLoop() -> void
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

auto Renderer::cleanUp() -> void
{
#ifdef DEBUG
    LOAD_VULKAN_FUNC(
       vkDestroyDebugUtilsMessengerEXT, instance, debugMessenger, nullptr);
#endif
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}
}