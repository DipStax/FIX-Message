#include "Message.hpp"

#include <gtest/gtest.h>

class Message_insert_allrequired : public testing::Test
{
    public:
        static constexpr const char Tag1[] = "1";
        static constexpr const char Tag2[] = "2";
        static constexpr const char Tag3[] = "3";
        static constexpr const char Tag4[] = "4";
        static constexpr const char Tag5[] = "5";

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

    map_msg.emplace_back(Tag1, value1);
    map_msg.emplace_back(Tag2, value2);
    map_msg.emplace_back(Tag3, value3);
    map_msg.emplace_back(Tag4, value4);
    map_msg.emplace_back(Tag5, value5);

    try {
        real_message.from_string(map_msg);
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(real_message.get<Tag1>().Value, std::stoi(value1)) << "Expected: " << value1;
    EXPECT_EQ(real_message.get<Tag2>().Value, std::stof(value2)) << "Expected: " << value2;
    EXPECT_EQ(real_message.get<Tag3>().Value, value3[0]) << "Expected: " << value3;
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

    map_msg.emplace_back(Tag5, value5);
    map_msg.emplace_back(Tag4, value4);
    map_msg.emplace_back(Tag3, value3);
    map_msg.emplace_back(Tag2, value2);
    map_msg.emplace_back(Tag1, value1);

    try {
        real_message.from_string(map_msg);
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    EXPECT_EQ(real_message.get<Tag1>().Value, std::stoi(value1)) << "Expected: " << value1;
    EXPECT_EQ(real_message.get<Tag2>().Value, std::stof(value2)) << "Expected: " << value2;
    EXPECT_EQ(real_message.get<Tag3>().Value, value3[0]) << "Expected: " << value3;
    EXPECT_TRUE(real_message.get<Tag4>().Value);
    EXPECT_EQ(real_message.get<Tag5>().Value, value5);
}
