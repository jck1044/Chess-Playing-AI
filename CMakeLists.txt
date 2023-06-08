cmake_minimum_required(VERSION 3.25)
project(Chess-Playing-AI)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chess-Playing-AI main.cpp)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Chess-Playing-AI sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
