cmake_minimum_required(VERSION 3.16)

project(tromboneless_core LANGUAGES C CXX)

# Use C++17 (no std::span)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Optional: export compile_commands.json for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Paths ----
set(VL53_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/vl53l1x_api)
set(VL53_CORE_DIR ${VL53_API_DIR}/core)
set(VL53_PLATFORM_DIR ${VL53_API_DIR}/platform)

# ---- Vendor library: VL53L1X API (C) ----
file(GLOB VL53_CORE_SOURCES
     "${VL53_CORE_DIR}/*.c"
)
file(GLOB VL53_PLATFORM_SOURCES
     "${VL53_PLATFORM_DIR}/*.c"
)

add_library(vl53l1x_api STATIC
    ${VL53_CORE_SOURCES}
    ${VL53_PLATFORM_SOURCES}
)

target_include_directories(vl53l1x_api PUBLIC
    ${VL53_CORE_DIR}
    ${VL53_PLATFORM_DIR}
)

# Some vendor C code can be noisy; keep warnings modest here
target_compile_options(vl53l1x_api PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra -Wno-unused-parameter>
)

# ---- Your application target ----
add_executable(app
    src/main.cpp
    src/drivers/i2c_bus.cpp
    # Add these when you create them:
    # src/drivers/gpio_interrupt.cpp
    # src/drivers/tof_sensor.cpp
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(app PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(app PRIVATE
    vl53l1x_api
)