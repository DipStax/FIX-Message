#pragma once

#include <optional>

#include "FIX-Message/RejectError.hpp"

std::optional<fix::RejectError> from_FIX(const std::string &_value, char &_out);
void to_FIX(std::string &_out, const char _value);

std::optional<fix::RejectError> from_FIX(const std::string &_value, int &_out);
void to_FIX(std::string &_out, const int _value);

// std::optional<fix::RejectError> from_FIX(const std::string &_value, size_t &_out);
void to_FIX(std::string &_out, const size_t _value);

std::optional<fix::RejectError> from_FIX(const std::string &_value, uint16_t &_out);
void to_FIX(std::string &_out, const uint16_t _value);

std::optional<fix::RejectError> from_FIX(const std::string &_value, float &_out);
void to_FIX(std::string &_out, const float _value);

std::optional<fix::RejectError> from_FIX(const std::string &_value, std::string &_out);
void to_FIX(std::string &_out, const std::string &_value);

std::optional<fix::RejectError> from_FIX(const std::string &_value, bool &_out);
void to_FIX(std::string &_out, const bool _value);