#pragma once

#include <string>
#include <iterator>
#include <optional>

#include "Exception.hpp"

namespace fix
{
    template<class T>
    T TagValueConvertor(const std::string &_value);

    template<>
    inline char TagValueConvertor(const std::string &_value);

    template<>
    inline int TagValueConvertor(const std::string &_value);

    template<>
    inline float TagValueConvertor(const std::string &_value);

    template<>
    inline std::string TagValueConvertor(const std::string &_value);

    template<>
    inline bool TagValueConvertor(const std::string &_value);

    template<class T>
    std::optional<T> TagValueConvertorOptional(const std::string &_value);
}

#include "TagValueConvertor.inl"