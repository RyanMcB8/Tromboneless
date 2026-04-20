cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIDAR_DIR ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API)

link_directories(/usr/local/lib)

enable_testing()
include(CTest)

add_subdirectory(modules/rtmidi)
add_subdirectory(modules/rpi_ads1115)

target_compile_definitions(rtmidi PRIVATE __LINUX_ALSA__)
target_link_libraries(rtmidi PUBLIC asound)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

add_library(TrombonelessCoreLibrary STATIC
    ${SOURCES}
    ${PROJECT_SOURCE_DIR}/modules/rpi_ads1115/ads1115rpi.cpp
    ${PROJECT_SOURCE_DIR}/../Shared/tromboneless_data.cpp
    ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/core/VL53L1X_api.c
    ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/platform/vl53l1_platform.cpp
)

target_include_directories(TrombonelessCoreLibrary PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/drivers
    ${PROJECT_SOURCE_DIR}/Drivers
    ${PROJECT_SOURCE_DIR}/../Shared
    ${PROJECT_SOURCE_DIR}/modules/rpi_ads1115
    ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/core
    ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/platform
    )

target_link_libraries(TrombonelessCoreLibrary PUBLIC
    rtmidi
    gpiod
    gpiodcxx
    asound
)

# Ignoring warning from RtMidi library.
target_compile_options(TrombonelessCoreLibrary
    PRIVATE
        -Wno-zero-as-null-pointer-constant
)
add_executable(TrombonelessCore)

target_link_libraries(TrombonelessCore PUBLIC
    TrombonelessCoreLibrary
)

file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE TrombonelessCoreLibrary)
    target_include_directories(${TEST_NAME} PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/tests
    )


    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
