cmake_minimum_required(VERSION 3.10)

project(ToF_Project)

# Use modern standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

# Optional but recommended flags
add_compile_options(-Wall -Wextra -Wpedantic)

# ---------------------------
# Include directories
# ---------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/vl53l1x_api/api/core
    ${CMAKE_SOURCE_DIR}/external/vl53l1x_api/api/platform
)

# ---------------------------
# Your C++ source files
# ---------------------------
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/drivers/i2c_bus.cpp
    ${CMAKE_SOURCE_DIR}/src/drivers/tof_sensor.cpp
)

# ---------------------------
# ST VL53L1 C driver sources
# ---------------------------
file(GLOB VL53_SOURCES
    ${CMAKE_SOURCE_DIR}/external/vl53l1x_api/api/core/*.c
    ${CMAKE_SOURCE_DIR}/external/vl53l1x_api/api/platform/*.c
)

# ---------------------------
# Create executable
# ---------------------------
add_executable(tof_test
    ${SOURCES}
    ${VL53_SOURCES}
)