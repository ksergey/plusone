/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/bit_flags.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

enum Enum
{
    Case1 = (1 << 0),
    Case2 = (1 << 1),
    Case3 = (1 << 2),
    Case4 = (1 << 3)
};

using plusone::bit_flags;

TEST_CASE("Default")
{
    bit_flags< Enum > flags;

    REQUIRE( flags.value() == 0 );
    REQUIRE( !flags.is_set(Case1) );
    REQUIRE( !flags.is_set(Case2) );
    REQUIRE( !flags.is_set(Case3) );
    REQUIRE( !flags.is_set(Case4) );

    flags = bit_flags< Enum >{Case1};
    REQUIRE( flags.is_set(Case1) );
    REQUIRE( !flags.is_set(Case2) );
    REQUIRE( !flags.is_set(Case3) );
    REQUIRE( !flags.is_set(Case4) );

    flags.remove(Case1);
    REQUIRE( !flags.is_set(Case1) );
    REQUIRE( !flags.is_set(Case2) );
    REQUIRE( !flags.is_set(Case3) );
    REQUIRE( !flags.is_set(Case4) );

    flags
        .add(Case2)
        .add(Case3)
        .add(Case4);
    REQUIRE( !flags.is_set(Case1) );
    REQUIRE( flags.is_set(Case2) );
    REQUIRE( flags.is_set(Case3) );
    REQUIRE( flags.is_set(Case4) );

    flags.clear();
    REQUIRE( !flags.is_set(Case1) );
    REQUIRE( !flags.is_set(Case2) );
    REQUIRE( !flags.is_set(Case3) );
    REQUIRE( !flags.is_set(Case4) );

    bit_flags< Enum > flags_test{Case2};
    REQUIRE( flags_test != flags );

    flags.add(Case2);
    REQUIRE( flags_test == flags );
    REQUIRE( flags_test == Case2 );
}
