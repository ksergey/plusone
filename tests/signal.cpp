/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/signal.hpp>

void test(int)
{}

TEST_CASE("Signal base")
{
    auto handler1 = plusone::signal::get_handler(SIGINT);
    auto handler2 = plusone::signal::get_handler(SIGINT);
    REQUIRE( handler1 == handler2 );
    
    plusone::signal::set_handler(SIGINT, test);
    auto handler3 = plusone::signal::get_handler(SIGINT);
    REQUIRE( handler3 != handler1 );
    REQUIRE( handler3 == &test );

    plusone::signal::set_handler(SIGINT, SIG_DFL);
    auto handler4 = plusone::signal::get_handler(SIGINT);
    REQUIRE( handler4 == SIG_DFL );

    plusone::signal::set_handler(SIGINT, SIG_IGN);
    auto handler5 = plusone::signal::get_handler(SIGINT);
    REQUIRE( handler5 == SIG_IGN );
    
}
