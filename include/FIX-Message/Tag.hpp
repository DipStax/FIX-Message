#pragma once

#include "meta/concept.hpp"
#include "meta/using.hpp"

namespace fix
{
    template<fix::TagName Name, class Type>
    struct Tag
    {
        static constexpr fix::TagName tag = Name;

        using ValueType = Type;

        Type Value;
    };

    template<fix::TagName Name, bool IsOptional>
    struct TagNo : Tag<Name, std::conditional_t<IsOptional, std::optional<uint16_t>, uint16_t>>
    {
        static constexpr const bool Optional = IsOptional;
    };

    template<class ...TagLists>
    struct TagList
    {
    };
}