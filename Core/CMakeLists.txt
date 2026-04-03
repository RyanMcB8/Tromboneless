cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0 LANGUAGES C CXX)

# Use C++20 for C++ files, C99 for C files
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IDEs / tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable unit tests
enable_testing()

#including the GPIO library
include(ExternalProject)

ExternalProject_Add(libgpiod_external
    PREFIX ${CMAKE_BINARY_DIR}/modules/external/libgpiod
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/modules/external/libgpiod
    CONFIGURE_COMMAND ./autogen.sh && ./configure --prefix=${CMAKE_BINARY_DIR}/modules/external/libgpiod/install
    BUILD_COMMAND make -j${CMAKE_GENERATOR_CPU_COUNT}
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 1
)

# -------------------------------
# Core Library
# -------------------------------
add_library(TrombonelessCoreLibrary STATIC)
add_dependencies(TrombonelessCoreLibrary libgpiod_external)

# Include directories for the library
target_include_directories(TrombonelessCoreLibrary PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/drivers
    ${PROJECT_SOURCE_DIR}/shared
    ${PROJECT_SOURCE_DIR}/lib/rtmidi
    ${CMAKE_BINARY_DIR}/modules/external/libgpiod/install/include
)

# Add source files
file(GLOB_RECURSE CORE_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE RTMIDI_SOURCES "${PROJECT_SOURCE_DIR}/lib/rtmidi/*.cpp")

#  Ignoring the hardware files to build on none Pi computer.
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/tof_sensor\\.cpp$")
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/eventHandler\\.cpp$")
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/ads1115rpi\\.cpp$")


target_sources(TrombonelessCoreLibrary PRIVATE ${CORE_SOURCES} ${RTMIDI_SOURCES})

# Link any required external libraries (e.g., ALSA)
target_link_libraries(TrombonelessCoreLibrary PRIVATE
         asound
         ${CMAKE_BINARY_DIR}/modules/external/libgpiod/install/lib/libgpiod.so 
         )

# -------------------------------
# Main executable
# -------------------------------
add_executable(TrombonelessCore
    src/main.cpp
)


target_link_libraries(TrombonelessCore PRIVATE TrombonelessCoreLibrary)

# -------------------------------
# Tests
# -------------------------------
# Automatically find all test cpp files in tests/
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

foreach(TEST_FILE ${TEST_SOURCES})
    # Extract the file name without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE TrombonelessCoreLibrary)
    target_include_directories(${TEST_NAME} PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/tests
        )

    # Register with CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()