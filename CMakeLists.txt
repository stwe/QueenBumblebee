cmake_minimum_required(VERSION 4.3)

project(QueenBumblebee)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR
        "\n[QueenBumblebee] Error: This project requires the GCC compiler (g++).\n"
        "Detected compiler: ${CMAKE_CXX_COMPILER_ID}\n"
        "Please configure your IDE/Toolchain to use GCC."
    )
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(CATCH_INSTALL_HELPERS OFF CACHE INTERNAL "")
set(CATCH_BUILD_TESTING OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(spdlog Catch2)

add_subdirectory(src)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
