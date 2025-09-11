#include "TagValueConvertor.hpp"

#include <gtest/gtest.h>

class Parsing_optional : public testing::Test
{
};

TEST_F(Parsing_optional, empty_string)
{
    const std::string value = "";

    try {
        ASSERT_FALSE(fix::TagValueConvertorOptional<int>(value).has_value());
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_optional, valid_value)
{
    const std::string value = "123456789";

    try {
        std::optional<int> result = fix::TagValueConvertorOptional<int>(value);

        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), std::stoi(value));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}

TEST_F(Parsing_optional, invalid_value)
{
    const std::string value = "error";

    try {
        fix::TagValueConvertorOptional<int>(value);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Expected a integer value");
        EXPECT_STREQ(_reject.what(), fix::RejectException::IncorrectFormat);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
}
