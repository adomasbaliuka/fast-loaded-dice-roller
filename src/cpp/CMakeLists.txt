cmake_minimum_required(VERSION 3.19)
project(fldr_cpp
        LANGUAGES C CXX
        DESCRIPTION "C++ port of Fast Loaded Dice Roller"
        )
enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)


add_executable(unit_tests unit_tests_fldr.cpp)

target_compile_features(unit_tests
        PUBLIC cxx_std_17
        )

target_include_directories(unit_tests
        PRIVATE

        )

target_link_libraries(unit_tests
        PRIVATE
        gtest_main
        )

# adds tests via CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)
