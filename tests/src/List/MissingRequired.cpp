#include "TagConvertor.hpp"

#include "FIX-Message/List.hpp"

#include <gtest/gtest.h>

class List_missing_required : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag1[] = "2";
        static constexpr const char Tag2[] = "3";

        using List = fix::List<
            fix::TagNo<TagNo, false>,
            fix::Tag<Tag1, int>,
            fix::Tag<Tag2, std::optional<int>>
        >;
};

TEST_F(List_missing_required, empty_list)
{
    const std::string value = "";
    fix::MapMessage map_msg;
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(Tag2, value);

    real_list.TagNo.Value = 1;
    reject = real_list.from_string(map_msg, it);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Missing required tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::ReqTagMissing);
    EXPECT_STREQ(reject.value().Tag.c_str(), Tag1);
}

TEST_F(List_missing_required, second_missing)
{
    const std::string value1_1 = "123456789";
    const std::string value1_2 = "";
    const std::string value2_1 = "";
    fix::MapMessage map_msg;
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    map_msg.emplace_back(Tag1, value1_1);
    map_msg.emplace_back(Tag2, value1_2);
    map_msg.emplace_back(Tag2, value2_1);

    real_list.TagNo.Value = 2;
    reject = real_list.from_string(map_msg, it);

    ASSERT_TRUE(reject.has_value());
    EXPECT_STREQ(reject.value().Message.c_str(), "Missing required tag");
    EXPECT_STREQ(reject.value().Reason.c_str(), fix::RejectError::ReqTagMissing);
    EXPECT_STREQ(reject.value().Tag.c_str(), Tag1);
}
