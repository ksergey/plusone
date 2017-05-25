cmake_minimum_required(VERSION 3.0)

# Project name
project(plusone)

# Setting the cpp standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(PLUSONE_BUILD_EXAMPLES "Build library examples" ON)
option(PLUSONE_USE_CURL "Enable Curl extension" ON)

# Libraries
set(libs)

# CURL extension
if (PLUSONE_USE_CURL)
    find_package(CURL REQUIRED)
    include_directories(${CURL_INCLUDE_DIR})
    set(libs ${libs} ${CURL_LIBRARIES})
endif()

# Create library entry
add_library(plusone INTERFACE)
target_include_directories(plusone INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plusone INTERFACE ${libs})

# Build examples
if (PLUSONE_BUILD_EXAMPLES)
    message(STATUS "Plusone: build examples (yes)")
    add_subdirectory(examples)
else()
    message(STATUS "Plusone: build examples (no)")
endif()
