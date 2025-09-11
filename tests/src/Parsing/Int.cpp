#include "TagValueConvertor.hpp"

#include <gtest/gtest.h>

class Parsing_int : public testing::Test
{
};

TEST_F(Parsing_int, valid_positive)
{
    const std::string value = "123456789";

    try {
        ASSERT_EQ(fix::TagValueConvertor<int>(value), std::stoi(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_int, valid_negative)
{
    const std::string value = "-123456789";

    try {
        ASSERT_EQ(fix::TagValueConvertor<int>(value), std::stoi(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_int, invalid)
{
    const std::string value = "error";

    try {
        fix::TagValueConvertor<int>(value);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Expected a integer value");
        EXPECT_STREQ(_reject.what(), fix::RejectException::IncorrectFormat);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}