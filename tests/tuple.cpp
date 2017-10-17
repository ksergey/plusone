/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <string>
#include <iostream>
#include <plusone/tuple.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("Tagged Tuple")
{
    struct x1 {};
    struct x2 {};

    using tuple = plusone::tagged_tuple<
        plusone::pair< x1, int >,
        plusone::pair< x2, std::string >
    >;

    tuple test{0, "xxx"};
    REQUIRE( plusone::get< x1 >(test) == 0 );
    REQUIRE( plusone::get< x2 >(test) == std::string("xxx") );

    plusone::get< x1 >(test) = 999;
    REQUIRE( plusone::get< x1 >(test) == 999 );

    tuple move = std::move(test);
    REQUIRE( plusone::get< x1 >(move) == 999 );
    REQUIRE( plusone::get< x2 >(move) == std::string("xxx") );
}

TEST_CASE("For each")
{
    plusone::for_each(std::make_tuple("test", 1u, -5, 'c'), [](const auto& v){
        std::cout << v << '\n';
    });
}
