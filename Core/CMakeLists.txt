cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0 LANGUAGES C CXX)

# Use C++20 for C++ files, C99 for C files
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding the necessary submodules
add_subdirectory(modules/rtmidi)

# Enable unit tests
enable_testing()
include(CTest)

# ========================================
# Core Library
# ========================================

# Add source files
file(GLOB_RECURSE CORE_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

#  Ignoring the hardware files to build on none Pi computer.
# list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/tof_sensor\\.cpp$")
# list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/eventHandler\\.cpp$")
# list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/ads1115rpi\\.cpp$")

add_library(TrombonelessCoreLibrary STATIC ${CORE_SOURCES})

# Include directories for the library
target_include_directories(TrombonelessCoreLibrary PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/drivers
    ${PROJECT_SOURCE_DIR}/shared
    ${LIBGPIOD_INCLUDE_DIRS}
)

target_link_libraries(TrombonelessCoreLibrary PUBLIC rtmidi)
target_link_libraries(TrombonelessCoreLibrary PRIVATE 
    ${LIBGPIOD_LIB}
    asound
)

# ========================================
#               Main executable
# ========================================
add_executable(TrombonelessCore
    src/main.cpp
)


target_link_libraries(TrombonelessCore PRIVATE TrombonelessCoreLibrary)

# ========================================
#                   Tests
# ========================================
# Automatically find all test cpp files in tests/
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

# Searches through all C files within the tests directory and treats them as test script.
foreach(TEST_FILE ${TEST_SOURCES})
    # Extract the file name without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE TrombonelessCoreLibrary)
    target_include_directories(${TEST_NAME} PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/tests
        )

    # Add the test to the CTest list of tests.
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()