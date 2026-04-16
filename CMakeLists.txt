cmake_minimum_required(VERSION 3.22)

project(Tromboneless VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enabling the unit tests:
enable_testing()

add_subdirectory(${PROJECT_SOURCE_DIR}/Core)
add_subdirectory(${PROJECT_SOURCE_DIR}/App)