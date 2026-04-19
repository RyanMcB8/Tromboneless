cmake_minimum_required(VERSION 3.22)

#   Setting the project name
project(TrombonelessJuce VERSION 1.0.0)

#   Setting the C standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

#   Adding the necessary packages before trying to compile.
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)
find_package(CURL REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#   Adding the subdirectory.
set(JUCE_DIR "${PROJECT_SOURCE_DIR}/modules/juce")

# List of JUCE modules you want
set(JUCE_MODULES
    juce_core
    juce_gui_basics
)

# Function to add JUCE module as a library
function(add_juce_module MODULE_NAME)
    add_library(${MODULE_NAME} STATIC
        ${JUCE_DIR}/modules/${MODULE_NAME}/juce_${MODULE_NAME}.cpp
        # Add any extra sources the module requires
    )
    target_include_directories(${MODULE_NAME} PUBLIC
        ${JUCE_DIR}/modules
        ${JUCE_DIR}/modules/${MODULE_NAME}
    )
endfunction()

# Add the modules
foreach(mod ${JUCE_MODULES})
    add_juce_module(${mod})
endforeach()

# Creation of the tromboneless juce library.
add_library(TrombonelessJuce) 
    
target_include_directories(TrombonelessJuce
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc)

target_sources(TrombonelessJuce
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/TrombonelessJuce.cpp)

target_link_libraries(TrombonelessJuce
    PUBLIC
        juce::juce_gui_basics
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
    PRIVATE
        juce::juce_core
)



