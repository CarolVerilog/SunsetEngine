#include "renderer/vulkan/global.hpp"

#ifdef DEBUG
std::vector<const char*> validationLayers;
VkDebugUtilsMessengerEXT debugMessenger;
#endif

VkInstance         instance;
VkPhysicalDevice   physicalDevice;
VkDevice           device;
VkQueue            graphicsQueue;