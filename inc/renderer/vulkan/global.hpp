#include <vulkan/vulkan.h>
#include <vector>

#ifdef DEBUG
extern std::vector<const char*> validationLayers;
extern VkDebugUtilsMessengerEXT debugMessenger;
#endif

extern VkInstance instance;
extern VkPhysicalDevice physicalDevice;
extern VkDevice         device;
extern VkQueue          graphicsQueue;