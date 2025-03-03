cmake_minimum_required(VERSION 3.25)
project(Chess-Playing-AI)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chess-Playing-AI chess.cpp gameplay.cpp gui.cpp
        test.cpp)
include_directories(/usr/local/include)

#find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
#include_directories(${SFML_INCLUDE_DIRS})
#target_link_libraries(Chess-Playing-AI sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Set the path to the SFML libraries
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1-macos-clang/lib")

# Find and link SFML libraries
find_library(SFML_GRAPHICS sfml-graphics PATHS ${SFML_DIR} REQUIRED)
find_library(SFML_WINDOW sfml-window PATHS ${SFML_DIR} REQUIRED)
find_library(SFML_SYSTEM sfml-system PATHS ${SFML_DIR} REQUIRED)

target_link_libraries(Chess-Playing-AI
        ${SFML_GRAPHICS}
        ${SFML_WINDOW}
        ${SFML_SYSTEM}
        )