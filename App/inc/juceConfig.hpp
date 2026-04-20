

#pragma once

// Define the platform macros for JUCE
#if defined(_WIN32) || defined(_WIN64)
    #define JUCE_WINDOWS 1
#elif defined(__APPLE__)
    #define JUCE_MAC 1
#elif defined(__linux__)
    #define JUCE_LINUX 1
#elif defined(__ANDROID__)
    #define JUCE_ANDROID 1
#else
    #error "Unknown platform!"
#endif

// Set the architecture (for 64-bit systems)
#if defined(__x86_64__) || defined(_WIN64)
    #define JUCE_64BIT 1
#else
    #define JUCE_32BIT 1
#endif

// Set endianness
#if defined(__LITTLE_ENDIAN__)
    #define JUCE_LITTLE_ENDIAN 1
#else
    #define JUCE_BIG_ENDIAN 1
#endif

// Define the compiler being used
#if defined(__clang__)
    #define JUCE_CLANG 1
#elif defined(__GNUC__)
    #define JUCE_GCC 1
#elif defined(_MSC_VER)
    #define JUCE_MSVC 1
#else
    #error "Unknown compiler"
#endif

// You can add other project-wide settings here
