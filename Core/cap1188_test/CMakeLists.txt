cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(cap1188_test)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiodcxx)

add_executable(cap1188_test
    cap1188_test.cpp
    ../src/drivers/cap_sensor.cpp
    ../src/drivers/i2c_bus.cpp
)

target_include_directories(cap1188_test PRIVATE
    ../include
    ${GPIOD_INCLUDE_DIRS}
)

target_link_libraries(cap1188_test
    ${GPIOD_LIBRARIES}
    Threads::Threads
)

find_package(Threads REQUIRED)