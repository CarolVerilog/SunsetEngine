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
extern VkFormat swapchainImageFormat;
extern VkExtent2D swapchainExtent;
}
