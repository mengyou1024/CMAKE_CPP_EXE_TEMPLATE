cmake_minimum_required(VERSION 3.20)

project(cpp_template)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src CURRENT_SRC)

add_executable(${PROJECT_NAME} ${CURRENT_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_options(${PROJECT_NAME} PUBLIC
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<C_COMPILER_ID:MSVC>:/W4>"
    "$<$<CXX_COMPILER_ID:MSVC>:/W4>"
)
