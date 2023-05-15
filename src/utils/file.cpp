#include "utils/file.hpp"
#include "utils/type.hpp"

#include <fstream>
#include <stdexcept>

namespace sunset
{
std::vector<char> readFile(std::string_view path)
{
    std::ifstream file(path.data(), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + std::string(path.data()));
    }

    uint32            size = file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);

    return buffer;
}
}
