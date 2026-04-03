cmake_minimum_required(VERSION 3.22)

# Name this standalone test project
project(TrombonelessCoreTests VERSION 1.0.0 LANGUAGES CXX)

# Use C++20 for everything in this test project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to the parent project root
set(PROJECT_ROOT ..)

# Add test executable
add_executable(TrombonelessCoreTests
    synth_audio_test.cpp
)

# Include directories for the core library and any headers needed
target_include_directories(TrombonelessCoreTests PRIVATE
    ${PROJECT_ROOT}/include
    ${PROJECT_ROOT}/tests
)


# Enable CTest for this test executable
enable_testing()
add_test(NAME SynthAudioTest COMMAND TrombonelessCoreTests)