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

    map_msg.emplace_back(TagNo, value);
    try {
        real_message.from_string(map_msg);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Expected a value for required No Tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::ReqTagMissing);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}

TEST_F(Group_required, invalid_zero)
{
    const std::string value = "0";
    fix::MapMessage map_msg;
    Message real_message{};

    map_msg.emplace_back(TagNo, value);
    try {
        real_message.from_string(map_msg);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Invalid value for required No Tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::ValueOORange);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}

TEST_F(Group_required, invalid_negative)
{
    const std::string value = "-1";
    fix::MapMessage map_msg;
    Message real_message{};

    map_msg.emplace_back(TagNo, value);
    try {
        real_message.from_string(map_msg);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Invalid value for required No Tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::ValueOORange);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
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

    map_msg.emplace_back(TagNo, value);
    try {
        real_message.from_string(map_msg);
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    ASSERT_FALSE(real_message.getList<TagNo>().TagNo.Value.has_value());
}


TEST_F(Group_optional, valid_tagno_empty)
{
    const std::string value = "";
    fix::MapMessage map_msg{};
    Message real_message{};

    map_msg.emplace_back(TagNo, value);
    try {
        real_message.from_string(map_msg);
    } catch (const fix::RejectException &_reject) {
        FAIL() << _reject.what() << ": " << _reject.reason();
    } catch (const std::exception &_exception) {
        FAIL() << _exception.what();
    }
    ASSERT_FALSE(real_message.getList<TagNo>().TagNo.Value.has_value());
}

TEST_F(Group_optional, invalid_negative)
{
    const std::string value = "-1";
    fix::MapMessage map_msg{};
    Message real_message{};

    map_msg.emplace_back(TagNo, value);
    try {
        real_message.from_string(map_msg);
    } catch (fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Invalid value for optional No Tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::ValueOORange);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an reject exception";
}