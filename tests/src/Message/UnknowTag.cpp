#include "Tag.hpp"
#include "TagConvertor.hpp"

#include "FIX-Message/Message.hpp"

#include <gtest/gtest.h>

class UnknowTag_standard_tag : public testing::Test
{
    public:
        static constexpr const fix::TagName UnknownTag = 2;
        static constexpr const char UnknownTagValue[] = "2";

        using Message = fix::Message<'0',
            fix::Tag<Tag1, std::string>
        >;
};

TEST_F(UnknowTag_standard_tag, unknown_tag)
{
    const std::string value = "error";
    fix::MapMessage map_msg;
    Message real_message{};
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(UnknownTagValue, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Unknown tag");
    EXPECT_EQ(reject.value().Reason, fix::RejectError::UndefineTag);
    EXPECT_EQ(reject.value().Tag, UnknownTag);
}

class UnknownTag_list_tag : public testing::Test
{
    public:
        static constexpr const fix::TagName UnknownTag = 3;
        static constexpr const char UnknownTagValue[] = "3";

        using Message = fix::Message<'0',
            fix::TagList<
                fix::List<
                    fix::TagNo<TagRef, true>,
                    fix::Tag<Tag1, std::optional<std::string>>
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

    map_msg.emplace_back(TagRefValue, value_no);
    map_msg.emplace_back(UnknownTagValue, value);
    reject = real_message.from_string(map_msg);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Unknown tag");
    EXPECT_EQ(reject.value().Reason, fix::RejectError::UndefineTag);
    EXPECT_EQ(reject.value().Tag, UnknownTag);
}