/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/static_string.hpp>

TEST_CASE("Static string uninitialized 0")
{
    constexpr auto uninitialized = plusone::static_string< 8 >();
    REQUIRE( uninitialized.size() == 0 );
    REQUIRE( uninitialized.empty() );
    REQUIRE( uninitialized.data()[0] == '\0' );
    REQUIRE( uninitialized[0] == '\0' );
    REQUIRE( uninitialized.at(0) == '\0' );
    REQUIRE( uninitialized.capacity() == 8 );

}

TEST_CASE("Static string uninitialized 1")
{
    auto uninitialized = plusone::static_string< 8 >();
    REQUIRE( uninitialized.size() == 0 );
    REQUIRE( uninitialized.empty() );
    REQUIRE( uninitialized.data()[0] == '\0' );
    REQUIRE( uninitialized[0] == '\0' );
    REQUIRE( uninitialized.at(0) == '\0' );
    REQUIRE( uninitialized.capacity() == 8 );
    CHECK_THROWS_AS( uninitialized.at(1), const std::out_of_range& );

    uninitialized.push_back('X');

    REQUIRE( uninitialized.size() == 1 );
    REQUIRE( !uninitialized.empty() );
    REQUIRE( uninitialized.data()[0] == 'X' );
    REQUIRE( uninitialized.data()[1] == '\0' );
    REQUIRE( uninitialized[0] == 'X' );
    REQUIRE( uninitialized.at(0) == 'X' );
    REQUIRE( uninitialized.at(1) == '\0' );
    REQUIRE( uninitialized.capacity() == 8 );
    REQUIRE( uninitialized.front() == 'X' );
    REQUIRE( uninitialized.back() == 'X' );

    uninitialized.push_back('Y');

    REQUIRE( uninitialized.size() == 2 );
    REQUIRE( !uninitialized.empty() );
    REQUIRE( uninitialized.data()[0] == 'X' );
    REQUIRE( uninitialized.data()[1] == 'Y' );
    REQUIRE( uninitialized.data()[2] == '\0' );
    REQUIRE( uninitialized[0] == 'X' );
    REQUIRE( uninitialized[1] == 'Y' );
    REQUIRE( uninitialized.at(0) == 'X' );
    REQUIRE( uninitialized.at(1) == 'Y' );
    REQUIRE( uninitialized.capacity() == 8 );
    REQUIRE( uninitialized.front() == 'X' );
    REQUIRE( uninitialized.back() == 'Y' );

    uninitialized.clear();
    REQUIRE( uninitialized.size() == 0 );
    REQUIRE( uninitialized.empty() );
    REQUIRE( uninitialized.data()[0] == '\0' );
    REQUIRE( uninitialized[0] == '\0' );
    REQUIRE( uninitialized.at(0) == '\0' );
    REQUIRE( uninitialized.capacity() == 8 );
    CHECK_THROWS_AS( uninitialized.at(1), const std::out_of_range& );

    uninitialized.append("Hello", 3);
    REQUIRE( uninitialized.size() == 3 );
    REQUIRE( !uninitialized.empty() );
    REQUIRE( uninitialized[0] == 'H' );
    REQUIRE( uninitialized[1] == 'e' );
    REQUIRE( uninitialized[2] == 'l' );

    uninitialized.append("lo");
    REQUIRE( uninitialized.size() == 5 );
    REQUIRE( uninitialized.back() == 'o' );
}
