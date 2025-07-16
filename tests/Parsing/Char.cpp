#include "TagValueConvertor.hpp"

#include <gtest/gtest.h>

class Parsing_char : public testing::Test
{
};

TEST_F(Parsing_char, valid)
{
    const std::string value = "c";

    try {
        ASSERT_EQ(fix::TagValueConvertor<char>(value), value[0]) << "Expected: " << value;
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}


TEST_F(Parsing_char, invalid)
{
    const std::string value = "error";

    try {
        fix::TagValueConvertor<char>(value);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Expected a char value");
        EXPECT_STREQ(_reject.what(), fix::RejectException::IncorrectFormat);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}
