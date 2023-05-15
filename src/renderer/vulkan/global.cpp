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
std::vector<VkImageView> swapchainImageViews;
VkFormat swapchainImageFormat;
VkExtent2D swapchainExtent;

VkPipelineLayout pipelineLayout;
VkRenderPass renderPass;
VkPipeline graphicsPipeline;
}
