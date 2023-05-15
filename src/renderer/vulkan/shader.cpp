#include "renderer/vulkan/global.hpp"
#include "renderer/vulkan/shader.hpp"
#include "utils/file.hpp"

#include <stdexcept>

namespace sunset
{
auto createShaderModule(std::string_view shaderPath) -> VkShaderModule
{
    auto code = readFile(shaderPath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (
    vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) !=
    VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan shader module.");
    }

    return shaderModule;
}

auto destroyShaderModule(VkShaderModule shaderModule) -> void {
    vkDestroyShaderModule(device, shaderModule, nullptr);
}
}
