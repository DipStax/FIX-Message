#pragma once

#include "meta/using.hpp"

namespace fix
{
    template<fix::TagName, class>
    struct Tag;
}

namespace fix::meta
{
    template<class T>
    struct is_tag : std::false_type {};

    template<fix::TagName Name, class ValueType>
    struct is_tag<fix::Tag<Name, ValueType>> : std::true_type {};

    template<class T>
    constexpr const bool is_tag_v = is_tag<T>::value;
}