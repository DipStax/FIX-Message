#pragma once

#include <iostream>

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
    struct TagNo : Tag<Name, std::conditional_t<IsOptional, std::optional<int>, int>>
    {
    };

    template<class ...TagLists>
    struct TagList
    {
    };
}