#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace fix
{
    using MessageType = char;
    using TagName = uint16_t;
    using MapMessage = std::vector<std::pair<std::string, std::string>>;
}