#include <algorithm>

#include "List.hpp"
#include "RejectError.hpp"

namespace fix
{
    template<class RefTagNo, class ...Tags>
    std::optional<RejectError> List<RefTagNo, Tags...>::from_string(const MapMessage &_mapmsg, size_t &_pos)
    {
        std::unordered_set<std::string> set_tag{};
        size_t nelem = 0;
        std::optional<RejectError> reject = std::nullopt;

        if constexpr (IsOptional<typename TagNoType::ValueType>) {
            if (TagNo.Value.has_value())
                nelem = TagNo.Value.value();
            else
                nelem = 0;
        } else {
            nelem = TagNo.Value;
        }
        reject = internal_from_string(_mapmsg, _pos);
        if (reject.has_value())
            return reject.value();
        if (m_data.size() != nelem)
            return RejectError{ RejectError::ValueOORange, "Wrong number of element", TagNoType::tag };
        return std::nullopt;
    }

    template<class RefTagNo, class ...Tags>
    void List<RefTagNo, Tags...>::to_string(std::string &_result)
    {
        if constexpr (TagNoType::Optional) {
            if (m_data.size() != 0) {
                to_FIX(_result, TagNoType::tag);
                _result.push_back('=');
                to_FIX(_result, m_data.size());
                _result.push_back('\1');
            }
        } else {
            to_FIX(_result, TagNoType::tag);
            _result.push_back('=');
            to_FIX(_result, m_data.size());
            _result.push_back('\1');
        }
        for (const DataTuple &_tuple : m_data)
            to_string_tag<Tags...>(_result, _tuple);
    }

    template<class RefTagNo, class ...Tags>
    void List<RefTagNo, Tags...>::add(DataTuple &&_data)
    {
        m_data.push_back(std::move(_data));
    }

    template<class RefTagNo, class ...Tags>
    constexpr const List<RefTagNo, Tags...>::DataTuple &List<RefTagNo, Tags...>::at(size_t _idx) const
    {
        return m_data.at(_idx);
    }

    template<class RefTagNo, class ...Tags>
    constexpr List<RefTagNo, Tags...>::DataTuple &List<RefTagNo, Tags...>::at(size_t _idx)
    {
        return m_data.at(_idx);
    }

    template<class RefTagNo, class ...Tags>
    constexpr size_t List<RefTagNo, Tags...>::size() const
    {
        return m_data.size();
    }

    template<class RefTagNo, class ...Tags>
    List<RefTagNo, Tags...>::iterator List<RefTagNo, Tags...>::begin()
    {
        return m_data.begin();
    }

    template<class RefTagNo, class ...Tags>
    List<RefTagNo, Tags...>::iterator List<RefTagNo, Tags...>::end()
    {
        return m_data.end();
    }

    template<class RefTagNo, class ...Tags>
    List<RefTagNo, Tags...>::const_iterator List<RefTagNo, Tags...>::cbegin() const
    {
        return m_data.begin();
    }

    template<class RefTagNo, class ...Tags>
    List<RefTagNo, Tags...>::const_iterator List<RefTagNo, Tags...>::cend() const
    {
        return m_data.end();
    }

    template<class RefTagNo, class ...Tags>
    std::optional<RejectError> List<RefTagNo, Tags...>::internal_from_string(const MapMessage &_mapmsg, size_t &_pos)
    {
        bool stop = false;
        xstd::Expected<bool, RejectError> expected{false};
        std::optional<RejectError> reject;

        while (!stop && _pos < _mapmsg.size()) {
            DataTuple tuple{};
            std::unordered_set<TagName> set_tag{};

            for (; _pos < _mapmsg.size(); _pos++) {
                const std::pair<std::string, std::string> &pair = _mapmsg.at(_pos);
                const std::string &key = pair.first;
                const std::string &value = pair.second;
                TagName keytag = 0;

                if (key.empty())
                    return RejectError{ RejectError::InvalidTag, "Tag is empty" };
                if (!std::all_of(key.begin(), key.end(), [] (char _c) { return std::isdigit(_c); }))
                    return RejectError{ RejectError::InvalidTag, "Tag should be numeric" };
                keytag = std::stoi(key);

                if (set_tag.contains(keytag))
                    break;
                expected = try_insert<Tags...>(tuple, keytag, value);
                if (expected.has_value()) {
                    if (expected.value()) {
                        set_tag.insert(keytag);
                    } else {
                        stop = true;
                        break;
                    }
                } else {
                    return expected.error();
                }
            }
            reject = verify_required_tag<Tags...>(set_tag);
            if (reject.has_value())
                return reject.value();
            m_data.push_back(std::move(tuple));
        }
        return std::nullopt;
    }

