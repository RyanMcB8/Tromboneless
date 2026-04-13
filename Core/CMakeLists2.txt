cmake_minimum_required(VERSION 3.10)

project(ToF_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

set(VL53L1_ULD_ROOT
    ${CMAKE_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5
)

add_executable(tof_test
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/drivers/i2c_bus.cpp
    ${CMAKE_SOURCE_DIR}/src/drivers/tof_sensor.cpp
    ${VL53L1_ULD_ROOT}/API/platform/vl53l1_platform.cpp
    ${VL53L1_ULD_ROOT}/API/core/VL53L1X_api.c
)

target_include_directories(tof_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${VL53L1_ULD_ROOT}/API/platform
    ${VL53L1_ULD_ROOT}/API/core
)

target_link_libraries(tof_test
    gpiodcxx
    gpiod
)