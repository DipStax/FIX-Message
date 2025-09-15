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
        TagName tagkey = 0;

        if (_key.empty())
            return xstd::Unexpected<RejectError>({ RejectError::InvalidTag, "Tag is empty" });

        std::optional<fix::RejectError> reject = from_FIX(_key, tagkey);

        if (reject.has_value())
            return xstd::Unexpected<RejectError>(std::move(reject.value()));

        xstd::Expected<bool, RejectError> error = try_insert_positional<PosTags...>(tagkey, _value);

        if (error.has_value()) {
            if (error.value())
                return true;
            if constexpr (sizeof...(Tags) > 0)
                return try_insert_nonpositional<Tags...>(tagkey, _value);
            else
                return false;
        }
        return error;
    }

    template<class ...PosTags, class ...Tags>
    std::optional<RejectError> Header<fix::PositionalTag<PosTags...>, Tags...>::verify() const
    {
        return verify_tag<PosTags...>();
    }

    template<class ...PosTags, class ...Tags>
    std::string Header<fix::PositionalTag<PosTags...>, Tags...>::to_string() const
    {
        std::string result{};

        to_string_positional<PosTags...>(result);
        if constexpr (sizeof...(Tags) > 0)
            to_string_tag<Tags...>(result);
        return result;
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

                        error = from_FIX(_value, value);
                        if (!error.has_value())
                            tag.first.Value = value;
                    }
                } else {
                    if (_value.empty())
                        return xstd::Unexpected<RejectError>({ RejectError::EmptyValue, "Expected a value", Tag::tag });
                    error = from_FIX(_value, tag.first.Value);
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

                    error = from_FIX(_value, value);
                    if (!error.has_value())
                        get<Tag::tag>().Value = value;
                }
            } else {
                if (_value.empty())
                    return xstd::Unexpected<RejectError>({ RejectError::EmptyValue, "Expected a value", Tag::tag });
                error = from_FIX(_value, get<Tag::tag>().Value);
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
    std::optional<RejectError> Header<fix::PositionalTag<PosTags...>, Tags...>::verify_tag() const
    {
        if (!internal_getPositional<Tag>().second)
            return RejectError{ RejectError::ReqTagMissing, "Missing positional tag", Tag::tag };
        if constexpr (sizeof...(RemainTag) > 0)
            return verify_tag<RemainTag...>();
        else
            return std::nullopt;
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    void Header<fix::PositionalTag<PosTags...>, Tags...>::to_string_positional(std::string &_result) const
    {
        to_FIX(_result, Tag::tag);
        _result.push_back('=');
        to_FIX(_result, getPositional<Tag::tag>().Value);
        _result.push_back('\1');
        if constexpr (sizeof...(RemainTag) > 0)
            to_string_positional<RemainTag...>(_result);
    }

    template<class ...PosTags, class ...Tags>
    template<class Tag, class ...RemainTag>
    void Header<fix::PositionalTag<PosTags...>, Tags...>::to_string_tag(std::string &_result) const
    {
        if constexpr (IsOptional<typename Tag::ValueType>) {
            const typename Tag::ValueType &tag = get<Tag::tag>().Value;

            if (tag.has_value()) {
                to_FIX(_result, Tag::tag);
                _result.push_back('=');
                to_FIX(_result, tag.value());
                _result.push_back('\1');
            }
        } else {
            to_FIX(_result, Tag::tag);
            _result.push_back('=');
            to_FIX(_result, get<Tag::tag>().Value);
            _result.push_back('\1');
        }
        if constexpr (sizeof...(RemainTag) > 0)
            to_string_tag<RemainTag...>(_result);
    }
}