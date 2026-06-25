cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME DisappearingPairs)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(DisappearingPairs DisappearingPairs.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        a721f1b20c605f635413e22d8b7427a8b4f3956c
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
  DisappearingPairsTest
  DisappearingPairsTest.cpp
  DisappearingPairs.cpp
)
target_link_libraries(
  DisappearingPairsTest
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(DisappearingPairsTest)