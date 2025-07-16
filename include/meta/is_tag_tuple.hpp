#pragma once

#include "is_tag.hpp"

namespace fix::meta
{
    template<class, template<typename> class>
    struct unpack_tuple_and : std::false_type {};

    template<class ...Tags, template<typename> class Trait>
    struct unpack_tuple_and<std::tuple<Tags...>, Trait>
    {
        static constexpr bool value = std::conjunction_v<Trait<Tags>...>;
    };

    template<class TagTuple>
    constexpr bool is_tag_tuple_v = unpack_tuple_and<TagTuple, is_tag>::value;
}