// #include "Message.hpp"
#include "TagConvertor.hpp"

#include "List.hpp"

#include <gtest/gtest.h>

class List_insert_simplecase : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag1[] = "2";
        static constexpr const char Tag2[] = "3";

        using List = fix::List<
            fix::TagNo<TagNo, false>,
            fix::Tag<Tag1, int>
        >;
};

TEST_F(List_insert_simplecase, stop_end)
{
    const std::string value = "123456789";
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    real_list.TagNo.Value = 1;

    map_msg.emplace_back(Tag1, value);
    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    EXPECT_EQ(it, map_msg.size());
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_EQ(fix::get<Tag1>(tuple).Value, std::stoi(value)) << "Expected: " << value;
}

TEST_F(List_insert_simplecase, stop_before_end)
{
    const std::string value = "123456789";
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    real_list.TagNo.Value = 1;

    map_msg.emplace_back(Tag1, value);
    map_msg.emplace_back(Tag2, value);

    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    EXPECT_EQ(it, map_msg.size() - 1);
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_EQ(fix::get<Tag1>(tuple).Value, std::stoi(value)) << "Expected: " << value;
}

class List_insert_allrequired : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag1[] = "2";
        static constexpr const char Tag2[] = "3";
        static constexpr const char Tag3[] = "4";
        static constexpr const char Tag4[] = "5";
        static constexpr const char Tag5[] = "6";

        using List = fix::List<
            fix::TagNo<TagNo, false>,
            fix::Tag<Tag1, int>,
            fix::Tag<Tag2, float>,
            fix::Tag<Tag3, char>,
            fix::Tag<Tag4, bool>,
            fix::Tag<Tag5, std::string>
        >;
};

TEST_F(List_insert_allrequired, single_entry)
{
    const std::string value1 = "123456789";
    const std::string value2 = "12345.6789";
    const std::string value3 = "c";
    const std::string value4 = "true";
    const std::string value5 = "string";
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    real_list.TagNo.Value = 1;

    map_msg.emplace_back(Tag1, value1);
    map_msg.emplace_back(Tag2, value2);
    map_msg.emplace_back(Tag3, value3);
    map_msg.emplace_back(Tag4, value4);
    map_msg.emplace_back(Tag5, value5);

    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_EQ(fix::get<Tag1>(tuple).Value, std::stoi(value1)) << "Expected: " << value1;
    EXPECT_EQ(fix::get<Tag2>(tuple).Value, std::stof(value2)) << "Expected: " << value2;
    EXPECT_EQ(fix::get<Tag3>(tuple).Value, value3[0]) << "Expected: " << value3;
    EXPECT_TRUE(fix::get<Tag4>(tuple).Value);
    EXPECT_EQ(fix::get<Tag5>(tuple).Value, value5);
}

TEST_F(List_insert_allrequired, multi_entry)
{
    const std::string value1_1 = "123456789";
    const std::string value1_2 = "12345.6789";
    const std::string value1_3 = "c";
    const std::string value1_4 = "true";
    const std::string value1_5 = "string";

    const std::string value2_1 = "987654321";
    const std::string value2_2 = "98765.4321";
    const std::string value2_3 = "a";
    const std::string value2_4 = "false";
    const std::string value2_5 = "gnirts";

    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    real_list.TagNo.Value = 2;

    map_msg.emplace_back(Tag1, value1_1);
    map_msg.emplace_back(Tag2, value1_2);
    map_msg.emplace_back(Tag3, value1_3);
    map_msg.emplace_back(Tag4, value1_4);
    map_msg.emplace_back(Tag5, value1_5);

    map_msg.emplace_back(Tag1, value2_1);
    map_msg.emplace_back(Tag2, value2_2);
    map_msg.emplace_back(Tag3, value2_3);
    map_msg.emplace_back(Tag4, value2_4);
    map_msg.emplace_back(Tag5, value2_5);

    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 2);

    const List::DataTuple &tuple_1 = real_list.at(0);

    EXPECT_EQ(fix::get<Tag1>(tuple_1).Value, std::stoi(value1_1)) << "Expected: " << value1_1;
    EXPECT_EQ(fix::get<Tag2>(tuple_1).Value, std::stof(value1_2)) << "Expected: " << value1_2;
    EXPECT_EQ(fix::get<Tag3>(tuple_1).Value, value1_3[0]) << "Expected: " << value1_3;
    EXPECT_TRUE(fix::get<Tag4>(tuple_1).Value);
    EXPECT_EQ(fix::get<Tag5>(tuple_1).Value, value1_5);

    const List::DataTuple &tuple_2 = real_list.at(1);

    EXPECT_EQ(fix::get<Tag1>(tuple_2).Value, std::stoi(value2_1)) << "Expected: " << value2_1;
    EXPECT_EQ(fix::get<Tag2>(tuple_2).Value, std::stof(value2_2)) << "Expected: " << value2_2;
    EXPECT_EQ(fix::get<Tag3>(tuple_2).Value, value2_3[0]) << "Expected: " << value2_3;
    EXPECT_FALSE(fix::get<Tag4>(tuple_2).Value);
    EXPECT_EQ(fix::get<Tag5>(tuple_2).Value, value2_5);
}


