#pragma once

#include <optional>

namespace fix::meta
{
    template <class T>
    struct is_optional : std::false_type
    {
    };

    template <class T>
    struct is_optional<std::optional<T>> : std::true_type
    {
    };

    template<class T>
    constexpr const bool is_optional_v = is_optional<T>::value;
}