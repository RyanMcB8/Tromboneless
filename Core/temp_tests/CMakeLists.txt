cmake_minimum_required(VERSION 3.22)

# Name this standalone test project
project(TrombonelessCoreTests VERSION 1.0.0 LANGUAGES C CXX)

# Use C++20 for everything in this test project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# /srv/project/Tromboneless/Core/modules/external/libgpiod/bindings/cxx/internal.hpp
# /srv/project/Tromboneless/Core/modules/external/libgpiod/lib/internal.h

set(LIDAR_DIR ${PROJECT_SOURCE_DIR}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API)

# Debug Mode
set(CMAKE_BUILD_TYPE Debug)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to the parent project root
set(PROJECT_ROOT ..)

# Add test executable
add_executable(TrombonelessCoreTests
    # synth_audio_test.cpp
    # midi_test.cpp
    # internal_synth_bend_test.cpp
    rendering_internal_synth.cpp
)

# Include directories for the core library and any headers needed
target_include_directories(TrombonelessCoreTests PRIVATE
    ${PROJECT_ROOT}/include/drivers
    ${PROJECT_ROOT}/include
    ${PROJECT_ROOT}/temp_tests
    ${PROJECT_ROOT}/modules/rpi_ads1115
    ${PROJECT_ROOT}/modules/rtmidi
    ${PROJECT_ROOT}/../Shared
    ${PROJECT_ROOT}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/core
    ${PROJECT_ROOT}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/platform
    ${LIDAR_DIR}/core
    ${LIDAR_DIR}/platform
)

# Add source files
target_sources(TrombonelessCoreTests PRIVATE

    ${PROJECT_ROOT}/src/drivers/audio_output.cpp
    ${PROJECT_ROOT}/src/drivers/tof_sensor.cpp
    ${PROJECT_ROOT}/src/drivers/i2c_bus.cpp
    ${PROJECT_ROOT}/src/drivers/USBMidi.cpp
    ${PROJECT_ROOT}/src/drivers/cap_sensor.cpp  

    ${PROJECT_ROOT}/src/MidiCoordinator.cpp
    ${PROJECT_ROOT}/src/MidiMessage.cpp
    ${PROJECT_ROOT}/src/MidiMessageBuilder.cpp
    ${PROJECT_ROOT}/src/eventHandler.cpp

    ${PROJECT_ROOT}/src/audioRender.cpp

    ${PROJECT_ROOT}/src/synth.cpp
    ${PROJECT_ROOT}/src/tromboneSynth.cpp
    ${PROJECT_ROOT}/src/envelope.cpp  

    ${PROJECT_ROOT}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/core/VL53L1X_api.c
    ${PROJECT_ROOT}/external/STSW-IMG009/STSW-IMG009_v3.5.5/API/platform/vl53l1_platform.cpp

    ${PROJECT_ROOT}/modules/rtmidi/RtMidi.cpp
    ${PROJECT_ROOT}/modules/rpi_ads1115/ads1115rpi.cpp
)

link_directories("/usr/local/lib/")
target_link_libraries(TrombonelessCoreTests PRIVATE
    asound   
    gpiod 
    gpiodcxx
)

target_compile_definitions(TrombonelessCoreTests PRIVATE __LINUX_ALSA__)

# Enable CTest for this test executable
enable_testing()
add_test(NAME SynthAudioTest COMMAND TrombonelessCoreTests)