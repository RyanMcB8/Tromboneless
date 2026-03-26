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