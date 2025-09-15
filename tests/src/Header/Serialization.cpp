#include "Tag.hpp"
#include "TagConvertor.hpp"

#include "FIX-Message/Header.hpp"

#include <gtest/gtest.h>

#include <format>

class Header_serialization_nononpositional : public testing::Test
{
    public:
        using Header = fix::Header<
            fix::PositionalTag<
                fix::Tag<Tag1, std::string>,
                fix::Tag<Tag2, int>,
                fix::Tag<Tag3, float>,
                fix::Tag<Tag4, bool>,
                fix::Tag<Tag5, char>
            >
        >;
};


TEST_F(Header_serialization_nononpositional, simple)
{

    const std::string value1 = "string";
    const int value2 = 123456798;
    const float value3 = 12345.6789;
    const bool value4 = true;
    const char value5 = 'c';

    Header header{};

    header.getPositional<Tag1>().Value = value1;
    header.getPositional<Tag2>().Value = value2;
    header.getPositional<Tag3>().Value = value3;
    header.getPositional<Tag4>().Value = value4;
    header.getPositional<Tag5>().Value = value5;

    const std::string result = std::format("{}={}\1", Tag1, value1)
        + std::format("{}={}\1", Tag2, value2)
        + std::format("{}={:.3f}\1", Tag3, value3)
        + std::format("{}={}\1", Tag4, static_cast<int>(value4))
        + std::format("{}={}\1", Tag5, value5);

    EXPECT_EQ(header.to_string(), result);
}