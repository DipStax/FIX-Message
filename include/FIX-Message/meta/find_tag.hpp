#include <type_traits>
#include <cstring>

#include "FIX-Message/meta/using.hpp"

namespace fix
{
    template<fix::TagName, class>
    struct Tag;

    template<fix::TagName, bool>
    struct TagNo;

    template<class, class ...>
    struct List;
}

namespace fix::meta
{
    template<fix::TagName Name, class T>
    struct is_tag_with_name : std::false_type {};

    template<fix::TagName Name, fix::TagName NameTag, class TypeImp>
    struct is_tag_with_name<Name, fix::Tag<NameTag, TypeImp>>
        : std::bool_constant<Name == NameTag>
    {
        using type = fix::Tag<Name, TypeImp>;
    };

    template<fix::TagName Name, class ... Tags>
    struct find_tag_from_name
    {
        using type = typename std::disjunction<
            is_tag_with_name<Name, Tags>...,
            std::type_identity<void>
        >::type;
    };

    template<fix::TagName Name, class ...Tags>
    using find_tag_from_name_t = typename find_tag_from_name<Name, Tags...>::type;

    template<fix::TagName Name, class ...Tags>
    using find_tagtype_t = typename find_tag_from_name_t<Name, Tags...>::ValueType;


    template<fix::TagName Name, class T>
    struct is_taglist_with_name : std::false_type {};

    template<fix::TagName Name, fix::TagName NameTagNo, bool IsOptional, class ...Tags>
    struct is_taglist_with_name<Name, fix::List<fix::TagNo<NameTagNo, IsOptional>, Tags...>>
        : std::bool_constant<Name == NameTagNo>
    {
        using type = fix::List<fix::TagNo<NameTagNo, IsOptional>, Tags...>;
    };

    template<fix::TagName Name, class ...TagList>
    struct find_taglist_from_name
    {
        using type = typename std::disjunction<
            is_taglist_with_name<Name, TagList>...,
            std::type_identity<void>
        >::type;
    };

    template<fix::TagName Name, class ...TagList>
    using find_taglist_from_name_t = typename find_taglist_from_name<Name, TagList...>::type;


    template<fix::TagName Name, class T>
    struct find_tag_from_name_in_tuple : std::false_type {};

    template<fix::TagName Name, class ...Tags>
    struct find_tag_from_name_in_tuple<Name, std::tuple<Tags...>>
    {
        using type = std::disjunction<
            is_tag_with_name<Name, Tags>...,
            std::type_identity<void>
        >::type;
    };

    template<fix::TagName Name, class TagTuple>
    using find_tag_from_name_in_tuple_t = typename find_tag_from_name_in_tuple<Name, TagTuple>::type;
}