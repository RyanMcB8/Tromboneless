cmake_minimum_required(VERSION 3.22)

project(TrombonelessSharedLibrary VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ========================================
#               Main Library
# ========================================

add_library(TrombonelessSharedLibrary)

#   Adding the source file
target_sources(TrombonelessSharedLibrary PUBLIC
${PROJECT_SOURCE_DIR}/tromboneless_data.cpp)

#   Adding the header file
target_include_directories(TrombonelessSharedLibrary PUBLIC
${PROJECT_SOURCE_DIR}
)