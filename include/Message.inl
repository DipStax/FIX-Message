#include <sstream>
#include <iomanip>

#include "Message.hpp"
#include "TagValueConvertor.hpp"

namespace fix
{
    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    void Message<MsgType, fix::TagList<TagLists...>, Tags...>::from_string(const MapMessage &_mapmsg)
    {
        std::unordered_set<std::string> set_tag{};

        for (size_t it = 0; it < _mapmsg.size(); it++) {
            const std::pair<std::string, std::string> &pair = _mapmsg.at(it);
            const std::string &key = pair.first;
            const std::string &value = pair.second;

            if constexpr (sizeof...(Tags) > 0) {
                if (!try_insert<Tags...>(key, value)) {
                    try_insert_tagno(key, value, _mapmsg, it);
                } else {
                    if (set_tag.contains(key))
                        throw RejectException("Duplicate tag value", RejectException::InvalidTag);
                    set_tag.emplace(key);
                }
            } else {
                try_insert_tagno(key, value, _mapmsg, it);
            }
        }
        if constexpr (sizeof...(Tags) > 0)
            verify_required_tag<Tags...>(set_tag);
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    std::string Message<MsgType, fix::TagList<TagLists...>, Tags...>::to_string() const
    {
        std::stringstream stream;

        stream << std::fixed << std::setprecision(4);
        if constexpr (sizeof...(Tags) > 0)
            to_string_tag<Tags...>(stream);
        if constexpr (sizeof...(TagLists) > 0)
            to_string_list<TagLists...>(stream);
        return stream.str();
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<TagName Name>
        requires IsInListOf_Tag<Name, Tags...>
    const Tag<Name, meta::find_tagtype_t<Name, Tags...>> &Message<MsgType, fix::TagList<TagLists...>, Tags...>::get() const
    {
        return std::get<Tag<Name, meta::find_tagtype_t<Name, Tags...>>>(m_tags);
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<TagName Name>
        requires IsInListOf_Tag<Name, Tags...>
    Tag<Name, meta::find_tagtype_t<Name, Tags...>> &Message<MsgType, fix::TagList<TagLists...>, Tags...>::get()
    {
        return std::get<Tag<Name, meta::find_tagtype_t<Name, Tags...>>>(m_tags);
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<TagName Name>
        requires IsInListOf_TagList<Name, TagLists...>
    const meta::find_taglist_from_name_t<Name, TagLists...> &Message<MsgType, fix::TagList<TagLists...>, Tags...>::getList() const
    {
        return std::get<meta::find_taglist_from_name_t<Name, TagLists...>>(m_taglists);
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<TagName Name>
        requires IsInListOf_TagList<Name, TagLists...>
    meta::find_taglist_from_name_t<Name, TagLists...> &Message<MsgType, fix::TagList<TagLists...>, Tags...>::getList()
    {
        return std::get<meta::find_taglist_from_name_t<Name, TagLists...>>(m_taglists);
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class Tag, class ...RemainTag>
    bool Message<MsgType, fix::TagList<TagLists...>, Tags...>::try_insert(const std::string &_key, const std::string &_value)
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
        if constexpr (sizeof...(RemainTag) != 0) {
            return try_insert<RemainTag...>(_key, _value);
        } else {
            return false;
        }
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    void Message<MsgType, fix::TagList<TagLists...>, Tags...>::try_insert_tagno(const std::string &_key, const std::string &_value, const MapMessage &_mapmsg, size_t &_it)
    {
        if constexpr (sizeof...(TagLists) > 0) {
            if (!is_reftagno<TagLists...>(_key, _value, _mapmsg, _it))
                throw RejectException("Unknown tag", RejectException::UndefineTag);
            _it--;
        } else {
            throw RejectException("Unknown tag", RejectException::UndefineTag);
        }
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class TagList, class ...RemainTagList>
    bool Message<MsgType, fix::TagList<TagLists...>, Tags...>::is_reftagno(const std::string &_key, const std::string &_value, const MapMessage &_mapmsg, size_t &_it)
    {
        if (std::strcmp(TagList::tagno, _key.c_str()) != 0) {
            if constexpr (sizeof...(RemainTagList) != 0)
                return is_reftagno<RemainTagList...>(_key, _value);
            else
                return false;
        }

        TagList &taglist = std::get<TagList>(m_taglists);

        if constexpr (IsOptional<typename TagList::TagNoType::ValueType>) {
            taglist.TagNo.Value = TagValueConvertorOptional<int>(_value);
            if (taglist.TagNo.Value.has_value()) {
                if (taglist.TagNo.Value.value() < 0)
                    throw RejectException("Invalid value for optional No Tag", RejectException::ValueOORange);
                if (taglist.TagNo.Value.value() == 0)
                    taglist.TagNo.Value = std::nullopt;
            }
        } else {
            if (_value.empty())
                throw RejectException("Expected a value for required No Tag", RejectException::ReqTagMissing);
            taglist.TagNo.Value = TagValueConvertor<int>(_value);
            if (taglist.TagNo.Value <= 0)
                throw RejectException("Invalid value for required No Tag", RejectException::ValueOORange);
        }
        taglist.from_string(_mapmsg, ++_it);
        return true;
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class Tag, class ...RemainTag>
    void Message<MsgType, fix::TagList<TagLists...>, Tags...>::verify_required_tag(const std::unordered_set<std::string> &_set)
    {
        if constexpr (!IsOptional<typename Tag::ValueType>)
            if (!_set.contains(Tag::tag))
                throw RejectException("Missing required tag", RejectException::ReqTagMissing);
        if constexpr (sizeof...(RemainTag) > 0)
            verify_required_tag<RemainTag...>(_set);
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class Tag, class ...RemainTag>
    void Message<MsgType, fix::TagList<TagLists...>, Tags...>::to_string_tag(std::stringstream &_stream) const
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

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class TagList, class ...RemainTagList>
    void Message<MsgType, fix::TagList<TagLists...>, Tags...>::to_string_list(std::stringstream &_stream) const
    {
        getList<TagList>().to_string(_stream);
        if constexpr (sizeof...(RemainTagList) > 0)
            to_string_list<RemainTagList...>(_stream);
    }
}