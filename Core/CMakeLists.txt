cmake_minimum_required(VERSION 3.22)

project(Tromboneless VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wconversion -Wpedantic)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(midi_tof_test
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${SRC_FILES}
)

file(GLOB RTMIDI_SRC CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/lib/rtmidi/*.cpp"
)

target_sources(midi_tof_test PRIVATE
    ${RTMIDI_SRC}
)

target_include_directories(midi_tof_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/rtmidi
)

target_compile_definitions(midi_tof_test PRIVATE
    __LINUX_ALSA__
)

target_link_libraries(midi_tof_test PRIVATE
    asound
    gpiodcxx
    gpiod
)