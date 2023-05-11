#include "utils/type.hpp"

#include <vulkan/vulkan.h>

#include <optional>

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

auto findQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices;
}
