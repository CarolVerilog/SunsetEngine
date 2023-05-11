#include "utils/type.hpp"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace sunset
{
auto createDevice(
const std::vector<std::string>& extensionNames,
const std::vector<std::string>& layerNames) -> void;
auto destroyDevice() -> void;
}