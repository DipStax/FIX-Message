#include "TagValueConvertor.hpp"

#include <gtest/gtest.h>

class Parsing_float : public testing::Test
{
};

TEST_F(Parsing_float, valid_positive_nofp)
{
    const std::string value = "123456789";

    try {
        ASSERT_FLOAT_EQ(fix::TagValueConvertor<float>(value), std::stof(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_float, valid_positive_fp)
{
    const std::string value = "12345.6789";

    try {
        ASSERT_FLOAT_EQ(fix::TagValueConvertor<float>(value), std::stof(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_float, valid_negative_nofp)
{
    const std::string value = "-123456789";

    try {
        ASSERT_FLOAT_EQ(fix::TagValueConvertor<float>(value), std::stof(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_float, valid_negative_fp)
{
    const std::string value = "-12345.6789";

    try {
        ASSERT_FLOAT_EQ(fix::TagValueConvertor<float>(value), std::stof(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_float, invalid_string)
{
    const std::string value = "error";

    try {
        fix::TagValueConvertor<float>(value);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Expected a float value");
        EXPECT_STREQ(_reject.what(), fix::RejectException::IncorrectFormat);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_float, invalid_multi_decimal_pt)
{
    const std::string value = "123.456.789";

    try {
        fix::TagValueConvertor<float>(value);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Multiple decimal points");
        EXPECT_STREQ(_reject.what(), fix::RejectException::IncorrectFormat);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}