TEST_F(List_insert_allrequired, single_entry_not_order)
{
    const std::string value1 = "123456789";
    const std::string value2 = "12345.6789";
    const std::string value3 = "c";
    const std::string value4 = "true";
    const std::string value5 = "string";
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    real_list.TagNo.Value = 1;

    map_msg.emplace_back(Tag5, value5);
    map_msg.emplace_back(Tag4, value4);
    map_msg.emplace_back(Tag3, value3);
    map_msg.emplace_back(Tag2, value2);
    map_msg.emplace_back(Tag1, value1);

    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_EQ(fix::get<Tag1>(tuple).Value, std::stoi(value1)) << "Expected: " << value1;
    EXPECT_EQ(fix::get<Tag2>(tuple).Value, std::stof(value2)) << "Expected: " << value2;
    EXPECT_EQ(fix::get<Tag3>(tuple).Value, value3[0]) << "Expected: " << value3;
    EXPECT_TRUE(fix::get<Tag4>(tuple).Value);
    EXPECT_EQ(fix::get<Tag5>(tuple).Value, value5);
}

class List_insert_withoptional : public testing::Test
{
    public:
        static constexpr const char TagNo[] = "1";
        static constexpr const char Tag1[] = "2";
        static constexpr const char Tag2[] = "3";

        using List = fix::List<
            fix::TagNo<TagNo, false>,
            fix::Tag<Tag1, std::optional<int>>,
            fix::Tag<Tag2, float>
        >;
};

TEST_F(List_insert_withoptional, single_entry)
{
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    const std::string value1 = "123456789";
    const std::string value2 = "12345.6789";

    map_msg.emplace_back(Tag1, value1);
    map_msg.emplace_back(Tag2, value2);

    real_list.TagNo.Value = 1;
    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_FLOAT_EQ(fix::get<Tag2>(tuple).Value, std::stof(value2)) << "Expected: " << value2;
    ASSERT_TRUE(fix::get<Tag1>(tuple).Value.has_value());
    EXPECT_EQ(fix::get<Tag1>(tuple).Value.value(), std::stoi(value1)) << "Expected: " << value1;
}

TEST_F(List_insert_withoptional, single_entry_optional_empty)
{
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    const std::string value1 = "";
    const std::string value2 = "12345.6789";

    map_msg.emplace_back(Tag1, value1);
    map_msg.emplace_back(Tag2, value2);

    real_list.TagNo.Value = 1;
    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_FLOAT_EQ(fix::get<Tag2>(tuple).Value, std::stof(value2)) << "Expected: " << value2;
    ASSERT_FALSE(fix::get<Tag1>(tuple).Value.has_value());
}

TEST_F(List_insert_withoptional, single_entry_optional_missing)
{
    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    const std::string value1 = "";
    const std::string value2 = "12345.6789";

    map_msg.emplace_back(Tag2, value2);

    real_list.TagNo.Value = 1;
    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 1);

    const List::DataTuple &tuple = real_list.at(0);

    EXPECT_FLOAT_EQ(fix::get<Tag2>(tuple).Value, std::stof(value2)) << "Expected: " << value2;
    ASSERT_FALSE(fix::get<Tag1>(tuple).Value.has_value());
}

TEST_F(List_insert_withoptional, multi_entry)
{

    fix::MapMessage map_msg{};
    List real_list{};
    size_t it = 0;
    std::optional<fix::RejectError> reject = std::nullopt;

    const std::string value1_1 = "123456789";
    const std::string value1_2 = "12345.6789";

    const std::string value2_1 = "";
    const std::string value2_2 = "9876.54321";

    const std::string value3_2 = "0";

    map_msg.emplace_back(Tag1, value1_1);
    map_msg.emplace_back(Tag2, value1_2);
    map_msg.emplace_back(Tag1, value2_1);
    map_msg.emplace_back(Tag2, value2_2);
    map_msg.emplace_back(Tag2, value3_2);

    real_list.TagNo.Value = 3;
    reject = real_list.from_string(map_msg, it);

    ASSERT_FALSE(reject.has_value());
    ASSERT_EQ(real_list.size(), 3);

    const List::DataTuple &tuple1 = real_list.at(0);

    EXPECT_FLOAT_EQ(fix::get<Tag2>(tuple1).Value, std::stof(value1_2)) << "Expected: " << value1_2;
    ASSERT_TRUE(fix::get<Tag1>(tuple1).Value.has_value());
    EXPECT_EQ(fix::get<Tag1>(tuple1).Value, std::stoi(value1_1)) << "Expected: " << value1_1;

    const List::DataTuple &tuple2 = real_list.at(1);

    EXPECT_FLOAT_EQ(fix::get<Tag2>(tuple2).Value, std::stof(value2_2)) << "Expected: " << value2_2;
    ASSERT_FALSE(fix::get<Tag1>(tuple2).Value.has_value());

    const List::DataTuple &tuple3 = real_list.at(2);

    EXPECT_FLOAT_EQ(fix::get<Tag2>(tuple3).Value, std::stof(value3_2)) << "Expected: " << value3_2;
    ASSERT_FALSE(fix::get<Tag1>(tuple3).Value.has_value());
}