cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME_VAR "cpptemplate" CACHE STRING "Project name")
project(${PROJECT_NAME_VAR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if we can reach Google's DNS (as a connection test)
execute_process(
    COMMAND ping -c 1 8.8.8.8
    RESULT_VARIABLE PING_RESULT
    OUTPUT_QUIET
)

if(PING_RESULT EQUAL 0)
    # Google Test setup
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
    )
    set(GTEST_AVAILABLE TRUE)

    ## For windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
else()
    message(WARNING "No internet connection available. Unit tests will be disabled.")
    set(GTEST_AVAILABLE FALSE)
endif()

# Function to get all subdirectories
function(get_all_subdirs result dir)
    file(GLOB_RECURSE children LIST_DIRECTORIES true "${dir}/*")
    set(subdirs "")
    foreach(child ${children})
        if(IS_DIRECTORY ${child})
            list(APPEND subdirs ${child})
        endif()
    endforeach()
    set(${result} ${subdirs} PARENT_SCOPE)
endfunction()

# Debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CheckCXXCompilerFlag)
    foreach(flag IN ITEMS
            -g3 -O0
            -fsanitize=address -fsanitize=undefined -fsanitize=leak
            -fno-omit-frame-pointer -fstack-protector
            -Werror -Wextra -Wall -Wpedantic)
        check_cxx_compiler_flag(${flag} COMPILER_SUPPORTS_${flag})
        if(COMPILER_SUPPORTS_${flag})
            add_compile_options($<$<CONFIG:Debug>:${flag}>)
            if(flag MATCHES "fsanitize")
                add_link_options($<$<CONFIG:Debug>:${flag}>)
            endif()
        endif()
    endforeach()
endif()

# Get all source directories
get_all_subdirs(SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Project library
set(LIB_SOURCES
        src/main.cpp
)

list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_library(${PROJECT_NAME_VAR}_lib STATIC ${LIB_SOURCES})
target_include_directories(${PROJECT_NAME_VAR}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${SUBDIRS})
if (NOT MSVC)
    target_compile_options(${PROJECT_NAME_VAR}_lib PRIVATE -Wall -Wextra -Werror -Wpedantic)
else()
    target_compile_options(${PROJECT_NAME_VAR}_lib PRIVATE /W4 /WX)
endif()

# Main executable
add_executable(${PROJECT_NAME_VAR} src/main.cpp)
target_link_libraries(${PROJECT_NAME_VAR} PRIVATE ${PROJECT_NAME_VAR}_lib)

# Debug target
add_custom_target(dbg_build
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug -B${CMAKE_BINARY_DIR} -S${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
)

# Unit tests setup
if(GTEST_AVAILABLE)
    enable_testing()
    set(TEST_SOURCES
        unit_tests/sample_function_test.cpp
    )
    add_executable(unit_tests ${TEST_SOURCES})
    target_link_libraries(unit_tests PRIVATE GTest::gtest_main ${PROJECT_NAME_VAR}_lib)
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()
