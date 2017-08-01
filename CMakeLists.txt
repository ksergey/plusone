cmake_minimum_required(VERSION 3.0)

project(plusone VERSION 10 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(Plusone_BUILD_EXAMPLES "Build library examples" ON)
option(Plusone_BUILD_TESTS "Build library tests" ON)
option(Plusone_USE_SSL "Build library with SSL support" ON)

# Don't build doctest tests
set(DOCTEST_WITH_TESTS OFF CACHE BOOL "")

add_subdirectory(deps/fmt EXCLUDE_FROM_ALL)
add_subdirectory(deps/doctest EXCLUDE_FROM_ALL)

add_library(plusone INTERFACE)
target_include_directories(plusone INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plusone INTERFACE fmt::fmt-header-only)

# Find OpenSSL in case use SSL
if (Plusone_USE_SSL)
    find_package(OpenSSL REQUIRED)
    target_include_directories(plusone SYSTEM INTERFACE ${OPENSSL_INCLUDE_DIRS})
    target_link_libraries(plusone INTERFACE ${OPENSSL_LIBRARIES})
endif()

if (Plusone_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (Plusone_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
