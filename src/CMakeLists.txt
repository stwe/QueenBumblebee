FILE(GLOB TARGET_H
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/polyglot/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/uci/*.h"
)

FILE(GLOB TARGET_CPP
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/polyglot/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/uci/*.cpp"
)

list(REMOVE_ITEM TARGET_CPP "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp")
add_library(queen_bumblebee_lib STATIC ${TARGET_CPP} ${TARGET_H})

target_include_directories(queen_bumblebee_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(queen_bumblebee_lib PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(queen_bumblebee_lib PRIVATE QB_DEBUG_BUILD SPDLOG_NO_EXCEPTIONS)
else()
    target_compile_definitions(queen_bumblebee_lib PRIVATE SPDLOG_NO_EXCEPTIONS)
endif()

add_executable(QueenBumblebee Main.cpp)
target_link_libraries(QueenBumblebee PRIVATE queen_bumblebee_lib)

add_custom_command(TARGET QueenBumblebee POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/books"
    "$<TARGET_FILE_DIR:QueenBumblebee>/books"
)
