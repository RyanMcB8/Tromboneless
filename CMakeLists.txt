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


#   Enabling tests
enable_testing()
include(CTest)

#   Find all source files in App/src.
file(GLOB_RECURSE APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/*.cpp")

#   Creating the app executable.
juce_add_gui_app(TrombonelessApp
    PRODUCT_NAME "Tromboneless"
)

#   Adding the source files.
target_sources(TrombonelessApp PRIVATE ${APP_SOURCES})

#   Adding the include file directories. 
target_include_directories(TrombonelessApp
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Shared
        ${PROJECT_SOURCE_DIR}/App/inc
    PRIVATE
        ${GTK3_INCLUDE_DIRS}
)

#   Adding additional compile defintions.
target_compile_definitions(TrombonelessApp PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:TrombonelessApp,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:TrombonelessApp,JUCE_VERSION>"
)

#   Linking the libraries
target_link_libraries(TrombonelessApp
    PRIVATE
        juce::juce_gui_basics
        ${GTK3_LIBRARIES}
        CURL::libcurl
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        TrombonelessCoreLibrary
)

#   GTK compile flags
target_compile_options(TrombonelessApp PRIVATE ${GTK3_CFLAGS_OTHER})
