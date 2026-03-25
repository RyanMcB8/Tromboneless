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