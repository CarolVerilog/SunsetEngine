#pragma once

#include <string_view>
#include <vulkan/vulkan.h>

namespace sunset
{
auto createShaderModule(std::string_view shaderPath) -> VkShaderModule;
auto destroyShaderModule(VkShaderModule shaderModule) -> void;
}
