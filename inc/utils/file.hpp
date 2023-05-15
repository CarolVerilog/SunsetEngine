#pragma once

#include <vector>
#include <string_view>

namespace sunset
{
    std::vector<char> readFile(std::string_view path);
}
