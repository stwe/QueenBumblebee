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

target_include_directories(queen_bumblebee_lib
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(queen_bumblebee_lib
    PUBLIC
        spdlog::spdlog_header_only
)

target_compile_definitions(queen_bumblebee_lib
    PRIVATE
        SPDLOG_NO_EXCEPTIONS
    PUBLIC
        $<$<CONFIG:Debug>:QB_DEBUG_BUILD>
)

add_executable(QueenBumblebee Main.cpp)
target_link_libraries(QueenBumblebee
    PRIVATE
        queen_bumblebee_lib
        spdlog::spdlog
)

target_compile_options(queen_bumblebee_lib PRIVATE -march=native)

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT error)
if(lto_supported)
    set_target_properties(queen_bumblebee_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_target_properties(QueenBumblebee PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_custom_command(TARGET QueenBumblebee POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/books
    "$<TARGET_FILE_DIR:QueenBumblebee>/books"
)
