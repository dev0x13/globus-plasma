cmake_minimum_required(VERSION 3.8)
project(globus-plasma)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wall -Wextra -Wno-return-type -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(src/)

add_subdirectory(libs/pybind11)

add_subdirectory(src/)
