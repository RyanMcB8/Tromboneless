cmake_minimum_required(VERSION 3.22)

project(TrombonelessApp VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adding the necessary packages for juce to build.
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
find_package(PkgConfig REQUIRED)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)
find_package(CURL REQUIRED)

# Generate compile_commands.json for better IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding the shared file for intercommunication between the App and core
add_subdirectory(../Shared ${CMAKE_BINARY_DIR}/Shared)

# Add JUCE as a subdirectory (assuming it's added as a git submodule)
add_subdirectory(modules/JUCE)

# Enable unit tests
enable_testing()
include(CTest)

# ========================================
#               Main Library
# ========================================
# Add source files
file(GLOB_RECURSE APP_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(TrombonelessAppLibrary STATIC ${APP_SOURCES})

target_include_directories(TrombonelessAppLibrary PUBLIC
${PROJECT_SOURCE_DIR}/inc
PRIVATE
${GTK3_INCLUDE_DIRS}
)

# Create the GUI application
juce_add_gui_app(TrombonelessApp
    PRODUCT_NAME "Tromboneless")

# Set compile definitions
target_compile_definitions(TrombonelessAppLibrary
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:TrombonelessAppLibrary,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:TrombonelessAppLibrary,JUCE_VERSION>")

# Link libraries
target_link_libraries(TrombonelessAppLibrary
    PUBLIC
        juce::juce_gui_basics
        TrombonelessSharedLibrary
        ${GTK3_LIBRARIES}
        CURL::libcurl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# ========================================
#               Main executable
# ========================================

target_include_directories(TrombonelessApp PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(TrombonelessApp PUBLIC TrombonelessAppLibrary CURL::libcurl ${GTK3_LIBRARIES})
target_compile_options(TrombonelessApp PRIVATE ${GTK3_CFLAGS_OTHER})

# ========================================
#                   Tests
# ========================================
# Automatically find all test cpp files in tests/
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

# Searches through all C files within the tests directory and treats them as test script.
foreach(TEST_FILE ${TEST_SOURCES})
    # Extract the file name without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE TrombonelessAppLibrary)
    target_include_directories(${TEST_NAME} PRIVATE 
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/tests
        )

    # Add the test to the CTest list of tests.
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

