/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/static_string.hpp>

TEST_CASE("Static string")
{
    auto str = plusone::static_string< 8 >();
    REQUIRE( str.size() == 0 );
    REQUIRE( str.empty() );
    REQUIRE( str.data()[0] == '\0' );
    REQUIRE( str[0] == '\0' );
    REQUIRE( str.at(0) == '\0' );
    REQUIRE( str.capacity() == 8 );
    CHECK_THROWS_AS( str.at(1), const std::out_of_range& );

    str.push_back('X');

    REQUIRE( str.size() == 1 );
    REQUIRE( !str.empty() );
    REQUIRE( str.data()[0] == 'X' );
    REQUIRE( str.data()[1] == '\0' );
    REQUIRE( str[0] == 'X' );
    REQUIRE( str.at(0) == 'X' );
    REQUIRE( str.at(1) == '\0' );
    REQUIRE( str.capacity() == 8 );
    REQUIRE( str.front() == 'X' );
    REQUIRE( str.back() == 'X' );

    str.push_back('Y');

    REQUIRE( str.size() == 2 );
    REQUIRE( !str.empty() );
    REQUIRE( str.data()[0] == 'X' );
    REQUIRE( str.data()[1] == 'Y' );
    REQUIRE( str.data()[2] == '\0' );
    REQUIRE( str[0] == 'X' );
    REQUIRE( str[1] == 'Y' );
    REQUIRE( str.at(0) == 'X' );
    REQUIRE( str.at(1) == 'Y' );
    REQUIRE( str.capacity() == 8 );
    REQUIRE( str.front() == 'X' );
    REQUIRE( str.back() == 'Y' );

    str.clear();
    REQUIRE( str.size() == 0 );
    REQUIRE( str.empty() );
    REQUIRE( str.data()[0] == '\0' );
    REQUIRE( str[0] == '\0' );
    REQUIRE( str.at(0) == '\0' );
    REQUIRE( str.capacity() == 8 );
    CHECK_THROWS_AS( str.at(1), const std::out_of_range& );

    str.append("Hello", 3);
    REQUIRE( str.size() == 3 );
    REQUIRE( !str.empty() );
    REQUIRE( str[0] == 'H' );
    REQUIRE( str[1] == 'e' );
    REQUIRE( str[2] == 'l' );

    str.append("lo");
    REQUIRE( str.size() == 5 );
    REQUIRE( str.back() == 'o' );

    REQUIRE( str == std::string("Hello") );
    REQUIRE( std::string("Hello") == str);
}

TEST_CASE("Static string 2")
{
    plusone::static_string< 4 > s("test");
    REQUIRE( s.size() == 4 );
    REQUIRE( s[0] == 't' );
    REQUIRE( s[1] == 'e' );
    REQUIRE( s[2] == 's' );
    REQUIRE( s[3] == 't' );
    REQUIRE( s == "test" );
}
