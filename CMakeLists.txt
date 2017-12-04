cmake_minimum_required(VERSION 3.0)

project(plusone VERSION 22 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

# Doctest download target
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# TODO(me): Remove from this CMake?
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_directory_property(hasParent PARENT_DIRECTORY)
if (hasParent)
    set(BUILD_TEST_AND_EXAMPLES OFF)
else()
    set(BUILD_TEST_AND_EXAMPLES ON)
endif()

option(Plusone_BUILD_EXAMPLES "Build library examples" ${BUILD_TEST_AND_EXAMPLES})
option(Plusone_BUILD_TESTS "Build library tests" ${BUILD_TEST_AND_EXAMPLES})
option(Plusone_USE_SSL "Build library with SSL support" OFF)
option(Plusone_VDSO_CLOCK_GETTIME "Build library with VDSO clock_gettime" OFF)

# Use external fmt lib if target exists
if (NOT TARGET fmt::fmt-header-only)
    add_subdirectory(deps/fmt EXCLUDE_FROM_ALL)
endif()

# Create library
add_library(plusone INTERFACE)
add_library(ksergey::plusone ALIAS plusone)
target_include_directories(plusone INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plusone INTERFACE fmt::fmt-header-only)

# Find OpenSSL in case use SSL
if (Plusone_USE_SSL)
    find_package(OpenSSL REQUIRED)
    target_include_directories(plusone SYSTEM INTERFACE ${OPENSSL_INCLUDE_DIRS})
    target_link_libraries(plusone INTERFACE ${OPENSSL_LIBRARIES})
endif()

if (Plusone_VDSO_CLOCK_GETTIME)
    target_compile_definitions(plusone INTERFACE -DPLUSONE_VDSO_CLOCK_GETTIME)
    target_link_libraries(plusone INTERFACE dl)
endif()

if (Plusone_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (Plusone_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
