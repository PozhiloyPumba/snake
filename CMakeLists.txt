cmake_minimum_required(VERSION 3.10)
project(Snake_Game)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

find_program(CLANG_FORMAT "clang-format")

if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.[ch]pp)
    
    add_custom_target (
        format
        COMMAND clang-format
        -i
        -style=file
        ${ALL_CXX_SOURCE_FILES}
    )
endif()

add_compile_options("-Wall" "-Wextra" "$<$<CONFIG:RELEASE>:-O2>")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-Wno-subobject-linkage")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/source/*.cpp)

set (EXEC_FILE "snake")
add_executable (${EXEC_FILE} ${SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE include)
target_link_libraries(${EXEC_FILE} sfml-graphics sfml-audio sfml-window)