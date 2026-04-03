cmake_minimum_required(VERSION 3.22)

project(TrombonelessTests VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enabling the unit tests:
enable_testing()

# Should add an executable for each section.
add_executable(TrombonelessCoreTests synth_audio_test.cpp)

target_include_directories(TrombonelessCore PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/shared
    )

# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/Core/*.cpp")
target_sources(TrombonelessCore
    PRIVATE
        ${SOURCES}
        )

add_test(synthTest TrombonelessTests)