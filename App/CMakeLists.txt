cmake_minimum_required(VERSION 3.22)

project(Tromboneless VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add JUCE as a subdirectory (assuming it's added as a git submodule)
add_subdirectory(JUCE)

# Create the GUI application
juce_add_gui_app(Tromboneless
    PRODUCT_NAME "Tromboneless")

target_include_directories(Tromboneless PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/shared
    )

# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
target_sources(Tromboneless
    PRIVATE
        ${SOURCES})

# Set compile definitions
target_compile_definitions(Tromboneless
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Tromboneless,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Tromboneless,JUCE_VERSION>")

# Link libraries
target_link_libraries(Tromboneless
    PRIVATE
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)