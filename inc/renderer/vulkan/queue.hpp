#pragma once

#include "utils/type.hpp"

#include <vulkan/vulkan.h>

#include <optional>

namespace sunset
{
struct QueueFamilyIndices
{
    auto isComplete() -> bool;

    std::optional<uint32> graphicsFamily;
    std::optional<uint32> presentFamily;
};

auto findQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices;
}
