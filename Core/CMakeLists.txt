cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enabling the unit tests:
enable_testing()

add_executable(TrombonelessCore src/main.cpp)
# add_executable(TestCore tests/testSynth.cpp)

# target_link_libraries(TestInstantiator vtkCommon)
# add_test(NAME TestCore
#          COMMAND TestCore)

# target_include_directories(TestCore PRIVATE
#     ${PROJECT_SOURCE_DIR}/include
#     )    

# target_sources(TestCore
#     PRIVATE
#         ${PROJECT_SOURCE_DIR}/tests
#         )

target_include_directories(TrombonelessCore PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Drivers
    # ${PROJECT_SOURCE_DIR}/shared
    )

# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
target_sources(TrombonelessCore
    PRIVATE
        ${SOURCES}
        )


# # Add shared source files
# file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/shared/*.cpp")
# target_sources(TrombonelessCore
#     PRIVATE
#         ${SOURCES}
#         )
