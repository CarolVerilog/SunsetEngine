#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace sunset
{
namespace
{
template <class String>
concept StringType = requires() {
    std::is_same<String, std::string>::value ||
    std::is_same<String, std::wstring>::value ||
    std::is_same<String, std::string_view>::value ||
    std::is_same<String, std::string_view>::value;
};
}

template <StringType String>
auto toCStringArray(const std::vector<String>& strArray)
{
    std::vector<decltype(strArray.begin()->data())> cStrArray;
    std::ranges::for_each(
    strArray, [&](const String& str) { cStrArray.push_back(str.data()); });

    return cStrArray;
}
}
