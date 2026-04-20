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
find_package(CURL REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#   Adding the subdirectory.
set(JUCE_USE_PCH 1)
add_subdirectory(Core)
add_subdirectory(App/modules/JUCE)

#   Enabling tests
enable_testing()
include(CTest)

#   Find all source files in App/src.
file(GLOB APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/*.cpp")
#file(GLOB_RECURSE JUCE_BINARY "${JUCE_LOC}/../extras/BinaryBuilder/*.cpp")
list(REMOVE_ITEM APP_SOURCES "${PROJECT_SOURCE_DIR}/App/src/main.cpp")

juce_add_gui_app(Tromboneless
    ${PROJECT_SOURCE_DIR}/App/src/main.cpp
)

add_library(TrombonelessAppLib SHARED 
	${APP_SOURCES} 
)

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
	juce::juce_graphics
	juce::juce_gui_basics
)

target_sources(Tromboneless PUBLIC ${PROJECT_SOURCE_DIR}/App/src/main.cpp)


#   Adding additional compile defintions.
target_compile_definitions(TrombonelessAppLib PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="Tromboneless"
    JUCE_APPLICATION_VERSION_STRING="1.0.0"
    
)



target_link_libraries(Tromboneless PUBLIC 
	TrombonelessAppLib 
	)

target_compile_options(Tromboneless PRIVATE ${GTK3_CFLAGS_OTHER})

set_target_properties(Tromboneless PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ========================================
#                   Tests
# ========================================
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/App/tests/*.cpp")

add_executable(TrombonelessAppTests ${TEST_SOURCES})

target_link_libraries(TrombonelessAppTests
    PUBLIC TrombonelessAppLib
)

target_include_directories(TrombonelessAppTests PRIVATE
    ${PROJECT_SOURCE_DIR}/App/inc
    ${PROJECT_SOURCE_DIR}/App/tests
)

add_test(NAME TrombonelessAppTests COMMAND TrombonelessAppTests)