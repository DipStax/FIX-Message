#pragma once


#include "Exception.hpp"
#include "meta/concept.hpp"
#include "Tag.hpp"
#include "TagValueConvertor.hpp"
#include "meta/is_required_tag.hpp"

namespace fix
{
    template<class ...Tags>
    class PositionalTag
    {
    };

    template<class T, class ...Ts>
    class Header;

    template<class ...PosTags, class ...Tags>
    class Header<fix::PositionalTag<PosTags...>, Tags...>
    {
        public:
            static_assert(sizeof...(PosTags) > 0, "At least one positional tag is required");
            static_assert(std::conjunction_v<fix::meta::is_required_tag<PosTags>...>, "Only required tag are allowed in positional tag");

            [[nodiscard]] bool try_insert(const std::string &_key, const std::string &_value);

            [[nodiscard]] std::string to_string() const;

            template<fix::TagName Name>
                requires IsInListOf_Tag<Name, Tags...>
            [[nodiscard]] constexpr const Tag<Name, meta::find_tagtype_t<Name, Tags...>> &get() const;
            template<fix::TagName Name>
                requires IsInListOf_Tag<Name, Tags...>
            [[nodiscard]] constexpr Tag<Name, meta::find_tagtype_t<Name, Tags...>> &get();

            template<fix::TagName Name>
            [[nodiscard]] constexpr const Tag<Name, meta::find_tagtype_t<Name, PosTags...>> &getPositional() const;
            template<fix::TagName Name>
            [[nodiscard]] constexpr Tag<Name, meta::find_tagtype_t<Name, PosTags...>> &getPositional();

        private:
            template<class Tag>
                requires IsInListOf_Tag<Tag::tag, PosTags...>
            [[nodiscard]] constexpr std::pair<Tag, bool> &internal_getPositional();
            template<class Tag>
                requires IsInListOf_Tag<Tag::tag, PosTags...>
            [[nodiscard]] constexpr const std::pair<Tag, bool> &internal_getPositional() const;

            template<class Tag, class ...RemainTag>
            inline bool try_insert_positional(const std::string &_key, const std::string &_value);
            template<class Tag, class ...RemainTag>
            inline bool try_insert_nonpositional(const std::string &_key, const std::string _value);

            template<class Tag, class ...RemainTag>
            inline void to_string_positional(std::stringstream &_stream) const;
            template<class Tag, class ...RemainTag>
            inline void to_string_tag(std::stringstream &_stream) const;

            std::tuple<std::pair<PosTags, bool>...> m_pos;
            std::tuple<Tags...> m_data;
    };
}

#include "Header.inl"