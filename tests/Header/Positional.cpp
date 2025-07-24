#include "Header.hpp"

#include <gtest/gtest.h>

class Header_positional_insert : public testing::Test
{
    public:
        static constexpr const char Tag1[] = "1";
        static constexpr const char Tag2[] = "2";
        static constexpr const char Tag3[] = "3";
        static constexpr const char Tag4[] = "4";

        using Header = fix::Header<
            fix::PositionalTag<
                fix::Tag<Tag1, std::string>,
                fix::Tag<Tag2, std::string>,
                fix::Tag<Tag3, int>
            >
        >;
};

TEST_F(Header_positional_insert, correct_order)
{
    const std::string value1 = "string1";
    const std::string value2 = "string2";
    const std::string value3 = "123456789";

    Header header{};

    try {
        ASSERT_TRUE(header.try_insert(Tag1, value1));
        ASSERT_TRUE(header.try_insert(Tag2, value2));
        ASSERT_TRUE(header.try_insert(Tag3, value3));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(header.getPositional<Tag1>().Value, value1);
    EXPECT_EQ(header.getPositional<Tag2>().Value, value2);
    EXPECT_EQ(header.getPositional<Tag3>().Value, std::stoi(value3));
}

TEST_F(Header_positional_insert, wrong_order_first)
{
    const std::string value1 = "string1";

    Header header{};

    try {
        // fix the void result
        (void)header.try_insert(Tag2, value1);
    } catch (const fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Invalid positional tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::InvalidTag);
        SUCCEED();
        return;
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an exception";
}

TEST_F(Header_positional_insert, wrong_order_second)
{
    const std::string value1 = "string1";
    const std::string value2 = "string2";

    Header header{};

    try {
        ASSERT_TRUE(header.try_insert(Tag1, value1));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    try {
        // fix the void result
        (void)header.try_insert(Tag3, value2);
    } catch (const fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Invalid positional tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::InvalidTag);
        SUCCEED();
        return;
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an exception";
}

TEST_F(Header_positional_insert, insert_end)
{
    const std::string value1 = "string1";
    const std::string value2 = "string2";
    const std::string value3 = "123456789";
    const std::string value4 = "end";

    Header header{};

    try {
        ASSERT_TRUE(header.try_insert(Tag1, value1));
        ASSERT_TRUE(header.try_insert(Tag2, value2));
        ASSERT_TRUE(header.try_insert(Tag3, value3));
        ASSERT_FALSE(header.try_insert(Tag4, value4));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(header.getPositional<Tag1>().Value, value1);
    EXPECT_EQ(header.getPositional<Tag2>().Value, value2);
    EXPECT_EQ(header.getPositional<Tag3>().Value, std::stoi(value3));
}
