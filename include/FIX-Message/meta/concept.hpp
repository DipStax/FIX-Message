#pragma once

#include <concepts>

#include "FIX-Message/meta/find_tag.hpp"
#include "FIX-Message/meta/is_optional.hpp"
#include "FIX-Message/meta/is_tag_tuple.hpp"

namespace fix
{
    template<fix::TagName Name, class ...Tags>
    concept IsInListOf_Tag = !std::is_same_v<void, fix::meta::find_tag_from_name_t<Name, Tags...>>;

    template<fix::TagName Name, class ...TagList>
    concept IsInListOf_TagList = !std::is_same_v<void, fix::meta::find_taglist_from_name_t<Name, TagList...>>;

    template<class T>
    concept IsOptional = fix::meta::is_optional_v<T>;

    template<class T>
    concept IsTag = fix::meta::is_tag_v<T> && requires (T _tag) {
        typename T::ValueType;
        { T::tag } -> std::same_as<const char * const &>;
        { _tag.Value } -> std::same_as<typename T::ValueType>;
    };

    template<class TagTuple>
    concept IsTagTuple = meta::is_tag_tuple_v<TagTuple>;

    template<fix::TagName Name, class TagTuple>
    concept IsInTagTuple = !std::is_same_v<void, fix::meta::find_tag_from_name_in_tuple_t<Name, TagTuple>>;
}