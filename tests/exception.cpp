/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cmath>
#include <plusone/exception.hpp>

using plusone::throw_ex;

TEST_CASE("throw_ex 1")
{
    try {
        throw_ex< plusone::exception >("Hello {}", "World");
    } catch(const plusone::exception& e) {
        REQUIRE( std::string("Hello World") == e.what() );
    }

    try {
        throw_ex< std::runtime_error >("Hello {}", "World");
    } catch(const std::runtime_error& e) {
        REQUIRE( std::string("Hello World") == e.what() );
    }

    try {
        throw_ex< std::runtime_error >("Hello {}", "World");
    } catch(const std::exception& e) {
        REQUIRE( std::string("Hello World") == e.what() );
    }
}
