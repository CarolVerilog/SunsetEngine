#include "renderer/vulkan/command.hpp"

#include "renderer/vulkan/global.hpp"
#include "renderer/vulkan/queue.hpp"

#include <vulkan/vulkan.h>

#include <stdexcept>

namespace sunset
{
auto createCommandPool() -> void
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (
    vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) !=
    VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan command pool!");
    }
}

auto createCommandBuffer() -> void
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) !=
    VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

auto recordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex)
-> void
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = renderPass;
    renderPassInfo.framebuffer       = swapchainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchainExtent;

    VkClearValue clearColor        = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues    = &clearColor;

    vkCmdBeginRenderPass(
    commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(
    commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    viewport.width  = swapchainExtent.width;
    viewport.height = swapchainExtent.height;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    scissor.extent = swapchainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record Vulkan command buffer!");
    }
}

auto destroyCommandPool() -> void
{
    vkDestroyCommandPool(device, commandPool, nullptr);
}

auto destroyCommandBuffer() -> void {}
}
