#pragma once

#include <optional>

#include "FIX-Message/RejectError.hpp"

std::optional<fix::RejectError> TagConvertor(const std::string &_value, char &_out);

std::optional<fix::RejectError> TagConvertor(const std::string &_value, int &_out);

std::optional<fix::RejectError> TagConvertor(const std::string &_value, float &_out);

std::optional<fix::RejectError> TagConvertor(const std::string &_value, std::string &_out);

std::optional<fix::RejectError> TagConvertor(const std::string &_value, bool &_out);