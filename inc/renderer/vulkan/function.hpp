#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace sunset{
namespace{
template<class Func, class... Args>
auto loadVulkanFunc(const char* funcName, VkInstance instance, Args... args) {
    auto func = (Func) vkGetInstanceProcAddr(instance, funcName);

    if (func != nullptr) {
        return func(instance, args...);
    }
    else {
        throw std::runtime_error("Vulkan extension not present.");
    }
}
}

#define LOAD_VULKAN_FUNC(FUNC, INSTANCE, ARGS...) loadVulkanFunc<PFN_##FUNC>(#FUNC, INSTANCE, ARGS)

}
