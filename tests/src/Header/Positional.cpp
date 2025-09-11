#include "TagConvertor.hpp"
#include "RejectError.hpp"

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

        Header header{};

        static constexpr const char Value1[] = "string1";
        static constexpr const char Value2[] = "string2";
        static constexpr const char Value3[] = "123456789";
};

TEST_F(Header_positional_insert, correct_order)
{
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag1, Value1);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag2, Value2);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag3, Value3);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    EXPECT_EQ(header.getPositional<Tag1>().Value, Value1);
    EXPECT_EQ(header.getPositional<Tag2>().Value, Value2);
    EXPECT_EQ(header.getPositional<Tag3>().Value, std::stoi(Value3));
}

TEST_F(Header_positional_insert, wrong_order_first)
{
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag2, Value1);

    ASSERT_FALSE(expected.has_value());
    EXPECT_STREQ(expected.error().Message.c_str(), "Invalid positional tag");
    EXPECT_STREQ(expected.error().Reason.c_str(), fix::RejectError::InvalidTag);
}

TEST_F(Header_positional_insert, wrong_order_second)
{
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag1, Value1);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag3, Value2);

    ASSERT_FALSE(expected.has_value());
    EXPECT_STREQ(expected.error().Message.c_str(), "Invalid positional tag");
    EXPECT_STREQ(expected.error().Reason.c_str(), fix::RejectError::InvalidTag);
}

TEST_F(Header_positional_insert, insert_end)
{
    const std::string value4 = "end";
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag1, Value1);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag2, Value2);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag3, Value3);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag4, value4);

    ASSERT_TRUE(expected.has_value());
    ASSERT_FALSE(expected.value());

    EXPECT_EQ(header.getPositional<Tag1>().Value, Value1);
    EXPECT_EQ(header.getPositional<Tag2>().Value, Value2);
    EXPECT_EQ(header.getPositional<Tag3>().Value, std::stoi(Value3));
}
