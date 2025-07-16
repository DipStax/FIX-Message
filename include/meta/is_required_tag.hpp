#pragma once

#include "meta/using.hpp"
#include "meta/is_optional.hpp"

namespace fix
{
    template<fix::TagName, class>
    struct Tag;
}

namespace fix::meta
{
    template<class T>
    struct is_required_tag : std::false_type {};

    template<fix::TagName Name, class ValueType>
    struct is_required_tag<fix::Tag<Name, ValueType>> : std::bool_constant<!is_optional_v<ValueType>> {};

    template<class T>
    constexpr const bool is_required_tag_v = is_required_tag<T>::value;
}