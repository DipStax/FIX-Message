#include <charconv>

#include "TagConvertor.hpp"
#include "FIX-Message/meta/limit.hpp"

std::optional<fix::RejectError> from_FIX(const std::string &_value, char &_out)
{
    if (_value.size() > 1)
        return fix::RejectError{ fix::RejectError::ValueOORange, "Expected a char" };
    _out = _value[0];
    return std::nullopt;
}

void to_FIX(std::string &_out, const char _value)
{
    _out.push_back(_value);
}

std::optional<fix::RejectError> from_FIX(const std::string &_value, int &_out)
{
    auto [ptr, ec] = std::from_chars(_value.data(), _value.data() + _value.size(), _out);

    if (ec == std::errc() && ptr == _value.data() + _value.size())
        return std::nullopt;
    if (ec == std::errc::invalid_argument)
        return fix::RejectError{ fix::RejectError::IncorrectFormat, "Expected an int" };
    else if (ec == std::errc::result_out_of_range)
        return fix::RejectError{ fix::RejectError::ValueOORange, "Value out of range of int" };
    return fix::RejectError{ fix::RejectError::IncorrectFormat, "Unknow error will parsing int" };
}

void to_FIX(std::string &_out, const int _value)
{
    char buffer[fix::meta::NumericLimit::MaxAlloc<int>()];
    auto [ptr, _] = std::to_chars(buffer, buffer + sizeof(buffer), _value);

    _out.append(buffer, ptr);
}

void to_FIX(std::string &_out, const size_t _value)
{
    char buffer[fix::meta::NumericLimit::MaxAlloc<size_t>()];
    auto [ptr, _] = std::to_chars(buffer, buffer + sizeof(buffer), _value);

    _out.append(buffer, ptr);
}

std::optional<fix::RejectError> from_FIX(const std::string &_value, float &_out)
{
    auto [ptr, ec] = std::from_chars(_value.data(), _value.data() + _value.size(), _out);

    if (ec == std::errc() && ptr == _value.data() + _value.size())
        return std::nullopt;
    if (ec == std::errc::invalid_argument)
        return fix::RejectError{ fix::RejectError::IncorrectFormat, "Expected an float" };
    else if (ec == std::errc::result_out_of_range)
        return fix::RejectError{ fix::RejectError::ValueOORange, "Value out of range of float" };
    return fix::RejectError{ fix::RejectError::IncorrectFormat, "Unknow error will parsing float" };
}


void to_FIX(std::string &_out, const float _value)
{
    char buffer[fix::meta::NumericLimit::MaxAlloc<float, 3>()];
    auto [ptr, _] = std::to_chars(buffer, buffer + sizeof(buffer), _value, std::chars_format::fixed, 3);

    _out.append(buffer, ptr);
}

std::optional<fix::RejectError> from_FIX(const std::string &_value, std::string &_out)
{
    _out = _value;
    return std::nullopt;
}

void to_FIX(std::string &_out, const std::string &_value)
{
    _out.append(_value);
}

std::optional<fix::RejectError> from_FIX(const std::string &_value, bool &_out)
{
    if (_value == "1" || _value == "true") {
        _out = true;
        return std::nullopt;
    }
    if (_value == "0" || _value == "false") {
        _out = false;
        return std::nullopt;
    }
    return fix::RejectError{ fix::RejectError::IncorrectFormat, "Expected a boolean value" };
}

void to_FIX(std::string &_out, const bool _value)
{
    _out.push_back(_value ? '1' : '0');
}