    template<class RefTagNo, class ...Tags>
    template<class Tag, class ...RemainTag>
    xstd::Expected<bool, RejectError> List<RefTagNo, Tags...>::try_insert(DataTuple &_tuple, TagName _key, const std::string _value)
    {
        if (Tag::tag == _key) {
            std::optional<RejectError> error = std::nullopt;

            if constexpr (IsOptional<typename Tag::ValueType>) {
                if (_value.empty()) {
                    std::get<Tag>(_tuple).Value = std::nullopt;
                } else {
                    typename Tag::ValueType::value_type value;

                    error = from_FIX(_value, value);
                    if (!error.has_value())
                        std::get<Tag>(_tuple).Value = value;
                }
            } else {
                if (_value.empty())
                    return xstd::Unexpected<RejectError>({ RejectError::EmptyValue, "Expected a value", Tag::tag });
                error = from_FIX(_value, std::get<Tag>(_tuple).Value);
            }
            if (error.has_value()) {
                error.value().Tag = Tag::tag;
                return xstd::Unexpected<RejectError>(error.value());
            }
            return true;
        }
        if constexpr (sizeof...(RemainTag) != 0) {
            return try_insert<RemainTag...>(_tuple, _key, _value);
        } else {
            return false;
        }
    }

    template<class RefTagNo, class ...Tags>
    template<class Tag, class ...RemainTag>
    std::optional<RejectError> List<RefTagNo, Tags...>::verify_required_tag(const std::unordered_set<TagName> &_set)
    {
        if constexpr (!IsOptional<typename Tag::ValueType>)
            if (!_set.contains(Tag::tag))
                return RejectError{ RejectError::ReqTagMissing, "Missing required tag", Tag::tag };
        if constexpr (sizeof...(RemainTag) > 0)
            return verify_required_tag<RemainTag...>(_set);
        else
            return std::nullopt;
    }

    template<class RefTagNo, class ...Tags>
    template<class Tag, class ...RemainTag>
    void List<RefTagNo, Tags...>::to_string_tag(std::string &_result, const DataTuple &_tuple)
    {
        if constexpr (IsOptional<typename Tag::ValueType>) {
            const typename Tag::ValueType &tag = fix::get<Tag::tag>(_tuple).Value;

            if (tag.has_value()) {
                to_FIX(_result, Tag::tag);
                _result.push_back('=');
                to_FIX(_result, tag.value());
                _result.push_back('\1');
            }
        } else {
            to_FIX(_result, Tag::tag);
            _result.push_back('=');
            to_FIX(_result, fix::get<Tag::tag>(_tuple).Value);
            _result.push_back('\1');
        }
        if constexpr (sizeof...(RemainTag) > 0)
            to_string_tag<RemainTag...>(_result, _tuple);
    }

    template<fix::TagName Name, IsTagTuple DataTuple>
        requires IsInTagTuple<Name, DataTuple>
    constexpr const meta::find_tag_from_name_in_tuple_t<Name, DataTuple> &get(const DataTuple &_data)
    {
        return std::get<meta::find_tag_from_name_in_tuple_t<Name, DataTuple>>(_data);
    }

    template<fix::TagName Name, IsTagTuple DataTuple>
        requires IsInTagTuple<Name, DataTuple>
    constexpr meta::find_tag_from_name_in_tuple_t<Name, DataTuple> &get(DataTuple &_data)
    {
        return std::get<meta::find_tag_from_name_in_tuple_t<Name, DataTuple>>(_data);
    }
}