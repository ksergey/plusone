/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/process.hpp>

TEST_CASE("Process")
{
    REQUIRE(!plusone::process::running());
    plusone::process::install_signal_handlers();
    REQUIRE(plusone::process::running());
    plusone::signal::raise(SIGINT);
    REQUIRE(!plusone::process::running());
}
