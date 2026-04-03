cmake_minimum_required(VERSION 3.22)

project(TrombonelessCore VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Enabling the unit tests:
enable_testing()

add_executable(TrombonelessCore src/main.cpp)

#ALSA
target_compile_definitions(TrombonelessCore PRIVATE __LINUX_ALSA__)
target_link_libraries(TrombonelessCore PRIVATE asound)

target_include_directories(TrombonelessCore PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/rtmidi
    ${PROJECT_SOURCE_DIR}/shared
    )

# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
target_sources(TrombonelessCore
    PRIVATE
        ${SOURCES}
        )

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/lib/rtmidi/*.cpp")
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
