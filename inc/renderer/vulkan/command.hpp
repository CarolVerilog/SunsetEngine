#include "utils/type.hpp"

#include <vulkan/vulkan.h>

namespace sunset
{
auto createCommandPool() -> void;
auto createCommandBuffer() -> void;
auto recordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex)
-> void;
auto destroyCommandPool() -> void;
auto destroyCommandBuffer() -> void;
}
