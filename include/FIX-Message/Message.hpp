#pragma once

#include "List.hpp"

namespace fix
{
    template<fix::MessageType MsgType, class ...Tags>
    class Message;

    template<fix::MessageType MsgType, class ...Tags>
    class Message : public Message<MsgType, fix::TagList<>, Tags...>
    {
    };

    template<fix::MessageType MsgType, class ...TagLists, class ...Tags>
    class Message<MsgType, fix::TagList<TagLists...>, Tags...>
    {
        public:
            std::optional<RejectError> from_string(const MapMessage &_mapmsg);

            [[nodiscard]] std::string to_string() const;

            template<TagName Name>
                requires IsInListOf_Tag<Name, Tags...>
            [[nodiscard]] const Tag<Name, meta::find_tagtype_t<Name, Tags...>> &get() const;
            template<TagName Name>
                requires IsInListOf_Tag<Name, Tags...>
            [[nodiscard]] Tag<Name, meta::find_tagtype_t<Name, Tags...>> &get();

            template<TagName Name>
                requires IsInListOf_TagList<Name, TagLists...>
            [[nodiscard]] const meta::find_taglist_from_name_t<Name, TagLists...> &getList() const;
            template<TagName Name>
                requires IsInListOf_TagList<Name, TagLists...>
            [[nodiscard]] meta::find_taglist_from_name_t<Name, TagLists...> &getList();

        private:
            template<class Tag, class ...RemainTag>
            [[nodiscard]] inline xstd::Expected<bool, RejectError> try_insert(TagName _key, const std::string &_value);
            [[nodiscard]] std::optional<RejectError> try_insert_tagno(TagName _key, const std::string &_value, const MapMessage &_mapmsg, size_t &_it);
            template<class TagList, class ...RemainTagList>
            [[nodiscard]] inline xstd::Expected<bool, RejectError> is_reftagno(TagName _key, const std::string &_value, const MapMessage &_mapmsg, size_t &_it);
            template<class Tag, class ...RemainTag>
            [[nodiscard]] inline static std::optional<RejectError> verify_required_tag(const std::unordered_set<TagName> &_set);

            template<class Tag, class ...RemainTag>
            inline void to_string_tag(std::stringstream &_stream) const;
            template<class TagList, class ...RemainTagList>
            inline void to_string_list(std::stringstream &_stream) const;

            std::tuple<Tags...> m_tags;
            std::tuple<TagLists...> m_taglists;
    };
}

#include "Message.inl"