cmake_minimum_required(VERSION 3.20)

project(cpp_template)

set(CMAKE_CXX_STANDARD 23)

include(cmake/Utils.cmake)

add_subdirectory_path("components")

resolve_source_tree("src" SRC_FILES INC_FILES INC_DIRS)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

find_package(fmt CONFIG REQUIRED)

target_compile_options(${PROJECT_NAME} PUBLIC
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<C_COMPILER_ID:MSVC>:/W4>"
    "$<$<CXX_COMPILER_ID:MSVC>:/W4>"
)

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

if(TARGET cpp_lib_template)
    target_link_libraries(${PROJECT_NAME} PRIVATE cpp_lib_template)
endif()

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

if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(test)
endif()
