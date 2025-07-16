#include "TagValueConvertor.hpp"

#include <gtest/gtest.h>

class Parsing_string : public testing::Test
{
};

TEST_F(Parsing_string, valid)
{
    const std::string value = "this is a string";

    try {
        ASSERT_EQ(fix::TagValueConvertor<std::string>(value), value);
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
}