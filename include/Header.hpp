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
            static_assert(std::conjunction_v<fix::meta::is_required_tag<PosTags>...>, "Only required tag are allowed in positional tag");

            bool try_insert(const std::string &_key, const std::string &_value)
            {
                if (try_insert_positional<PosTags...>(_key, _value))
                    return true;
                if constexpr (sizeof...(Tags) > 0)
                    return try_insert_nonpositional<Tags...>(_key, _value);
                else
                    return false;
            }

            template<fix::TagName Name>
                requires IsInListOf_Tag<Name, Tags...>
            constexpr const Tag<Name, meta::find_tagtype_t<Name, Tags...>> &get() const
            {
                return std::get<Tag<Name, meta::find_tagtype_t<Name, Tags...>>>(m_data);
            }
            template<fix::TagName Name>
                requires IsInListOf_Tag<Name, Tags...>
            constexpr Tag<Name, meta::find_tagtype_t<Name, Tags...>> &get()
            {
                return std::get<Tag<Name, meta::find_tagtype_t<Name, Tags...>>>(m_data);
            }

            template<fix::TagName Name>
            constexpr const Tag<Name, meta::find_tagtype_t<Name, PosTags...>> &getPositional() const
            {
                return std::get<std::pair<Tag<Name, meta::find_tagtype_t<Name, PosTags...>>, bool>>(m_pos).first;
            }

            template<fix::TagName Name>
            constexpr Tag<Name, meta::find_tagtype_t<Name, PosTags...>> &getPositional()
            {
                return std::get<std::pair<Tag<Name, meta::find_tagtype_t<Name, PosTags...>>, bool>>(m_pos).first;
            }

        private:
            template<class Tag>
                requires IsInListOf_Tag<Tag::tag, PosTags...>
            constexpr std::pair<Tag, bool> &internal_getPositional()
            {
                return std::get<std::pair<Tag, bool>>(m_pos);
            }

            template<class Tag, class ...RemainTag>
            inline bool try_insert_positional(const std::string &_key, const std::string &_value)
            {
                std::pair<Tag, bool> &tag = internal_getPositional<Tag>();

                if (!tag.second) {
                    if (std::strcmp(Tag::tag, _key.c_str()) == 0) {
                        if constexpr (IsOptional<typename Tag::ValueType>) {
                            tag.first.Value = TagValueConvertorOptional<typename Tag::ValueType::value_type>(_value);
                        } else {
                            if (_value.empty())
                                throw RejectException("Expected a value", RejectException::EmptyValue);
                            tag.first.Value = TagValueConvertor<typename Tag::ValueType>(_value);
                        }
                        tag.second = true;
                        return true;
                    }
                    throw RejectException("Invalid positional tag", RejectException::InvalidTag);
                }
                if constexpr (sizeof...(RemainTag) > 0) {
                    return try_insert_positional<RemainTag...>(_key, _value);
                } else {
                    return false;
                }
            }

            template<class Tag, class ...RemainTag>
            inline bool try_insert_nonpositional(const std::string &_key, const std::string _value)
            {
                if (std::strcmp(Tag::tag, _key.c_str()) == 0) {
                    if constexpr (IsOptional<typename Tag::ValueType>) {
                        get<Tag::tag>().Value = TagValueConvertorOptional<typename Tag::ValueType::value_type>(_value);
                    } else {
                        if (_value.empty())
                            throw RejectException("Expected a value", RejectException::EmptyValue);
                        get<Tag::tag>().Value = TagValueConvertor<typename Tag::ValueType>(_value);
                    }
                    return true;
                }
                if constexpr (sizeof...(RemainTag) > 0) {
                    return try_insert_nonpositional<RemainTag...>(_key, _value);
                } else {
                    return false;
                }
            }

            std::tuple<std::pair<PosTags, bool>...> m_pos;
            std::tuple<Tags...> m_data;
    };
}