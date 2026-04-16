cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(modules/rtmidi)

add_executable(TrombonelessCore src/main.cpp)

target_include_directories(TrombonelessCore PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/drivers
    ${PROJECT_SOURCE_DIR}/Drivers
    ${PROJECT_SOURCE_DIR}/shared
    )
target_link_libraries(TrombonelessCore
    PUBLIC
        rtmidi
        gpiod
        gpiodxx
)
# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
target_sources(TrombonelessCore
    PRIVATE
        ${SOURCES}
        )
