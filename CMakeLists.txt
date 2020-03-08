cmake_minimum_required(VERSION 3.12)

file (STRINGS "${CMAKE_SOURCE_DIR}/VERSION" globus_version)

project(globus-plasma VERSION ${globus_version})

message("Globus ${globus_version}")

set(CMAKE_CXX_STANDARD 14)

option (use_python "Build Python wrapper"      ON)
option (use_debug  "Build with debug messages" ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/_stage-${globus_version})

if (UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wall -Wextra -fPIC -Werror=return-type")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    if (NOT use_debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
    endif()
elseif(MSVC)
   if (NOT use_debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(src/)

if (use_python)
    add_subdirectory(libs/pybind11)
endif()

add_subdirectory(src/)
