#include <iostream>

constexpr const char Tag[] = "1";
constexpr const char Tag2[] = "1";

// using List = fix::List<fix::TagNo<Tag, false>, fix::Tag<Tag2, int>>;

// class Router
// {
//     public:
//         Router()
//             : m_router(this)
//         {
//         }
//         ~Router() = default;

//         void onInput()
//         {
//             router.run(...);
//         }

//     private:
//         fix::Router<Router,
//             Router::onReject,
//             fix::ContextInfo<Context, Context::GetMessage, Context::GetHeader>
//             fix::Switch<
//                 fix::Default<Router::Reject>
//                 fix::Case<Router::handleAuth, fix::msg::Logon, fix::msg::Logout>,
//                 fix::Case<Router::handleHeartBeat, fix::msf::HeartBeat>
//             >
//         > m_router;
// }

int main()
{
    // fix::Logon logon{};

    // std::vector<std::pair<std::string, std::string>> data;


    // data.push_back({ fix::tag::HeartBtInt, "5" });
    // data.push_back({ fix::tag::ResetSeqNumFlag, "true" });

    // static_assert(fix::meta::is_optional_v<std::optional<int>>, "expected true");
    // static_assert(std::is_same_v<
    //     fix::meta::find_taglist_from_name_t<Tag, List>,
    //     List
    // >, "expected true");


    // static_assert(std::is_same_v<
    //     fix::Tag<Tag, int>,
    //     fix::meta::find_tag_from_name_in_tuple_t<Tag, std::tuple<fix::Tag<Tag, int>>>
    // >, "expected true");

    // logon.from_string(data);
    // const fix::Tag<fix::tag::HeartBtInt, int> &tag = logon.get<fix::tag::HeartBtInt>();
    // std::cout << tag.Value << std::endl;
    // const fix::Tag<fix::tag::ResetSeqNumFlag, std::optional<bool>> &tag_rsnf = logon.get<fix::tag::ResetSeqNumFlag>();
    // std::cout << tag_rsnf.Value.has_value() << " " << tag_rsnf.Value.value() << std::endl;
    return 0;
}