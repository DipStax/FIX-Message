#include "Tag.hpp"

#include "FIX-Message/Message.hpp"

#include <gtest/gtest.h>

#include <format>

class Message_serialization_nooptional : public testing::Test
{
    public:
        using Message = fix::Message<'0',
            fix::Tag<Tag1, std::string>,
            fix::Tag<Tag2, int>,
            fix::Tag<Tag3, float>,
            fix::Tag<Tag4, bool>,
            fix::Tag<Tag5, char>
        >;
};

TEST_F(Message_serialization_nooptional, normal)
{
    const std::string value1 = "string";
    const int value2 = 123456789;
    const float value3 = 12345.6789;
    const bool value4 = true;
    const char value5 = 'c';

    Message msg{};

    msg.get<Tag1>().Value = value1;
    msg.get<Tag2>().Value = value2;
    msg.get<Tag3>().Value = value3;
    msg.get<Tag4>().Value = value4;
    msg.get<Tag5>().Value = value5;

    const std::string result = std::format("{}={}\1", Tag1, value1)
        + std::format("{}={}\1", Tag2, value2)
        + std::format("{}={:.4f}\1", Tag3, value3)
        + std::format("{}={}\1", Tag4, static_cast<int>(value4))
        + std::format("{}={}\1", Tag5, value5);

    EXPECT_EQ(msg.to_string(), result);
}

class Message_serialization_optional : public testing::Test
{
    public:
        using Message = fix::Message<'0',
            fix::Tag<Tag1, std::string>,
            fix::Tag<Tag2, int>,
            fix::Tag<Tag3, std::optional<float>>,
            fix::Tag<Tag4, std::optional<bool>>,
            fix::Tag<Tag5, char>
        >;

        Message msg{};
};

TEST_F(Message_serialization_optional, no_empty_value)
{
    const std::string value1 = "string";
    const int value2 = 123456789;
    const float value3 = 12345.6789;
    const bool value4 = true;
    const char value5 = 'c';

    msg.get<Tag1>().Value = value1;
    msg.get<Tag2>().Value = value2;
    msg.get<Tag3>().Value = value3;
    msg.get<Tag4>().Value = value4;
    msg.get<Tag5>().Value = value5;

    const std::string result = std::format("{}={}\1", Tag1, value1)
        + std::format("{}={}\1", Tag2, value2)
        + std::format("{}={:.4f}\1", Tag3, value3)
        + std::format("{}={}\1", Tag4, static_cast<int>(value4))
        + std::format("{}={}\1", Tag5, value5);

    EXPECT_EQ(msg.to_string(), result);
}

TEST_F(Message_serialization_optional, all_empty_value)
{
    const std::string value1 = "string";
    const int value2 = 123456789;
    const char value5 = 'c';

    msg.get<Tag1>().Value = value1;
    msg.get<Tag2>().Value = value2;
    msg.get<Tag5>().Value = value5;

    const std::string result = std::format("{}={}\1", Tag1, value1)
        + std::format("{}={}\1", Tag2, value2)
        + std::format("{}={}\1", Tag5, value5);

    EXPECT_EQ(msg.to_string(), result);
}

TEST_F(Message_serialization_optional, mix_value)
{
    const std::string value1 = "string";
    const int value2 = 123456789;
    const bool value4 = true;
    const char value5 = 'c';

    msg.get<Tag1>().Value = value1;
    msg.get<Tag2>().Value = value2;
    msg.get<Tag4>().Value = value4;
    msg.get<Tag5>().Value = value5;

    const std::string result = std::format("{}={}\1", Tag1, value1)
        + std::format("{}={}\1", Tag2, value2)
        + std::format("{}={}\1", Tag4, static_cast<int>(value4))
        + std::format("{}={}\1", Tag5, value5);

    EXPECT_EQ(msg.to_string(), result);
}