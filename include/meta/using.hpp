#pragma once

#include <vector>

namespace fix
{
    using MessageType = char;
    using TagName = const char *;
    using MapMessage = std::vector<std::pair<std::string, std::string>>;
}