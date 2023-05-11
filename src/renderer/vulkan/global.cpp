#include "renderer/vulkan/global.hpp"

namespace sunset
{
#ifdef DEBUG
VkDebugUtilsMessengerEXT debugMessenger;
#endif

VkInstance       instance;

VkSurfaceKHR     surface;

VkPhysicalDevice physicalDevice;
VkDevice         device;
VkQueue          graphicsQueue;
VkQueue          presentQueue;

VkSwapchainKHR   swapchain;
std::vector<VkImage> swapchainImages;
VkFormat swapchainImageFormat;
VkExtent2D swapchainExtent;
}
