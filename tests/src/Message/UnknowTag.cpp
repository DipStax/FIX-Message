#include "TagConvertor.hpp"

#include "FIX-Message/Message.hpp"

#include <gtest/gtest.h>

class UnknowTag_standard_tag : public testing::Test
{
    public:
        static constexpr const char Tag[] = "1";
        static constexpr const char UnknownTag[] = "2";

        using Message = fix::Message<'0',
            fix::Tag<Tag, std::string>
        >;
};

TEST_F(UnknowTag_standard_tag, unknown_tag)
{
    const std::string value = "error";
    fix::MapMessage map_msg;
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(UnknownTag, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Unknown tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::UndefineTag);
    EXPECT_STREQ(reject.value().Tag.c_str(), UnknownTag);
}

class UnknownTag_list_tag : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag[] = "2";
        static constexpr const char UnknownTag[] = "3";

        using Message = fix::Message<'0',
            fix::TagList<
                fix::List<
                    fix::TagNo<TagNo, true>,
                    fix::Tag<Tag, std::optional<std::string>>
                >
            >
        >;
};

TEST_F(UnknownTag_list_tag, unknown_tag)
{
    const std::string value_no = "1";
    const std::string value = "error";
    fix::MapMessage map_msg;
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(TagNo, value_no);
    map_msg.emplace_back(UnknownTag, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Unknown tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::UndefineTag);
    EXPECT_STREQ(reject.value().Tag.c_str(), UnknownTag);
}