#include <sstream>
#include <iomanip>

#include "Header.hpp"

namespace fix
{
    template<class ...PosTags, class ...Tags>
    bool Header<fix::PositionalTag<PosTags...>, Tags...>::try_insert(const std::string &_key, const std::string &_value)
    {
        if (try_insert_positional<PosTags...>(_key, _value))
            return true;
        if constexpr (sizeof...(Tags) > 0)
            return try_insert_nonpositional<Tags...>(_key, _value);
        else
            return false;
    }

    template<class ...PosTags, class ...Tags>
    std::string Header<fix::PositionalTag<PosTags...>, Tags...>::to_string() const
    {
        std::stringstream stream;

        stream << std::fixed << std::setprecision(4);
        to_string_positional<PosTags...>(stream);
        if constexpr (sizeof...(Tags) > 0)
            to_string_tag<Tags...>(stream);
        return stream.str();
    }

    template<class ...PosTags, class ...Tags>
    template<fix::TagName Name>
        requires IsInListOf_Tag<Name, Tags...>
    constexpr const Tag<Name, meta::find_tagtype_t<Name, Tags...>> &Header<fix::PositionalTag<PosTags...>, Tags...>::get() const
    {
        return std::get<Tag<Name, meta::find_tagtype_t<Name, Tags...>>>(m_data);
    }

    template<class ...PosTags, class ...Tags>
    template<fix::TagName Name>
        requires IsInListOf_Tag<Name, Tags...>
    constexpr Tag<Name, meta::find_tagtype_t<Name, Tags...>> &Header<fix::PositionalTag<PosTags...>, Tags...>::get()
    {
        return std::get<Tag<Name, meta::find_tagtype_t<Name, Tags...>>>(m_data);
    }

    template<class ...PosTags, class ...Tags>
    template<fix::TagName Name>
    constexpr const Tag<Name, meta::find_tagtype_t<Name, PosTags...>> &Header<fix::PositionalTag<PosTags...>, Tags...>::getPositional() const
    {
        return std::get<std::pair<Tag<Name, meta::find_tagtype_t<Name, PosTags...>>, bool>>(m_pos).first;
    }

    template<class ...PosTags, class ...Tags>
    template<fix::TagName Name>
    constexpr Tag<Name, meta::find_tagtype_t<Name, PosTags...>> &Header<fix::PositionalTag<PosTags...>, Tags...>::getPositional()
    {
        return std::get<std::pair<Tag<Name, meta::find_tagtype_t<Name, PosTags...>>, bool>>(m_pos).first;
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag>
        requires IsInListOf_Tag<Tag::tag, PosTags...>
    constexpr std::pair<Tag, bool> &Header<fix::PositionalTag<PosTags...>, Tags...>::internal_getPositional()
    {
        return std::get<std::pair<Tag, bool>>(m_pos);
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag>
        requires IsInListOf_Tag<Tag::tag, PosTags...>
    constexpr const std::pair<Tag, bool> &Header<fix::PositionalTag<PosTags...>, Tags...>::internal_getPositional() const
    {
        return std::get<std::pair<Tag, bool>>(m_pos);
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    bool Header<fix::PositionalTag<PosTags...>, Tags...>::try_insert_positional(const std::string &_key, const std::string &_value)
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

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    bool Header<fix::PositionalTag<PosTags...>, Tags...>::try_insert_nonpositional(const std::string &_key, const std::string _value)
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

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    void Header<fix::PositionalTag<PosTags...>, Tags...>::to_string_positional(std::stringstream &_stream) const
    {
        _stream << Tag::tag << "=" << getPositional<Tag::tag>().Value << "\1";
        if constexpr (sizeof...(RemainTag) > 0)
            to_string_positional<RemainTag...>(_stream);
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    void Header<fix::PositionalTag<PosTags...>, Tags...>::to_string_tag(std::stringstream &_stream) const
    {
        if constexpr (IsOptional<typename Tag::ValueType>) {
            const typename Tag::ValueType &tag = get<Tag::tag>().Value;

            if (tag.has_value())
                _stream << Tag::tag << "=" << tag.value() << "\1";
        } else {
            _stream << Tag::tag << "=" << get<Tag::tag>().Value << "\1";
        }
        if constexpr (sizeof...(RemainTag) > 0)
            to_string_tag<RemainTag...>(_stream);
    }
}