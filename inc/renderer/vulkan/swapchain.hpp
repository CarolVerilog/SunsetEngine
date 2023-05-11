#include "utils/type.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace sunset
{
struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

auto getSwapchainSupportDetails(VkPhysicalDevice device)
-> SwapchainSupportDetails;
auto createSwapchain(uint32 width, uint32 height) -> void;
auto destroySwapchain()->void;
}
