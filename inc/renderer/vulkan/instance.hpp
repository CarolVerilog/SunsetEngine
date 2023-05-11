#include "utils/type.hpp"

#include <string>
#include <vector>

namespace sunset
{
auto createInstance(
    const std::vector<std::string>& extensionNames,
    const std::vector<std::string>& layerNames) -> void;
auto destroyInstance() -> void;
}