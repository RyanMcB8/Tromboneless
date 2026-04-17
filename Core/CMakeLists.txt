cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0)

# Use C++20 for everything in this test project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIDAR_DIR ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API)

link_directories(/usr/local/lib)
# find_library(GPIODXX_LIBRARY gpiodxx PATHS /usr/local/lib /usr/lib/x86_64-linux-gnu)
# Enable unit tests
enable_testing()
include(CTest)

add_subdirectory(modules/rtmidi)

# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(TrombonelessCoreLibrary STATIC ${SOURCES} "${PROJECT_SOURCE_DIR}/modules/rpi_ads1115/ads1115rpi.cpp")


target_include_directories(TrombonelessCoreLibrary PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${LIDAR_DIR}/core
    ${LIDAR_DIR}/platform
    ${PROJECT_SOURCE_DIR}/include/drivers
    ${PROJECT_SOURCE_DIR}/Drivers
    ${PROJECT_SOURCE_DIR}/../Shared
    ${PROJECT_SOURCE_DIR}/modules/external/libgpiod/lib
    ${PROJECT_SOURCE_DIR}/modules/rpi_ads1115
    )

target_link_libraries(TrombonelessCoreLibrary
    PUBLIC
        rtmidi
        gpiod
        gpiodcxx
        asound
)


add_executable(TrombonelessCore "${PROJECT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(TrombonelessCore
    PUBLIC
        TrombonelessCoreLibrary
)

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
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/tests
        )

    # Add the test to the CTest list of tests.
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()