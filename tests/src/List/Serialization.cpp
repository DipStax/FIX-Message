#include "List.hpp"

#include <gtest/gtest.h>

#include <format>

class List_serialization_nooptional : public testing::Test
{
    public:
        static constexpr const char TagRef[] = "0";
        static constexpr const char Tag1[] = "1";
        static constexpr const char Tag2[] = "2";
        static constexpr const char Tag3[] = "3";
        static constexpr const char Tag4[] = "4";
        static constexpr const char Tag5[] = "5";

        using List = fix::List<
            fix::TagNo<TagRef, true>,
            fix::Tag<Tag1, std::string>,
            fix::Tag<Tag2, int>,
            fix::Tag<Tag3, float>,
            fix::Tag<Tag4, bool>,
            fix::Tag<Tag5, char>
        >;
};

TEST_F(List_serialization_nooptional, single_list)
{
    List list{};
    List::DataTuple tuple{};
    std::stringstream stream{};

    const std::string value1 = "string";
    const int value2 = 123456798;
    const float value3 = 12345.6789;
    const bool value4 = true;
    const char value5 = 'c';

    fix::get<Tag1>(tuple).Value = value1;
    fix::get<Tag2>(tuple).Value = value2;
    fix::get<Tag3>(tuple).Value = value3;
    fix::get<Tag4>(tuple).Value = value4;
    fix::get<Tag5>(tuple).Value = value5;
    list.add(std::move(tuple));

    const std::string result = std::format("{}={}\1", TagRef, 1)
        + std::format("{}={}\1", Tag1, value1)
        + std::format("{}={}\1", Tag2, value2)
        + std::format("{}={:.4f}\1", Tag3, value3)
        + std::format("{}={}\1", Tag4, static_cast<int>(value4))
        + std::format("{}={}\1", Tag5, value5);

    stream << std::fixed << std::setprecision(4);
    list.to_string(stream);
    EXPECT_EQ(stream.str(), result);
}

TEST_F(List_serialization_nooptional, multi_list)
{
    List list{};
    List::DataTuple tuple1{};
    List::DataTuple tuple2{};
    std::stringstream stream{};

    const std::string value1_1 = "string1";
    const int value1_2 = 123456798;
    const float value1_3 = 12345.6789;
    const bool value1_4 = true;
    const char value1_5 = 'c';

    fix::get<Tag1>(tuple1).Value = value1_1;
    fix::get<Tag2>(tuple1).Value = value1_2;
    fix::get<Tag3>(tuple1).Value = value1_3;
    fix::get<Tag4>(tuple1).Value = value1_4;
    fix::get<Tag5>(tuple1).Value = value1_5;
    list.add(std::move(tuple1));


    const std::string value2_1 = "string2";
    const int value2_2 = 987654321;
    const float value2_3 = 98765.4321;
    const bool value2_4 = false;
    const char value2_5 = 'z';

    fix::get<Tag1>(tuple2).Value = value2_1;
    fix::get<Tag2>(tuple2).Value = value2_2;
    fix::get<Tag3>(tuple2).Value = value2_3;
    fix::get<Tag4>(tuple2).Value = value2_4;
    fix::get<Tag5>(tuple2).Value = value2_5;
    list.add(std::move(tuple2));

    const std::string result = std::format("{}={}\1", TagRef, 2)
        + std::format("{}={}\1", Tag1, value1_1)
        + std::format("{}={}\1", Tag2, value1_2)
        + std::format("{}={:.4f}\1", Tag3, value1_3)
        + std::format("{}={}\1", Tag4, static_cast<int>(value1_4))
        + std::format("{}={}\1", Tag5, value1_5)
        + std::format("{}={}\1", Tag1, value2_1)
        + std::format("{}={}\1", Tag2, value2_2)
        + std::format("{}={:.4f}\1", Tag3, value2_3)
        + std::format("{}={}\1", Tag4, static_cast<int>(value2_4))
        + std::format("{}={}\1", Tag5, value2_5);


    stream << std::fixed << std::setprecision(4);
    list.to_string(stream);
    EXPECT_EQ(stream.str(), result);
}

class List_serialization_optional : public testing::Test
{
    public:
        static constexpr const char TagRef[] = "0";
        static constexpr const char Tag1[] = "1";
        static constexpr const char Tag2[] = "2";
        static constexpr const char Tag3[] = "3";

        using List = fix::List<
            fix::TagNo<TagRef, true>,
            fix::Tag<Tag1, std::string>,
            fix::Tag<Tag2, std::optional<int>>,
            fix::Tag<Tag3, std::optional<float>>
        >;
};

TEST_F(List_serialization_optional, single_list)
{
    List list{};
    List::DataTuple tuple{};
    std::stringstream stream{};

    const std::string value1 = "string";

    fix::get<Tag1>(tuple).Value = value1;
    list.add(std::move(tuple));

    const std::string result = std::format("{}={}\1", TagRef, 1)
        + std::format("{}={}\1", Tag1, value1);

    list.to_string(stream);
    EXPECT_EQ(stream.str(), result);
}

TEST_F(List_serialization_optional, multi_list_fullempty)
{
    List list{};
    List::DataTuple tuple1{};
    List::DataTuple tuple2{};
    std::stringstream stream{};

    const std::string value1_1 = "string1";
    const std::string value2_1 = "string2";

    fix::get<Tag1>(tuple1).Value = value1_1;
    list.add(std::move(tuple1));

    fix::get<Tag1>(tuple2).Value = value2_1;
    list.add(std::move(tuple2));

    const std::string result = std::format("{}={}\1", TagRef, 2)
        + std::format("{}={}\1", Tag1, value1_1)
        + std::format("{}={}\1", Tag1, value2_1);

    list.to_string(stream);
    EXPECT_EQ(stream.str(), result);
}

TEST_F(List_serialization_optional, multi_list_withvalue)
{
    List list{};
    List::DataTuple tuple1{};
    List::DataTuple tuple2{};
    std::stringstream stream{};

    const std::string value1_1 = "string1";
    const int value1_2 = 123465789;
    const std::string value2_1 = "string2";
    const float value2_3 = 12345.6798;

    fix::get<Tag1>(tuple1).Value = value1_1;
    fix::get<Tag2>(tuple1).Value = value1_2;
    list.add(std::move(tuple1));

    fix::get<Tag1>(tuple2).Value = value2_1;
    fix::get<Tag3>(tuple2).Value = value2_3;
    list.add(std::move(tuple2));

    const std::string result = std::format("{}={}\1", TagRef, 2)
        + std::format("{}={}\1", Tag1, value1_1)
        + std::format("{}={}\1", Tag2, value1_2)
        + std::format("{}={}\1", Tag1, value2_1)
        + std::format("{}={:.4f}\1", Tag3, value2_3);

    stream << std::fixed << std::setprecision(4);
    list.to_string(stream);
    EXPECT_EQ(stream.str(), result);
}