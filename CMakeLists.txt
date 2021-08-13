cmake_minimum_required(VERSION 3.10)
project(zxy)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_FLAGS "-g -pthread")
link_libraries(/usr/local/lib/libgtest.a)

add_subdirectory(string)
