#pragma once

#include <unordered_set>

#include "meta/concept.hpp"
#include "Tag.hpp"

namespace fix
{
    template<class RefTagNo, class ...Tags>
    class List
    {
        public:
            // add static assert to check if their is at least one mandatory tag

            static constexpr fix::TagName tagno = RefTagNo::tag;

            using DataTuple = std::tuple<Tags...>;
            using TagNoType = RefTagNo;

            using DataContainer = std::vector<DataTuple>;
            using iterator = DataContainer::iterator;
            using const_iterator = DataContainer::const_iterator;

            TagNoType TagNo;

            void from_string(const MapMessage &_mapmsg, size_t &_pos);

            void to_string(std::stringstream &_stream);

            void add(DataTuple &&_data);

            [[nodiscard]] constexpr const DataTuple &at(size_t _idx) const;
            [[nodiscard]] constexpr DataTuple &at(size_t _idx);

            [[nodiscard]] constexpr size_t size() const;

            [[nodiscard]] iterator begin();
            [[nodiscard]] iterator end();

            [[nodiscard]] const_iterator cbegin() const;
            [[nodiscard]] const_iterator cend() const;

        private:
            void internal_from_string(const MapMessage &_mapmsg, size_t &_pos);
            template<class Tag, class ...RemainTag>
            inline bool try_insert(DataTuple &_tuple, const std::string &_key, const std::string _value);
            template<class Tag, class ...RemainTag>
            inline static void verify_required_tag(const std::unordered_set<std::string> &_set);

            template<class Tag, class ...RemainTag>
            inline void to_string_tag(std::stringstream &_stream, const DataTuple &_tuple);

            DataContainer m_data;
    };

    template<fix::TagName Name, IsTagTuple DataTuple>
        requires IsInTagTuple<Name, DataTuple>
    [[nodiscard]] constexpr const meta::find_tag_from_name_in_tuple_t<Name, DataTuple> &get(const DataTuple &_data);
    template<fix::TagName Name, IsTagTuple DataTuple>
        requires IsInTagTuple<Name, DataTuple>
    [[nodiscard]] constexpr meta::find_tag_from_name_in_tuple_t<Name, DataTuple> &get(DataTuple &_data);
}

#include "List.inl"