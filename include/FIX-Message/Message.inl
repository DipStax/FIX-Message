#include <sstream>
#include <iomanip>
#include <iostream>

#include "Message.hpp"

namespace fix
{
    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    std::optional<RejectError> Message<MsgType, fix::TagList<TagLists...>, Tags...>::from_string(const MapMessage &_mapmsg)
    {
        std::unordered_set<TagName> set_tag{};
        std::optional<RejectError> reject;

        for (size_t it = 0; it < _mapmsg.size(); it++) {
            const std::pair<std::string, std::string> &pair = _mapmsg.at(it);
            const std::string &key = pair.first;
            const std::string &value = pair.second;
            TagName keytag = 0;

            if (key.empty())
                return RejectError{ RejectError::InvalidTag, "Tag is empty" };
            if (!std::all_of(key.begin(), key.end(), [] (char _c) { return std::isdigit(_c); }))
                return RejectError{ RejectError::InvalidTag, "Tag should be numeric" };
            keytag = std::stoi(key);

            if constexpr (sizeof...(Tags) > 0) {
                xstd::Expected<bool, RejectError> error = try_insert<Tags...>(keytag, value);

                if (error.has_value()) {
                    if (!error.value()) {
                        reject = try_insert_tagno(keytag, value, _mapmsg, it);
                        if (reject.has_value())
                            return reject.value();
                    } else {
                        if (set_tag.contains(keytag))
                            return RejectError{ RejectError::InvalidTag, "Duplicate tag value", keytag };
                        set_tag.emplace(keytag);
                    }
                } else {
                    return error.error();
                }
            } else {
                reject = try_insert_tagno(keytag, value, _mapmsg, it);
                if (reject.has_value())
                    return reject.value();
            }
        }
        if constexpr (sizeof...(Tags) > 0)
            return verify_required_tag<Tags...>(set_tag);
        return std::nullopt;
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
    xstd::Expected<bool, RejectError> Message<MsgType, fix::TagList<TagLists...>, Tags...>::try_insert(TagName _key, const std::string &_value)
    {
        if (Tag::tag == _key) {
            std::optional<RejectError> error;

            if constexpr (IsOptional<typename Tag::ValueType>) {
                if (_value.empty()) {
                    get<Tag::tag>().Value = std::nullopt;
                } else {
                    typename Tag::ValueType::value_type value;

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
        if constexpr (sizeof...(RemainTag) != 0) {
            return try_insert<RemainTag...>(_key, _value);
        } else {
            return false;
        }
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    std::optional<RejectError> Message<MsgType, fix::TagList<TagLists...>, Tags...>::try_insert_tagno(TagName _key, const std::string &_value, const MapMessage &_mapmsg, size_t &_it)
    {
        if constexpr (sizeof...(TagLists) > 0) {
            xstd::Expected<bool, RejectError> error = is_reftagno<TagLists...>(_key, _value, _mapmsg, _it);

            if (error.has_value()) {
                if (!error.value())
                    return RejectError{ RejectError::UndefineTag, "Unknown tag", _key };
            } else {
                return error.error();
            }
            _it--;
        } else {
            return RejectError{ RejectError::UndefineTag, "Unknown tag", _key };
        }
        return std::nullopt;
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class TagList, class ...RemainTagList>
    xstd::Expected<bool, RejectError> Message<MsgType, fix::TagList<TagLists...>, Tags...>::is_reftagno(TagName _key, const std::string &_value, const MapMessage &_mapmsg, size_t &_it)
    {
        if (TagList::tagno != _key) {
            if constexpr (sizeof...(RemainTagList) != 0)
                return is_reftagno<RemainTagList...>(_key, _value);
            else
                return false;
        }

        TagList &taglist = std::get<TagList>(m_taglists);
        std::optional<RejectError> error = std::nullopt;

        if constexpr (IsOptional<typename TagList::TagNoType::ValueType>) {
            if (_value.empty()) {
                taglist.TagNo.Value = std::nullopt;
            } else {
                typename TagList::TagNoType::ValueType::value_type value;

                error = TagConvertor(_value, value);
                if (!error.has_value())
                    taglist.TagNo.Value = value;
            }
        } else {
            if (_value.empty())
               return xstd::Unexpected<RejectError>({ RejectError::ReqTagMissing, "Expected a value for required No Tag", TagList::tagno });
            error = TagConvertor(_value, taglist.TagNo.Value);
        }
        if (error.has_value()) {
            error.value().Tag = TagList::tagno;
            return xstd::Unexpected<RejectError>(error.value());
        }
        if constexpr (IsOptional<typename TagList::TagNoType::ValueType>) {
            if (taglist.TagNo.Value.has_value()) {
                if (taglist.TagNo.Value.value() < 0)
                    return xstd::Unexpected<RejectError>({ RejectError::ValueOORange, "Invalid value for optional No Tag", TagList::tagno });
                if (taglist.TagNo.Value.value() == 0)
                    taglist.TagNo.Value = std::nullopt;
            }
        } else {
            if (taglist.TagNo.Value <= 0)
                return xstd::Unexpected<RejectError>({ RejectError::ValueOORange, "Invalid value for required No Tag", TagList::tagno });
        }
        error = taglist.from_string(_mapmsg, ++_it);
        if (error.has_value())
            return xstd::Unexpected<RejectError>(error.value());
        return true;
    }

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    template<class Tag, class ...RemainTag>
    std::optional<RejectError> Message<MsgType, fix::TagList<TagLists...>, Tags...>::verify_required_tag(const std::unordered_set<TagName> &_set)
    {
        if constexpr (!IsOptional<typename Tag::ValueType>)
            if (!_set.contains(Tag::tag))
                return RejectError{ RejectError::ReqTagMissing, "Missing required tag", Tag::tag };
        if constexpr (sizeof...(RemainTag) > 0)
            return verify_required_tag<RemainTag...>(_set);
        return std::nullopt;
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