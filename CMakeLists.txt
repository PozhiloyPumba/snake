cmake_minimum_required(VERSION 3.10)
project(Snake_Game)


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

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/source/*.cpp)

set (EXEC_FILE "snake")

add_executable (${EXEC_FILE} ${SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE include)

set (COMPILER_SETTINGS
    -std=c++2a
    -Wall
    -Wextra

    -Wsign-promo
    -Wdouble-promotion

    -Wfloat-equal

    -Wdiv-by-zero

    -O2
)

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})
