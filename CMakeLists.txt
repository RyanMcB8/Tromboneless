cmake_minimum_required(VERSION 3.22)

#   Setting the project name
project(Tromboneless VERSION 1.0.0)

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

add_subdirectory(App/modules/JUCE)
add_subdirectory(Core)

#   Enabling tests
enable_testing()
include(CTest)

#   Find all source files in App/src.
file(GLOB_RECURSE APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/*.cpp")
list(REMOVE_ITEM APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/main.cpp")

add_library(TrombonelessJuceLib)
add_library(TrombonelessAppLib STATIC ${APP_SOURCES})

#   Adding the include file directories. 
target_include_directories(TrombonelessAppLib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Shared
        ${PROJECT_SOURCE_DIR}/App/inc
        ${GTK3_INCLUDE_DIRS}
)

#   Linking the libraries

target_link_libraries(TrombonelessJuceLib
    PUBLIC
        juce::juce_gui_basics
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
target_link_libraries(TrombonelessAppLib
    PUBLIC
        ${GTK3_LIBRARIES}
        CURL::libcurl
        TrombonelessJuceLib
        TrombonelessCoreLibrary
)

#   Creating the app executable.
juce_add_gui_app(TrombonelessApp
    PRODUCT_NAME "Tromboneless"
)

target_sources(TrombonelessApp PRIVATE ${PROJECT_SOURCE_DIR}/App/src/main.cpp )

#   Adding the source files.
target_sources(TrombonelessAppLib PUBLIC ${APP_SOURCES})


#   Adding additional compile defintions.
target_compile_definitions(TrombonelessAppLib PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:TrombonelessApp,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:TrombonelessApp,JUCE_VERSION>"
    
)

target_link_libraries(TrombonelessApp PRIVATE TrombonelessAppLib)

target_compile_options(TrombonelessApp PRIVATE ${GTK3_CFLAGS_OTHER})

# ========================================
#                   Tests
# ========================================
# Automatically find all test cpp files in tests/
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/App/tests/*.cpp")

# Searches through all C files within the tests directory and treats them as test script.
foreach(TEST_FILE ${TEST_SOURCES} )
    # Extract the file name without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE TrombonelessAppLib)
    target_include_directories(${TEST_NAME} PRIVATE 
        ${PROJECT_SOURCE_DIR}/App/inc
        ${PROJECT_SOURCE_DIR}/App/tests
        )

    target_compile_options(${TEST_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})


    # Add the test to the CTest list of tests.
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()