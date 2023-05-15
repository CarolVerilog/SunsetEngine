#include "renderer/vulkan/pipeline_layout.hpp"

#include "renderer/vulkan/global.hpp"

#include <vulkan/vulkan.h>

#include <stdexcept>

namespace sunset
{
void createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount         = 0;        // Optional
    pipelineLayoutInfo.pSetLayouts            = nullptr;  // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0;        // Optional
    pipelineLayoutInfo.pPushConstantRanges    = nullptr;  // Optional

    if (
    vkCreatePipelineLayout(
    device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void destroyPipelineLayout()
{
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}
}
