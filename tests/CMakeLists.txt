file(GLOB TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(qb_tests ${TEST_SOURCES})

target_link_libraries(qb_tests PRIVATE queen_bumblebee_lib Catch2::Catch2WithMain spdlog::spdlog_header_only)
target_include_directories(qb_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(qb_tests PRIVATE QB_DEBUG_BUILD SPDLOG_NO_EXCEPTIONS)
else()
    target_compile_definitions(qb_tests PRIVATE SPDLOG_NO_EXCEPTIONS)
endif()

include(CTest)
include(Catch)
catch_discover_tests(qb_tests)
