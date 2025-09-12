#include "TagConvertor.hpp"
#include "Tag.hpp"

#include "FIX-Message/RejectError.hpp"
#include "FIX-Message/Header.hpp"

#include <gtest/gtest.h>

class Header_positional_insert : public testing::Test
{
    public:
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
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag1Value, Value1);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag2Value, Value2);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag3Value, Value3);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    EXPECT_EQ(header.getPositional<Tag1>().Value, Value1);
    EXPECT_EQ(header.getPositional<Tag2>().Value, Value2);
    EXPECT_EQ(header.getPositional<Tag3>().Value, std::stoi(Value3));
}

TEST_F(Header_positional_insert, wrong_order_first)
{
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag2Value, Value1);

    ASSERT_FALSE(expected.has_value());
    EXPECT_STREQ(expected.error().Message.c_str(), "Invalid positional tag");
    EXPECT_EQ(expected.error().Reason, fix::RejectError::InvalidTag);
    EXPECT_EQ(expected.error().Tag, Tag1);
}

TEST_F(Header_positional_insert, wrong_order_second)
{
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag1Value, Value1);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag3Value, Value2);

    ASSERT_FALSE(expected.has_value());
    EXPECT_STREQ(expected.error().Message.c_str(), "Invalid positional tag");
    EXPECT_EQ(expected.error().Reason, fix::RejectError::InvalidTag);
    EXPECT_EQ(expected.error().Tag, Tag2);
}

TEST_F(Header_positional_insert, insert_end)
{
    const std::string value4 = "end";
    xstd::Expected<bool, fix::RejectError> expected = header.try_insert(Tag1Value, Value1);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag2Value, Value2);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag3Value, Value3);

    ASSERT_TRUE(expected.has_value());
    ASSERT_TRUE(expected.value());

    expected = header.try_insert(Tag4Value, value4);

    ASSERT_TRUE(expected.has_value());
    ASSERT_FALSE(expected.value());

    EXPECT_EQ(header.getPositional<Tag1>().Value, Value1);
    EXPECT_EQ(header.getPositional<Tag2>().Value, Value2);
    EXPECT_EQ(header.getPositional<Tag3>().Value, std::stoi(Value3));
}
