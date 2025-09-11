#pragma once

#include <vector>
#include <string>

namespace fix
{
    using MessageType = char;
    using TagName = const char *;
    using MapMessage = std::vector<std::pair<std::string, std::string>>;
}