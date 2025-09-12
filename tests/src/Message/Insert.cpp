#include "TagConvertor.hpp"
#include "Tag.hpp"

#include "FIX-Message/Message.hpp"

#include <gtest/gtest.h>

class Message_insert_allrequired : public testing::Test
{
    public:
        using Message = fix::Message<'0',
            fix::Tag<Tag1, int>,
            fix::Tag<Tag2, float>,
            fix::Tag<Tag3, char>,
            fix::Tag<Tag4, bool>,
            fix::Tag<Tag5, std::string>
        >;
};

TEST_F(Message_insert_allrequired, order_entry)
{
    const std::string value1 = "123456789";
    const std::string value2 = "12345.6789";
    const std::string value3 = "c";
    const std::string value4 = "true";
    const std::string value5 = "string";
    fix::MapMessage map_msg{};
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(Tag1Value, value1);
    map_msg.emplace_back(Tag2Value, value2);
    map_msg.emplace_back(Tag3Value, value3);
    map_msg.emplace_back(Tag4Value, value4);
    map_msg.emplace_back(Tag5Value, value5);

    reject = real_message.from_string(map_msg);

    ASSERT_FALSE(reject.has_value());
    EXPECT_EQ(real_message.get<Tag1>().Value, std::stoi(value1));
    EXPECT_EQ(real_message.get<Tag2>().Value, std::stof(value2));
    EXPECT_EQ(real_message.get<Tag3>().Value, value3[0]);
    EXPECT_TRUE(real_message.get<Tag4>().Value);
    EXPECT_EQ(real_message.get<Tag5>().Value, value5);
}

TEST_F(Message_insert_allrequired, not_order_entry)
{
    const std::string value1 = "123456789";
    const std::string value2 = "12345.6789";
    const std::string value3 = "c";
    const std::string value4 = "true";
    const std::string value5 = "string";
    fix::MapMessage map_msg{};
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(Tag5Value, value5);
    map_msg.emplace_back(Tag4Value, value4);
    map_msg.emplace_back(Tag3Value, value3);
    map_msg.emplace_back(Tag2Value, value2);
    map_msg.emplace_back(Tag1Value, value1);

    reject = real_message.from_string(map_msg);

    ASSERT_FALSE(reject.has_value());
    EXPECT_EQ(real_message.get<Tag1>().Value, std::stoi(value1)) << "Expected: " << value1;
    EXPECT_EQ(real_message.get<Tag2>().Value, std::stof(value2)) << "Expected: " << value2;
    EXPECT_EQ(real_message.get<Tag3>().Value, value3[0]) << "Expected: " << value3;
    EXPECT_TRUE(real_message.get<Tag4>().Value);
    EXPECT_EQ(real_message.get<Tag5>().Value, value5);
}
