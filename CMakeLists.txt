cmake_minimum_required(VERSION 2.8.9)

project(nearestNeighbor)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(nearestNeighbor ${SOURCES})
