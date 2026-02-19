cmake_minimum_required(VERSION 3.23)

project(tromboneless_SOF)

add_executable(trombonelessProgram)
target_sources(trombonelessProgram
    PRIVATE
        main.cpp
)

add_library(windowCreation)
find_package(wxWidgets REQUIRED)

target_sources(windowCreation
  PRIVATE
    src/Application.cpp
    src/MainFrame.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/Application.h
      include/MainFrame.h
)




# Creates a subdirectory for the project.
# add_subdirectory(windowCreation)

# target_link_libraries(generalProgram
#   PRIVATE
#     generalLibrary
# )

# target_sources(generalLibrary
#   # Adding any private headers to be compiled
#   PRIVATE
#     FILE_SET internalOnlyHeaders
#     TYPE HEADERS
#     FILES
#     #   InternalOnlyHeader.h

#   # Adding interface headers to be compiled
#   INTERFACE
#     FILE_SET consumerOnlyHeaders
#     TYPE HEADERS
#     FILES
#     #   ConsumerOnlyHeader.h

#   # Adding public headers to be compiled
#   PUBLIC
#     FILE_SET publicHeaders
#     TYPE HEADERS
#     FILES
#     #   PublicHeader.h
# )