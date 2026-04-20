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

# add_subdirectory(App)
# Disable everything you don’t want
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS OFF CACHE BOOL "" FORCE)
set(JUCE_INSTALL_DESTINATION "lib/cmake/JUCE" CACHE STRING "" FORCE)
set(JUCE_BUILD_TESTS OFF CACHE BOOL "" FORCE)


add_subdirectory(App/modules/JUCE EXCLUDE_FROM_ALL)
add_subdirectory(Core)

#   Enabling tests
enable_testing()
include(CTest)

#   Find all source files in App/src.
file(GLOB APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/*.cpp")
list(REMOVE_ITEM APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/main.cpp")


add_library(TrombonelessAppLib SHARED ${APP_SOURCES})

#   Adding the include file directories. 
target_include_directories(TrombonelessAppLib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Shared
        ${PROJECT_SOURCE_DIR}/App/inc
        ${GTK3_INCLUDE_DIRS}
)

#   Linking the libraries


target_link_libraries(TrombonelessAppLib
    PUBLIC
        ${GTK3_LIBRARIES}
        CURL::libcurl
        TrombonelessCoreLibrary
        juce::juce_core
        juce::juce_gui_basics
        juce::juce_graphics
)

#   Creating the app executable.
juce_add_gui_app(Tromboneless
    PRODUCT_NAME "Tromboneless"
)

add_dependencies(Tromboneless TrombonelessAppLib)

target_sources(Tromboneless PUBLIC ${PROJECT_SOURCE_DIR}/App/src/main.cpp)


#   Adding additional compile defintions.
target_compile_definitions(TrombonelessAppLib PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Tromboneless,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Tromboneless,JUCE_VERSION>"
    
)

#target_include_directories(Tromboneless
#    PUBLIC
#        ${PROJECT_SOURCE_DIR}/Shared
#        ${PROJECT_SOURCE_DIR}/App/inc)

target_link_libraries(Tromboneless PUBLIC TrombonelessAppLib)

target_compile_options(Tromboneless PUBLIC ${GTK3_CFLAGS_OTHER})

set_target_properties(Tromboneless PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ========================================
#                   Tests
# ========================================
# Automatically find all test cpp files in tests/
if(BUILD_TESTING)
    file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/App/tests/*.cpp")

    # Searches through all C files within the tests directory and treats them as test script.
    foreach(TEST_FILE ${TEST_SOURCES} )
        # Extract the file name without path and extension
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        add_executable(${TEST_NAME} ${TEST_FILE})
	add_dependencies(${TEST_NAME} TrombonelessAppLib)
        target_link_libraries(${TEST_NAME} LINK_PUBLIC TrombonelessAppLib)
        target_include_directories(${TEST_NAME} PRIVATE 
            ${PROJECT_SOURCE_DIR}/App/inc
            ${PROJECT_SOURCE_DIR}/App/tests
            )

        # Add the test to the CTest list of tests.
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()
