cmake_minimum_required(VERSION 3.20)

project(cpp_template)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src CURRENT_SRC)

add_executable(${PROJECT_NAME} ${CURRENT_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(fmt CONFIG REQUIRED)

target_compile_options(${PROJECT_NAME} PUBLIC
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<C_COMPILER_ID:MSVC>:/W4>"
    "$<$<CXX_COMPILER_ID:MSVC>:/W4>"
)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

function(add_clang_tidy_pre_build target_name)
    # 查找clang-tidy可执行文件
    find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

    if(CLANG_TIDY_EXECUTABLE)
        message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXECUTABLE}")
    else()
        message(WARNING "clang-tidy not found, use:\n\t`choco install llvm`(Windows)\n\t`sudo apt install llvm`(Debain) \nto install clang-tidy")
        return()
    endif()

    get_target_property(_target_sources ${target_name} SOURCES)
    set(_clang_tidy_src "")

    foreach(_item ${_target_sources})
        get_filename_component(_asb_path ${_item} ABSOLUTE)
        list(APPEND _clang_tidy_src ${_asb_path})
    endforeach(_item ${_target_sources})

    add_custom_command(
        TARGET ${target_name}
        PRE_BUILD
        COMMAND clang-tidy -p "${CMAKE_BINARY_DIR}" --warnings-as-errors=* -header-filter=.* ${_clang_tidy_src}
        COMMENT "Check for clang-tidy issues before building ${target_name}"
        VERBATIM
    )
endfunction(add_clang_tidy_pre_build target_name)

add_clang_tidy_pre_build(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME})

install(CODE [[

file(
    GET_RUNTIME_DEPENDENCIES
    RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
    UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
    LIBRARIES $<TARGET_FILE:cpp_template>
    DIRECTORIES $<TARGET_FILE_DIR:cpp_template>
    PRE_INCLUDE_REGEXES $<TARGET_FILE_DIR:cpp_template>
    PRE_EXCLUDE_REGEXES "system32"
    POST_INCLUDE_REGEXES $<TARGET_FILE_DIR:cpp_template>
    POST_EXCLUDE_REGEXES "system32"
)

foreach(DEP_LIB ${RESOLVED_DEPS})
    file(INSTALL ${DEP_LIB} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach()

]])

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "1.0.0") 
set(CPACK_GENERATOR "ZIP") 
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF) 
include(CPack) 
