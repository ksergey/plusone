cmake_minimum_required(VERSION 3.0)

project(plusone VERSION 3 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PLUSONE_BUILD_EXAMPLES "Build library examples" ON)

add_subdirectory(deps/fmt EXCLUDE_FROM_ALL)

set(libs fmt::fmt-header-only)

add_library(plusone INTERFACE)
target_include_directories(plusone INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plusone INTERFACE ${libs})

if (PLUSONE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
