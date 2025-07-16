#include "Message.hpp"

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

    map_msg.emplace_back(Tag2, value);

    real_list.TagNo.Value = 1;
    try {
        real_list.from_string(map_msg, it);
    } catch (const fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Missing required tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::ReqTagMissing);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an exception";
}

TEST_F(List_missing_required, second_missing)
{
    const std::string value1_1 = "123456789";
    const std::string value1_2 = "";
    const std::string value2_1 = "";
    fix::MapMessage map_msg;
    List real_list{};
    size_t it = 0;

    map_msg.emplace_back(Tag1, value1_1);
    map_msg.emplace_back(Tag2, value1_2);
    map_msg.emplace_back(Tag2, value2_1);

    real_list.TagNo.Value = 2;
    try {
        real_list.from_string(map_msg, it);
    } catch (const fix::RejectException &_reject) {
        EXPECT_STREQ(_reject.reason(), "Missing required tag");
        EXPECT_STREQ(_reject.what(), fix::RejectException::ReqTagMissing);
        SUCCEED();
        return;
    } catch (std::exception &_exception) {
        FAIL() << _exception.what();
    }
    FAIL() << "Expected an exception";
}
