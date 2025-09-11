#include "TagConvertor.hpp"

std::optional<fix::RejectError> TagConvertor(const std::string &_value, char &_out)
{
    if (_value.size() != 1)
        return fix::RejectError{ fix::RejectError::IncorrectFormat, "Expected a char value" };
    _out = _value[0];
    return std::nullopt;
}

std::optional<fix::RejectError> TagConvertor(const std::string &_value, int &_out)
{
    std::string::const_iterator it = _value.begin();

    if (*it == '-')
        it++;
    for (; it != _value.end(); it++)
        if (!std::isdigit(*it))
            return fix::RejectError{ fix::RejectError::IncorrectFormat, "Expected a integer value" };
    _out = std::stoi(_value);
    return std::nullopt;
}

std::optional<fix::RejectError> TagConvertor(const std::string &_value, float &_out)
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
                    return fix::RejectError{ fix::RejectError::IncorrectFormat, "Multiple decimal points" };
                floating_point = true;
                continue;
            } else {
                return fix::RejectError{ fix::RejectError::IncorrectFormat, "Expected a float value" };
            }
        }
    }
    _out = std::stof(_value);
    return std::nullopt;
}

std::optional<fix::RejectError> TagConvertor(const std::string &_value, std::string &_out)
{
    _out = _value;
    return std::nullopt;
}

std::optional<fix::RejectError> TagConvertor(const std::string &_value, bool &_out)
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