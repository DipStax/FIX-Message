cmake_minimum_required(VERSION 3.16)
project(fix_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -g3)
include_directories(${CMAKE_SOURCE_DIR}/include)
include(FetchContent)



file(GLOB SRSC_fix
    src/Exception.cpp
)

add_library(fix_message STATIC ${SRSC_fix})




add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE fix_message)




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
    tests/**.cpp
)

add_executable(test_message
    ${SRC_Tests}
)
target_include_directories(test_message PRIVATE include)
target_include_directories(test_message PRIVATE test)

target_link_libraries(test_message fix_message GTest::gtest_main)

gtest_discover_tests(test_message)