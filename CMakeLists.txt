cmake_minimum_required(VERSION 3.13)
project(ntio_sockets VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(LOG_LEVEL "LOG_LEVEL_DEBUG" CACHE STRING "Log level LOG_LEVEL_<LEVEL>. Use empty string to switch it off")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

add_subdirectory(src)
add_subdirectory(examples)