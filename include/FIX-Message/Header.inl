#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

#include "Header.hpp"
#include "Expected.hpp"

namespace fix
{
    template<class ...PosTags, class ...Tags>
    xstd::Expected<bool, RejectError> Header<fix::PositionalTag<PosTags...>, Tags...>::try_insert(const std::string &_key, const std::string &_value)
    {
        if (_key.empty())
            return xstd::Unexpected<RejectError>({ RejectError::InvalidTag, "Tag is empty" });
        if (!std::all_of(_key.begin(), _key.end(), [] (char _c) { return std::isdigit(_c); }))
            return xstd::Unexpected<RejectError>({ RejectError::InvalidTag, "Tag should be numeric" });

        fix::TagName key = std::stoi(_key);

        xstd::Expected<bool, RejectError> expected = try_insert_positional<PosTags...>(key, _value);

        if (expected.has_value()) {
            if (expected.value())
                return true;
            if constexpr (sizeof...(Tags) > 0)
                return try_insert_nonpositional<Tags...>(key, _value);
            else
                return false;
        }
        return expected;
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
    xstd::Expected<bool, RejectError> Header<fix::PositionalTag<PosTags...>, Tags...>::try_insert_positional(TagName _key, const std::string &_value)
    {
        std::pair<Tag, bool> &tag = internal_getPositional<Tag>();

        if (!tag.second) {
            if (Tag::tag == _key) {
                std::optional<RejectError> error = std::nullopt;

                if constexpr (IsOptional<typename Tag::ValueType>) {
                    if (_value.empty()) {
                        tag.first.Value = std::nullopt;
                    } else {
                        typename Tag::ValueType::value_type value{};

                        error = TagConvertor(_value, value);
                        if (!error.has_value())
                            tag.first.Value = value;
                    }
                } else {
                    if (_value.empty())
                        return xstd::Unexpected<RejectError>({ RejectError::EmptyValue, "Expected a value", Tag::tag });
                    error = TagConvertor(_value, tag.first.Value);
                }
                if (error.has_value()) {
                    error.value().Tag = Tag::tag;
                    return xstd::Unexpected<RejectError>(error.value());
                }
                tag.second = true;
                return true;
            }
            return xstd::Unexpected<RejectError>({ RejectError::InvalidTag, "Invalid positional tag", Tag::tag });
        }
        if constexpr (sizeof...(RemainTag) > 0) {
            return try_insert_positional<RemainTag...>(_key, _value);
        } else {
            return false;
        }
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    xstd::Expected<bool, RejectError> Header<fix::PositionalTag<PosTags...>, Tags...>::try_insert_nonpositional(TagName _key, const std::string _value)
    {
        if (Tag::tag == _key) {
            std::optional<RejectError> error = std::nullopt;

            if constexpr (IsOptional<typename Tag::ValueType>) {
                if (_value.empty()) {
                    get<Tag::tag>().Value = std::nullopt;
                } else {
                    typename Tag::ValueType::value_type value{};

                    error = TagConvertor(_value, value);
                    if (!error.has_value())
                        get<Tag::tag>().Value = value;
                }
            } else {
                if (_value.empty())
                    return xstd::Unexpected<RejectError>({ RejectError::EmptyValue, "Expected a value", Tag::tag });
                error = TagConvertor(_value, get<Tag::tag>().Value);
            }
            if (error.has_value()) {
                error.value().Tag = Tag::tag;
                return xstd::Unexpected<RejectError>(error.value());
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