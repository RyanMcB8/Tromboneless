cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wconversion -pedantic)

# === Executable ===
add_executable(TrombonelessCore)

# === Include directories ===
target_include_directories(TrombonelessCore PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/rtmidi
)

# === Source files ===
# All your project source (including ADS + coordinator etc)
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# RtMidi source
file(GLOB RTMIDI_SRC "${PROJECT_SOURCE_DIR}/lib/rtmidi/*.cpp")

target_sources(TrombonelessCore PRIVATE
    ${SRC_FILES}
    ${RTMIDI_SRC}
)

# === ALSA + GPIO ===
target_compile_definitions(TrombonelessCore PRIVATE __LINUX_ALSA__)
target_link_libraries(TrombonelessCore PRIVATE asound gpiodcxx)
cmake_minimum_required(VERSION 3.10)

project(ToF_Project)

# Language standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Your executable
add_executable(tof_test
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/drivers/i2c_bus.cpp
    ${CMAKE_SOURCE_DIR}/src/drivers/tof_sensor.cpp
)

# Include paths
target_include_directories(tof_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link required libraries
target_link_libraries(tof_test
    gpiodcxx
    gpiod
)
