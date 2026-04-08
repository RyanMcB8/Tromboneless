cmake_minimum_required(VERSION 3.22)

# Name this standalone test project
project(TrombonelessCoreTests VERSION 1.0.0 LANGUAGES CXX)

# Use C++20 for everything in this test project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to the parent project root
set(PROJECT_ROOT ..)

# Add test executable
add_executable(TrombonelessCoreTests
    # synth_audio_test.cpp
    # midi_test.cpp
    synth_internal_bend_test.cpp
)

# Include directories for the core library and any headers needed
target_include_directories(TrombonelessCoreTests PRIVATE
    ${PROJECT_ROOT}/include/drivers
    ${PROJECT_ROOT}/include
    ${PROJECT_ROOT}/tests
    ${PROJECT_ROOT}/modules/rtmidi
)

# Add source files
target_sources(TrombonelessCoreTests PRIVATE

    ${PROJECT_ROOT}/src/drivers/audio_output.cpp
    #${PROJECT_ROOT}/src/drivers/ads1115rpi.cpp
    ${PROJECT_ROOT}/src/drivers/tof_sensor.cpp
    ${PROJECT_ROOT}/src/drivers/i2c_bus.cpp
    ${PROJECT_ROOT}/src/drivers/USBMidi.cpp

    ${PROJECT_ROOT}/src/MidiCoordinator.cpp
    ${PROJECT_ROOT}/src/MidiMessage.cpp
    ${PROJECT_ROOT}/src/MidiMessageBuilder.cpp
    ${PROJECT_ROOT}/src/eventHandler.cpp
    ${PROJECT_ROOT}/src/synth.cpp
    ${PROJECT_ROOT}/src/tromboneSynth.cpp

    ${PROJECT_ROOT}/modules/rtmidi/RtMidi.cpp

)

target_link_libraries(TrombonelessCoreTests PRIVATE
    asound    
    gpiodcxx
)

target_compile_definitions(TrombonelessCoreTests PRIVATE __LINUX_ALSA__)

# Enable CTest for this test executable
enable_testing()
add_test(NAME SynthAudioTest COMMAND TrombonelessCoreTests)