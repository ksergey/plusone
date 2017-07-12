cmake_minimum_required(VERSION 3.0)

# Project name
project(plusone VERSION 1 LANGUAGES CXX)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

# Setting the cpp standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_EXAMPLES "Build library examples" ON)

# Libraries
set(libs)

# Create library entry
add_library(plusone INTERFACE)
target_include_directories(plusone INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plusone INTERFACE ${libs})

# Build examples
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
