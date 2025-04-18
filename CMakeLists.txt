cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER clang)
project(mangen C)

set(CMAKE_C_STANDARD 11)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

option(ENABLE_TESTS "Build with unit and e2e tests" ON)
option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
option(ENABLE_VERBOSE "Enable verbose output" OFF)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Sanitizers enabled: ${ENABLE_SANITIZERS}")
message(STATUS "Tests enabled: ${ENABLE_TESTS}")
message(STATUS "Verbose output: ${ENABLE_VERBOSE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

if(ENABLE_SANITIZERS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
endif()

if(ENABLE_VERBOSE)
    add_compile_definitions(VERBOSE_OUTPUT)
endif()

execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(WRITE "${SRC_DIR}/version.h" "#define VERSION \"${GIT_COMMIT_HASH}\"\n")

set(SOURCES
        ${SRC_DIR}/main.c
        ${SRC_DIR}/walker.c
        ${SRC_DIR}/hash.c
        ${SRC_DIR}/exclude.c
)

add_executable(mangen ${SOURCES})

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(${TEST_DIR})
endif()
