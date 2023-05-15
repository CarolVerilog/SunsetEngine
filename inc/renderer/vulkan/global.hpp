#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace sunset
{
#ifdef DEBUG
extern VkDebugUtilsMessengerEXT debugMessenger;
#endif

extern VkInstance       instance;

extern VkSurfaceKHR     surface;

extern VkPhysicalDevice physicalDevice;
extern VkDevice         device;
extern VkQueue          graphicsQueue;
extern VkQueue          presentQueue;

extern VkSwapchainKHR   swapchain;
extern std::vector<VkImage> swapchainImages;
extern std::vector<VkImageView> swapchainImageViews;
extern std::vector<VkFramebuffer> swapchainFramebuffers;
extern VkFormat swapchainImageFormat;
extern VkExtent2D swapchainExtent;

extern VkViewport viewport;
extern VkRect2D scissor;

extern VkPipelineLayout pipelineLayout;
extern VkRenderPass renderPass;
extern VkPipeline graphicsPipeline;

extern VkCommandPool commandPool;
extern VkCommandBuffer commandBuffer;
}
