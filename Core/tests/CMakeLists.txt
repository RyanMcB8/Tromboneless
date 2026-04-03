cmake_minimum_required(VERSION 3.22)

# Name this little standalone test project
project(TrombonelessCoreTests VERSION 1.0.0)

# Use C++20 for everything in this test project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json, which helps VS Code / clangd / IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to the parent project root, relative to THIS CMakeLists.txt file.
# If this test folder sits directly inside Core/, then .. is correct.
set(PROJECT_ROOT ..)

# Create the test executable from the test's own main/source file.
# Only files listed here or added later with target_sources() get compiled.
add_executable(TrombonelessCoreTests
    synth_audio_test.cpp
)

# Tell the compiler where to look for headers when it sees #include "...".
# These are directories, not files, so do NOT use * here.
#
# Add whichever folders actually contain the headers your test includes.
# For example:
# - use ${PROJECT_ROOT}/include if you have a dedicated include folder
# - use ${PROJECT_ROOT}/src if headers are included like "drivers/audio_output.hpp"
target_include_directories(TrombonelessCoreTests PRIVATE
    ${PROJECT_ROOT}/include
    ${PROJECT_ROOT}/include/drivers
    ${PROJECT_ROOT}/src
)

# Add specific .cpp implementation files needed by this test.
# This is where you explicitly say which project source files should be compiled
# into this test executable.
#
# Nothing else is compiled unless you list it here or in add_executable().
target_sources(TrombonelessCoreTests PRIVATE
    ${PROJECT_ROOT}/src/drivers/audio_output.cpp
    ${PROJECT_ROOT}/src/synth.cpp
    ${PROJECT_ROOT}/src/tromboneSynth.cpp
)

# Link external libraries needed by the source files above.
# audio_output.cpp uses ALSA functions like snd_pcm_open, so we must link asound.
target_link_libraries(TrombonelessCoreTests PRIVATE
    asound
)