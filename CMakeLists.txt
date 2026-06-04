cmake_minimum_required(VERSION 3.28)

project(QueenBumblebee)

set(CMAKE_CXX_STANDARD 23)

# add_definitions(-DLOG_TO_TERMINAL)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(src)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
