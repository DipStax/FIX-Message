cmake_minimum_required(VERSION 3.16)
project(FIX_Message LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -g3)

include(FetchContent)

file(GLOB SRSC_fix
    include/**.hpp
    include/**.inl
)

add_library(fix_message INTERFACE ${SRSC_fix})

target_include_directories(fix_message INTERFACE include)

enable_testing()

set(GTEST_VERSION 1.17.0)
FetchContent_Declare(
  googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v${GTEST_VERSION}
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

file(GLOB_RECURSE SRC_Tests
    tests/src/**.cpp
)

add_executable(fix_message_test
    ${SRC_Tests}
)
target_include_directories(fix_message_test PRIVATE tests/include)

target_link_libraries(fix_message_test fix_message GTest::gtest_main)

gtest_discover_tests(fix_message_test)