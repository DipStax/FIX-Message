#include "TagConvertor.hpp"

#include "Message.hpp"

#include <gtest/gtest.h>

class Group_required : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag[] = "2";

        using Message = fix::Message<'0',
            fix::TagList<
                fix::List<
                    fix::TagNo<TagNo, false>,
                    fix::Tag<Tag, int>
                >
            >
        >;
};

TEST_F(Group_required, invalid_empty)
{
    const std::string value = "";
    fix::MapMessage map_msg;
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Expected a value for required No Tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::ReqTagMissing);
    EXPECT_STREQ(reject.value().Tag.c_str(), TagNo);
}

TEST_F(Group_required, invalid_zero)
{
    const std::string value = "0";
    fix::MapMessage map_msg;
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Invalid value for required No Tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::ValueOORange);
    EXPECT_STREQ(reject.value().Tag.c_str(), TagNo);
}

TEST_F(Group_required, invalid_negative)
{
    const std::string value = "-1";
    fix::MapMessage map_msg;
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Invalid value for required No Tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::ValueOORange);
    EXPECT_STREQ(reject.value().Tag.c_str(), TagNo);
}

class Group_optional : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag[] = "2";

        using Message = fix::Message<'0',
            fix::TagList<
                fix::List<
                    fix::TagNo<TagNo, true>,
                    fix::Tag<Tag, int>
                >
            >
        >;
};

TEST_F(Group_optional, valid_tagno_zero)
{
    const std::string value = "0";
    fix::MapMessage map_msg{};
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value);
    reject = real_message.from_string(map_msg);

    ASSERT_FALSE(reject.has_value());
    ASSERT_FALSE(real_message.getList<TagNo>().TagNo.Value.has_value());
}


TEST_F(Group_optional, valid_tagno_empty)
{
    const std::string value = "";
    fix::MapMessage map_msg{};
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value);
    reject = real_message.from_string(map_msg);

    ASSERT_FALSE(reject.has_value());
    ASSERT_FALSE(real_message.getList<TagNo>().TagNo.Value.has_value());
}

TEST_F(Group_optional, invalid_negative)
{
    const std::string value = "-1";
    fix::MapMessage map_msg{};
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Invalid value for optional No Tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::ValueOORange);
    EXPECT_STREQ(reject.value().Tag.c_str(), TagNo);
}