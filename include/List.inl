#include <sstream>
#include <iomanip>

#include "List.hpp"
#include "Exception.hpp"
#include "TagValueConvertor.hpp"

namespace fix
{
    template<class RefTagNo, class ...Tags>
    void List<RefTagNo, Tags...>::from_string(const MapMessage &_mapmsg, size_t &_pos)
    {
        std::unordered_set<std::string> set_tag{};
        size_t nelem = 0;

        if constexpr (IsOptional<typename TagNoType::ValueType>) {
            if (TagNo.Value.has_value())
                nelem = TagNo.Value.value();
            else
                nelem = 0;
        } else {
            nelem = TagNo.Value;
        }
        internal_from_string(_mapmsg, _pos);
        if (m_data.size() != nelem)
            throw RejectException("Wrong number of element", RejectException::ValueOORange);
    }

    template<class RefTagNo, class ...Tags>
    void List<RefTagNo, Tags...>::to_string(std::stringstream &_stream)
    {
        _stream << TagNoType::tag << "=" << m_data.size() << "\1";
        for (const DataTuple &_tuple : m_data)
        to_string_tag<Tags...>(_stream, _tuple);
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
    void List<RefTagNo, Tags...>::internal_from_string(const MapMessage &_mapmsg, size_t &_pos)
    {
        bool stop = false;

        while (!stop && _pos < _mapmsg.size()) {
            DataTuple tuple{};
            std::unordered_set<std::string> set_tag{};

            for (; _pos < _mapmsg.size(); _pos++) {
                const std::pair<std::string, std::string> &pair = _mapmsg.at(_pos);
                const std::string &key = pair.first;
                const std::string &value = pair.second;

                if (set_tag.contains(key)) {
                    break;
                } else if (try_insert<Tags...>(tuple, key, value)) {
                    set_tag.insert(key);
                } else {
                    stop = true;
                    break;
                }
            }
            verify_required_tag<Tags...>(set_tag);
            m_data.push_back(std::move(tuple));
        }
    }

    template<class RefTagNo, class ...Tags>
    template<class Tag, class ...RemainTag>
    bool List<RefTagNo, Tags...>::try_insert(DataTuple &_tuple, const std::string &_key, const std::string _value)
    {
        if (std::strcmp(Tag::tag, _key.c_str()) == 0) {
            if constexpr (IsOptional<typename Tag::ValueType>) {
                std::get<Tag>(_tuple).Value = TagValueConvertorOptional<typename Tag::ValueType::value_type>(_value);
            } else {
                if (_value.empty())
                    throw RejectException("Expected a value", RejectException::EmptyValue);
                std::get<Tag>(_tuple).Value = TagValueConvertor<typename Tag::ValueType>(_value);
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
    void List<RefTagNo, Tags...>::verify_required_tag(const std::unordered_set<std::string> &_set)
    {
        if constexpr (!IsOptional<typename Tag::ValueType>)
            if (!_set.contains(Tag::tag))
                throw RejectException("Missing required tag", RejectException::ReqTagMissing);
        if constexpr (sizeof...(RemainTag) > 0)
            verify_required_tag<RemainTag...>(_set);
    }

    template<class RefTagNo, class ...Tags>
    template<class Tag, class ...RemainTag>
    void List<RefTagNo, Tags...>::to_string_tag(std::stringstream &_stream, const DataTuple &_tuple)
    {
        if constexpr (IsOptional<typename Tag::ValueType>) {
            const typename Tag::ValueType &tag = get<Tag::tag>(_tuple).Value;

            if (tag.has_value())
                _stream << Tag::tag << "=" << tag.value() << "\1";
        } else {
            _stream << Tag::tag << "=" << get<Tag::tag>(_tuple).Value << "\1";
        }
        if constexpr (sizeof...(RemainTag) > 0)
            to_string_tag<RemainTag...>(_stream, _tuple);
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