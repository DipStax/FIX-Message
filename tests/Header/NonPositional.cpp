#include "Header.hpp"

#include <gtest/gtest.h>

class Header_nonpositional_insert : public testing::Test
{
    public:
        static constexpr const char Tag1[] = "1";
        static constexpr const char Tag2[] = "2";
        static constexpr const char Tag3[] = "3";
        static constexpr const char Tag4[] = "4";

        using Header = fix::Header<
            fix::PositionalTag<
                fix::Tag<Tag1, std::string>
            >,
            fix::Tag<Tag2, std::string>,
            fix::Tag<Tag3, int>
        >;

        static constexpr const char Value1[] = "string1";
        static constexpr const char Value2[] = "string2";
        static constexpr const char Value3[] = "123456789";

        Header header{};

        void SetUp() override
        {
            ASSERT_TRUE(header.try_insert(Tag1, Value1));
            ASSERT_EQ(header.getPositional<Tag1>().Value, Value1);
        }
};

TEST_F(Header_nonpositional_insert, valid)
{
    try {
        ASSERT_TRUE(header.try_insert(Tag2, Value2));
        ASSERT_TRUE(header.try_insert(Tag3, Value3));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.reason() << ": " << _reject.what();
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(header.get<Tag2>().Value, Value2);
    EXPECT_EQ(header.get<Tag3>().Value, std::stoi(Value3));
}

TEST_F(Header_nonpositional_insert, valid_not_order)
{
    try {
        ASSERT_TRUE(header.try_insert(Tag3, Value3));
        ASSERT_TRUE(header.try_insert(Tag2, Value2));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.reason() << ": " << _reject.what();
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(header.get<Tag2>().Value, Value2);
    EXPECT_EQ(header.get<Tag3>().Value, std::stoi(Value3));
}

TEST_F(Header_nonpositional_insert, insert_end)
{
    const std::string value4 = "end";

    try {
        ASSERT_TRUE(header.try_insert(Tag3, Value3));
        ASSERT_TRUE(header.try_insert(Tag2, Value2));
        ASSERT_FALSE(header.try_insert(Tag4, value4));
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.reason() << ": " << _reject.what();
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(header.get<Tag2>().Value, Value2);
    EXPECT_EQ(header.get<Tag3>().Value, std::stoi(Value3));
}

