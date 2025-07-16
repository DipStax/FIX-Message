#pragma once

#include <string>
#include <iterator>
#include <optional>

#include "Exception.hpp"
#include "TagValueConvertor.hpp"

namespace fix
{
    template<class T>
    T TagValueConvertor(const std::string &_value)
    {
        static_assert(false, "Not supported type");
    }

    template<>
    char TagValueConvertor(const std::string &_value)
    {
        if (_value.size() != 1)
            throw RejectException("Expected a char value", RejectException::IncorrectFormat);
        return _value[0];
    }

    template<>
    int TagValueConvertor(const std::string &_value)
    {
        std::string::const_iterator it = _value.begin();

        if (*it == '-')
            it++;
        for (; it != _value.end(); it++)
            if (!std::isdigit(*it))
                throw RejectException("Expected a integer value", RejectException::IncorrectFormat);
        return std::stoi(_value);
    }

    template<>
    float TagValueConvertor(const std::string &_value)
    {
        std::string::const_iterator it = _value.begin();
        std::string::const_iterator end = _value.end();
        bool floating_point = false;

        if (*it == '-')
            it++;

        for (; it != end; it++) {
            if (!std::isdigit(*it)) {
                if (*it == '.') {
                    if (floating_point)
                        throw RejectException("Multiple decimal points", RejectException::IncorrectFormat);
                    floating_point = true;
                    continue;
                } else {
                    throw RejectException("Expected a float value", RejectException::IncorrectFormat);
                }
            }
        }
        return std::stof(_value);
    }

    template<>
    std::string TagValueConvertor(const std::string &_value)
    {
        return _value;
    }

    template<>
    bool TagValueConvertor(const std::string &_value)
    {
        if (_value == "1" || _value == "true")
            return true;
        if (_value == "0" || _value == "false")
            return false;
        throw RejectException("Expected a boolean value", RejectException::IncorrectFormat);
    }

    template<class T>
    std::optional<T> TagValueConvertorOptional(const std::string &_value)
    {
        if (_value.empty())
            return std::nullopt;
        return TagValueConvertor<T>(_value);
    }
}