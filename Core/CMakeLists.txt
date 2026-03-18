# Cmake version
cmake_minimum_required(VERSION 3.13)

project(
    # Project name
    tromboneless_firmware

    # Version number
    VERSION 0.0.1

    # Enable languages
    LANGUAGES C CXX ASM
)
include(GNUInstallDirs)

set(TARGET_NAME ${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# src directory
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
# test directory
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
# inc directory
set(INC_DIR ${PROJECT_SOURCE_DIR}/inc)
# lib directory
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

# Collect all source files
file(GLOB TARGET_SRCS
    ${SOURCE_DIR}/*.cpp
    ${TEST_DIR}/*.cpp
    
)


# Create the executable target
add_executable(${TARGET_NAME} ${TARGET_SRCS})

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE
    # Project include
    ${INC_DIR}

)

# Link libraries
target_link_libraries(${TARGET_NAME}
    # include libraries used in code!
    gpiodcxx
    # ads1115rpi
    i2c
    #...
)

# Compile definitions
target_compile_definitions(${TARGET_NAME} PUBLIC
    # Enable logging
    ENABLE_LOGGING=1
)


# Compile options
target_compile_options(${TARGET_NAME} PRIVATE
    -Og
    -Wall
)

# Testing
enable_testing()

file(GLOB TEST_SRCS
    ${TEST_DIR}/*.cpp
    ${LIB_DIR}/*.cpp
)

if(TEST_SRCS)
    add_executable(${TARGET_NAME}_tests ${TEST_SRCS})

    target_include_directories(${TARGET_NAME}_tests PRIVATE 
    ${INC_DIR}
    ${LIB_DIR}
    )

    target_link_libraries(${TARGET_NAME}_tests
        gpiodcxx
        i2c
    )

    add_test(NAME unit_tests COMMAND ${TARGET_NAME}_tests)
endif()
