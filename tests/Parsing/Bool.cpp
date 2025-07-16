#include "TagValueConvertor.hpp"

#include <gtest/gtest.h>

class Parsing_bool : public testing::Test
{
};

TEST_F(Parsing_bool, valid_true_as_num)
{
    const std::string value = "1";

    try {
        ASSERT_TRUE(fix::TagValueConvertor<bool>(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_bool, valid_true_as_string)
{
    const std::string value = "true";

    try {
        ASSERT_TRUE(fix::TagValueConvertor<bool>(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_bool, valid_false_as_num)
{
    const std::string value = "0";

    try {
        ASSERT_FALSE(fix::TagValueConvertor<bool>(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_bool, valid_false_as_string)
{
    const std::string value = "false";

    try {
        ASSERT_FALSE(fix::TagValueConvertor<bool>(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_bool, invalid)
{
    const std::string value = "error";

    try {
        fix::TagValueConvertor<bool>(value);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Expected a boolean value");
        EXPECT_STREQ(_reject.what(), fix::RejectException::IncorrectFormat);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}